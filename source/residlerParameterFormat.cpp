#include "residlerParameterFormat.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
residlerParameterFormat::residlerParameterFormat ()
{
	string errorname = "ERRORTYPE";
	for (int i=0; i<knumParameters; ++i) paramList.push_back(errorname);
	for (int i=0; i<knumRoutingSources; ++i) routingSourceList.push_back(errorname);
	for (int i=0; i<knumRoutingDestinations; ++i) routingDestinationList.push_back(errorname);
	for (int i=0; i<knumGlideTypes; ++i) glideList.push_back(errorname);
	for (int i=0; i<knumWaveformTypes; ++i) waveformList.push_back(errorname);
	for (int i=0; i<kOnOffType; ++i) onOffList.push_back(errorname);
	
	for (int i=0; i<knumParameters; ++i) {
		switch (i) {
			case kMasterVolume :		paramList[i] = "MasterVolume";			break;
			case kMasterTune :			paramList[i] = "MasterTune";			break;
			case kMaxPolyphony :		paramList[i] = "MaxPolyphony";			break;

			case kVoice0On :			paramList[i] = "Voice0On";				break;
			case kVoice0Tune :			paramList[i] = "Voice0Tune";			break;
			case kVoice0PW :			paramList[i] = "Voice0PW";				break;
			case kVoice0Waveform :		paramList[i] = "Voice0Waveform";		break;
			case kVoice0RingMod :		paramList[i] = "Voice0RingMod";			break;
			case kVoice0Sync :			paramList[i] = "Voice0Sync";			break;
			case kVoice0FilterOn :		paramList[i] = "Voice0FilterOn";		break;
			case kVoice0EnvAttack :		paramList[i] = "Voice0EnvAttack";		break;
			case kVoice0EnvDecay :		paramList[i] = "Voice0EnvDecay";		break;
			case kVoice0EnvSustain :	paramList[i] = "Voice0EnvSustain";		break;
			case kVoice0EnvRelease :	paramList[i] = "Voice0EnvRelease";		break;

			case kVoice1On :			paramList[i] = "Voice1On";				break;
			case kVoice1Tune :			paramList[i] = "Voice1Tune";			break;
			case kVoice1PW :			paramList[i] = "Voice1PW";				break;
			case kVoice1Waveform :		paramList[i] = "Voice1Waveform";		break;
			case kVoice1RingMod :		paramList[i] = "Voice1RingMod";			break;
			case kVoice1Sync :			paramList[i] = "Voice1Sync";			break;
			case kVoice1FilterOn :		paramList[i] = "Voice1FilterOn";		break;
			case kVoice1EnvAttack :		paramList[i] = "Voice1EnvAttack";		break;
			case kVoice1EnvDecay :		paramList[i] = "Voice1EnvDecay";		break;
			case kVoice1EnvSustain :	paramList[i] = "Voice1EnvSustain";		break;
			case kVoice1EnvRelease :	paramList[i] = "Voice1EnvRelease";		break;

			case kVoice2On :			paramList[i] = "Voice2On";				break;
			case kVoice2Tune :			paramList[i] = "Voice2Tune";			break;
			case kVoice2PW :			paramList[i] = "Voice2PW";				break;
			case kVoice2Waveform :		paramList[i] = "Voice2Waveform";		break;
			case kVoice2RingMod :		paramList[i] = "Voice2RingMod";			break;
			case kVoice2Sync :			paramList[i] = "Voice2Sync";			break;
			case kVoice2FilterOn :		paramList[i] = "Voice2FilterOn";		break;
			case kVoice2EnvAttack :		paramList[i] = "Voice2EnvAttack";		break;
			case kVoice2EnvDecay :		paramList[i] = "Voice2EnvDecay";		break;
			case kVoice2EnvSustain :	paramList[i] = "Voice2EnvSustain";		break;
			case kVoice2EnvRelease :	paramList[i] = "Voice2EnvRelease";		break;

			case kFilterCutoff :		paramList[i] = "FilterCutoff";			break;
			case kFilterResonance :		paramList[i] = "FilterResonance";		break;
			case kFilterHP :			paramList[i] = "FilterHP";				break;
			case kFilterBP :			paramList[i] = "FilterBP";				break;
			case kFilterLP :			paramList[i] = "FilterLP";				break;

			case kLFO1Type :			paramList[i] = "LFO1Type";				break;
			case kLFO1Rate :			paramList[i] = "LFO1Rate";				break;
			case kLFO1Depth :			paramList[i] = "LFO1Depth";				break;

			case kRoutingSource0 :		paramList[i] = "RoutingSource0";		break;
			case kRoutingSource1 :		paramList[i] = "RoutingSource1";		break;
			case kRoutingSource2 :		paramList[i] = "RoutingSource2";		break;
			case kRoutingSource3 :		paramList[i] = "RoutingSource3";		break;

			case kRoutingDestination0 :	paramList[i] = "RoutingDestination0";	break;
			case kRoutingDestination1 :	paramList[i] = "RoutingDestination1";	break;
			case kRoutingDestination2 :	paramList[i] = "RoutingDestination2";	break;
			case kRoutingDestination3 :	paramList[i] = "RoutingDestination3";	break;

			case kGlideRate :			paramList[i] = "GlideRate";				break;
			case kGlideBend :			paramList[i] = "GlideBend";				break;
			case kGlideType :			paramList[i] = "Glide";					break;
				default: break;	}}

	for (int i=0; i<knumWaveformTypes; ++i) {
		switch (i) {
			case kWaveformNoise :			waveformList[i] = "Noise";				break;
			case kWaveformPulse :			waveformList[i] = "Pulse";				break;
			case kWaveformTriangle :		waveformList[i] = "Triangle";			break;
			case kWaveformSaw :				waveformList[i] = "Saw";				break;
			case kWaveformPulseTriangle :	waveformList[i] = "PulseTriangle";		break;
			case kWaveformPulseSaw :		waveformList[i] = "PulseSaw";			break;
			case kWaveformPulseTriangleSaw :waveformList[i] = "PulseTriangleSaw";	break;
			case kWaveformTriangleSaw :		waveformList[i] = "TriangleSaw";		break;
				default: break;	}}

	for (int i=0; i<knumRoutingSources; ++i) {
		switch (i) {
			case kSourceVelocity :			routingSourceList[i] = "Velocity";	break;
			case kSourceNotePitch :			routingSourceList[i] = "NotePitch";	break;
			case kSourceSIDVoice3 :			routingSourceList[i] = "SIDVoice3";	break;
			case kSourceSIDEnv3 :			routingSourceList[i] = "SIDEnv3";	break;
			case kSourceLFO1 :				routingSourceList[i] = "LFO1";		break;
				default: break;	}}

	for (int i=0; i<knumRoutingDestinations; ++i) {
		switch (i) {
			case kDestMasterVolume :	routingDestinationList[i] = "MasterVolume";	break;
			case kDestMasterTune :		routingDestinationList[i] = "MasterTune";	break;
			case kDestVoice0Tune :		routingDestinationList[i] = "Voice0Tune";	break;
			case kDestVoice1Tune :		routingDestinationList[i] = "Voice1Tune";	break;
			case kDestVoice2Tune :		routingDestinationList[i] = "Voice2Tune";	break;
			case kDestVoice0PW :		routingDestinationList[i] = "Voice0PW";		break;
			case kDestVoice1PW :		routingDestinationList[i] = "Voice1PW";		break;
			case kDestVoice2PW :		routingDestinationList[i] = "Voice2PW";		break;
			case kDestEnv0Att :			routingDestinationList[i] = "Env0Att";		break;
			case kDestEnv0Dec :			routingDestinationList[i] = "Env0Dec";		break;
			case kDestEnv0Sus :			routingDestinationList[i] = "Env0Sus";		break;
			case kDestEnv0Rel :			routingDestinationList[i] = "Env0Rel";		break;
			case kDestEnv1Att :			routingDestinationList[i] = "Env1Att";		break;
			case kDestEnv1Dec :			routingDestinationList[i] = "Env1Dec";		break;
			case kDestEnv1Sus :			routingDestinationList[i] = "Env1Sus";		break;
			case kDestEnv1Rel :			routingDestinationList[i] = "Env1Rel";		break;
			case kDestEnv2Att :			routingDestinationList[i] = "Env2Att";		break;
			case kDestEnv2Dec :			routingDestinationList[i] = "Env2Dec";		break;
			case kDestEnv2Sus :			routingDestinationList[i] = "Env2Sus";		break;
			case kDestEnv2Rel :			routingDestinationList[i] = "Env2Rel";		break;
			case kDestFilterCutoff :	routingDestinationList[i] = "FilterCutoff";	break;
			case kDestFilterRes :		routingDestinationList[i] = "FilterRes";	break;
			case kDestLFO1Rate :		routingDestinationList[i] = "LFO1Rate";		break;
			case kDestLFO1Depth :		routingDestinationList[i] = "LFO1Depth";	break;
				default: break;	}}

	for (int i=0; i<knumGlideTypes; ++i) {
		switch (i) {
			case kPoly :				glideList[i] = "Poly";				break;
			case kPolyLegato :			glideList[i] = "PolyLegato";		break;
			case kPolyGlide :			glideList[i] = "PolyGlide";			break;
			case kMono :				glideList[i] = "Mono";				break;
			case kMonoLegato :			glideList[i] = "MonoLegato";		break;
			case kMonoGlide :			glideList[i] = "MonoGlide";			break;
				default: break;	}}

	for (int i=0; i<kOnOffType; ++i) {
		switch (i) {
			case kOff :			onOffList[i] = "Off";	break;
			case kOn :			onOffList[i] = "On";	break;
				default: break;	}}
}

