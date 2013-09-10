#include "residlerController.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
FUID residlerController::uid (0x91E8798D, 0xEDE644C9, 0xB9EB444B, 0x5F0A8AA7);

//-----------------------------------------------------------------------------
residlerController::residlerController ()
{
	addBypassParameter = false;
}

//-----------------------------------------------------------------------------
residlerController::~residlerController ()
{
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerController::initialize (FUnknown* context)
{
	tresult res = BaseController::initialize (context);
	if (res == kResultTrue)
	{
		populateParameters();

		//hard coded factory presets. contains bullshit place holder data at the moment
		//todo: replace with file and folder based preset browser
		IndexedParameter* presetParam = new IndexedParameter (USTRING("Factory Presets"), USTRING("%"), residlerParameterFormat::knumPresets-1, 0, ParameterInfo::kIsProgramChange | ParameterInfo::kCanAutomate | ParameterInfo::kIsList, kPresetParam);
		parameters.addParameter (presetParam);
		int32 i = 0;
		presetParam->setIndexString (i++, UString128("default"));
		presetParam->setIndexString (i++, UString128("Preset 2"));
		presetParam->setIndexString (i++, UString128("Preset 3"));
		presetParam->setIndexString (i++, UString128("Preset 4"));
		presetParam->setIndexString (i++, UString128("Preset 5"));
		presetParam->setIndexString (i++, UString128("Preset 6"));
		presetParam->setIndexString (i++, UString128("Preset 7"));
		presetParam->setIndexString (i++, UString128("Preset 8"));
		presetParam->setIndexString (i++, UString128("Preset 9"));
		presetParam->setIndexString (i++, UString128("Preset 10"));

		//todo: midi learn stuff
		midiCCParamID[kCtrlModWheel] = kModWheelParam;
		parameters.addParameter (USTRING("MIDI Mod Wheel"), USTRING(""), 0, 0, 0, kModWheelParam);
		midiCCParamID[kPitchBend] = kPitchBendParam;
		parameters.addParameter (USTRING("MIDI Pitch Bend"), USTRING(""), 0, 0, 0.5, kPitchBendParam);
		midiCCParamID[kCtrlBreath] = kBreathParam;
		parameters.addParameter (USTRING("MIDI Breath"), USTRING(""), 0, 0, 0.5, kBreathParam);
		midiCCParamID[3] = kCtrler3Param;
		parameters.addParameter (USTRING("MIDI Ctrler3"), USTRING(""), 0, 0, 0.5, kCtrler3Param);
		midiCCParamID[kCtrlExpression] = kCtrler3Param;
		parameters.addParameter (USTRING("MIDI Expression"), USTRING(""), 0, 0, 0.5, kExpressionParam);
		midiCCParamID[kAfterTouch] = kAftertouchParam;
		parameters.addParameter (USTRING("MIDI Aftertouch"), USTRING(""), 0, 0, 0.5, kAftertouchParam);
	}
	return res;
}

//-----------------------------------------------------------------------------
void residlerController::populateParameters()
{
	ParamID pid = 0;

	for (pid; pid<residlerParameterFormat::knumParameters; ++pid) {

		const string str = paramFormat.paramName(pid);
		const wstring title (str.begin(), str.end());
			
		if(pid == residlerParameterFormat::kLFO1Target) {
			//fill a parameter list of LFO targets
			IndexedParameter* lfotargetParam = new IndexedParameter (title.c_str(), 0, residlerParameterFormat::knumLFOTargets-1, 0, ParameterInfo::kCanAutomate | ParameterInfo::kIsList, pid);
			for (int lfotarget = 0; lfotarget<residlerParameterFormat::knumLFOTargets; ++lfotarget) {

				const string lfostr = paramFormat.LFOTargetName(lfotarget);
				const wstring lfotitle (lfostr.begin(), lfostr.end());

				lfotargetParam->setIndexString (lfotarget, lfotitle.c_str());
			}
			parameters.addParameter (lfotargetParam);
		}
		else if(pid == residlerParameterFormat::kGlide) {
			//fill list of glide/mono/poly types
			IndexedParameter* glideParam = new IndexedParameter (title.c_str(), 0, residlerParameterFormat::knumGlideTypes-1, 0, ParameterInfo::kCanAutomate | ParameterInfo::kIsList, pid);
			for (int glide = 0; glide<residlerParameterFormat::knumGlideTypes; ++glide) {

				const string glidestr = paramFormat.glideName(glide);
				const wstring glidetitle (glidestr.begin(), glidestr.end());

				glideParam->setIndexString (glide, glidetitle.c_str());
			}
			parameters.addParameter (glideParam);
		}
		else { //fill all common parameters
			parameters.addParameter (title.c_str(), USTRING(""), 0, 0.5, ParameterInfo::kCanAutomate, pid);
		}
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerController::terminate ()
{
	return BaseController::terminate ();
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerController::setParamNormalized (ParamID tag, ParamValue value)
{
	tresult res = BaseController::setParamNormalized (tag, value);
	if (res == kResultOk && tag == kPresetParam) // preset change
	{
		int32 program = parameters.getParameter (tag)->toPlain (value);
		for (int32 i = 0; i < residlerParameterFormat::knumParameters; i++)
		{
			BaseController::setParamNormalized (i, residlerParameterFormat::programParams[program][i]);
		}
		componentHandler->restartComponent (kParamValuesChanged);
	}
	return res;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerController::getParamStringByValue (ParamID tag, ParamValue valueNormalized, String128 string)
{
	return BaseController::getParamStringByValue (tag, valueNormalized, string);
	/*
	UString128 result;
		switch (tag)
		{
			default:
				return BaseController::getParamStringByValue (tag, valueNormalized, string);
		}
		result.copyTo (string, 128);
		return kResultTrue;*/
	
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerController::getParamValueByString (ParamID tag, TChar* string, ParamValue& valueNormalized)
{
	// TODO
	return BaseController::getParamValueByString (tag, string, valueNormalized);
	/*
	switch (tag)
		{
			default:
				return BaseController::getParamValueByString (tag, string, valueNormalized);
		}
		return kResultFalse;*/
	
}

}}} // namespaces
