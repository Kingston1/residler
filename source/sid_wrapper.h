#pragma once

#include "residlerParameterFormat.h"
#include "sid.h"

namespace Steinberg {
namespace Vst {
namespace residler {

struct sidinfo
{
	double clockRate;
	double sampleRate;
};

class sid_wrapper
{
public:

void reset(struct sidinfo *sidinfo_in);
#if 0
void start_gen(int frames, float *buf);
int run_gen(int frames);
#endif
int gen(int maxcycles, int maxframes, float *buf);

void note_on(int16 note, float velocity);
void note_off(int16 note);
void set_param(residlerParameterFormat::paramIds, double);

private:
	reSIDfp::SID resid;
	struct sidinfo sidinfo;

	int estimate_cycles(int frames_left);

	static const int res_buf_size = 65536;
	short residual_buf[res_buf_size];
	int residual_buf_fill;

	struct voiceState
	{
		voiceState ()
			: noise (true)
			, pulse (false)
			, saw (false)
			, triangle (false)
			, ringMod (false)
			, sync (false) 
			, gate (false)
			, voiceOn (false)
			, currentNote(0)
			, pitchMult (0.5f)
			, attack (8)
			, decay (8)
			, sustain (8) 
			, release (8)

		{}
		bool noise;
		bool pulse;
		bool saw;
		bool triangle;
		bool ringMod;
		bool sync;
		bool gate;
		bool voiceOn;
		int16 currentNote;
		double pitchMult;
		unsigned char attack;
		unsigned char decay;
		unsigned char sustain;
		unsigned char release;
	} voiceState[3];

	struct filterState
	{	filterState ()
			: resonance (8)
			, voice0FilterOn (false)
			, voice1FilterOn (false)
			, voice2FilterOn (false)
			, HP (false)
			, BP (false)
			, LP (true)
		{}
		unsigned char resonance;
		bool voice0FilterOn;
		bool voice1FilterOn;
		bool voice2FilterOn;
		bool HP;
		bool BP;
		bool LP;
	} filterState;

	void waveformSelector(int, double);
	unsigned char prepareSIDControlReg(int);
	unsigned char setSIDRESFilt();
	unsigned char setSIDModeVol();

};

}}} // namespaces