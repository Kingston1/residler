#include "residlerParameterFormat.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
residlerParameterFormat::residlerParameterFormat ()
{
	string errorname = "ERRORTYPE";
	for (int i=0; i<knumParameters; ++i) paramList.push_back(errorname);
	for (int i=0; i<knumLFOTargets; ++i) LFOTargetList.push_back(errorname);
	for (int i=0; i<knumGlideTypes; ++i) glideList.push_back(errorname);
	for (int i=0; i<knumOscTypes; ++i) waveformList.push_back(errorname);
	
	for (int i=0; i<knumParameters; ++i) {
		switch (i) {
			case kMasterVolume :		paramList[i] = "MasterVolume";		break;
			case kMasterTune :			paramList[i] = "MasterTune";		break;
			//osc1
			case kOsc1Tune :			paramList[i] = "Osc1Tune";			break;
			case kOsc1Waveform :		paramList[i] = "Osc1Waveform";		break;
			case kOsc1PW :				paramList[i] = "Osc1PW";			break;
			case kOsc1RingMod :			paramList[i] = "Osc1RingMod";		break;
			case kOsc1Sync :			paramList[i] = "Osc1Sync";			break;
			case kOsc1Filter :			paramList[i] = "Osc1Filter";		break;
			//env1
			case kEnv1Att :				paramList[i] = "Env1Att";			break;
			case kEnv1Dec :				paramList[i] = "Env1Dec";			break;
			case kEnv1Sus :				paramList[i] = "Env1Sus";			break;
			case kEnv1Rel :				paramList[i] = "Env1Rel";			break;
			//osc2
			case kOsc2Tune :			paramList[i] = "Osc2Tune";			break;
			case kOsc2Waveform :		paramList[i] = "Osc2Waveform";		break;
			case kOsc2PW :				paramList[i] = "Osc2PW";			break;
			case kOsc2RingMod :			paramList[i] = "Osc2RingMod";		break;
			case kOsc2Sync :			paramList[i] = "Osc2Sync";			break;
			case kOsc2Filter :			paramList[i] = "Osc2Filter";		break;
			//env2
			case kEnv2Att :				paramList[i] = "Env2Att";			break;
			case kEnv2Dec :				paramList[i] = "Env2Dec";			break;
			case kEnv2Sus :				paramList[i] = "Env2Sus";			break;
			case kEnv2Rel :				paramList[i] = "Env2Rel";			break;
			//osc3
			case kOsc3Tune :			paramList[i] = "Osc3Tune";			break;
			case kOsc3Waveform :		paramList[i] = "Osc3Waveform";		break;
			case kOsc3PW :				paramList[i] = "Osc3PW";			break;
			case kOsc3RingMod :			paramList[i] = "Osc3RingMod";		break;
			case kOsc3Sync :			paramList[i] = "Osc3Sync";			break;
			case kOsc3Filter :			paramList[i] = "Osc3Filter";		break;
			//env3
			case kEnv3Att :				paramList[i] = "Env3Att";			break;
			case kEnv3Dec :				paramList[i] = "Env3Dec";			break;
			case kEnv3Sus :				paramList[i] = "Env3Sus";			break;
			case kEnv3Rel :				paramList[i] = "Env3Rel";			break;
			//filter
			case kFilterCutOff :		paramList[i] = "FilterCutOff";		break;
			case kFilterRes :			paramList[i] = "FilterRes";			break;
			case kFilterHP :			paramList[i] = "kFilterHP";			break;
			case kFilterBP :			paramList[i] = "kFilterBP";			break;
			case kFilterLP :			paramList[i] = "kFilterLP";			break;
			//lfo1
			case kLFO1Type :			paramList[i] = "LFO1Type";			break;
			case kLFO1Rate :			paramList[i] = "LFO1Rate";			break;
			case kLFO1Depth :			paramList[i] = "LFO1Depth";			break;
			case kLFO1Target :			paramList[i] = "LFO1Target";		break;
			//misc
			case kGlideRate :			paramList[i] = "GlideRate";			break;
			case kGlideBend :			paramList[i] = "GlideBend";			break;
			case kGlideType :			paramList[i] = "Glide";				break;
		}	
	}

	for (int i=0; i<knumOscTypes; ++i) {
		switch (i) {
			case kOscNoise :			waveformList[i] = "OscNoise";			break;
			case kOscPulse :			waveformList[i] = "OscPulse";			break;
			case kOscTri :				waveformList[i] = "OscTri";				break;
			case kOscSaw :				waveformList[i] = "OscSaw";				break;
			case kOscNoisePulse :		waveformList[i] = "OscNoisePulse";		break;
			case kOscNoisePulseTri :	waveformList[i] = "OscNoisePulseTri";	break;
			case kOscNoisePulseTriSaw :	waveformList[i] = "OscNoisePulseTriSaw";break;
			case kOscNoisePulseSaw :	waveformList[i] = "OscNoisePulseSaw";	break;
			case kOscNoiseTri :			waveformList[i] = "OscNoiseTri";		break;
			case kOscNoiseTriSaw :		waveformList[i] = "OscNoiseTriSaw";		break;
			case kOscNoiseSaw :			waveformList[i] = "OscNoiseSaw";		break;
			case kOscPulseTri :			waveformList[i] = "OscPulseTri";		break;
			case kOscPulseSaw :			waveformList[i] = "OscPulseSaw";		break;
			case kOscPulseTriSaw :		waveformList[i] = "OscPulseTriSaw";		break;
			case kOscTriSaw :			waveformList[i] = "OscTriSaw";			break;
			case kOscTriPulse :			waveformList[i] = "OscTriPulse";		break;
		}
	}

	for (int i=0; i<knumLFOTargets; ++i) {
		switch (i) {
			case kLFOMasterVolume :		LFOTargetList[i] = "LFOMasterVolume";	break;
			case kLFOOsc1Tune :			LFOTargetList[i] = "LFOOsc1Tune";		break;
			case kLFOOsc2Tune :			LFOTargetList[i] = "LFOOsc2Tune";		break;
			case kLFOOsc3Tune :			LFOTargetList[i] = "LFOOsc3Tune";		break;
			case kLFOOsc1PW :			LFOTargetList[i] = "LFOOsc1PW";			break;
			case kLFOOsc2PW :			LFOTargetList[i] = "LFOOsc2PW";			break;
			case kLFOOsc3PW :			LFOTargetList[i] = "LFOOsc3PW";			break;
			case kLFOEnv1Att :			LFOTargetList[i] = "LFOEnv1Att";		break;
			case kLFOEnv1Dec :			LFOTargetList[i] = "LFOEnv1Dec";		break;
			case kLFOEnv1Sus :			LFOTargetList[i] = "LFOEnv1Sus";		break;
			case kLFOEnv1Rel :			LFOTargetList[i] = "LFOEnv1Rel";		break;
			case kLFOEnv2Att :			LFOTargetList[i] = "LFOEnv2Att";		break;
			case kLFOEnv2Dec :			LFOTargetList[i] = "LFOEnv2Dec";		break;
			case kLFOEnv2Sus :			LFOTargetList[i] = "LFOEnv2Sus";		break;
			case kLFOEnv2Rel :			LFOTargetList[i] = "LFOEnv2Rel";		break;
			case kLFOEnv3Att :			LFOTargetList[i] = "LFOEnv3Att";		break;
			case kLFOEnv3Dec :			LFOTargetList[i] = "LFOEnv3Dec";		break;
			case kLFOEnv3Sus :			LFOTargetList[i] = "LFOEnv3Sus";		break;
			case kLFOEnv3Rel :			LFOTargetList[i] = "LFOEnv3Rel";		break;
			case kLFOFilterCutoff :		LFOTargetList[i] = "LFOFilterCutoff";	break;
			case kLFOFilterRes :		LFOTargetList[i] = "LFOFilterRes";		break;
		}
	}

	for (int i=0; i<knumGlideTypes; ++i) {
		switch (i) {
			case kPoly :				glideList[i] = "Poly";				break;
			case kPolyLegato :			glideList[i] = "PolyLegato";		break;
			case kPolyGlide :			glideList[i] = "PolyGlide";			break;
			case kMono :				glideList[i] = "Mono";				break;
			case kMonoLegato :			glideList[i] = "MonoLegato";		break;
			case kMonoGlide :			glideList[i] = "MonoGlide";			break;
		}
	}
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
