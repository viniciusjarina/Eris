// INCLUDE FILES 
#include "SmsSender.h"            // CSmsSender
#include "SmsSenderObserver.h"          // MSmsSenderObserver
#include "SmsMtmSettings.h"             // CSmsMtmSettings

#include <smsuaddr.h>                   // TSmsAddr
#include <gsmubuf.h>                    // CSmsBuffer
#include <smsustrm.h>                   // RSmsSocketReadStream
#include <gsmumsg.h>                    // CSmsMessage

#include "SmsSocketExample.pan"

// ================= MEMBER FUNCTIONS =========================================
//
// ----------------------------------------------------------------------------
// CSmsSender::CSmsSender(MSmsSenderObserver& aObserver)
// Default C++ constructor.
// ----------------------------------------------------------------------------
//
CSmsSender::CSmsSender(MSmsSenderObserver& aObserver) :
    CActive(EPriorityStandard), iObserver(aObserver)
    {
    }

// ----------------------------------------------------------------------------
// CSmsSender::~CSmsSender()
// Destructor.
// ----------------------------------------------------------------------------
//
CSmsSender::~CSmsSender()
    {
    // cancel any request, if outstanding
    Cancel();
    
    delete iNumbers;
    delete iSmsMessage;
    
    iWriteSocket.Close();
    iFs.Close();
    iSocketServ.Close();
    }

