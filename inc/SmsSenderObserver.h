#ifndef SMSENGINEOBSERVER_H_
#define SMSENGINEOBSERVER_H_

class MSmsSenderObserver
	{
public:
	virtual void MessageSentComplete() = 0;
	virtual void HandleError(TInt reason) = 0;
	};

#endif /*SMSENGINEOBSERVER_H_*/
