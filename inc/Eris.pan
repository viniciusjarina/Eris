/*
============================================================================
 Name        : Eris.pan
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009 JarinSoft. (http://viniciusjarina.googlepages.com/)
 Description : Application panic codes
============================================================================
*/

#ifndef __ERIS_PAN__
#define __ERIS_PAN__

/** Eris application panic codes */
enum TErisPanics
    {
    EErisUi = 1
    // add further panics here
    };

inline void Panic(TErisPanics aReason)
    {
    _LIT(applicationName,"Eris");
    User::Panic(applicationName, aReason);
    }

#endif // __ERIS_PAN__
