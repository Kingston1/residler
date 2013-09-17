#include "residlerProcessor.h"
#include "residlerController.h"

#include "base/source/fdebug.h"

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

	sid = NULL;

	DBPRT0("INITIALIZED processor\n");
}

//-----------------------------------------------------------------------------
residlerProcessor::~residlerProcessor ()
{
	DBPRT0("goodbye processor.\n");

	if (sid)
		delete sid;
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

	sid->gen(0, sampleFrames, out1);

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
				sid->set_param((residlerParameterFormat::paramIds)paramId, value);
			}
			return true;
		}
	}
	return false;
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
					//noteOn(e.noteOn.pitch, e.noteOn.velocity, e.sampleOffset);
					sid->note_on(e.noteOn.pitch, e.noteOn.velocity);
					break;
				}
				case Event::kNoteOffEvent:
				{
					//e.sampleOffset;
					//e.noteOn.pitch;
					//0;
					//e.noteOn.noteId;
					//noteOff(e.noteOn.pitch,e.sampleOffset);
					sid->note_off(e.noteOn.pitch);
					break;
				}
				default:
					continue;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void residlerProcessor::recalculate ()
{
	DBPRT1("recalculate with rate %f\n", getSampleRate());

	if (!sid)
	{
		//set up reSID etc. initialisation
		sid = new sid_wrapper;
	}

	struct sidinfo sidinfo;
	sidinfo.sampleRate = getSampleRate();
	sidinfo.clockRate = 985248.f;
	sid->reset(&sidinfo);
}

}}} // namespaces
