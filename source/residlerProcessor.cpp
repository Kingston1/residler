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
	if (state)
		recalculate ();

	return BaseProcessor::setActive (state);
}

//-----------------------------------------------------------------------------
void residlerProcessor::setParameter (ParamID index, ParamValue newValue, int32 sampleOffset)
{
	//todo: sampleOffset of parameters is ignored currently. no sample accurate automation!
	//see processEvents() below
	//parameter que is needed to handle these events in doProcessing() with any kind of sample accuracy
	//sampleOffset==timestamp

	if (index < residlerParameterFormat::knumParameters)
		BaseProcessor::setParameter (index, newValue, sampleOffset);
	
	
	//todo: program changes will come from some file based system in the future
	else if (index == BaseController::kPresetParam) // program change
	{
		int32 program = std::min<int32> (residlerParameterFormat::knumPresets-1, (int32)(newValue * residlerParameterFormat::knumPresets));
		const float* newParams = residlerParameterFormat::programParams[program];
		if (newParams)
		{
			for (int32 i = 0; i < residlerParameterFormat::knumParameters; i++)
				params[i] = newParams[i];
		}
	}


	//todo: handle this midi stuff elsewhere
	else if (index == BaseController::kModWheelParam) // mod wheel
	{
		newValue *= 127.;
		//modwhl = 0.000005f * (newValue*newValue);
	}
	else if (index == BaseController::kPitchBendParam) // pitch bend
	{
		if (newValue <= 1)
			newValue = (newValue - 0.5) * 0x2000;
		//ipbend = (float)exp (0.000014102 * (double)newValue);
		//pbend = 1.0f / ipbend;
	}
	else if (index == BaseController::kBreathParam)
	{
		newValue *= 127.;
		//filtwhl = 0.02f * newValue;
	}
	else if (index == BaseController::kCtrler3Param)
	{
		newValue *= 127.;
		//filtwhl = -0.03f * newValue;
	}
	else if (index == BaseController::kExpressionParam)
	{
		newValue *= 127.;
		//rezwhl = 0.0065f * (float)(154 - newValue);
	}
	else if (index == BaseController::kAftertouchParam)
	{
		newValue *= 127.;
		//press = 0.00001f * (float)(newValue * newValue);
	}
}

//-----------------------------------------------------------------------------
void residlerProcessor::doProcessing (ProcessData& data)
{
	int32 sampleFrames = data.numSamples;
	
	float* out1 = data.outputs[0].channelBuffers32[0];
	float* out2 = data.outputs[0].channelBuffers32[1];

	//see SDK note_expression_synth_processor.cpp process (ProcessData& data)
	//good ideas on how to use data.inputParameterChanges and IParamValueQueue there

	//default, mark buffer empty
	memset (out1, 0, sampleFrames * sizeof (float));
	memset (out2, 0, sampleFrames * sizeof (float));
	
}

//-----------------------------------------------------------------------------
void residlerProcessor::processEvents (IEventList* events)
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
					break;
				}
				case Event::kNoteOffEvent:
				{
					//e.sampleOffset;
					//e.noteOn.pitch;
					//0;
					//e.noteOn.noteId;
					break;
				}
				default:
					continue;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void residlerProcessor::noteOn (int32 note, int32 velocity, int32 noteID)
{
	//might be a good idea to expand this method significantly
}

//-----------------------------------------------------------------------------
void residlerProcessor::recalculate ()
{
	//set up reSID etc. initialisation
}

}}} // namespaces

