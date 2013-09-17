#include "sid_wrapper.h"

namespace Steinberg {
namespace Vst {
namespace residler {

const int scaledFrequency(const int16 note, const double pitchMult, const double clockRate)
{
	double scaler = 0x1000000 / clockRate;
	double pitch = note - 57 + (pitchMult * 24 - 12);
	double freq = 440.f * pow(2.0, pitch/12.0);
	return (int)(scaler * freq);
}

void sid_wrapper::reset(struct sidinfo *sidinfo_in)
{
	sidinfo = *sidinfo_in;
	double passband = 0.9 * (sidinfo.sampleRate / 2.0);
	resid.setChipModel(reSIDfp::MOS6581);
	resid.setSamplingParameters(sidinfo.clockRate, reSIDfp::RESAMPLE, sidinfo.sampleRate, passband);

	//some default values for testing
	resid.write(0x17, setSIDRESFilt());
	resid.write(0x18, setSIDModeVol());

	resid.write(0x05, (voiceState[0].attack | voiceState[0].decay) );
	resid.write(0x06, (voiceState[0].sustain | voiceState[0].release) );
	
	resid.write(0x0C, (voiceState[1].attack | voiceState[1].decay) );
	resid.write(0x0D, (voiceState[1].sustain | voiceState[1].release) );

	resid.write(0x13, (voiceState[2].attack | voiceState[2].decay) );
	resid.write(0x14, (voiceState[2].sustain | voiceState[2].release) );

	residual_buf_fill = 0;
}

void sid_wrapper::note_on(int16 note, float velocity)
{
	voiceState[0].currentNote = note;
	int voice0freq = scaledFrequency(note, voiceState[0].pitchMult, sidinfo.clockRate);
	resid.write(0x00, (voice0freq >> 0) & 0xFF);
	resid.write(0x01, (voice0freq >> 8) & 0xFF);

	voiceState[1].currentNote = note;
	int voice1freq = scaledFrequency(note, voiceState[1].pitchMult, sidinfo.clockRate);
	resid.write(0x07, (voice1freq >> 0) & 0xFF);
	resid.write(0x08, (voice1freq >> 8) & 0xFF);

	voiceState[2].currentNote = note;
	int voice2freq = scaledFrequency(note, voiceState[2].pitchMult, sidinfo.clockRate);
	resid.write(0x0E, (voice2freq >> 0) & 0xFF);
	resid.write(0x0F, (voice2freq >> 8) & 0xFF);

	if (voiceState[0].voiceOn) resid.write(0x04, prepareSIDControlReg(0) | 0x01 ); //voice0 gate on
	if (voiceState[1].voiceOn) resid.write(0x0B, prepareSIDControlReg(1) | 0x01 ); //voice1 gate on
	if (voiceState[2].voiceOn) resid.write(0x12, prepareSIDControlReg(2) | 0x01 ); //voice2 gate on
}



//-----------------------------------------------------------------------------
void sid_wrapper::note_off(int16 note)
{
	if (voiceState[0].voiceOn) resid.write(0x04, prepareSIDControlReg(0) | 0x00 ); //voice0 gate off
	if (voiceState[1].voiceOn) resid.write(0x0B, prepareSIDControlReg(1) | 0x00 ); //voice1 gate off
	if (voiceState[2].voiceOn) resid.write(0x12, prepareSIDControlReg(2) | 0x00 ); //voice2 gate off
}


//-----------------------------------------------------------------------------
void sid_wrapper::set_param(residlerParameterFormat::paramIds param, double value)
{
	switch (param) {
//------voice0
		case residlerParameterFormat::kVoice0On: voiceState[0].voiceOn = (bool)value; break;
		case residlerParameterFormat::kVoice0Tune: {
			voiceState[0].pitchMult = value;
			int freq = scaledFrequency(voiceState[0].currentNote, value, sidinfo.clockRate);
			resid.write(0x00, (freq >> 0) & 0xFF);
			resid.write(0x01, (freq >> 8) & 0xFF); } break;
		case residlerParameterFormat::kVoice0PW: {
			//12-bit value, bits 4-7 if PW_HI not used
			int pw = (value * 0xFFF);
			resid.write(0x02, (pw >> 0) & 0xFF);
			resid.write(0x03, (pw >> 8) & 0xFF);  } break;
		//voice0 ControlReg
		case residlerParameterFormat::kVoice0Waveform: waveformSelector(0, value); break;
		case residlerParameterFormat::kVoice0RingMod: voiceState[0].ringMod = (bool)value; break;
		case residlerParameterFormat::kVoice0Sync: voiceState[0].sync = (bool)value; break;
		//voice0 env
		case residlerParameterFormat::kVoice0EnvAttack: {
			voiceState[0].attack = (unsigned char)(value * 0xF) << 4;
			resid.write(0x05, voiceState[0].attack | voiceState[0].decay ); } break;
		case residlerParameterFormat::kVoice0EnvDecay: {
			voiceState[0].decay = (unsigned char)(value * 0xF);
			resid.write(0x05, voiceState[0].attack | voiceState[0].decay ); } break;
		case residlerParameterFormat::kVoice0EnvSustain: {
			voiceState[0].sustain = (unsigned char)(value * 0xF) << 4;
			resid.write(0x06, voiceState[0].sustain | voiceState[0].release ); } break;
		case residlerParameterFormat::kVoice0EnvRelease: {
			voiceState[0].release = (unsigned char)(value * 0xF);
			resid.write(0x06, voiceState[0].sustain | voiceState[0].release ); } break;
//------voice1
		case residlerParameterFormat::kVoice1On: voiceState[1].voiceOn = (bool)value; break;
		case residlerParameterFormat::kVoice1Tune: {
			voiceState[1].pitchMult = value;
			int freq = scaledFrequency(voiceState[1].currentNote, value, sidinfo.clockRate);
			resid.write(0x07, (freq >> 0) & 0xFF);
			resid.write(0x08, (freq >> 8) & 0xFF); } break;
		case residlerParameterFormat::kVoice1PW: {
			//12-bit value, bits 4-7 if PW_HI not used
			int pw = (value * 0xFFF);
			resid.write(0x09, (pw >> 0) & 0xFF);
			resid.write(0x0A, (pw >> 8) & 0xFF);  } break;
		//voice1 ControlReg
		case residlerParameterFormat::kVoice1Waveform: waveformSelector(1, value); break;
		case residlerParameterFormat::kVoice1RingMod: voiceState[1].ringMod = (bool)value; break;
		case residlerParameterFormat::kVoice1Sync: voiceState[1].sync = (bool)value; break;
		//voice1 env
		case residlerParameterFormat::kVoice1EnvAttack: {
			voiceState[1].attack = (unsigned char)(value * 0xF) << 4;
			resid.write(0x0C, voiceState[1].attack | voiceState[1].decay ); } break;
		case residlerParameterFormat::kVoice1EnvDecay: {
			voiceState[1].decay = (unsigned char)(value * 0xF);
			resid.write(0x0C, voiceState[1].attack | voiceState[1].decay ); } break;
		case residlerParameterFormat::kVoice1EnvSustain: {
			voiceState[1].sustain = (unsigned char)(value * 0xF) << 4;
			resid.write(0x0D, voiceState[1].sustain | voiceState[1].release ); } break;
		case residlerParameterFormat::kVoice1EnvRelease: {
			voiceState[1].release = (unsigned char)(value * 0xF);
			resid.write(0x0D, voiceState[1].sustain | voiceState[1].release ); } break;
//------voice2
		case residlerParameterFormat::kVoice2On: voiceState[2].voiceOn = (bool)value; break;
		case residlerParameterFormat::kVoice2Tune: {
			voiceState[2].pitchMult = value;
			int freq = scaledFrequency(voiceState[2].currentNote, value, sidinfo.clockRate);
			resid.write(0x0E, (freq >> 0) & 0xFF);
			resid.write(0x0F, (freq >> 8) & 0xFF); } break;
		case residlerParameterFormat::kVoice2PW: {
			//12-bit value, bits 4-7 if PW_HI not used
			int pw = (value * 0xFFF);
			resid.write(0x10, (pw >> 0) & 0xFF);
			resid.write(0x11, (pw >> 8) & 0xFF);  } break;
		//voice2 ControlReg
		case residlerParameterFormat::kVoice2Waveform: waveformSelector(2, value); break;
		case residlerParameterFormat::kVoice2RingMod: voiceState[2].ringMod = (bool)value; break;
		case residlerParameterFormat::kVoice2Sync: voiceState[2].sync = (bool)value; break;
		//voice2 env
		case residlerParameterFormat::kVoice2EnvAttack: {
			voiceState[2].attack = (unsigned char)(value * 0xF) << 4;
			resid.write(0x13, voiceState[2].attack | voiceState[2].decay ); } break;
		case residlerParameterFormat::kVoice2EnvDecay: {
			voiceState[2].decay = (unsigned char)(value * 0xF);
			resid.write(0x13, voiceState[2].attack | voiceState[2].decay ); } break;
		case residlerParameterFormat::kVoice2EnvSustain: {
			voiceState[2].sustain = (unsigned char)(value * 0xF) << 4;
			resid.write(0x14, voiceState[2].sustain | voiceState[2].release ); } break;
		case residlerParameterFormat::kVoice2EnvRelease: {
			voiceState[2].release = (unsigned char)(value * 0xF);
			resid.write(0x14, voiceState[2].sustain | voiceState[2].release ); } break;
//------Filter Cutoff
		case residlerParameterFormat::kFilterCutoff: {
			//11-bit value, bits 3-7 if FC_LO not used
			int cutoff = value * 0x7FF;
			resid.write(0x15, (cutoff >> 0) & 0xFF);
			resid.write(0x16, (cutoff >> 3) & 0xFF);  } break;
//------Filter RES/Filt
		case residlerParameterFormat::kVoice0FilterOn: {
			filterState.voice0FilterOn = (bool)value;
			resid.write(0x17, setSIDRESFilt()); } break;
		case residlerParameterFormat::kVoice1FilterOn: {
			filterState.voice1FilterOn = (bool)value;
			resid.write(0x17, setSIDRESFilt()); } break;
		case residlerParameterFormat::kVoice2FilterOn: {
			filterState.voice2FilterOn = (bool)value;
			resid.write(0x17, setSIDRESFilt()); } break;
		case residlerParameterFormat::kFilterResonance: {
			filterState.resonance = (unsigned char)(value * 0xF) << 4;
			resid.write(0x17, setSIDRESFilt()); } break;
//------Filter Mode/Vol
		case residlerParameterFormat::kFilterHP: {
			filterState.HP = (bool)value;
			resid.write(0x18, setSIDModeVol()); } break;
		case residlerParameterFormat::kFilterBP: {
			filterState.BP = (bool)value;
			resid.write(0x18, setSIDModeVol()); } break;
		case residlerParameterFormat::kFilterLP: {
			filterState.LP = (bool)value;
			resid.write(0x18, setSIDModeVol()); } break;
		default: break;	}
}

//-----------------------------------------------------------------------------
void sid_wrapper::waveformSelector(int voiceIndex, double value)
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
unsigned char sid_wrapper::prepareSIDControlReg(int voiceIndex)
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
unsigned char sid_wrapper::setSIDRESFilt()
{
	unsigned char reg = (unsigned char)filterState.voice0FilterOn
							| ((unsigned char)filterState.voice1FilterOn << 1)
							| ((unsigned char)filterState.voice2FilterOn << 2);
	return (reg | filterState.resonance);
}

unsigned char sid_wrapper::setSIDModeVol()
{
	unsigned char reg = ((unsigned char)filterState.LP << 4)
						| ((unsigned char)filterState.BP << 5)
						| ((unsigned char)filterState.HP << 6);
	return (reg | 0x0F);
}


static inline int min(int v1, int v2)
{
	return v1 < v2 ? v1 : v2;
}

static const int safe_extra_cyclecount = 1000;

int sid_wrapper::estimate_cycles(int frames_left)
{
	return (int)((frames_left * sidinfo.clockRate) / sidinfo.sampleRate) + safe_extra_cyclecount;
}

#if 0
void sid_wrapper::start_gen(int frames, float *buf)
{
	i_buf = buf;
	i_frame = 0;
	i_maxframes = frames;
}

int sid_wrapper::run_gen(int frames)
{
	return i_maxframes - i_frame;
}
#endif

int sid_wrapper::gen(int maxcycles, int maxframes, float *buf)
{
	int i;
	int fr_cycles;

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

		residual_buf_fill += resid.clock(fr_cycles, &(residual_buf[this_frames]));

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

	return this_frames;
}

}}} // namespaces