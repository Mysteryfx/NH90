#ifndef OSG_COMPARATORS_H
#define OSG_COMPARATORS_H

#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/Matrixf>
#include <osg/Matrixd>
#include <osg/Matrix3>

namespace ed
{
	/**
	 * DeltaLengthComparator - компаратор для сравнения векторов с учетом абсолютной погрешности. Тип T должен иметь метод length()
	 */
	template <typename T> struct DeltaLengthComparator
	{
		double delta;

		DeltaLengthComparator(double delta = 0.0) : delta(delta) {}

		bool operator () (const T& lhs, const T& rhs)
		{
			if ((lhs - rhs).length() < delta)
				return (0.0 < 0.0);

			return (lhs < rhs);
		}
	};

	static const float FLOAT_THRESHOLD=1.0e-5f;
	static const double DOUBLE_THRESHOLD=1.0e-10;

	template <typename _T> struct CmpTraits {};
	template<> struct CmpTraits<osg::Vec2f>
	{
		typedef float Type;
		static const unsigned int N=2;

		Type threshold;

		CmpTraits():threshold(FLOAT_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Vec3f>
	{
		typedef float Type;
		static const unsigned int N=3;

		Type threshold;

		CmpTraits():threshold(FLOAT_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Vec4f>
	{
		typedef float Type;
		static const unsigned int N=4;

		Type threshold;

		CmpTraits():threshold(FLOAT_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Quatd>
	{
		typedef float Type;
		static const unsigned int N=4;

		Type threshold;

		CmpTraits():threshold(DOUBLE_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Quatf>
	{
		typedef float Type;
		static const unsigned int N=4;

		Type threshold;

		CmpTraits():threshold(FLOAT_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Matrix3>
	{
		typedef float Type;
		static const unsigned int N=9;

		Type threshold;

		CmpTraits():threshold(FLOAT_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Matrixf>
	{
		typedef float Type;
		static const unsigned int N=16;

		Type threshold;

		CmpTraits():threshold(FLOAT_THRESHOLD) {}
	};
	template<> struct CmpTraits<float>
	{
		typedef float Type;

		Type threshold;

		CmpTraits():threshold(FLOAT_THRESHOLD) {}
	};

	template<> struct CmpTraits<osg::Vec2d>
	{
		typedef double Type;
		static const unsigned int N=2;

		Type threshold;

		CmpTraits():threshold(DOUBLE_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Vec3d>
	{
		typedef double Type;
		static const unsigned int N=3;

		Type threshold;

		CmpTraits():threshold(DOUBLE_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Vec4d>
	{
		typedef double Type;
		static const unsigned int N=4;

		Type threshold;

		CmpTraits():threshold(DOUBLE_THRESHOLD) {}
	};
	template<> struct CmpTraits<osg::Matrixd>
	{
		typedef double Type;
		static const unsigned int N=16;

		Type threshold;

		CmpTraits():threshold(DOUBLE_THRESHOLD) {}
	};
	template<> struct CmpTraits<double>
	{
		typedef double Type;

		Type threshold;

		CmpTraits():threshold(DOUBLE_THRESHOLD) {}
	};
	
	/// Returns true if difference between elements is less then \param threshold, otherwise returns false.
	template <typename _T,typename _F,unsigned int N> inline bool cmp(const _T& v1,const _T& v2,_F threshold)
	{
		for(unsigned int i=0; i<N; ++i)
		{
			if(std::abs(v1[i]-v2[i])>threshold) return false;
		}
		return true;
	}

	/// Returns true if difference between elements is less then \param threshold, otherwise returns false.
	template <typename _T> inline bool cmp(const _T& v1,const _T& v2,typename CmpTraits<_T>::Type threshold)
	{
		return cmp<_T,CmpTraits<_T>::Type,CmpTraits<_T>::N>(v1,v2,threshold);
	}

	template <typename _T> inline bool cmp(const _T& v1,const _T& v2)
	{
		return cmp<_T,CmpTraits<_T>::Type,CmpTraits<_T>::N>(v1,v2,(CmpTraits<_T>()).threshold);
	}

	inline bool cmp(float v1,float v2,CmpTraits<float>::Type threshold=(CmpTraits<float>()).threshold)
	{
		return (std::abs(v1-v2)<=threshold);
	}

	inline bool cmp(double v1,double v2,CmpTraits<double>::Type threshold=(CmpTraits<float>()).threshold)
	{
		return (std::abs(v1-v2)<=threshold);
	}

	inline bool cmp(int v1,int v2, int threshold = 0)
	{
		return v1 == v2;
	}

}

#endif