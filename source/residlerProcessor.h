#ifndef __residlerProcessor__
#define __residlerProcessor__

#include "BaseProcessor.h"
#include "residlerParameterFormat.h"

#include "sid_wrapper.h"

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
	
private:
	residlerParameterFormat paramFormat;

	// this 65536 because 65536 is nice, big and round

	int last_update_times[residlerParameterFormat::knumParameters];

	sid_wrapper *sid;
};


}}} // namespaces

#endif
