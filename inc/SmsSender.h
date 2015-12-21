#ifndef SMSSOCKETENGINE_H_
#define SMSSOCKETENGINE_H_
// INCLUDES 
#include <e32base.h>
#include <es_sock.h>                    // RSocketServ
#include <f32file.h>                    // RFs
#include <badesca.h>                    // CDesCArrayFlat

// LIBS
// esock.lib (RSocketServ), smsu.lib (TSmsAddr), gsmu.lib (CSmsBuffer),
// efsrv.lib (RFs), estor.lib (RSmsSocketWriteStream),
// bafl.lib (CDesCArrayFlat)

// CAPS
// NetworkServices (RSocket::Bind), ReadUserData (RSocket::Bind), 
// WriteUserData (RSocket::Bind)

// FORWARD DECLARATIONS
class MSmsSenderObserver;
class CSmsMessage;

// CLASS DECLARATION
/**
 * CSmsSender class.
 * CSmsSender declaration.
 * 
 */
class CSmsSender : public CActive
    {
    public: // Constructors and destructor

        /**
         * NewL()
         * Creates new CSmsEngine object.
         * @param Reference to the MSmsSenderObserver object.
         * @return Pointer to the created instance of CSmsSender.
         */
        static CSmsSender* NewL(MSmsSenderObserver& aObserver);

        /**
         * NewLC()
         * Creates new CSmsSender object.
         * @param Reference to the MSmsSenderObserver object.
         * @return Pointer to the created instance of CSmsSender.
         */
        static CSmsSender* NewLC(MSmsSenderObserver& aObserver);

        /**
         * ~CSmsSender()
         * Destructor.
         */
        ~CSmsSender();

    protected: // From CActive

        /**
         * DoCancel()
         * Implements cancellation of an outstanding request.
         */
        void DoCancel();

        /**
         * RunL()
         * Handles an active objects request completion event.
         */
        void RunL();

        /**
         * RunError()
         * Handles a leave occurring in the request completion event handler RunL().
         * @param aError The leave code.
         * @return KErrNone if error was handled, otherwise system-wide error.
         */
        TInt RunError(TInt aError);
        
    private: // New functions
        
        /**
         * DoSendSmsMessageL()
         * Performs actual sms sending.
         * @param aNumber Recepient's number.
         */
        void DoSendSmsMessageL(TPtrC aNumber);

    public: // New functions
        
        /**
         * SendSmsMessageL()
         * Sends sms to all recipients.
         * @param aNumbers Array of recipients' numbers. Takes ownership!
         * @param aText Content of the message.
         * @param aUnicode ETrue if messages should be sent using UCS2 alphabet.
         */
        void SendSmsMessageMultipleL(CDesCArrayFlat* aNumbers, 
                             const TDesC& aText, 
                             const TBool aUnicode = ETrue);
        
        void SendSmsMessageL(const TDesC & aNumber, 
                             const TDesC& aText, 
                             const TBool aUnicode = ETrue);
                
    private: // Constructors

        /**
         * CSmsSender()
         * Default C++ constructor.
         * @param Reference to the MSmsSenderObserver object.
         */
        CSmsSender(MSmsSenderObserver& aObserver);

        /**
         * ConstructL()
         * Default EPOC constructor.
         */
        void ConstructL();
        
    private: // data

        MSmsSenderObserver&     iObserver;
        
        RSocketServ             iSocketServ;
        RSocket                 iWriteSocket;
        RFs                     iFs;
        
        CDesCArrayFlat*         iNumbers;
        CSmsMessage*            iSmsMessage;
        
        TPckgBuf<TUint>         iBuf;
    };

#endif /*SMSSOCKETENGINE_H_*/
