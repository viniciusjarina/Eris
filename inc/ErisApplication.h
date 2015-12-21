/*
============================================================================
 Name        : ErisApplication.h
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009 JarinSoft. (http://viniciusjarina.googlepages.com/)
 Description : Main application class
============================================================================
*/

#ifndef __ERISAPPLICATION_H__
#define __ERISAPPLICATION_H__

// INCLUDES
#include <aknapp.h>

// CLASS DECLARATION

/**
* CErisApplication application class.
* Provides factory to create concrete document object.
* An instance of CErisApplication is the application part of the
* AVKON application framework for the Eris application.
*/
class CErisApplication : public CAknApplication
    {
    public: // Functions from base classes

        /**
        * From CApaApplication, AppDllUid.
        * @return Application's UID (KUidErisApp).
        */
        TUid AppDllUid() const;

    protected: // Functions from base classes

        /**
        * From CApaApplication, CreateDocumentL.
        * Creates CErisDocument document object. The returned
        * pointer in not owned by the CErisApplication object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif // __ERISAPPLICATION_H__

// End of File
