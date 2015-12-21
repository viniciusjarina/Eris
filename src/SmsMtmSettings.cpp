// INCLUDE FILES 
#include "SmsMtmSettings.h"             // CSmsMtmSettings

#include <mtclreg.h>                    // CClientMtmRegistry
#include <smsclnt.h>                    // CSmsClientMtm
#include <smutset.h>                    // CSmsSettings
#include <smut.h>                       // KUidMsgTypeSMS

// ================= MEMBER FUNCTIONS ========================================
//
// ---------------------------------------------------------------------------
// CSmsMtmSettings::CSmsMtmSettings()
// Default C++ constructor.
// ---------------------------------------------------------------------------
//
CSmsMtmSettings::CSmsMtmSettings()
    {
    }

// ---------------------------------------------------------------------------
// CSmsMtmSettings::~CSmsMtmSettings()
// Destructor.
// ---------------------------------------------------------------------------
//
CSmsMtmSettings::~CSmsMtmSettings()
    {
    delete iSmsMtm;
    delete iMtmRegistry;
    delete iMsvSession;
    }

// ---------------------------------------------------------------------------
// CSmsMtmSettings::NewL()
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSmsMtmSettings* CSmsMtmSettings::NewL()
    {
    CSmsMtmSettings* self = CSmsMtmSettings::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CSmsMtmSettings::NewLC()
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSmsMtmSettings* CSmsMtmSettings::NewLC()
    {
    CSmsMtmSettings* self = new (ELeave) CSmsMtmSettings();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CSmsMtmSettings::ConstructL()
// Default EPOC constructor.
// ---------------------------------------------------------------------------
//
void CSmsMtmSettings::ConstructL()
    {
    iMsvSession = CMsvSession::OpenSyncL(*this);
    iMsvSession->SetReceiveEntryEvents(EFalse);
    iMtmRegistry = CClientMtmRegistry::NewL(*iMsvSession);
    iSmsMtm = static_cast<CSmsClientMtm*>(iMtmRegistry->NewMtmL(KUidMsgTypeSMS));
    }

// ---------------------------------------------------------------------------
// CSmsMtmSettings::HandleSessionEventL(TMsvSessionEvent aEvent, 
//                                      TAny* aArg1, 
//                                      TAny* aArg2, 
//                                      TAny* aArg3)
// Called on session events.
// ---------------------------------------------------------------------------
//
void CSmsMtmSettings::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, 
                                          TAny* /*aArg1*/, 
                                          TAny* /*aArg2*/, 
                                          TAny* /*aArg3*/)
    {
    // no actions needed
    }

// ---------------------------------------------------------------------------
// CSmsMtmSettings::GetDefaultScNumberL() const
// Retrieves the default service center number.
// ---------------------------------------------------------------------------
//
TPtrC CSmsMtmSettings::GetDefaultScNumberL() const
    {
    // check if default SC is set
    const CSmsSettings& smsSettings = iSmsMtm->ServiceSettings();
    if (!smsSettings.ServiceCenterCount())
        {
        User::Leave(KErrNotFound);
        }
    
    TInt scDefaultIndex = smsSettings.DefaultServiceCenter();

    return smsSettings.GetServiceCenter(scDefaultIndex).Address();
    }
