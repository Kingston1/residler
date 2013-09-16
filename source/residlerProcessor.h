#ifndef __residlerProcessor__
#define __residlerProcessor__

#include "BaseProcessor.h"
#include "residlerParameterFormat.h"

#include "sid.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
class residlerProcessor : public BaseProcessor
{
public:
	residlerProcessor ();
	~residlerProcessor ();
	
	tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API terminate ();
	tresult PLUGIN_API setActive (TBool state);

	void doProcessing (ProcessData& data);
	
//-----------------------------------------------------------------------------
	static FUnknown* createInstance (void*) { return (IAudioProcessor*)new residlerProcessor; }
	static FUID uid;
//-----------------------------------------------------------------------------
protected:
	void recalculate ();
	void noteOn(int16 note, float velocity, int32 sampleOffset);
	void noteOff(int16 note, int32 sampleOffset);

private:
	void event_test (IEventList* events);
	bool parameter_test (IParameterChanges* changes);
	void parameterScaling(residlerParameterFormat::paramIds, double);
	
	void waveformSelector(int, double);
	unsigned char prepareSIDControlReg(int);
	unsigned char setSIDRESFilt();
	unsigned char setSIDModeVol();

private:
	residlerParameterFormat paramFormat;

#define RES_BUF_SIZE (65536)
	short residual_buf[RES_BUF_SIZE];
	int residual_buf_fill;

	struct sidInfo
	{
		double clockRate;
		double sampleRate;
	} sidInfo;

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

	inline int estimate_cycles(int frames_left);
	float *gen(int maxcycles, int maxframes, float *buf);

	reSIDfp::SID *resid;
};

}}} // namespaces

#endif
