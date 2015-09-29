#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H

#include "FFT.h"
#include "Terrain.h"
#include "IWaveGenerator.h"

#include  "osg/Vec2f"

namespace PondsMath
{

class WaveGenerator : public IWaveGenerator
{
public:
    TERRAIN_API WaveGenerator();
    TERRAIN_API ~WaveGenerator();

	//init(-1.1, 6, 6, 0.0008f, osg::Vec2f(2.f, 3.f), 30.f, 30.f);
    TERRAIN_API void init(float lambda, unsigned int x_power, unsigned int y_power, float a, const osg::Vec2f &wind, float dx, float dy);

	TERRAIN_API float *getH(double t) const;
	TERRAIN_API float *getX(double t) const;
	TERRAIN_API float *getZ(double t) const;

    TERRAIN_API osg::Vec3f getNormalAtPoint(double t, float x, float z);
	TERRAIN_API float getHeightAtPoint(double t, float x, float z);
private:
    FFT *_fft;

    float _a;
	osg::Vec2f _wind;
	float _world_x, _world_y;
	int _x_size, _y_size;
	float _lambda;
	double _time;
	
	bool _initialized;

	unsigned int x_pow;
	unsigned int y_pow;

	std::complex<float> *h0;
	std::complex<float> *h;
	std::complex<float> *dx;
	std::complex<float> *dz;

	float *_h[phase], *_dx[phase], *_dz[phase];
	int *_indices;
	
	void precalculate_h0();
	unsigned get_x_pow();
	unsigned get_y_pow();
	void store(int PH, unsigned int index, const osg::Vec2f &k, float k_len, const std::complex<float> &result);
	void update(int PH);
	float get_world_x(int index);
	float get_world_y(int index);
	unsigned int get_x_size() const{return _x_size;};
	int get_y_size() const{return _y_size;};
};
}
#endif
