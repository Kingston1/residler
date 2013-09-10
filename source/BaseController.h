#ifndef __BaseController__
#define __BaseController__

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vstparameters.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/base/ustring.h"
#include "BaseParameter.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
class BaseController : public EditControllerEx1, public IMidiMapping
{
public:
	BaseController ();

	tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API setComponentState (IBStream* state);
	tresult PLUGIN_API notify (IMessage* message);

	int32 PLUGIN_API getProgramListCount ();
	tresult PLUGIN_API getProgramListInfo (int32 listIndex, ProgramListInfo& info /*out*/);
	tresult PLUGIN_API getProgramName (ProgramListID listId, int32 programIndex, String128 name /*out*/);

	tresult PLUGIN_API getMidiControllerAssignment (int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& tag/*out*/);

	ParameterContainer& getParameters () { return parameters; }

	//-----------------------------
	DELEGATE_REFCOUNT (EditControllerEx1)
	tresult PLUGIN_API queryInterface (const char* iid, void** obj);
	//-----------------------------

	enum {
		kBypassParam = 'bpas',
		kPresetParam = 'prst',
		kModWheelParam = 'modw',
		kBreathParam = 'brth',
		kCtrler3Param = 'ct03',
		kExpressionParam = 'expr',
		kPitchBendParam = 'pitb',
		kSustainParam = 'sust',
		kAftertouchParam = 'aftt',
	};

protected:
	double getSampleRate () const { return sampleRate; }
	int32 midiCCParamID[kCountCtrlNumber];
	double sampleRate;
	bool addBypassParameter;
};

}}} // namespaces

#endif
