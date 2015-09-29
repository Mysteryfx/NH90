#pragma once
/* from
http://en.wikipedia.org/wiki/Image_editing#Contrast_change_and_brightening
*/
#include "Math\_math.h"
struct BrightnessContrastAlgorithm
{
	float b,c;
	//brightness contrast in range of 0..1 , (common usage for avionic)
	BrightnessContrastAlgorithm(float brightness,float contrast)
	{
		const float range_modifier = 0.8f;
		b = range_modifier * 2.0f * (brightness - 0.5f);
		c = range_modifier * 2.0f * (contrast   - 0.5f);
		c = tan((c + 1.0f) * Math::toRad(45.0f));
	}
	inline float process(float value)
	{
		//where value is the input color value in the 0..1 range and b and c are in the -1..1 range.
		if (b < 0.0f) value = value * (1.0f + b);
		else		  value = value + (1.0f - value) * b;
		value = (value - 0.5f) * c + 0.5f;
		return std::min(value,1.0f);
	};
};