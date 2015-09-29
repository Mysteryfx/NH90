#ifndef __FLAT_SHADOWS_PARAMS_H__
#define __FLAT_SHADOWS_PARAMS_H__

static const float MIN_FLAT_SHADOW_ANGLE = 0.05f;	// косинус угла между нормалью и солнцем, при котором исчезает тень
static const float BLEND_FLAT_SHADOW_ANGLE = 1.0f;	// ... при котором начинает исчезать тень
/// if(std::abs(n.y()) < FLAT_SHADOW_NORMAL_MAX_VERT_BIAS) return;
static const float FLAT_SHADOW_NORMAL_MAX_VERT_BIAS	= 0.8f;

static const float FLAT_SHADOW_VISIBILITY_DIST = 1000.0f; /// max distance of flat shadows visibility

/// Calculates color of flat shadow.
inline float calcFlatShadowAttenuation(float cosSunDir, bool overcast){
	float blend = 1.0f;
	if(cosSunDir < BLEND_FLAT_SHADOW_ANGLE){
		blend *= (MIN_FLAT_SHADOW_ANGLE - cosSunDir) / (MIN_FLAT_SHADOW_ANGLE - BLEND_FLAT_SHADOW_ANGLE);
	}

	if(overcast){
		blend *= 0.3f;
	}

	return blend;
}

#endif
