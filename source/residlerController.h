#ifndef __residlerController__
#define __residlerController__

#include "BaseController.h"
#include "residlerProcessor.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
class residlerController : public BaseController
{
public:
	residlerController ();
	~residlerController ();
	
	tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API terminate ();

	tresult PLUGIN_API setParamNormalized (ParamID tag, ParamValue value);
	tresult PLUGIN_API getParamStringByValue (ParamID tag, ParamValue valueNormalized, String128 string);
	tresult PLUGIN_API getParamValueByString (ParamID tag, TChar* string, ParamValue& valueNormalized);

//-----------------------------------------------------------------------------
	static FUnknown* createInstance (void*) { return (IEditController*)new residlerController; }
	static FUID uid;

private:
	void populateParameters();
	IndexedParameter* populateIndexParameter(const TChar *, const vector<string>*, ParamID);

private:
	residlerParameterFormat paramFormat;
};

}}} // namespaces

#endif
