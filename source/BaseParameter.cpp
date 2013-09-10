#include "BaseParameter.h"

namespace Steinberg {
namespace Vst {
namespace residler {

//-----------------------------------------------------------------------------
BaseParameter::BaseParameter (const TChar* title, const TChar* units, int32 stepCount, ParamValue defaultValueNormalized, int32 flags, int32 tag, UnitID unitID)
{
	Steinberg::UString (info.title, USTRINGSIZE (info.title)).assign (title);
	Steinberg::UString uUnits (info.units, USTRINGSIZE (info.units));
	if (units)
	{
		uUnits.assign (units);
	}

	info.stepCount = stepCount;
	info.defaultNormalizedValue = defaultValueNormalized;
	info.flags = flags;
	info.id = tag;
	info.unitId = unitID;
}

//-----------------------------------------------------------------------------
bool BaseParameter::fromString (const TChar* string, ParamValue& valueNormalized) const
{
	UString wrapper ((TChar*)string, USTRINGSIZE (String128));
	double value;
	if (wrapper.scanFloat (value))
	{
		if (value < 0)
			value = 0;
		else if (value > 1)
			value = 1;
		valueNormalized = toNormalized (value);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool BaseParameter::setNormalized (ParamValue v)
{
	return Parameter::setNormalized (v);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
IndexedParameter::IndexedParameter (const TChar* title, const TChar* units, int32 stepCount, ParamValue defaultValueNormalized, int32 flags, int32 tag, UnitID unitID)
: BaseParameter (title, units, stepCount, defaultValueNormalized, flags, tag, unitID)
, indexString (0)
{
	indexString = new String128[stepCount+1];
	for (int32 i = 0; i <= stepCount; i++)
		indexString[i][0] = 0;
}

//-----------------------------------------------------------------------------
IndexedParameter::~IndexedParameter ()
{
	delete [] indexString;
}

//-----------------------------------------------------------------------------
ParamValue IndexedParameter::toPlain (ParamValue valueNormalized) const
{
	return std::min<ParamValue> (info.stepCount, valueNormalized * (info.stepCount+1));
}

//-----------------------------------------------------------------------------
ParamValue IndexedParameter::toNormalized (ParamValue plainValue) const
{
	return plainValue / (double)info.stepCount;
}

//-----------------------------------------------------------------------------
void IndexedParameter::toString (ParamValue valueNormalized, String128 string) const
{
	memcpy (string, indexString[(int32)toPlain (valueNormalized)], 128 * sizeof (TChar));
}

//-----------------------------------------------------------------------------
bool IndexedParameter::fromString (const TChar* string, ParamValue& valueNormalized) const
{
	if (string[0] == 0)
		return false;

	for (int32 i = 0; i <= info.stepCount; i++)
	{
		int32 pos = 0;
		do
		{
			if (string[pos] != indexString[i][pos])
				break;
			pos++;
			if (string[pos] == 0 && indexString[i][pos] == 0)
			{
				valueNormalized = toNormalized (i);
				return true;
			}
		} while (string[pos] != 0 && indexString[i][pos] != 0);
	}
	UString128 str(string);
	int64 value;
	if (str.scanInt (value) && value <= info.stepCount)
	{
		valueNormalized = toNormalized (value);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void IndexedParameter::setIndexString (int32 index, const String128 str)
{
	memcpy (indexString[index], str, 128 * sizeof (TChar));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ScaledParameter::ScaledParameter (	const TChar* title, 
									const TChar* units, 
									int32 stepCount, 
									ParamValue defaultValueNormalized,
									int32 flags,
									int32 tag,
									ParamValue minValue,
									ParamValue maxValue,
									bool printAsInteger,
									UnitID unitID)
: BaseParameter (title, units, stepCount, defaultValueNormalized, flags, tag, unitID)
, minValue (minValue)
, maxValue (maxValue)
, printAsInteger (printAsInteger)
{
	setPrecision (2);
}

//-----------------------------------------------------------------------------
ParamValue ScaledParameter::toPlain (ParamValue valueNormalized) const
{
	return valueNormalized * (maxValue - minValue) + minValue;
}

//-----------------------------------------------------------------------------
ParamValue ScaledParameter::toNormalized (ParamValue plainValue) const
{
	return (plainValue - minValue) / (maxValue - minValue);
}

//-----------------------------------------------------------------------------
void ScaledParameter::toString (ParamValue valueNormalized, String128 string) const
{
	UString wrapper (string, USTRINGSIZE (String128));
	if (printAsInteger)
		wrapper.printInt (toPlain (valueNormalized));
	else
		wrapper.printFloat (toPlain (valueNormalized), precision);
}

//-----------------------------------------------------------------------------
bool ScaledParameter::fromString (const TChar* string, ParamValue& valueNormalized) const
{
	UString wrapper ((TChar*)string, USTRINGSIZE (String128));
	double value;
	if (wrapper.scanFloat (value))
	{
		if (value < minValue)
			value = minValue;
		else if (value > maxValue)
			value = maxValue;
		valueNormalized = toNormalized (value);
		return true;
	}
	return false;
}

}}} // namespaces
