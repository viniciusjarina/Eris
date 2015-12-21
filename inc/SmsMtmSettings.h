#ifndef SMSMTMSETTINGS_H_
#define SMSMTMSETTINGS_H_
// INCLUDES 
#include <e32base.h>
#include <msvapi.h>                     // MMsvSessionObserver

// LIBS
// msgs.lib (CMsvSession), smcm.lib (CSmsSettings)

// CAPS
// ReadUserData (CClientMtmRegistry::NewMtmL), 
// WriteUserData (CClientMtmRegistry::NewMtmL)

// FORWARD DECLARATIONS
class CSmsClientMtm;
class CClientMtmRegistry;

// CLASS DECLARATION
/**
 * CSmsMtmSettings class.
 * CSmsMtmSettings declaration.
 * 
 */
class CSmsMtmSettings : public CBase, MMsvSessionObserver
    {
    public: // Constructors and destructor

        /**
         * NewL()
         * Creates new CSmsMtmSettings object.
         * @return Pointer to the created instance of CSmsMtmSettings.
         */
        static CSmsMtmSettings* NewL();

        /**
         * NewLC()
         * Creates new CSmsMtmSettings object.
         * @return Pointer to the created instance of CSmsMtmSettings.
         */
        static CSmsMtmSettings* NewLC();

        /**
         * ~CSmsMtmSettings()
         * Destructor.
         */
        ~CSmsMtmSettings();

    private: // from MMsvSessionObserver
        
        /**
         * HandleSessionEventL()
         * Indicates an event has occurred.
         * @param aEvent Indicates the event type.
         * @param aArg1, aArg2, aArg3 Event type-specific argument value.
         */
        void HandleSessionEventL(TMsvSessionEvent aEvent, 
                                 TAny* aArg1,
                                 TAny* aArg2,
                                 TAny* aArg3);

    public: // New functions
        
        /**
         * GetDefaultScNumberL()
         * Retrieves the default service center number.
         * @return SC number.
         */
        TPtrC GetDefaultScNumberL() const;
        
    private: // Constructors

        /**
         * CSmsMtmSettings()
         * Default C++ constructor.
         */
        CSmsMtmSettings();

        /**
         * ConstructL()
         * Default EPOC constructor.
         */
        void ConstructL();

    private: // data
        
        CSmsClientMtm*          iSmsMtm;
        CMsvSession*            iMsvSession;
        CClientMtmRegistry*     iMtmRegistry;
    };


#endif /*SMSMTMSETTINGS_H_*/
