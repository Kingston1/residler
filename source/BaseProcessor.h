#ifndef __BaseProcessor__
#define __BaseProcessor__

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
class BaseProcessor : public AudioEffect
{
protected:
	BaseProcessor ();
	~BaseProcessor ();

	virtual void doProcessing (ProcessData& data) = 0;
	virtual bool bypassProcessing (ProcessData& data);
	virtual void checkSilence (ProcessData& data);
	virtual void setBypass (bool state, int32 sampleOffset);
	virtual void setParameter (ParamID index, ParamValue newValue, int32 sampleOffset);
	virtual void allocParameters (int32 numParams);
	virtual void recalculate () {}
	
	bool isBypassed () const { return bypassState; }
	double getSampleRate () const { return processSetup.sampleRate; }

	tresult PLUGIN_API process (ProcessData& data);

	tresult PLUGIN_API setupProcessing (ProcessSetup& setup);
	tresult PLUGIN_API setActive (TBool state);
	tresult PLUGIN_API setBusArrangements (	SpeakerArrangement* inputs, 
											int32 numIns,
											SpeakerArrangement* outputs, 
											int32 numOuts);

	tresult PLUGIN_API setState (IBStream* state);
	tresult PLUGIN_API getState (IBStream* state);

	ParamValue* params;
	uint32 numParams;
	int32 bypassRamp;
	
	float* bypassBuffer0;
	float* bypassBuffer1;

	bool bypassState;
};

}}} // namespace

#endif
