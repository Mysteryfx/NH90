#ifndef _AtmosphereTransparency_h_
#define _AtmosphereTransparency_h_

#include <algorithm>

namespace Lua
{
	class Config;
}

namespace AI
{

struct Layer
{
	float hMin;
	float hMax;
};

class AtmosphereTransparency
{
public:
	enum SpectrumBand { SPECTRUM_BAND_VISIBLE, SPECTRUM_BAND_IR };
	enum Test
	{
		TEST_VOID			= 0,
		TEST_AIR			= 1 << 0,
		TEST_OVERCAST		= 1 << 1,
		TEST_FOG			= 1 << 2,
		TEST_PRECIPITATION	= 1 << 3,
        TEST_ALL            = 0xFFFFFFFF
	};
	class ConstTransparencyLayer
	{
	public:
		ConstTransparencyLayer() {;}
		ConstTransparencyLayer(float kIn, const Layer & layerIn) : k(kIn), layer(layerIn) {;}
		inline float getKh(float h1, float h2) const
		{
			float wh = std::min(std::max(layer.hMin, h2), layer.hMax) -	std::min(std::max(layer.hMin, h1), layer.hMax);
			return k * 1.0E-3f * wh;
		}
	private:
		float k;
		Layer layer;
	};
	AtmosphereTransparency();
	void load(Lua::Config & config);
	float getVisibility(float h1, float h2, SpectrumBand spectrumBand, Test desiredTests) const; //get visibility distance due the transparency factor, m
	float getContrast(float h1, float h2, float d, SpectrumBand spectrumBand, Test desiredTests) const; //get target contrast due the transparency factor
	static AtmosphereTransparency & instance();
private:
	float getKh_(float h1, float h2, SpectrumBand spectrumBand, Test desiredTests) const;
	unsigned int availableTests;
	float Imin;
	float kAir;
	ConstTransparencyLayer air;
	ConstTransparencyLayer overcast;
	ConstTransparencyLayer fog;
	ConstTransparencyLayer precipitations;
};

}

#endif _AtmosphereTransparency_h_