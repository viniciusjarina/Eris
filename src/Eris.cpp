/*
============================================================================
 Name        : Eris.cpp
 Author      : Vinicius Jarina
 Version     :
 Copyright   : Copyright (c) 2009 JarinSoft. (http://viniciusjarina.googlepages.com/)
 Description : Application entry point
============================================================================
*/

// INCLUDE FILES
#include <eikstart.h>
#include "ErisApplication.h"


LOCAL_C CApaApplication* NewApplication()
	{
	return new CErisApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}
