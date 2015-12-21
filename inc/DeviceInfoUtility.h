/*
 * ============================================================================
 * Name : DeviceInfoUtility.h
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

#ifndef __DEVICEINFOUTILITY_H__
#define __DEVICEINFOUTILITY_H__

// INCLUDES
// System Includes
#include <e32base.h>  // CBase, link against euser.lib
#include <Etel3rdParty.h> // For CTelephony link against etel3rdparty.lib
// User includes

// CONSTANTS
const TInt KIMEINumberLength(15);

// TYPEDEFS
/**
 * Typedef to represent the IMEI number.
 */
typedef TBuf<CTelephony::KPhoneManufacturerIdSize> TManufacturer;
typedef TBuf<CTelephony::KPhoneModelIdSize> TModel;
typedef TBuf<CTelephony::KPhoneSerialNumberSize> TSerialNumber;
typedef TBuf<CTelephony::KNetworkCountryCodeSize> TCountryCode;
typedef TBuf<CTelephony::KNetworkIdentitySize> TNetworkId;
typedef TBuf<CTelephony::KIMSISize> TSubscriberId;

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * \class CDeviceInfoUtility
 *
 * \author Vinod Kumar K V
 *
 * \date 14/02/2007
 *
 * \brief The class to get the IMEI number of the device.
 *
 * \par Please note that in order to use this class the 
 *		user has to link against etel3rdparty.lib, as the 
 *		CTelephony which is used in this class needs it.
 */
class CDeviceInfoUtility : public CActive
	{
	public: // Constructors and destructor

		/**
		 * Create new CDeviceInfoUtility object.
		 * \return a pointer to the created instance of CDeviceInfoUtility
		 */
		static CDeviceInfoUtility* NewL();

		/**
		 * Create new CDeviceInfoUtility object.
		 * \return a pointer to the created instance of CDeviceInfoUtility
		 */
		static CDeviceInfoUtility* NewLC();

		/**
		 * Destroy the object and release all memory of the objects.
		 */
		virtual ~CDeviceInfoUtility();

			private:    // Default constructors

		/**
		 * Default constructor
		 */
		CDeviceInfoUtility();

		/**
		 * Second-phase constructor.
		 */
		void ConstructL();

	protected:    // From CActive

		/**
		 * Handles an active object's request completion event.
		 */
		void DoCancel();

		/**
		 * Implements cancellation of an outstanding request.
		 */
		void RunL();

		/**
		 * Handles a leave occurring in the request completion event handler RunL().
		 * \prarm aError The default implementation returns aError.
		 * \return The error codes.
		 */
		TInt RunError(TInt aError);

	private: // New functions

		enum TDevInfoUtilityType
			{
			EDevInfoUtilityTypeNone,
			EDevInfoUtilityTypePhoneInfo,
			EDevInfoUtilityTypeNetworkInfo,
			EDevInfoUtilityTypeSubscriberInfo
			};

		void GetInfo(CDeviceInfoUtility::TDevInfoUtilityType aDevInfoUtilityType);

	public: // New functions

		void GetPhoneInfo(TManufacturer& aManufacturer, TModel& aModel, TSerialNumber& aSerialNumber);
		void GetNetworkInfo(TUint& aCellId, TUint& aLocationAreaCode, TNetworkId& aNetworkId,TCountryCode& aCountryCode);
		void GetSubscriberInfo(TSubscriberId& aSubscriberId);

	private: // Member data
		/**
		 * The object to provide the simple interface to get 
		 * the phone's telephony informations.
		 * Owned by CDeviceInfoUtility.
		 */
		CTelephony* iTelephony;

		/**
		 * Controls a single scheduling loop in the current active scheduler
		 * Owned by CDeviceInfoUtility object.
		 */
		CActiveSchedulerWait* iActiveSchedulerWait;

		/**
		 * To store the state of the Active Object.
		 */
		TDevInfoUtilityType iDevInfoUtilityType;

		/**
		 * Defines the mobile phone identity.
		 */
		CTelephony::TPhoneIdV1Pckg iPhoneIdV1Pckg; 
		CTelephony::TNetworkInfoV1Pckg iNetworkInfoV1Pckg;
		CTelephony::TSubscriberIdV1Pckg iSubscriberIdV1Pckg;

		/**
		 * Defines the mobile phone identity.
		 */
		CTelephony::TPhoneIdV1 iPhoneIdV1;
		CTelephony::TNetworkInfoV1 iNetworkInfoV1;
		CTelephony::TSubscriberIdV1 iSubscriberIdV1;

		TManufacturer iManufacturer;
		TModel iModel;
		TSerialNumber iSerialNumber;		

		TUint iCellId;
		TUint iLocationAreaCode;
		TNetworkId iNetworkId;
		TCountryCode iCountryCode;

		TSubscriberId iSubscriberId;		
	};

#endif // __DEVICEINFOUTILITY_H__

// End of File
