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
		kMaxPolyphony,

		kVoice0On, //kOnOffType
		kVoice0Tune,
		kVoice0PW,
		kVoice0Waveform, //WaveformTypes list
		kVoice0RingMod, //kOnOffType
		kVoice0Sync, //kOnOffType
		kVoice0FilterOn, //kOnOffType
		kVoice0EnvAttack,
		kVoice0EnvDecay,
		kVoice0EnvSustain,
		kVoice0EnvRelease,
		
		kVoice1On, //kOnOffType
		kVoice1Tune,
		kVoice1PW,
		kVoice1Waveform, //WaveformTypes list
		kVoice1RingMod, //kOnOffType
		kVoice1Sync, //kOnOffType
		kVoice1FilterOn, //kOnOffType
		kVoice1EnvAttack,
		kVoice1EnvDecay,
		kVoice1EnvSustain,
		kVoice1EnvRelease,

		kVoice2On, //kOnOffType
		kVoice2Tune,
		kVoice2PW,
		kVoice2Waveform, //waveformTypes list
		kVoice2RingMod, //kOnOffType
		kVoice2Sync, //kOnOffType
		kVoice2FilterOn, //kOnOffType
		kVoice2EnvAttack,
		kVoice2EnvDecay,
		kVoice2EnvSustain,
		kVoice2EnvRelease,

		kFilterCutoff,
		kFilterResonance,
		kFilterHP, //kOnOffType
		kFilterBP, //kOnOffType
		kFilterLP, //kOnOffType

		kLFO1Type,
		kLFO1Rate,
		kLFO1Depth,

		kRoutingSource0, //routingSourceList
		kRoutingSource1, //routingSourceList
		kRoutingSource2, //routingSourceList
		kRoutingSource3, //routingSourceList

		kRoutingDestination0, //routingDestinationList
		kRoutingDestination1, //routingDestinationList
		kRoutingDestination2, //routingDestinationList
		kRoutingDestination3, //routingDestinationList

		kGlideRate,
		kGlideBend,
		kGlideType, //GlideTypes list

		//end of list
		knumParameters
	};

	enum WaveformTypes {
		kWaveformNoise = 0,
		kWaveformPulse,
		kWaveformTriangle,
		kWaveformSaw,
		kWaveformPulseTriangle,
		kWaveformPulseSaw,
		kWaveformPulseTriangleSaw,
		kWaveformTriangleSaw,

		//end of list
		knumWaveformTypes
	};

	enum kRoutingSources {
		kSourceVelocity,
		kSourceNotePitch,
		kSourceSIDVoice3,
		kSourceSIDEnv3,
		kSourceLFO1,

		//end of list
		knumRoutingSources
	};

	enum kRoutingDestinations {
		kDestMasterVolume,
		kDestMasterTune,
		kDestVoice0Tune,
		kDestVoice1Tune,
		kDestVoice2Tune,
		kDestVoice0PW,
		kDestVoice1PW,
		kDestVoice2PW,
		kDestEnv0Att,
		kDestEnv0Dec,
		kDestEnv0Sus,
		kDestEnv0Rel,
		kDestEnv1Att,
		kDestEnv1Dec,
		kDestEnv1Sus,
		kDestEnv1Rel,
		kDestEnv2Att,
		kDestEnv2Dec,
		kDestEnv2Sus,
		kDestEnv2Rel,
		kDestFilterCutoff,
		kDestFilterRes,
		kDestLFO1Rate,
		kDestLFO1Depth,

		//end of list
		knumRoutingDestinations
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

	enum OnOffType {
		kOff = 0,
		kOn,
		//end of list
		kOnOffType
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
	const vector<string>* routingSourceNameList() const	{ return &routingSourceList; }
	const vector<string>* routingDestinationNameList() const	{ return &routingDestinationList; }
	const vector<string>* glideNameList() const		{ return &glideList; }
	const vector<string>* waveformNameList() const	{ return &waveformList; }
	const vector<string>* onOffNameList() const	{ return &onOffList; }

//-----------------------------------------------------------------------------
private:
	//void setParam(paramIds SIDparam, unsigned char value) {}
	vector<string> paramList;
	vector<string> routingSourceList;
	vector<string> routingDestinationList;
	vector<string> glideList;
	vector<string> waveformList;
	vector<string> onOffList;
};

}}} // namespaces
		
#endif
