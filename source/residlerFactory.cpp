#include "public.sdk/source/main/pluginfactoryvst3.h"
#include "residlerController.h"
#include "residlerVersion.h"

//-----------------------------------------------------------------------------
bool InitModule () { return true; }
bool DeinitModule () { return true; }

//-----------------------------------------------------------------------------
using namespace Steinberg::Vst;

BEGIN_FACTORY_DEF (stringCompanyName, 
				   "http://www.foo.fi", 
				   "mailto:foo@bar.fi")

//-----------------------------------------------------------------------------
// -- JX10
DEF_CLASS2 (INLINE_UID_FROM_FUID (residler::residlerProcessor::uid),
			PClassInfo::kManyInstances,
			kVstAudioEffectClass,
			"residler",
			Vst::kDistributable,
			"Instrument",
			FULL_VERSION_STR,
			kVstVersionString,
			residler::residlerProcessor::createInstance)

DEF_CLASS2 (INLINE_UID_FROM_FUID (residler::residlerController::uid),
			PClassInfo::kManyInstances,
			kVstComponentControllerClass,
			"residler",
			0,
			"",
			FULL_VERSION_STR,
			kVstVersionString,
			residler::residlerController::createInstance)
//-----------------------------------------------------------------------------
END_FACTORY
