#ifndef __residlerProcessor__
#define __residlerProcessor__

#include "BaseProcessor.h"
#include "residlerParameterFormat.h"

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
	void processEvents (IEventList* events);
	void recalculate ();
	void noteOn (int32 note, int32 velocity, int32 noteID);
	void setParameter (ParamID index, ParamValue newValue, int32 sampleOffset);

private:
	residlerParameterFormat paramFormat;
};

}}} // namespaces

#endif
