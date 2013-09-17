#include "residlerProcessor.h"
#include "residlerController.h"

#include "vstlogger.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
int scaledFrequency(int16 note, double pitchMult, double clockRate)
{
	double scaler = 0x1000000 / clockRate;
	double pitch = note - 57 + (pitchMult * 24 - 12);
	double freq = 440.f * pow(2.0, pitch/12.0);
	return (int)(scaler * freq);
}

//-----------------------------------------------------------------------------
FUID residlerProcessor::uid (0x82CD49DE, 0x13D743BA, 0xABDAC299, 0x1CE06F7C);

//-----------------------------------------------------------------------------
residlerProcessor::residlerProcessor ()
{
	setControllerClass (residlerController::uid);
	allocParameters (residlerParameterFormat::knumParameters);
	resid = NULL;

	vstlogger::log("INITIALIZED processor\n");
}

//-----------------------------------------------------------------------------
residlerProcessor::~residlerProcessor ()
{
	vstlogger::log("goodbye processor.\n");
	if (resid)
		delete resid;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerProcessor::initialize (FUnknown* context)
{
	tresult res = BaseProcessor::initialize (context);
	if (res == kResultTrue)
	{
		addEventInput (USTRING("MIDI in"), 1);
		addAudioOutput (USTRING("Stereo Out"), SpeakerArr::kStereo);

		const float* newParams = residlerParameterFormat::programParams[0];
		if (newParams)
		{
			for (int32 i = 0; i < residlerParameterFormat::knumParameters; i++)
				params[i] = newParams[i];
		}

		resid = NULL;

		//it's ok to do big allocs etc. big initialisations here, also samplerate changes
		recalculate ();
	}
	return res;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerProcessor::terminate ()
{

	return BaseProcessor::terminate ();
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API residlerProcessor::setActive (TBool state)
{
	return BaseProcessor::setActive (state);
}

//-----------------------------------------------------------------------------
void residlerProcessor::doProcessing (ProcessData& data)
{
	parameter_test (data.inputParameterChanges);
	
	event_test(data.inputEvents);

	int32 sampleFrames = data.numSamples;
	
	float* out1 = data.outputs[0].channelBuffers32[0];
	float* out2 = data.outputs[0].channelBuffers32[1];

	//see SDK note_expression_synth_processor.cpp process (ProcessData& data)
	//good ideas on how to use data.inputParameterChanges and IParamValueQueue there

	int i;
	for (i = 0; i < sampleFrames; i++)
	{
//		out1[i] = ((float)i / (float)sampleFrames) - 0.5f;
//		out2[i] = (((float)i / (float)sampleFrames) - 0.5f) * 0.1f;
		out2[i] = 0.0f;
	}

	gen(0, sampleFrames, out1);

	for (i = 0; i < sampleFrames; i++)
	{
		out2[i] = out1[i];
	}
}

//-----------------------------------------------------------------------------
bool residlerProcessor::parameter_test (IParameterChanges* changes)
{
	if (changes)
	{
		int32 count = changes->getParameterCount ();
		if (count > 0)
		{
			for (int32 i = 0; i < count; i++)
			{
				IParamValueQueue* queue = changes->getParameterData (i);
				if (!queue)
					continue;
				ParamID paramId = queue->getParameterId ();
				int32 pointCount = queue->getPointCount ();
				int32 sampleOffset;
				ParamValue value;
				queue->getPoint (pointCount - 1, sampleOffset, value);
				parameterScaling((residlerParameterFormat::paramIds)paramId, value);
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
void residlerProcessor::parameterScaling(residlerParameterFormat::paramIds param, double value)
{
	switch (param) {
//------voice0
		case residlerParameterFormat::kVoice0On: voiceState[0].voiceOn = (bool)value; break;
		case residlerParameterFormat::kVoice0Tune: {
			voiceState[0].pitchMult = value;
			int freq = scaledFrequency(voiceState[0].currentNote, value, sidInfo.clockRate);
			resid->write(0x00, (freq >> 0) & 0xFF);
			resid->write(0x01, (freq >> 8) & 0xFF); } break;
		case residlerParameterFormat::kVoice0PW: {
			//12-bit value, bits 4-7 if PW_HI not used
			int pw = (value * 0xFFF);
			resid->write(0x02, (pw >> 0) & 0xFF);
			resid->write(0x03, (pw >> 8) & 0xFF);  } break;
		//voice0 ControlReg
		case residlerParameterFormat::kVoice0Waveform: waveformSelector(0, value); break;
		case residlerParameterFormat::kVoice0RingMod: voiceState[0].ringMod = (bool)value; break;
		case residlerParameterFormat::kVoice0Sync: voiceState[0].sync = (bool)value; break;
		//voice0 env
		case residlerParameterFormat::kVoice0EnvAttack: {
			voiceState[0].attack = (unsigned char)(value * 0xF) << 4;
			resid->write(0x05, voiceState[0].attack | voiceState[0].decay ); } break;
		case residlerParameterFormat::kVoice0EnvDecay: {
			voiceState[0].decay = (unsigned char)(value * 0xF);
			resid->write(0x05, voiceState[0].attack | voiceState[0].decay ); } break;
		case residlerParameterFormat::kVoice0EnvSustain: {
			voiceState[0].sustain = (unsigned char)(value * 0xF) << 4;
			resid->write(0x06, voiceState[0].sustain | voiceState[0].release ); } break;
		case residlerParameterFormat::kVoice0EnvRelease: {
			voiceState[0].release = (unsigned char)(value * 0xF);
			resid->write(0x06, voiceState[0].sustain | voiceState[0].release ); } break;
//------voice1
		case residlerParameterFormat::kVoice1On: voiceState[1].voiceOn = (bool)value; break;
		case residlerParameterFormat::kVoice1Tune: {
			voiceState[1].pitchMult = value;
			int freq = scaledFrequency(voiceState[1].currentNote, value, sidInfo.clockRate);
			resid->write(0x07, (freq >> 0) & 0xFF);
			resid->write(0x08, (freq >> 8) & 0xFF); } break;
		case residlerParameterFormat::kVoice1PW: {
			//12-bit value, bits 4-7 if PW_HI not used
			int pw = (value * 0xFFF);
			resid->write(0x09, (pw >> 0) & 0xFF);
			resid->write(0x0A, (pw >> 8) & 0xFF);  } break;
		//voice1 ControlReg
		case residlerParameterFormat::kVoice1Waveform: waveformSelector(1, value); break;
		case residlerParameterFormat::kVoice1RingMod: voiceState[1].ringMod = (bool)value; break;
		case residlerParameterFormat::kVoice1Sync: voiceState[1].sync = (bool)value; break;
		//voice1 env
		case residlerParameterFormat::kVoice1EnvAttack: {
			voiceState[1].attack = (unsigned char)(value * 0xF) << 4;
			resid->write(0x0C, voiceState[1].attack | voiceState[1].decay ); } break;
		case residlerParameterFormat::kVoice1EnvDecay: {
			voiceState[1].decay = (unsigned char)(value * 0xF);
			resid->write(0x0C, voiceState[1].attack | voiceState[1].decay ); } break;
		case residlerParameterFormat::kVoice1EnvSustain: {
			voiceState[1].sustain = (unsigned char)(value * 0xF) << 4;
			resid->write(0x0D, voiceState[1].sustain | voiceState[1].release ); } break;
		case residlerParameterFormat::kVoice1EnvRelease: {
			voiceState[1].release = (unsigned char)(value * 0xF);
			resid->write(0x0D, voiceState[1].sustain | voiceState[1].release ); } break;
//------voice2
		case residlerParameterFormat::kVoice2On: voiceState[2].voiceOn = (bool)value; break;
		case residlerParameterFormat::kVoice2Tune: {
			voiceState[2].pitchMult = value;
			int freq = scaledFrequency(voiceState[2].currentNote, value, sidInfo.clockRate);
			resid->write(0x0E, (freq >> 0) & 0xFF);
			resid->write(0x0F, (freq >> 8) & 0xFF); } break;
		case residlerParameterFormat::kVoice2PW: {
			//12-bit value, bits 4-7 if PW_HI not used
			int pw = (value * 0xFFF);
			resid->write(0x10, (pw >> 0) & 0xFF);
			resid->write(0x11, (pw >> 8) & 0xFF);  } break;
		//voice2 ControlReg
		case residlerParameterFormat::kVoice2Waveform: waveformSelector(2, value); break;
		case residlerParameterFormat::kVoice2RingMod: voiceState[2].ringMod = (bool)value; break;
		case residlerParameterFormat::kVoice2Sync: voiceState[2].sync = (bool)value; break;
		//voice2 env
		case residlerParameterFormat::kVoice2EnvAttack: {
			voiceState[2].attack = (unsigned char)(value * 0xF) << 4;
			resid->write(0x13, voiceState[2].attack | voiceState[2].decay ); } break;
		case residlerParameterFormat::kVoice2EnvDecay: {
			voiceState[2].decay = (unsigned char)(value * 0xF);
			resid->write(0x13, voiceState[2].attack | voiceState[2].decay ); } break;
		case residlerParameterFormat::kVoice2EnvSustain: {
			voiceState[2].sustain = (unsigned char)(value * 0xF) << 4;
			resid->write(0x14, voiceState[2].sustain | voiceState[2].release ); } break;
		case residlerParameterFormat::kVoice2EnvRelease: {
			voiceState[2].release = (unsigned char)(value * 0xF);
			resid->write(0x14, voiceState[2].sustain | voiceState[2].release ); } break;
//------Filter Cutoff
		case residlerParameterFormat::kFilterCutoff: {
			//11-bit value, bits 3-7 if FC_LO not used
			int cutoff = value * 0x7FF;
			resid->write(0x15, (cutoff >> 0) & 0xFF);
			resid->write(0x16, (cutoff >> 3) & 0xFF);  } break;
//------Filter RES/Filt
		case residlerParameterFormat::kVoice0FilterOn: {
			filterState.voice0FilterOn = (bool)value;
			resid->write(0x17, setSIDRESFilt()); } break;
		case residlerParameterFormat::kVoice1FilterOn: {
			filterState.voice1FilterOn = (bool)value;
			resid->write(0x17, setSIDRESFilt()); } break;
		case residlerParameterFormat::kVoice2FilterOn: {
			filterState.voice2FilterOn = (bool)value;
			resid->write(0x17, setSIDRESFilt()); } break;
		case residlerParameterFormat::kFilterResonance: {
			filterState.resonance = (unsigned char)(value * 0xF) << 4;
			resid->write(0x17, setSIDRESFilt()); } break;
//------Filter Mode/Vol
		case residlerParameterFormat::kFilterHP: {
			filterState.HP = (bool)value;
			resid->write(0x18, setSIDModeVol()); } break;
		case residlerParameterFormat::kFilterBP: {
			filterState.BP = (bool)value;
			resid->write(0x18, setSIDModeVol()); } break;
		case residlerParameterFormat::kFilterLP: {
			filterState.LP = (bool)value;
			resid->write(0x18, setSIDModeVol()); } break;
		default: break;	}
}

//-----------------------------------------------------------------------------
void residlerProcessor::waveformSelector(int voiceIndex, double value)
{
	double waveformFract = 1.0f / residlerParameterFormat::knumWaveformTypes;

	if(value < waveformFract) {//Noise
		voiceState[voiceIndex].noise = true;
		voiceState[voiceIndex].pulse = false;
		voiceState[voiceIndex].triangle = false;
		voiceState[voiceIndex].saw = false;
	}
	else if(value < (waveformFract*2) ) {//Pulse
		voiceState[voiceIndex].noise = false;
		voiceState[voiceIndex].pulse = true;
		voiceState[voiceIndex].triangle = false;
		voiceState[voiceIndex].saw = false;
	}
	else if(value < (waveformFract*3) ) {//Triangle
		voiceState[voiceIndex].noise = false;
		voiceState[voiceIndex].pulse = false;
		voiceState[voiceIndex].triangle = true;
		voiceState[voiceIndex].saw = false;
	}
	else if(value < (waveformFract*4) ) {//Saw
		voiceState[voiceIndex].noise = false;
		voiceState[voiceIndex].pulse = false;
		voiceState[voiceIndex].triangle = false;
		voiceState[voiceIndex].saw = true;
	}
	else if(value < (waveformFract*5) ) {//PulseTriangle
		voiceState[voiceIndex].noise = false;
		voiceState[voiceIndex].pulse = true;
		voiceState[voiceIndex].triangle = true;
		voiceState[voiceIndex].saw = false;
	}
	else if(value < (waveformFract*6) ) {//PulseSaw
		voiceState[voiceIndex].noise = false;
		voiceState[voiceIndex].pulse = true;
		voiceState[voiceIndex].triangle = false;
		voiceState[voiceIndex].saw = true;
	}
	else if(value < (waveformFract*7) ) {//PulseTriangleSaw
		voiceState[voiceIndex].noise = false;
		voiceState[voiceIndex].pulse = true;
		voiceState[voiceIndex].triangle = true;
		voiceState[voiceIndex].saw = true;
	}
	else {//TriangleSaw
		voiceState[voiceIndex].noise = false;
		voiceState[voiceIndex].pulse = false;
		voiceState[voiceIndex].triangle = true;
		voiceState[voiceIndex].saw = true;
	}
}

//-----------------------------------------------------------------------------
unsigned char residlerProcessor::prepareSIDControlReg(int voiceIndex)
{
	unsigned char reg =   ((unsigned char)voiceState[voiceIndex].sync << 1)
						| ((unsigned char)voiceState[voiceIndex].ringMod << 2)
						| ((unsigned char)voiceState[voiceIndex].triangle << 4)
						| ((unsigned char)voiceState[voiceIndex].saw << 5)
						| ((unsigned char)voiceState[voiceIndex].pulse << 6)
						| ((unsigned char)voiceState[voiceIndex].noise << 7);
	return reg;
}

//-----------------------------------------------------------------------------
unsigned char residlerProcessor::setSIDRESFilt()
{
	unsigned char reg = (unsigned char)filterState.voice0FilterOn
							| ((unsigned char)filterState.voice1FilterOn << 1)
							| ((unsigned char)filterState.voice2FilterOn << 2);
	return (reg | filterState.resonance);
}

//-----------------------------------------------------------------------------
unsigned char residlerProcessor::setSIDModeVol()
{
	unsigned char reg = ((unsigned char)filterState.LP << 4)
						| ((unsigned char)filterState.BP << 5)
						| ((unsigned char)filterState.HP << 6);
	return (reg | 0x0F);
}

//-----------------------------------------------------------------------------
void residlerProcessor::event_test (IEventList* events)
{
	if (events)
	{
		int32 count = events->getEventCount ();
		for (int32 i = 0; i < count; i++)
		{
			Event e;
			events->getEvent (i, e);
			switch (e.type)
			{
				//grab the note on/off events and put them in a buffer/que of your own choice
				//handle sampleOffset of notes in doProcessing() to achieve sample accuracy
				//remember to flush buffer after each doProcessing() block
				//sampleOffset==timestamp
				
				case Event::kNoteOnEvent:
				{
					//e.sampleOffset;
					//e.noteOn.pitch;
					//e.noteOn.velocity;
					//e.noteOn.noteId;
					noteOn(e.noteOn.pitch, e.noteOn.velocity, e.sampleOffset);
					break;
				}
				case Event::kNoteOffEvent:
				{
					//e.sampleOffset;
					//e.noteOn.pitch;
					//0;
					//e.noteOn.noteId;
					noteOff(e.noteOn.pitch,e.sampleOffset);
					break;
				}
				default:
					continue;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void residlerProcessor::noteOn (int16 note, float velocity, int32 sampleOffset)
{
	voiceState[0].currentNote = note;
	int voice0freq = scaledFrequency(note, voiceState[0].pitchMult, sidInfo.clockRate);
	resid->write(0x00, (voice0freq >> 0) & 0xFF);
	resid->write(0x01, (voice0freq >> 8) & 0xFF);

	voiceState[1].currentNote = note;
	int voice1freq = scaledFrequency(note, voiceState[1].pitchMult, sidInfo.clockRate);
	resid->write(0x07, (voice1freq >> 0) & 0xFF);
	resid->write(0x08, (voice1freq >> 8) & 0xFF);

	voiceState[2].currentNote = note;
	int voice2freq = scaledFrequency(note, voiceState[2].pitchMult, sidInfo.clockRate);
	resid->write(0x0E, (voice2freq >> 0) & 0xFF);
	resid->write(0x0F, (voice2freq >> 8) & 0xFF);

	if (voiceState[0].voiceOn) resid->write(0x04, prepareSIDControlReg(0) | 0x01 ); //voice0 gate on
	if (voiceState[1].voiceOn) resid->write(0x0B, prepareSIDControlReg(1) | 0x01 ); //voice1 gate on
	if (voiceState[2].voiceOn) resid->write(0x12, prepareSIDControlReg(2) | 0x01 ); //voice2 gate on
}



//-----------------------------------------------------------------------------
void residlerProcessor::noteOff(int16 note, int32 sampleOffset)
{
	if (voiceState[0].voiceOn) resid->write(0x04, prepareSIDControlReg(0) | 0x00 ); //voice0 gate off
	if (voiceState[1].voiceOn) resid->write(0x0B, prepareSIDControlReg(1) | 0x00 ); //voice1 gate off
	if (voiceState[2].voiceOn) resid->write(0x12, prepareSIDControlReg(2) | 0x00 ); //voice2 gate off
}


//-----------------------------------------------------------------------------
void residlerProcessor::recalculate ()
{
	vstlogger::log("recalculate with rate %f\n", getSampleRate());

	if (!resid)
	{
		//set up reSID etc. initialisation
		resid = new reSIDfp::SID;
	}

	sidInfo.sampleRate = getSampleRate();
	sidInfo.clockRate = 985248.f;
	double passband = 0.9 * (sidInfo.sampleRate / 2.0);
	resid->setChipModel(reSIDfp::MOS6581);
	resid->setSamplingParameters(sidInfo.clockRate, reSIDfp::RESAMPLE, sidInfo.sampleRate, passband);

	//some default values for testing
	resid->write(0x17, setSIDRESFilt());
	resid->write(0x18, setSIDModeVol());

	resid->write(0x05, (voiceState[0].attack | voiceState[0].decay) );
	resid->write(0x06, (voiceState[0].sustain | voiceState[0].release) );
	
	resid->write(0x0C, (voiceState[1].attack | voiceState[1].decay) );
	resid->write(0x0D, (voiceState[1].sustain | voiceState[1].release) );

	resid->write(0x13, (voiceState[2].attack | voiceState[2].decay) );
	resid->write(0x14, (voiceState[2].sustain | voiceState[2].release) );

	residual_buf_fill = 0;
}

static inline int min(int v1, int v2)
{
	return v1 < v2 ? v1 : v2;
}

#define SAFE_EXTRA_CYCLECOUNT (1000)

int residlerProcessor::estimate_cycles(int frames_left)
{
	return (int)((frames_left * sidInfo.clockRate) / sidInfo.sampleRate) + SAFE_EXTRA_CYCLECOUNT;
}

float *residlerProcessor::gen(int maxcycles, int maxframes, float *buf)
{
	int i;
	int fr_cycles;

	if (!resid)
		return NULL;


	if (maxcycles == 0)
	{
		maxcycles = estimate_cycles(maxframes);
	}

	int this_frames = 0;

	if (maxframes <= residual_buf_fill)
	{
		this_frames = residual_buf_fill;
		fr_cycles = 0;
	}
	else
	{
		this_frames = residual_buf_fill;

		fr_cycles = estimate_cycles(maxframes - this_frames);
		fr_cycles = min(fr_cycles, maxcycles);

		residual_buf_fill += resid->clock(fr_cycles, &(residual_buf[this_frames]));

		this_frames = min(residual_buf_fill, maxframes);
	}

	if (this_frames > residual_buf_fill)
	{
		//fprintf(stderr, "trying to give %i frames out, but we only have %i\n", this_frames, residual_buf_fill);
		this_frames = residual_buf_fill;
	}

	for (i = 0; i < this_frames; i++)
		buf[i] = (float)residual_buf[i] / 32768.f;

	for (i = 0; i < (residual_buf_fill - this_frames); i++)
	{
		residual_buf[i] = residual_buf[this_frames + i];
	}
	residual_buf_fill = residual_buf_fill - this_frames;

	if (residual_buf_fill < 0)
	{
		//fprintf(stderr, "heh residual buf size meni alle nollan.\n");
		residual_buf_fill = 0;
	}

	return NULL;
}

}}} // namespaces
