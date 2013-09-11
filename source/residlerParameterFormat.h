#ifndef __residlerParameterFormat__
#define __residlerParameterFormat__

#include "pluginterfaces/base/ftypes.h"
#include <vector>

namespace Steinberg {
namespace Vst {
namespace residler {

using namespace std;

//-----------------------------------------------------------------------------
class residlerParameterFormat
{
public:
	enum paramIds {
		kMasterVolume = 0,
		kMasterTune,
		//osc1
		kOsc1Tune,
		kOsc1Waveform,//<----------list type
		kOsc1PW,
		kOsc1RingMod,
		kOsc1Sync,
		kOsc1Filter,
		//env1
		kEnv1Att,
		kEnv1Dec,
		kEnv1Sus,
		kEnv1Rel,
		//osc2
		kOsc2Tune,
		kOsc2Waveform,//<----------list type
		kOsc2PW,
		kOsc2RingMod,
		kOsc2Sync,
		kOsc2Filter,
		//env2
		kEnv2Att,
		kEnv2Dec,
		kEnv2Sus,
		kEnv2Rel,
		//osc3
		kOsc3Tune,
		kOsc3Waveform,//<----------list type
		kOsc3PW,
		kOsc3RingMod,
		kOsc3Sync,
		kOsc3Filter,
		//env3
		kEnv3Att,
		kEnv3Dec,
		kEnv3Sus,
		kEnv3Rel,
		//filter
		kFilterCutOff,
		kFilterRes,
		kFilterHP,
		kFilterBP,
		kFilterLP,
		//lfo1
		kLFO1Type,
		kLFO1Rate,
		kLFO1Depth,
		kLFO1Target, //<----------list type
		//misc
		kGlideRate,
		kGlideBend,
		kGlideType,

		//end of list
		knumParameters
	};

	enum waveformTypes {
		kOscNoise = 0,
		kOscPulse,
		kOscTri,
		kOscSaw,
		kOscNoisePulse,
		kOscNoisePulseTri,
		kOscNoisePulseTriSaw,
		kOscNoisePulseSaw,
		kOscNoiseTri,
		kOscNoiseTriSaw,
		kOscNoiseSaw,
		kOscPulseTri,
		kOscPulseSaw,
		kOscPulseTriSaw,
		kOscTriSaw,
		kOscTriPulse,

		//end of list
		knumOscTypes
	};

	enum LFOTargets {
		kLFOMasterVolume = 0,
		kLFOOsc1Tune,
		kLFOOsc2Tune,
		kLFOOsc3Tune,
		kLFOOsc1PW,
		kLFOOsc2PW,
		kLFOOsc3PW,
		kLFOEnv1Att,
		kLFOEnv1Dec,
		kLFOEnv1Sus,
		kLFOEnv1Rel,
		kLFOEnv2Att,
		kLFOEnv2Dec,
		kLFOEnv2Sus,
		kLFOEnv2Rel,
		kLFOEnv3Att,
		kLFOEnv3Dec,
		kLFOEnv3Sus,
		kLFOEnv3Rel,
		kLFOFilterCutoff,
		kLFOFilterRes,

		//end of list
		knumLFOTargets
	};

	enum GlideTypes {
		kPoly = 0,
		kPolyLegato,
		kPolyGlide,
		kMono,
		kMonoLegato,
		kMonoGlide,

		//end of list
		knumGlideTypes
	};

	//todo: get rid of these hard coded presets, file based system needed
	enum factoryPresets {
		knumPresets = 10//get rid of these
	};
	static float programParams[knumPresets][knumParameters];

//-----------------------------------------------------------------------------
	residlerParameterFormat ();

//-----------------------------------------------------------------------------
public:
	const vector<string>* paramNameList() const		{ return &paramList; }
	const vector<string>* LFOTargetNameList() const	{ return &LFOTargetList; }
	const vector<string>* glideNameList() const		{ return &glideList; }
	const vector<string>* waveformNameList() const	{ return &waveformList; }

//-----------------------------------------------------------------------------
private:
	vector<string> paramList;
	vector<string> LFOTargetList;
	vector<string> glideList;
	vector<string> waveformList;
};

}}} // namespaces
		
#endif
