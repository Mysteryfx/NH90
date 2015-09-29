#pragma  once
#include <osg\Vec3d>
#include <osg\Vec2d>

namespace MathTools
{
	static const double PI2 = 3.1415926536 * 2.0;
	static const  double g = 9.8f; // ускорение свободного падения.

	static const double GradToRad = osg::PI/180./*f*/;
	static const double RadToGrad = 180./*f*//osg::PI;

	template<typename T>
	inline T min_max(T a, T b, T c)
	{
		return std::min(std::max(a,b), c);
	}

	// Привести угол к диапазону [min, max)
	inline double AlignAngle(double angle, double max, double min)
	{
		while(angle>=max)
			angle -= PI2;
		while(angle<min)
			angle += PI2;
		return angle;
	}

	template<typename T> inline T clip (const T x, const T min, const T max)
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	template<class T>
		inline void blendIntoAccumulator (const double smoothRate,
		const T& newValue,
		T& smoothedAccumulator)
	{
		smoothedAccumulator = MathTools::interpolate (clip (smoothRate, 0./*f*/, 1./*f*/),
			smoothedAccumulator,
			newValue);
	}

	template<class T> inline T square (T x)
	{
		return x * x;
	}

	template<class T> inline T interpolate (double alpha, const T& x0, const T& x1)
	{
		return x0 + ((x1 - x0) * alpha);
	}
}

namespace osg
{
	inline double length2_ZX(const osg::Vec3d& v) { return v.x()*v.x()+v.z()*v.z(); }

	double polarZX(const osg::Vec3d& v);

	//Вращение против часовой стрелки
	osg::Vec2d rotateAboutGlobalY (double angle, const osg::Vec2d& v);

	inline Vec2d operator*(double c, const Vec2d& v)
	{
		return v*c;
	}

	inline Vec3d operator*(double c, const Vec3d& v)
	{
		return v*c;
	}

	template<class OsgVec>
	OsgVec normalize(const OsgVec& v)
	{
		OsgVec::value_type norm = v.length();
		if (norm>0.0)
		{
			OsgVec::value_type inv = 1.0f/norm;
			return v*inv;
		}         
		return v;
	}

	osg::Vec2d rightNorm(const osg::Vec2d& v);
	osg::Vec2d leftNorm(const osg::Vec2d& v);

	template<class OsgVec>
	inline double distance(const OsgVec& v1, const OsgVec& v2)
	{
		return(v1-v2).length();
	}

	template<class Vec>
	inline Vec parallelComponent (const Vec& unitBasis, const Vec& v)
	{
		const double projection = v*unitBasis;
		return unitBasis * projection;
	}

	template<class Vec>
	inline Vec perpendicularComponent (const Vec& unitBasis, const Vec& v)
	{
		return v - parallelComponent(unitBasis, v);
	}

	inline bool isCWTurn(const osg::Vec2d& v1, const osg::Vec2d& v2)
	{
		return (v1.y()*v2.x() - v1.x()*v2.y()) <= 0.;
	}

	//Вектор p3  находится справа по отношению к p2-p1
	inline bool isRightTurn(const osg::Vec2d& p1, const osg::Vec2d& p2, const osg::Vec2d& p3)
	{
		osg::Vec2d v1 = p1-p2;
		osg::Vec2d v2 = p3-p2;

		return isCWTurn(v1, v2);
	}

#ifdef _OPENSTEER
	inline osg::Vec3d toVec3d(const osg::Vec2d& v)
	{
		return osg::Vec3d(v.x(), 0, v.y());
	}

	inline osg::Vec2d toVec2d(const osg::Vec3d& v)
	{
		return osg::Vec2d(v.x(), v.z());
	}


	template<class Vec>
		inline osg::Vec2d toVec2dXZ(const Vec& v)
	{
		return osg::Vec2d(v.x(), v.z());
	}
#else
	template<class Vec>
		inline osg::Vec3d toVec3d(const Vec& v)
	{
		return osg::Vec3d(v.x, v.y, v.z);
	}

	template<class Vec3>
	inline osg::Vec2d toVec2d(const Vec3& v)
	{
		return osg::Vec2d(v.x, v.z);
	}

	template<>
	inline osg::Vec3d toVec3d(const osg::Vec2d& v)
	{
		return osg::Vec3d(v.x(), 0., v.y());
	}

	template<>
	inline osg::Vec2d toVec2d(const osg::Vec3d& v)
	{
		return osg::Vec2d(v.x(), v.z());
	}

#endif
	osg::Vec2d truncateLength(const double maxLength, const osg::Vec2d& v);

	static const osg::Vec3f zeroVF;
	static const osg::Vec2d zeroVD;
	static const osg::Vec3d zeroV3D;
	static const osg::Vec3d upVD(0, 1, 0);
	static const osg::Vec3d sideVD(0, 0, 1);
	static const osg::Vec2d forwardVD(1, 0);
}

namespace VecUtils
{
	osg::Vec3d rotateAboutGlobalY (double angle, const osg::Vec3d& v, double& bsin, double& bcos);
	osg::Vec3d findPerpendicularIn3d (const osg::Vec3d& direction);
}
