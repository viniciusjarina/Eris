#ifndef SMS_RECEIVER_H_
#define SMS_RECEIVER_H_

// INCLUDES 
#include <e32base.h>
#include <es_sock.h>                    // RSocketServ
#include <f32file.h>                    // RFs

// LIBS
// esock.lib (RSocketServ), smsu.lib (TSmsAddr), gsmu.lib (CSmsBuffer),
// efsrv.lib (RFs), estor.lib (RSmsSocketReadStream)

// CAPS
// NetworkServices (RSocket::Bind), ReadUserData (RSocket::Bind), 
// WriteUserData (RSocket::Bind)

// FORWARD DECLARATIONS
class MSmsReceiverObserver;

// CLASS DECLARATION
/**
 * CSmsReceiver class.
 * CSmsReceiver declaration.
 * 
 */
class CSmsReceiver : public CActive
    {
    public: // Constructors and destructor

        /**
         * NewL()
         * Creates new CSmsReceiver object.
         * @param aObserver Reference to the MSmsReceiverObserver object.
         * @return Pointer to the created instance of CSmsReceiver.
         */
        static CSmsReceiver* NewL(MSmsReceiverObserver& aObserver);

        /**
         * NewLC()
         * Creates new CSmsReceiver object.
         * @param aObserver Reference to the MSmsReceiverObserver object.
         * @return Pointer to the created instance of CSmsReceiver.
         */
        static CSmsReceiver* NewLC(MSmsReceiverObserver& aObserver);

        /**
         * ~CSmsReceiver()
         * Destructor.
         */
        ~CSmsReceiver();

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
         * Start()
         * Starts waiting for the actual socket data.
         */
        void Start();

    public: // New functions
        
        /**
         * StartListeningL()
         * Starts listening for an incoming SMS.
         */
        void StartListeningL();
        
        /**
         * StopListening()
         * Stops listening.
         */
        void StopListening();

    private: // Constructors

        /**
         * CSmsReceiver()
         * Default C++ constructor.
         * @param aObserver Reference to the MSmsReceiverObserver object.
         */
        CSmsReceiver(MSmsReceiverObserver& aObserver);

        /**
         * ConstructL()
         * Default EPOC constructor.
         */
        void ConstructL();

    private: // enum
        
        enum TSmsSocketEngineState
            {
            ESmsIdle,
            ESmsListening,
            ESmsSystemNotyfing
            };

    private: // data

        MSmsReceiverObserver&     iObserver;
        
        RSocketServ             iSocketServ;
        RSocket                 iReadSocket;
        
        RFs                     iFs;
        
        TBool                   iWait;
        TPckgBuf<TUint>         iBuf;
        
        TSmsSocketEngineState   iState;
    };


#endif /*SMS_RECEIVER_H_*/
