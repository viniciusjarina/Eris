/*
============================================================================
 Name        : ErisDocument.h
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009 JarinSoft. (http://viniciusjarina.googlepages.com/)
 Description : Application document class (model)
============================================================================
*/

#ifndef __ERISDOCUMENT_H__
#define __ERISDOCUMENT_H__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CErisAppUi;
class CEikApplication;


// CLASS DECLARATION

/**
* CErisDocument application class.
* An instance of class CErisDocument is the Document part of the
* AVKON application framework for the Eris application.
*/
class CErisDocument : public CAknDocument
    {
    public: // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        * Construct a CErisDocument for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CErisDocument.
        */
        static CErisDocument* NewL( CEikApplication& aApp );

        /**
        * NewLC.
        * Two-phased constructor.
        * Construct a CErisDocument for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CErisDocument.
        */
        static CErisDocument* NewLC( CEikApplication& aApp );

        /**
        * ~CErisDocument
        * Virtual Destructor.
        */
        virtual ~CErisDocument();

    public: // Functions from base classes

        /**
        * CreateAppUiL
        * From CEikDocument, CreateAppUiL.
        * Create a CErisAppUi object and return a pointer to it.
        * The object returned is owned by the Uikon framework.
        * @return Pointer to created instance of AppUi.
        */
        CEikAppUi* CreateAppUiL();

    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CErisDocument.
        * C++ default constructor.
        * @param aApp Application creating this document.
        */
        CErisDocument( CEikApplication& aApp );

    };

#endif // __ERISDOCUMENT_H__

// End of File
