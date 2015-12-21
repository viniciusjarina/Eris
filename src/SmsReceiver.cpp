// INCLUDE FILES 
#include "SmsReceiver.h"            // CSmsReceiver
#include "SmsReceiverObserver.h"          // MSmsReceiverObserver

#include <smsuaddr.h>                   // TSmsAddr
#include <gsmubuf.h>                    // CSmsBuffer
#include <smsustrm.h>                   // RSmsSocketReadStream
#include <gsmumsg.h>                    // CSmsMessage
    
// ================= MEMBER FUNCTIONS ========================================
//
// ---------------------------------------------------------------------------
// CSmsReceiver::CSmsReceiver(MSmsReceiverObserver& aObserver)
// Default C++ constructor.
// ---------------------------------------------------------------------------
//
CSmsReceiver::CSmsReceiver(MSmsReceiverObserver& aObserver) :
    CActive(EPriorityStandard),
    iObserver(aObserver)
    {
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::~CSmsReceiver()
// Destructor.
// ---------------------------------------------------------------------------
//
CSmsReceiver::~CSmsReceiver()
    {
    // cancel any request, if outstanding
    Cancel();
    
    iReadSocket.Close();
    iFs.Close();
    iSocketServ.Close();
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::NewL(MSmsReceiverObserver& aObserver)
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSmsReceiver* CSmsReceiver::NewL(MSmsReceiverObserver& aObserver)
    {
    CSmsReceiver* self = CSmsReceiver::NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::NewLC(MSmsReceiverObserver& aObserver)
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSmsReceiver* CSmsReceiver::NewLC(MSmsReceiverObserver& aObserver)
    {
    CSmsReceiver* self = new (ELeave) CSmsReceiver(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::ConstructL()
// Default EPOC constructor.
// ---------------------------------------------------------------------------
//
void CSmsReceiver::ConstructL()
    {
    CActiveScheduler::Add(this);
    
    User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iFs.Connect());
    
    StartListeningL();
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::DoCancel()
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void CSmsReceiver::DoCancel()
    {
    iReadSocket.CancelIoctl();
    iState = ESmsIdle;
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::RunL()
// Handles an active objects request completion event.
// ---------------------------------------------------------------------------
//
void CSmsReceiver::RunL()
    {
    if (iStatus == KErrNone)
        {
        if (iState == ESmsListening)
            {
            // allocate SMS buffer
            CSmsBuffer* buffer = CSmsBuffer::NewL();
            CleanupStack::PushL(buffer);
            
            // create new incoming message, pass ownership of the buffer!
            CSmsMessage* message = CSmsMessage::NewL(iFs, 
                                                     CSmsPDU::ESmsDeliver, 
                                                     buffer);
            CleanupStack::Pop(buffer);
            CleanupStack::PushL(message);

            // open socket read stream
            RSmsSocketReadStream readStream(iReadSocket);
            CleanupClosePushL(readStream);

            // read message
            message->InternalizeL(readStream);
            CleanupStack::PopAndDestroy(&readStream);
            
            TPtrC number = message->ToFromAddress();
            // extract the message body
            HBufC* body = HBufC::NewLC(message->Buffer().Length());
            TPtr bodyPtr(body->Des());
            message->Buffer().Extract(bodyPtr, 0, message->Buffer().Length());

            iObserver.MessageReceived(number, *body);
            CleanupStack::PopAndDestroy(2, message); // body, message
            
            // notify system about successful receiving
            iReadSocket.Ioctl(KIoctlReadMessageSucceeded, iStatus, 
                              NULL, KSolSmsProv);
            iState = ESmsSystemNotyfing;
            SetActive();
            }
        else
            {
            Start();
            }
        }
    else
        {
        iObserver.HandleReceiveError(iStatus.Int());
        }
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::RunError(TInt aError)
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt CSmsReceiver::RunError(TInt aError)
    {
    iObserver.HandleReceiveError(aError);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::Start()
// Starts waiting for the actual socket data.
// ---------------------------------------------------------------------------
//
void CSmsReceiver::Start()
    {
    // wait for an incoming data
    iReadSocket.Ioctl(KIOctlSelect, iStatus, &iBuf, KSOLSocket);
    iState = ESmsListening;
    SetActive();
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::StartListeningL()
// Starts listening for an incoming SMS.
// ---------------------------------------------------------------------------
//
void CSmsReceiver::StartListeningL()
    {
    // we can't handle several requests simultaneously
    if (IsActive())
        {
        User::Leave(KErrNotReady);
        }

    // just in case
    iReadSocket.Close();
    
    // open read socket
    User::LeaveIfError(iReadSocket.Open(iSocketServ,
                                        KSMSAddrFamily,
                                        KSockDatagram,
                                        KSMSDatagramProtocol));

    _LIT8(KMathTag, "#:!");

    // set match pattern
    TSmsAddr smsAddr; 
    smsAddr.SetSmsAddrFamily(ESmsAddrMatchText); 
    smsAddr.SetTextMatch(KMathTag); // put KNullDesC8 to catch all messages
    
    // use this to read the message from a certain port
    //smsAddr.SetSmsAddrFamily(ESmsAddrApplication8BitPort);
    //smsAddr.SetPort(16500); // GSM Application port from 16000 to 16999 
    
    // bind the socket
    User::LeaveIfError(iReadSocket.Bind(smsAddr));
    iBuf() = KSockSelectRead;
    
    Start();
    }

// ---------------------------------------------------------------------------
// CSmsReceiver::StopListening()
// Stops listening.
// ---------------------------------------------------------------------------
//
void CSmsReceiver::StopListening()
    {
    Cancel();
    
    iReadSocket.Close();
    }
