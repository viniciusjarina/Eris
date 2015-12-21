/*
============================================================================
 Name        : ErisAppUi.h
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009 JarinSoft. (http://viniciusjarina.googlepages.com/)
 Description : Main application UI class (controller)
============================================================================
*/

#ifndef __ERISAPPUI_H__
#define __ERISAPPUI_H__

// INCLUDES
#include <aknappui.h>

#include "SmsReceiver.h"
#include "SmsReceiverObserver.h"

#include "SmsSender.h"
#include "SmsSenderObserver.h"

// FORWARD DECLARATIONS
class CErisAppView;


// CLASS DECLARATION
/**
* CErisAppUi application UI class.
* Interacts with the user through the UI and request message processing
* from the handler class
*/
class CErisAppUi : public CAknAppUi, public MSmsReceiverObserver, public MSmsSenderObserver
    {
    public: // Constructors and destructor

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CErisAppUi.
        * C++ default constructor. This needs to be public due to
        * the way the framework constructs the AppUi
        */
        CErisAppUi();

        /**
        * ~CErisAppUi.
        * Virtual Destructor.
        */
        virtual ~CErisAppUi();

    private:  // Functions from base classes

        /**
        * From CEikAppUi, HandleCommandL.
        * Takes care of command handling.
        * @param aCommand Command to be handled.
        */
        void HandleCommandL( TInt aCommand );

        /**
        *  HandleStatusPaneSizeChange.
        *  Called by the framework when the application status pane
 		*  size is changed.
        */

		void HandleStatusPaneSizeChange();
        
	private: // New functions

		void OnMyPositionURL();
		void OnSendMyPosition();
		void OnHide();
		void OnQuit();
		void ShowInfoAbout();

		void ShowInfoDialog(const TDesC& aHeading, const TDesC& aMessage);
		void SendAppToBackground();
		void HideFromTaskList();
		
		void MessageReceived(TDesC & aNumber, TDesC & aMsg);
		void HandleReceiveError(TInt reason);
		
		void MessageSentComplete();
		void HandleError(TInt reason);

    private: // Data

        /**
        * The application view
        * Owned by CErisAppUi
        */
        CErisAppView* iAppView;
        
        CSmsReceiver* iSmsReceiver;
        CSmsSender* iSmsSender;
    };

#endif // __ERISAPPUI_H__

// End of File
