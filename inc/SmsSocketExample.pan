#ifndef SMSSOCKETEXAMPLE_PAN_
#define SMSSOCKETEXAMPLE_PAN_

// INCLUDES 
#include <e32std.h>
#include "log.h"

enum TSmsSocketExamplePanics
    {
    ESmsSocketEngineNumberIsEmpty = 1
    };

inline void Panic(TSmsSocketExamplePanics aReason)
    {
    _LIT(KSmsSocketExamplePanic, "SmsSocketExample");
    User::Panic(KSmsSocketExamplePanic, aReason);
    }


#endif /*SMSSOCKETEXAMPLE_PAN_*/
