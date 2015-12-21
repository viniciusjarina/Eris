#ifndef SMS_RECEIVE_OBSERVER_H_
#define SMS_RECEIVE_OBSERVER_H_

class MSmsReceiverObserver
	{
public:
	virtual void MessageReceived(TDesC & aNumber, TDesC & aMsg) = 0;
	virtual void HandleReceiveError(TInt reason) = 0;
	};

#endif /*SMS_RECEIVE_OBSERVER_H_*/