//garbage "preset" values for testing, change if you like. will be removed in the future.
//-----------------------------------------------------------------------------
float residlerParameterFormat::programParams[knumPresets][knumParameters] = { 
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	{0.88f, 0.51f, 0.5f, 0.0f, 0.49f, 0.5f, 0.46f, 0.76f, 0.69f, 0.1f, 0.69f, 1.0f, 0.86f, 0.76f, 0.57f, 0.3f, 0.8f, 0.68f, 0.66f, 0.79f, 0.13f, 0.25f, 0.45f, 0.5f},
	{0.88f, 0.51f, 0.5f, 0.16f, 0.49f, 0.5f, 0.49f, 0.82f, 0.66f, 0.08f, 0.89f, 0.85f, 0.69f, 0.76f, 0.47f, 0.12f, 0.22f, 0.55f, 0.66f, 0.89f, 0.34f, 0.0f, 1.0f, 0.5f},
	{1.0f, 0.26f, 0.14f, 0.0f, 0.35f, 0.5f, 0.3f, 0.25f, 0.7f, 0.0f, 0.63f, 0.0f, 0.35f, 0.0f, 0.25f, 0.0f, 0.5f, 1.0f, 0.3f, 0.81f, 0.5f, 0.5f, 0.5f, 0.5f},
	{0.41f, 0.5f, 0.79f, 0.0f, 0.08f, 0.32f, 0.49f, 0.01f, 0.34f, 0.0f, 0.93f, 0.61f, 0.87f, 1.0f, 0.93f, 0.11f, 0.48f, 0.98f, 0.32f, 0.81f, 0.5f, 0.0f, 0.5f, 0.5f},
	{0.29f, 0.76f, 0.26f, 0.0f, 0.18f, 0.76f, 0.35f, 0.15f, 0.77f, 0.14f, 0.54f, 0.0f, 0.42f, 0.13f, 0.21f, 0.0f, 0.56f, 0.0f, 0.32f, 0.2f, 0.58f, 0.22f, 0.53f, 0.5f},
	{1.0f, 0.65f, 0.24f, 0.4f, 0.34f, 0.85f, 0.65f, 0.63f, 0.75f, 0.16f, 0.5f, 0.0f, 0.3f, 0.0f, 0.25f, 0.17f, 0.5f, 1.0f, 0.03f, 0.81f, 0.5f, 0.0f, 0.68f, 0.5f},
	{0.0f, 0.25f, 0.5f, 1.0f, 0.46f, 0.5f, 0.51f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.25f, 0.37f, 0.5f, 1.0f, 0.38f, 0.81f, 0.62f, 0.0f, 0.5f, 0.5f},
	{0.84f, 0.51f, 0.15f, 0.45f, 0.41f, 0.42f, 0.54f, 0.01f, 0.58f, 0.21f, 0.67f, 0.0f, 0.09f, 1.0f, 0.25f, 0.2f, 0.85f, 1.0f, 0.3f, 0.83f, 0.09f, 0.4f, 0.49f, 0.5f},
	{0.71f, 0.75f, 0.53f, 0.18f, 0.24f, 1.0f, 0.56f, 0.52f, 0.69f, 0.19f, 0.7f, 1.0f, 0.14f, 0.65f, 0.95f, 0.07f, 0.91f, 1.0f, 0.15f, 0.84f, 0.33f, 0.0f, 0.49f, 0.5f}
};

}}} // namespace

//EOF
