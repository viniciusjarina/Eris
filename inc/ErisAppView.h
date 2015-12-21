/*
============================================================================
 Name        : ErisView.h
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009 JarinSoft. (http://viniciusjarina.googlepages.com/)
 Description : Application view
============================================================================
*/

#ifndef __ERISAPPVIEW_H__
#define __ERISAPPVIEW_H__

// INCLUDES
#include <coecntrl.h>

// CLASS DECLARATION
class CErisAppView : public CCoeControl
    {
    public: // New methods

        /**
        * NewL.
        * Two-phased constructor.
        * Create a CErisAppView object, which will draw itself to aRect.
        * @param aRect The rectangle this view will be drawn to.
        * @return a pointer to the created instance of CErisAppView.
        */
        static CErisAppView* NewL( const TRect& aRect );

        /**
        * NewLC.
        * Two-phased constructor.
        * Create a CErisAppView object, which will draw itself
        * to aRect.
        * @param aRect Rectangle this view will be drawn to.
        * @return A pointer to the created instance of CErisAppView.
        */
        static CErisAppView* NewLC( const TRect& aRect );

        /**
        * ~CErisAppView
        * Virtual Destructor.
        */
        virtual ~CErisAppView();

    public:  // Functions from base classes

        /**
        * From CCoeControl, Draw
        * Draw this CErisAppView to the screen.
        * @param aRect the rectangle of this view that needs updating
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CoeControl, SizeChanged.
        * Called by framework when the view size is changed.
        */
        virtual void SizeChanged();

    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        * Perform the second phase construction of a
        * CErisAppView object.
        * @param aRect The rectangle this view will be drawn to.
        */
        void ConstructL(const TRect& aRect);

        /**
        * CErisAppView.
        * C++ default constructor.
        */
        CErisAppView();

    };

#endif // __ERISAPPVIEW_H__

// End of File