// ----------------------------------------------------------------------------
// CSmsSender::NewL(MSmsSenderObserver& aObserver)
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CSmsSender* CSmsSender::NewL(MSmsSenderObserver& aObserver)
    {
    CSmsSender* self = CSmsSender::NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// CSmsSender::NewLC(MSmsSenderObserver& aObserver)
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CSmsSender* CSmsSender::NewLC(MSmsSenderObserver& aObserver)
    {
    CSmsSender* self = new (ELeave) CSmsSender(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ----------------------------------------------------------------------------
// CSmsSender::ConstructL()
// Default EPOC constructor.
// ----------------------------------------------------------------------------
//
void CSmsSender::ConstructL()
    {
    CActiveScheduler::Add(this);
    User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iFs.Connect());
    }

// ----------------------------------------------------------------------------
// CSmsSender::DoCancel()
// Implements cancellation of an outstanding request.
// ----------------------------------------------------------------------------
//
void CSmsSender::DoCancel()
    {
    iWriteSocket.CancelIoctl();
    }

// ----------------------------------------------------------------------------
// CSmsSender::RunL()
// Handles an active objects request completion event.
// ----------------------------------------------------------------------------
//
void CSmsSender::RunL()
    {
    if (iStatus == KErrNone)
        {
        // push first number from the numbers stack
        iNumbers->Delete(0);
        
        if (iNumbers->Count())
            {
            // send message to the next recepient
            DoSendSmsMessageL((*iNumbers)[0]);
            return;
            }
        else
            {
            iObserver.MessageSentComplete();
            }
        }
    else
        {
        iObserver.HandleError(iStatus.Int());
        }
    
    // cleanup
    delete iNumbers;
    iNumbers = NULL;
    delete iSmsMessage;
    iSmsMessage = NULL;
    iWriteSocket.Close();
    }

// ----------------------------------------------------------------------------
// CSmsSender::RunError(TInt aError)
// Handles a leave occurring in the request completion event handler RunL().
// ----------------------------------------------------------------------------
//
TInt CSmsSender::RunError(TInt aError)
    {
    iObserver.HandleError(aError);
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CSmsSender::DoSendSmsMessageL(TPtrC aNumber)
// Performs actual sms sending.
// ----------------------------------------------------------------------------
//
void CSmsSender::DoSendSmsMessageL(TPtrC aNumber)
    {
    // set current recepient
    iSmsMessage->SmsPDU().SetToFromAddressL(aNumber);
     
    // write message to the socket
    RSmsSocketWriteStream writeStream(iWriteSocket);
    CleanupClosePushL(writeStream);
    iSmsMessage->ExternalizeL(writeStream);
    CleanupStack::PopAndDestroy(&writeStream);
    
    iWriteSocket.Ioctl(KIoctlSendSmsMessage, iStatus, &iBuf, KSolSmsProv);
    SetActive();
    }

// ----------------------------------------------------------------------------
// CSmsSender::SendSmsMessageL(CDesCArrayFlat* aNumbers, 
//                                   const TDesC& aText, const TBool aUnicode)
// Sends sms to all recipients.
// ----------------------------------------------------------------------------
//
void CSmsSender::SendSmsMessageMultipleL(CDesCArrayFlat* aNumbers, 
                                       const TDesC& aText, const TBool aUnicode)
    {
    // make sure we have a proper number array
    __ASSERT_DEBUG(aNumbers && aNumbers->Count(),
            Panic(ESmsSocketEngineNumberIsEmpty));
    
    // we can't handle several requests simultaneously
    if (IsActive())
        {
        User::Leave(KErrNotReady);
        }
    
    // just in case
    delete iNumbers;
    iNumbers = NULL;
    delete iSmsMessage;
    iSmsMessage = NULL;
    iWriteSocket.Close();
    
    // open write socket
    User::LeaveIfError(iWriteSocket.Open(iSocketServ, 
                                         KSMSAddrFamily, 
                                         KSockDatagram, 
                                         KSMSDatagramProtocol));
    
    // set address family
    TSmsAddr smsAddr;
    smsAddr.SetSmsAddrFamily(ESmsAddrSendOnly);
    
    // bind the socket
    User::LeaveIfError(iWriteSocket.Bind(smsAddr));
    iBuf() = KSockSelectWrite;
        
    // allocate SMS buffer
    CSmsBuffer* buffer = CSmsBuffer::NewL();
    CleanupStack::PushL(buffer);
    
    // insert message text
    buffer->InsertL(0, aText);
        
    // create new outgoing message, pass ownerhip of the buffer!
    iSmsMessage = CSmsMessage::NewL(iFs, CSmsPDU::ESmsSubmit, buffer);
    CleanupStack::Pop(buffer);
    
    // setup default service center number
    CSmsMtmSettings* smsMtmSettings = CSmsMtmSettings::NewLC();
    TPtrC scNumber = smsMtmSettings->GetDefaultScNumberL();
    iSmsMessage->SmsPDU().SetServiceCenterAddressL(scNumber);
    CleanupStack::PopAndDestroy(smsMtmSettings);
    
    // set alphabet
    TSmsDataCodingScheme::TSmsAlphabet smsAlphabet = aUnicode ?
            TSmsDataCodingScheme::ESmsAlphabetUCS2 :
            TSmsDataCodingScheme::ESmsAlphabet7Bit;
    iSmsMessage->SmsPDU().SetAlphabet(smsAlphabet);
    
    // set destination port
    //TSmsId portAddressing = aUnicode ?
    //        CSmsInformationElement::ESmsIEIApplicationPortAddressing16Bit :
    //        CSmsInformationElement::ESmsIEIApplicationPortAddressing8Bit;
    //
    //TBuf8<4> smsPort(4);
    //smsPort.FillZ();
    //smsPort[0] = 0x40;  // 16500 = 0x4074 (GSM Application port from 16000 to 16999) 
    //smsPort[1] = 0x74;
    //iSmsMessage->SmsPDU().UserData().AddInformationElementL(portAddressing, smsPort);   
    
    // set status report request flag
    static_cast<CSmsSubmit*>(&iSmsMessage->SmsPDU())->SetStatusReportRequest(EFalse);
    
    // start sending mesage to the first recepient
    DoSendSmsMessageL((*aNumbers)[0]);
    
    // take ownership
    iNumbers = aNumbers;
}

void CSmsSender::SendSmsMessageL(const TDesC & aNumber, 
        const TDesC& aText, 
        const TBool aUnicode)
	{
	CDesCArrayFlat *arrNumbers = new (ELeave) CDesCArrayFlat (1);
	
	arrNumbers->AppendL(aNumber);
	
	SendSmsMessageMultipleL(arrNumbers, aText, aUnicode);
	}
