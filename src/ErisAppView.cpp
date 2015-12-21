/*
============================================================================
 Name        : ErisView.cpp
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009. (http://viniciusjarina.googlepages.com/)
 Description : Application view
============================================================================
*/

// INCLUDE FILES
#include <coemain.h>
#include "ErisAppView.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CErisAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CErisAppView* CErisAppView::NewL( const TRect& aRect )
    {
    CErisAppView* self = CErisAppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CErisAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CErisAppView* CErisAppView::NewLC( const TRect& aRect )
    {
    CErisAppView* self = new ( ELeave ) CErisAppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CErisAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CErisAppView::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CErisAppView::CErisAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CErisAppView::CErisAppView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CErisAppView::~CErisAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CErisAppView::~CErisAppView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CErisAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CErisAppView::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect( Rect());

    // Clears the screen
    gc.Clear( drawRect );
    
  	}

// -----------------------------------------------------------------------------
// CErisAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CErisAppView::SizeChanged()
    {  
    DrawNow();
    }
// End of File
