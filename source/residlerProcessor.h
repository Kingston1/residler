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
	void event_test (IEventList* events);
	void recalculate ();
	void noteOn(int16 note, float velocity, int32 sampleOffset);
	void noteOff(int16 note, int32 sampleOffset);
private:
	residlerParameterFormat paramFormat;


#define RES_BUF_SIZE (65536)
	short residual_buf[RES_BUF_SIZE];
	int residual_buf_fill;

	struct sidinfo
	{
		double clockrate;
		double samplerate;
	} sidinfo;

	inline int estimate_cycles(int frames_left);
	float *gen(int maxcycles, int maxframes, float *buf);

	reSIDfp::SID *resid;
};

}}} // namespaces

#endif
