#include "residlerProcessor.h"
#include "residlerController.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
FUID residlerProcessor::uid (0x82CD49DE, 0x13D743BA, 0xABDAC299, 0x1CE06F7C);

//-----------------------------------------------------------------------------
residlerProcessor::residlerProcessor ()
{
	setControllerClass (residlerController::uid);
	allocParameters (residlerParameterFormat::knumParameters);
	resid = NULL;
}

//-----------------------------------------------------------------------------
residlerProcessor::~residlerProcessor ()
{
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

		//it's ok to do big allocs etc. big initialisations here
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
	double scaler = 0x1000000 / sidinfo.clockrate;
	double freq = 440.f * pow(2.0, (double)((int)note-57)/12.0);
	int sidfreq = (int)(scaler * freq);

	int sustain = (int)(velocity * 0xF) << 4;

	resid->write(0x00, (sidfreq >> 0) & 0xFF);
	resid->write(0x01, (sidfreq >> 8) & 0xFF);
	resid->write(0x02, 0x00);
	resid->write(0x03, 0x08);
	resid->write(0x05, 0x94);
	resid->write(0x06, 0x0A | sustain);
	
	resid->write(0x04, 0x21);
}

void residlerProcessor::noteOff(int16 note, int32 sampleOffset)
{
	resid->write(0x04, 0x20);
}
//-----------------------------------------------------------------------------
void residlerProcessor::recalculate ()
{
	//set up reSID etc. initialisation
	resid = new reSIDfp::SID;

	sidinfo.samplerate = 44100.;
	sidinfo.clockrate = 1000000.f;
	double passband = 0.9 * (sidinfo.samplerate / 2.0);
	resid->setChipModel(reSIDfp::MOS6581);
	resid->setSamplingParameters(sidinfo.clockrate, reSIDfp::RESAMPLE, sidinfo.samplerate, passband);

	resid->write(0x17, 0xF1);
	resid->write(0x18, 0x1F);

	resid->write(0x16, 0x40);

	residual_buf_fill = 0;
}

static inline int min(int v1, int v2)
{
	return v1 < v2 ? v1 : v2;
}

#define SAFE_EXTRA_CYCLECOUNT (1000)

int residlerProcessor::estimate_cycles(int frames_left)
{
	return (int)((frames_left * sidinfo.clockrate) / sidinfo.samplerate) + SAFE_EXTRA_CYCLECOUNT;
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
