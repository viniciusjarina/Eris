/*
 * ============================================================================
 * Name : DeviceInfoUtility.cpp
 * Author : Vinod Kumar K V (mailto:vin2kalks@gmail.com)
 * Version : 1.0.0b
 * Date Createed : 23/02/2007
 * Date Modified : 23/02/2007
 * Copyright : Copyright (c) 1999 - 2007 vin2ktalks.
 *			  (http://vin2ktalks.googlepages.com/)
 * Description : The class to get the IMEI number of the device.
 *				 Please note that in order to use this class the 
 *				 user has to link against etel3rdparty.lib, as the 
 *				 CTelephony which is used in this class needs it.
 * ============================================================================
 */

// INCLUDE FILES
// Class include
#include "DeviceInfoUtility.h"
// System includes
// User includes

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::NewL()
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CDeviceInfoUtility* CDeviceInfoUtility::NewL()
    {
    CDeviceInfoUtility* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::NewLC()
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CDeviceInfoUtility* CDeviceInfoUtility::NewLC()
    {
    CDeviceInfoUtility* self = new ( ELeave ) CDeviceInfoUtility();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::~CDeviceInfoUtility()
// Destructor.
// ----------------------------------------------------------------------------
//
CDeviceInfoUtility::~CDeviceInfoUtility()
	{
	if(IsActive())
		{
		Cancel();
		}

	if(iTelephony)
		{
		delete iTelephony;
		iTelephony = NULL;
		}

	if(iActiveSchedulerWait)
		{
		delete iActiveSchedulerWait;
		iActiveSchedulerWait = NULL;
		}
	}

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::CDeviceInfoUtility()
// Constructor.
// ----------------------------------------------------------------------------
//
CDeviceInfoUtility::CDeviceInfoUtility() 
	: 
	CActive( CActive::EPriorityHigh ),
	iDevInfoUtilityType ( EDevInfoUtilityTypeNone ),
	iPhoneIdV1Pckg ( iPhoneIdV1 ),
	iNetworkInfoV1Pckg ( iNetworkInfoV1 ),
	iSubscriberIdV1Pckg ( iSubscriberIdV1 ),
	iManufacturer ( KNullDesC ),
	iModel ( KNullDesC ),
	iSerialNumber ( KNullDesC ),
	iCellId ( 0 ),
	iNetworkId ( KNullDesC ),
	iLocationAreaCode ( 0 ),
	iCountryCode ( KNullDesC ),
	iSubscriberId ( KNullDesC )
    {	
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::ConstructL()
// Perform second phase construction of this object.
// ----------------------------------------------------------------------------
//
void CDeviceInfoUtility::ConstructL()
    {
	// Initializing the Telephony interface.
	iTelephony = CTelephony::NewL();
	// Initializing the ActiveShedulerWait.
	iActiveSchedulerWait = new (ELeave) CActiveSchedulerWait();
    }

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::RunL()
// Respond to an event.
// ----------------------------------------------------------------------------
//
void CDeviceInfoUtility::RunL()
	{
	if(iStatus.Int() == KErrNone)
		{
		if(iActiveSchedulerWait->IsStarted())
			{
			iActiveSchedulerWait->AsyncStop();
			switch(iDevInfoUtilityType)
				{
				case EDevInfoUtilityTypePhoneInfo:
					{			
					// Copy the the details to the member data.
					iManufacturer.Copy(iPhoneIdV1.iManufacturer);
					iModel.Copy(iPhoneIdV1.iModel);
					iSerialNumber.Copy(iPhoneIdV1.iSerialNumber);
					break;
					}
				case EDevInfoUtilityTypeNetworkInfo:
					{
					// Copy the the details to the member data.
					iCellId = iNetworkInfoV1.iCellId;
					iLocationAreaCode = iNetworkInfoV1.iLocationAreaCode;
					iNetworkId.Copy(iNetworkInfoV1.iNetworkId);
					iCountryCode.Copy(iNetworkInfoV1.iCountryCode);
					break;
					}
				case EDevInfoUtilityTypeSubscriberInfo:
					{
					iSubscriberId.Copy(iSubscriberIdV1.iSubscriberId);
					break;
					}
				default:
					{
					break;
					}				
				}
			iDevInfoUtilityType = EDevInfoUtilityTypeNone;
			}
		}
	else
		{
		// Error occurred!
		}
	}

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::DoCancel()
// Cancel any outstanding requests.
// ----------------------------------------------------------------------------
//
void CDeviceInfoUtility::DoCancel()
	{
	switch(iDevInfoUtilityType)
		{
		case EDevInfoUtilityTypePhoneInfo:
			{
			iTelephony->CancelAsync(CTelephony::EGetPhoneIdCancel);
			break;
			}
		case EDevInfoUtilityTypeNetworkInfo:
			{
            iTelephony->CancelAsync(CTelephony::EGetCurrentNetworkInfoCancel);
			break;
			}
		case EDevInfoUtilityTypeSubscriberInfo:
			{
			iTelephony->CancelAsync(CTelephony::EGetSubscriberIdCancel);
			break;
			}
		default:
			{
			break;
			}
		}
	}

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::RunError(TInt aError)
// Handles a leave occurring in the request completion event handler RunL().
// ----------------------------------------------------------------------------
//
TInt CDeviceInfoUtility::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::GetInfo(TDevInfoUtilityType aDevInfoUtilityType)
// ----------------------------------------------------------------------------
//
void CDeviceInfoUtility::GetInfo(TDevInfoUtilityType aDevInfoUtilityType)
	{
	if(IsActive())
		{
		Cancel();
		}

	iDevInfoUtilityType = aDevInfoUtilityType;

	switch(aDevInfoUtilityType)
		{
		case EDevInfoUtilityTypePhoneInfo:
			{
			iTelephony->GetPhoneId(iStatus, iPhoneIdV1Pckg);
			break;
			}
		case EDevInfoUtilityTypeNetworkInfo:
			{
			iTelephony->GetCurrentNetworkInfo(iStatus, iNetworkInfoV1Pckg);
			break;
			}
		case EDevInfoUtilityTypeSubscriberInfo:
			{
			iTelephony->GetSubscriberId(iStatus,  iSubscriberIdV1Pckg);
			break;
			}
		default:
			{
			break;
			}
		}

	SetActive();
	iActiveSchedulerWait->Start();
	}

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::GetPhoneInfo(
//	TManufacturer& aManufacturer, TModel& aModel, TSerialNumber& aSerialNumber)
// ----------------------------------------------------------------------------
//
void CDeviceInfoUtility::GetPhoneInfo(
	TManufacturer& aManufacturer, TModel& aModel, TSerialNumber& aSerialNumber)
	{
	GetInfo(EDevInfoUtilityTypePhoneInfo);

	aManufacturer.Copy(iManufacturer);
	aModel.Copy(iModel);
	aSerialNumber.Copy(iSerialNumber);
	}

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::GetNetworkInfo(
//		TUint aCellId, TUint aLocationAreaCode, TCountryCode& aCountryCode)
// ----------------------------------------------------------------------------
//
void CDeviceInfoUtility::GetNetworkInfo(
	TUint& aCellId, TUint& aLocationAreaCode, TNetworkId& aNetworkId,TCountryCode& aCountryCode)
	{
	GetInfo(EDevInfoUtilityTypeNetworkInfo);

	aCellId = iCellId;
	aLocationAreaCode = iLocationAreaCode;
	aNetworkId.Copy(iNetworkId);
	aCountryCode.Copy(iCountryCode);
	}

// ----------------------------------------------------------------------------
// CDeviceInfoUtility::GetSubscriberInfo(TSubscriberId& aSubscriberId)
// ----------------------------------------------------------------------------
//
void CDeviceInfoUtility::GetSubscriberInfo(TSubscriberId& aSubscriberId)
	{
	GetInfo(EDevInfoUtilityTypeSubscriberInfo);
	
	aSubscriberId.Copy(iSubscriberId);
	}

// End of File
