/*
============================================================================
 Name        : ErisAppUi.cpp
 Author      : Vinicius Jarina (mailto:viniciusjarina@gmail.com)
 Version     :
 Copyright   : Copyright (c) 2009. (http://viniciusjarina.googlepages.com/)
 Description : Main application UI class (controller)
============================================================================
 */

// INCLUDE FILES
#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <aknmessagequerydialog.h> // for CAknMessageQueryDialog
#include <stringloader.h>
#include <Eris.rsg>
#include <f32file.h>
#include <s32file.h>

#include <apgtask.h> 
#include <APGWGNAM.H>

#include "Eris.pan"
#include "ErisAppUi.h"
#include "ErisAppView.h"
#include "DeviceInfoUtility.h"
#include "Eris.hrh"

#include "GetLocationURL.h"
#include "log.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CErisAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CErisAppUi::ConstructL()
{
	// Initialise app UI with standard value.
	BaseConstructL();

	
	HideApplicationFromFSW(ETrue);
	// Create view object
	iAppView = CErisAppView::NewL( ClientRect() );

	iSmsReceiver = CSmsReceiver::NewL(*this);
	iSmsSender = CSmsSender::NewL(*this);	

	CEikonEnv::Static()->SetSystem(ETrue);
		
	HideFromTaskList();
	SendAppToBackground();
}
// -----------------------------------------------------------------------------
// CErisAppUi::CErisAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CErisAppUi::CErisAppUi()
{
	// No implementation required
}

// -----------------------------------------------------------------------------
// CErisAppUi::~CErisAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CErisAppUi::~CErisAppUi()
{
	if ( iAppView )
	{
		delete iAppView;
		iAppView = NULL;
	}
	if(iSmsReceiver)
	{
		delete iSmsReceiver;
		iSmsReceiver = NULL;
	}
	if(iSmsSender)
	{
		delete iSmsSender;
		iSmsSender = NULL;
	}
}

// -----------------------------------------------------------------------------
// CErisAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CErisAppUi::HandleCommandL( TInt aCommand )
{
	switch( aCommand )
	{
	case EEikCmdExit:
	case EAknSoftkeyExit:
	{
		OnHide();
	}
	break;
	case EErisCmdMyPositionURL:
	{
		OnMyPositionURL();
	}
	break;
	case EErisCmdSendMyPosition:
	{
		OnSendMyPosition();
	}
	break;
	case EErisCmdQuit:
	{
		OnQuit();
		break;
	}
	case EErisCmdAbout:
	{
		ShowInfoAbout();
		break;
	}
	default:
		Panic( EErisUi );
		break;
	}
}
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CErisAppUi::HandleStatusPaneSizeChange()
{
	iAppView->SetRect( ClientRect() );
}

void CErisAppUi::OnMyPositionURL()
{
	int res;
	TBuf<250> networkInfoBuf; // It's recommended to use RBuf/HBufC here!
	_LIT(KNetworkInfoHeading, "Network info");
	_LIT(KNetworkNoNetwork, "No Network (no signal)");
	_LIT(KType, "g");

	res = GetLocationURL(networkInfoBuf,KType);

	if(res == 0) // No network information
	{
		networkInfoBuf.Append(KNetworkNoNetwork);
	}

	ShowInfoDialog(KNetworkInfoHeading, networkInfoBuf);
}

void CErisAppUi::OnSendMyPosition()
{
	int res;
	TBuf<160> locationURL; // It's recommended to use RBuf/HBufC here!
	_LIT(KNetworkInfoHeading, "Network info");
	_LIT(KNetworkNoNetwork, "No Network (no signal)");
	_LIT(KType, "g");

	_LIT(KNumber, "01184969252");

	res = GetLocationURL(locationURL,KType);
	
	if(res == 0) // No network information
	{
		locationURL.Append(KNetworkNoNetwork);
	}
	iSmsSender->SendSmsMessageL(KNumber, locationURL, EFalse);
}

void CErisAppUi::OnHide()
{
	SendAppToBackground();
}

void CErisAppUi::OnQuit()
{
	Exit();
}


// -----------------------------------------------------------------------------
// CErisAppUi::ShowInfoDialog()
// -----------------------------------------------------------------------------
//
void CErisAppUi::ShowInfoAbout()
{
	HBufC *heading = iEikonEnv->AllocReadResourceLC(R_ABOUT_HEADING_TEXT); 
	HBufC *message = iEikonEnv->AllocReadResourceLC(R_ABOUT_MESSAGE_TEXT);

	ShowInfoDialog(heading->Des(), message->Des());

	CleanupStack::PopAndDestroy(message);
	CleanupStack::PopAndDestroy(heading);
}

// -----------------------------------------------------------------------------
// CErisAppUi::ShowInfoDialog()
// -----------------------------------------------------------------------------
//
void CErisAppUi::ShowInfoDialog(const TDesC& aHeading, const TDesC& aMessage)
{
	CAknMessageQueryDialog* dlg = new (ELeave)CAknMessageQueryDialog(); 

	dlg->PrepareLC( R_MESSAGE_QUERY_DIALOG ); 

	dlg->SetMessageTextL(aMessage);
	dlg->QueryHeading()->SetTextL( aHeading ); 
	dlg->RunLD(); 
}

void CErisAppUi::SendAppToBackground()
{
	TApaTask task(iEikonEnv->WsSession());
	task.SetWgId( CEikonEnv::Static()->RootWin().Identifier());
	task.SendToBackground();
}

void CErisAppUi::HideFromTaskList()
{
	TInt wgId = iEikonEnv->RootWin().Identifier();
	RWsSession session = iEikonEnv->WsSession();
	CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC(session, wgId);
	wgName->SetHidden(ETrue);
	wgName->SetWindowGroupName(iEikonEnv->RootWin());
	CleanupStack::PopAndDestroy(); // wgName
}

// MessageReceived:
// Function called when phone receive a SMS with our pattern "#:!"
// aNumber - contain the number of sender
// aMsg - contain the content of message
void CErisAppUi::MessageReceived(TDesC & aNumber, TDesC & aMsg)
{
	_LIT(KTypeGoogleMaps, "g"); // Google Maps land mark link eg: /g/4g8j4/12wb
	_LIT(KTypeNokiaMaps , "n");
	_LIT(KTypeStaticMap , "s");
	
	int res;
	TBuf<200> locationURL; // It's recommended to use RBuf/HBufC here!
		
	TInt pos  = aMsg.LocateReverse('n'); // if was sended a 'n' character
	TInt pos2 = aMsg.LocateReverse('N');
	if(pos != KErrNotFound || pos2 != KErrNotFound)
	{
		res = GetLocationURL(locationURL,KTypeNokiaMaps);
		if(res == 0)
			return;
	}
	else
	{
		pos  = aMsg.LocateReverse('s');// if was sended a 's' character
		pos2 = aMsg.LocateReverse('S');
		if(pos != KErrNotFound || pos2 != KErrNotFound)
		{
			res = GetLocationURL(locationURL,KTypeStaticMap);
			if(res == 0)
				return;
		}
		else // if was not send any character the default is link to google maps
		{
			res = GetLocationURL(locationURL,KTypeGoogleMaps);
			if(res == 0)
				return;
		}
	}
	iSmsSender->SendSmsMessageL(aNumber, locationURL, EFalse);	
}

void CErisAppUi::HandleReceiveError(TInt reason)
{
	
}

void CErisAppUi::MessageSentComplete()
{
	
}

void CErisAppUi::HandleError(TInt reason)
{
	
}


// End of File
