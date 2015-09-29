#ifndef _NOISEGEN_H_
#define _NOISEGEN_H_
#include "_config.h"

#include <cmath>

class NoiseGen
{
public:
    enum { B = 0x100 };

	ED_CORE_EXTERN NoiseGen();
	/*конструктор для явной инициализации N-измерений, пока сделано для 1D и 3D шума*/
	ED_CORE_EXTERN NoiseGen(unsigned dims);
	ED_CORE_EXTERN ~NoiseGen();
	ED_CORE_EXTERN float Noise1(float arg, int repeat = B) const;
	ED_CORE_EXTERN float Noise2(float vec[2], int repeat = B) const;
	ED_CORE_EXTERN float Noise3(float vec[3], int repeat = B) const;
	ED_CORE_EXTERN float PerlinNoise1D(float x, float alpha, float beta, int octaves) const;
	ED_CORE_EXTERN float PerlinNoise2D(float x, float y, float alpha, float beta, int octaves) const;
	ED_CORE_EXTERN float PerlinNoise3D(float x, float y, float z, float alpha, float beta, int octaves) const;

private:

	void Init1D();
	void Init3D();

	int p[B + B + 2];
	float g3[B + B + 2][3];
	float g2[B + B + 2][2];
	float g1[B + B + 2];
};


class SimplexNoise
{
public:
	ED_CORE_EXTERN SimplexNoise();
	// Initialize permutation arrays
	ED_CORE_EXTERN void init();
	// 2D simplex noise
	ED_CORE_EXTERN double noise2D( double xin, double yin );

private:
	class Grad 
	{
	public:
		Grad( int x, int y, int z ) : x(x), y(y), z(z) {}
		int x, y, z;
	};

	inline int fastFloor( double x ) { int xi = static_cast<int>(x); return x < xi ? xi - 1 : xi; }
	inline double dot( const Grad& g, double x, double y ) { return g.x * x + g.y * y; }
	static const double F2;
	static const double G2;
	static const Grad grad3[12];
	static const int p[256];
	int perm[512];
	int permMod12[512];
};

#endif
