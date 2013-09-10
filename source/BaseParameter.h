#ifndef __BaseParameter__
#define __BaseParameter__

#include "public.sdk/source/vst/vstparameters.h"
#include "pluginterfaces/base/ustring.h"
#include <list>
#include <algorithm>

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
class BaseParameter : public Parameter
{
public:
	BaseParameter	(	const TChar* title, 
						const TChar* units, 
						int32 stepCount, 
						ParamValue defaultValueNormalized,
						int32 flags,
						int32 tag,
						UnitID unitID = kRootUnitId);

	bool fromString (const TChar* string, ParamValue& valueNormalized) const;
	bool setNormalized (ParamValue v);

};

//-----------------------------------------------------------------------------
class IndexedParameter : public BaseParameter
{
public:
	IndexedParameter (	const TChar* title, 
						const TChar* units, 
						int32 stepCount, 
						ParamValue defaultValueNormalized,
						int32 flags,
						int32 tag,
						UnitID unitID = kRootUnitId);

	ParamValue toPlain (ParamValue valueNormalized) const;
	ParamValue toNormalized (ParamValue plainValue) const;

	void toString (ParamValue valueNormalized, String128 string) const;
	bool fromString (const TChar* string, ParamValue& valueNormalized) const;

	void setIndexString (int32 index, const String128 str);
protected:
	~IndexedParameter ();
	String128* indexString;
};

//-----------------------------------------------------------------------------
class ScaledParameter : public BaseParameter
{
public:
	ScaledParameter (	const TChar* title, 
						const TChar* units, 
						int32 stepCount, 
						ParamValue defaultValueNormalized,
						int32 flags,
						int32 tag,
						ParamValue minValue = 0.,
						ParamValue maxValue = 1.,
						bool printAsInteger = false,
						UnitID unitID = kRootUnitId);

	ParamValue toPlain (ParamValue valueNormalized) const;
	ParamValue toNormalized (ParamValue plainValue) const;

	void toString (ParamValue valueNormalized, String128 string) const;
	bool fromString (const TChar* string, ParamValue& valueNormalized) const;
protected:
	ParamValue minValue;
	ParamValue maxValue;
	bool printAsInteger;
};

}}} // namespaces

#endif
