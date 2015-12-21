/*
============================================================================
 Name        : ErisDocument.cpp
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009. (http://viniciusjarina.googlepages.com/)
 Description : Application document class (model)
============================================================================
*/

// INCLUDE FILES
#include "ErisAppUi.h"
#include "ErisDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CErisDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CErisDocument* CErisDocument::NewL( CEikApplication&
                                                          aApp )
    {
    CErisDocument* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CErisDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CErisDocument* CErisDocument::NewLC( CEikApplication&
                                                           aApp )
    {
    CErisDocument* self =
        new ( ELeave ) CErisDocument( aApp );

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CErisDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CErisDocument::ConstructL()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CErisDocument::CErisDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CErisDocument::CErisDocument( CEikApplication& aApp )
    : CAknDocument( aApp )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CErisDocument::~CErisDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CErisDocument::~CErisDocument()
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CErisDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CErisDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it;
    // the framework takes ownership of this object
    return ( static_cast <CEikAppUi*> ( new ( ELeave )
                                        CErisAppUi ) );
    }

// End of File
