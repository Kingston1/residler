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
		//float (double) parameter types
		kMasterVolume = 0,
		kMasterTune,
		kOsc1Tune,
		kOsc2Tune,
		kOsc3Tune,
		kOsc1Noise,
		kOsc1Pulse,
		kOsc1Triangle,
		kOsc1Saw,
		//kOsc1TEST,
		kOsc1RingMod,
		kOsc1Sync,
		kOsc2Noise,
		kOsc2Pulse,
		kOsc2Triangle,
		kOsc2Saw,
		//kOsc2TEST,
		kOsc2RingMod,
		kOsc2Sync,
		kOsc3Noise,
		kOsc3Pulse,
		kOsc3Triangle,
		kOsc3Saw,
		//kOsc3TEST,
		kOsc3RingMod,
		kOsc3Sync,
		kOsc1PW,
		kOsc2PW,
		kOsc3PW,
		kEnv1Att,
		kEnv1Dec,
		kEnv1Sus,
		kEnv1Rel,
		kEnv2Att,
		kEnv2Dec,
		kEnv2Sus,
		kEnv2Rel,
		kEnv3Att,
		kEnv3Dec,
		kEnv3Sus,
		kEnv3Rel,
		kFilterCutOff,
		kFilterRes,
		kFilterOff,
		kFilterHP,
		kFilterBP,
		kFilterLP,
		kOsc1Filter,
		kOsc2Filter,
		kOsc3Filter,
		kLFO1Type,
		kLFO1Rate,
		kLFO1Depth,
		kGlideRate,
		kGlideBend,

		//list parameter types
		kGlide,
		kLFO1Target,

		//end of parameter list
		knumParameters
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

		//end of LFOTargets list
		knumLFOTargets
	};

	enum GlideTypes {
		kPoly = 0,
		kPolyLegato,
		kPolyGlide,
		kMono,
		kMonoLegato,
		kMonoGlide,

		//end of GlideTypes list
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
	const string paramName(int) const;
	const string LFOTargetName(int) const;
	const string glideName(int) const;

//-----------------------------------------------------------------------------
private:
	vector<string> paramList;
	vector<string> LFOTargetList;
	vector<string> glideList;
};

}}} // namespaces
		
#endif
