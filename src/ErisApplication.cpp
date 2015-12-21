/*
============================================================================
 Name        : ErisApplication.cpp
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009. (http://viniciusjarina.googlepages.com/)
 Description : Main application class
============================================================================
*/

// INCLUDE FILES
#include "ErisDocument.h"
#include "ErisApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidErisApp = { 0xeee3c37c };

// -----------------------------------------------------------------------------
// CErisApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CErisApplication::CreateDocumentL()
    {
    // Create an Eris document, and return a pointer to it
    return (static_cast<CApaDocument*>
                    ( CErisDocument::NewL( *this ) ) );
    }

// -----------------------------------------------------------------------------
// CErisApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CErisApplication::AppDllUid() const
    {
    // Return the UID for the Eris application
    return KUidErisApp;
    }

// End of File
