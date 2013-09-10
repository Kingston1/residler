#include "public.sdk/source/vst/vst2wrapper/vst2wrapper.h"
#include "residlerController.h"

using namespace Steinberg::Vst::residler;

//-----------------------------------------------------------------------------
::AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return Steinberg::Vst::Vst2Wrapper::create (GetPluginFactory (),
												residlerProcessor::uid,
												'r51D',
												audioMaster);
}

//EOF
