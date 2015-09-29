#ifndef MATRIX_MN_H__
#define MATRIX_MN_H__

#include <osg/Vec2f>
#include <osg/Vec2i>
#include <osg/Vec4ub>
#include <ed/vector.h>
#include <Util/Strings.h>
#include <string.h>

#pragma warning( disable:4200)

#define MATH_API

namespace ed
{
// Билинейная интерполяция
MATH_API void bilinear_interpolation(
	float u, float v,
	int countU, int countV,
	int& iu0, int& iu1, int& iv0, int& iv1,
	float& w00, float& w01, float& w10, float& w11, 
	bool bUtile=true, bool bVtile=true);
// Линейная интерполяция
MATH_API void interpolation(
	float u, float v,
	int countU, int countV,
	int& iu0, int& iu1, int& iv0, int& iv1,
	float& w00, float& w01, float& w10, float& w11, 
	bool bUtile=true, bool bVtile=true);

// расчет коэф. кубической интерполяции 2D
MATH_API void cubic_interpolation(
	const char* basis, 
	float x, float y, int Xsize, int Ysize, 
	int iu[4], int iv[4],
	float w[4][4], 
	bool bUtile, bool bVtile);


}

namespace ed
{
	// Простая матрица
	template <class T> class matrixMN
	{
	public:
		class row
		{
			int rowsize;
			int size;
		public:
			row(int i)
			{
				size=i;
			}
			void _row(int i)
			{
				size=i;
			}
			T& operator [](int i)
			{
				if(i<0 || i>=size) 
					throw "matrixMN: i="+ed::toString(i)+", index<0 or index>=" + ed::toString(size);
				T* array = (T*)(this+1);
				return array[i];
			}
			const T& operator [](int i) const
			{
				if(i<0 || i>=size) 
					throw "matrixMN: i="+ed::toString(i)+", index<0 or index>=" + ed::toString(size);
				const T* array = (T*)(this+1);
				return array[i];
			}
		};
		void init(int i, int j)
		{
			rowsize = i;
			size=j;
			if( data) 
				delete (char*)data;
			data = NULL;
			if( i==0 || j==0) return;
			int64_t s = rowdatasize();
			this->data = new char[ (size_t)s ];;
			memset(data, 0, (size_t)s);
			for(int ii=0; ii<j; ii++)
				(*this)[ii].row::_row(rowsize);
		}
		int64_t rowdatasize()
		{
			int64_t s = size*( sizeof(row) + rowsize*sizeof(T));
			return s;
		}
		// calc one mip level
		void filter(ed::matrixMN<T>& mip) const;
		void filter();

		static T zeroValue();
	public:
		int size, rowsize;
//		misc::ptr<char> data;
		char* data;
		row blancrow;

		// empty
		bool empty()
		{
			return data==0;
		}
		// байт на строку
		int getPitch()
		{
			return sizeof(row) + rowsize*sizeof(T);
		}
		void* getData()
		{ 
			return (void*)(data+sizeof(row));
		}
	public:
		matrixMN() : blancrow(0){data=0; init(0, 0);}
		matrixMN(const matrixMN<T>& arg) : blancrow(0)
		{
			data=0; 
			*this = arg;
		}
		matrixMN(int iX, int jY) : blancrow(0)
		{
			data=0; 
			init(iX, jY);
		}
		~matrixMN()
		{
			delete (char*) data;
		}
		bool isValid(const osg::Vec2i& p)
		{
			if( p.x()<0 || p.x()>=this->sizeX()) return false;
			if( p.y()<0 || p.y()>=this->sizeY()) return false;
			return true;
		}
		bool isValid(const osg::Vec2f& p)
		{
			if( p.x()<0 || p.x()>=1) return false;
			if( p.y()<0 || p.y()>=1) return false;
			return true;
		}
		T& operator [](const osg::Vec2i& p)
		{
			return (*this)[p.y()][p.x()];
		}
		T& operator [](const osg::Vec2f& p)
		{
			int w = this->sizeX();
			int h = this->sizeY();
			int xx = std::max( std::min( (int)(p.x()*w), w-1), 0);
			int yy = std::max( std::min( (int)(p.y()*h), h-1), 0);
			return (*this)[yy][xx];
		}
		row& operator [](int j)
		{
			if(j<0 || j>=size) 
				throw "matrixMN: i="+ed::toString(j)+", index<0 or index>=" + ed::toString(size);
			row* prow = (row*)( (char*)data + j*( sizeof(row)+rowsize*sizeof(T)));
			return *prow;
		}
		const row& operator [](int j) const
		{
			if(j<0 || j>=size) 
				throw "matrixMN: i="+ed::toString(j)+", index<0 or index>=" + ed::toString(size);
			const row* prow = (const row*)( (char*)data + j*( sizeof(row)+rowsize*sizeof(T)));
			return *prow;
		}
		matrixMN<T>& operator=(const matrixMN<T>& arg);

		int xSize()const{return rowsize;}
		int ySize()const{return size;}
		int sizeX()const{return rowsize;}
		int sizeY()const{return size;}

		// окрестности точки x y 
		void getNeibs( int x, int y, ed::vector<osg::Vec2i>& neibs);
		// окрестности точки x y крестом
		void getNeibsCrost( int x, int y, ed::vector<osg::Vec2i>& neibs);

		// 
		T* down(int iy, int ix)
		{
			if(iy>=size-1) return NULL;
			return &(*this)[iy+1][ix];
		}
		T* top(int iy, int ix)
		{
			if(iy==0) return NULL;
			return &(*this)[iy-1][ix];
		}
		T* rigth(int iy, int ix)
		{
			if(ix>=rowsize-1) return NULL;
			return &(*this)[iy][ix+1];
		}
		T* left(int iy, int ix)
		{
			if(ix==0) return NULL;
			return &(*this)[iy][ix-1];
		}

		//
		T* toprigth(int iy, int ix)
		{
			if(iy==0) return NULL;
			if(ix>=rowsize-1) return NULL;
			return &(*this)[iy-1][ix+1];
		}
		T* downrigth(int iy, int ix)
		{
			if(iy>=size-1) return NULL;
			if(ix>=rowsize-1) return NULL;
			return &(*this)[iy+1][ix+1];
		}
		T* topleft(int iy, int ix)
		{
			if(iy==0) return NULL;
			if(ix==0) return NULL;
			return &(*this)[iy-1][ix-1];
		}
		T* downleft(int iy, int ix)
		{
			if(iy>=size-1) return NULL;
			if(ix==0) return NULL;
			return &(*this)[iy+1][ix-1];
		}

		// Заполнить одинаковыми значениями
		void set(T value);
		// Мин. и макс. значение
		void GetMinMax(
			T& maxheigth, 
			T& minheigth);

		// точка с макс. значением
		T GetMaxPoint( int& x, int& y);
		// точка с минимальным значением
		T GetMinPoint( int& x, int& y);
		// Среднее значение
		T GetAverage();

	// set
	public:
		void set(float x, float y, const T& value);
		void add(float x, float y, const T& value);
		void add_interpolation(float x, float y, const T& value);
	// Интерполяция
	public:
		T nointerpolation(float x, float y, bool bUtile=false, bool bVtile=false) const;
		T interpolation(float x, float y, bool bUtile=false, bool bVtile=false) const;
		T bilinear_interpolation(float x, float y, bool bUtile=false, bool bVtile=false) const;
		T cubic_interpolation(float x, float y, bool bUtile=false, bool bVtile=false) const;
	// градиент (для float)
	public:
		osg::Vec2f gradient(float x, float y, bool bUtile=false, bool bVtile=false);
	};
}

namespace ed
{
	// Заполнить одинаковыми значениями
	template <class T> 
	void matrixMN<T>::set(T value)
	{
		int xsize = rowsize;
		int ysize = size;
		int iy, ix;
		for(iy = 0; iy<ysize; iy++)
			for(ix = 0; ix<xsize; ix++)
				(*this)[iy][ix] = value;
	}

	template <class T> 
	matrixMN<T>& matrixMN<T>::operator=(const matrixMN<T>& arg)
	{
		if( data)
			delete (char*) data;
		data = NULL;
		init( arg.rowsize, arg.size);
		int xsize = rowsize;
		int ysize = size;
		int iy, ix;
		for(iy = 0; iy<ysize; iy++)
			for(ix = 0; ix<xsize; ix++)
				(*this)[iy][ix] = arg[iy][ix];
		return *this;
	}

	// Мин. и макс. значение
	template <class T> 
	void matrixMN<T>::GetMinMax(
		T& maxheigth, 
		T& minheigth)
	{
		int xsize = rowsize;
		int ysize = size;
		maxheigth = (*this)[0][0];
		minheigth = (*this)[0][0];
		int iy, ix;
		for(iy = 0; iy<ysize; iy++)
		{
			for(ix = 0; ix<xsize; ix++)
			{
				T cur = (*this)[iy][ix];
				if( cur>maxheigth) maxheigth = cur;
				if( cur<minheigth) minheigth = cur;
			}
		}
	}
	// точка с макс. значением
	template <class T> 
	T matrixMN<T>::GetMaxPoint(
		int& x, int& y)
	{
		int xsize = rowsize;
		int ysize = size;
		x = y = 0;
		T res = (*this)[0][0];
		int iy, ix;
		for(iy = 0; iy<ysize; iy++)
		{
			for(ix = 0; ix<xsize; ix++)
			{
				T cur = (*this)[iy][ix];
				if( cur>res) 
				{
					res = cur;
					x = ix;
					y = iy;
				}
			}
		}
		return res;
	}
	// точка с минимальным значением
	template <class T> 
	T matrixMN<T>::GetMinPoint( int& x, int& y)
	{
		int xsize = rowsize;
		int ysize = size;
		x = y = 0;
		T res = (*this)[0][0];
		int iy, ix;
		for(iy = 0; iy<ysize; iy++)
		{
			for(ix = 0; ix<xsize; ix++)
			{
				T cur = (*this)[iy][ix];
				if( cur<res) 
				{
					res = cur;
					x = ix;
					y = iy;
				}
			}
		}
		return res;
	}

	// Среднее значение
	template <class T> 
	T matrixMN<T>::GetAverage()
	{
		int xsize = rowsize;
		int ysize = size;
		T t = (*this)[0][0];
		int iy, ix;
		for(iy = 0; iy<ysize; iy++)
		{
			for(ix = 0; ix<xsize; ix++)
			{
				if(iy==0 && ix==0) continue;
				T cur = (*this)[iy][ix];
				t += cur;
			}
		}
		t = t/(ysize*xsize);
		return t;
	}

	template <class T> 
	void matrixMN<T>::set(float fx, float fy, const T& value)
	{
		int w = xSize();
		int h = ySize();
		int x = (int)( fx*w - 0.5f);
		int y = (int)( fy*h - 0.5f);
		if(x<0 || x>=w) return;
		if(y<0 || y>=h) return;
		(*this)[y][x] = value;
	}
	template <class T> 
	void matrixMN<T>::add(float fx, float fy, const T& value)
	{
		int w = xSize();
		int h = ySize();
		int x = (int)( fx*w - 0.5f);
		int y = (int)( fy*h - 0.5f);
		if(x<0 || x>=w) return;
		if(y<0 || y>=h) return;
		(*this)[y][x] += value;
	}
	template <class T> 
	void matrixMN<T>::add_interpolation(float x, float y, const T& value)
	{
		x -= 0.5f/xSize();
		y -= 0.5f/ySize();
		int iu0, iu1, iv0, iv1;
		float w00, w01, w10, w11;
		ed::interpolation(x, y, xSize(), ySize(), 
			iu0, iu1, iv0, iv1, 
			w00, w01, w10, w11,
			false, false);
		(*this)[iv0][iu0] += value*w00;
		(*this)[iv1][iu0] += value*w01;
		(*this)[iv0][iu1] += value*w10;
		(*this)[iv1][iu1] += value*w11;
	}

	template <class T> 
	T matrixMN<T>::nointerpolation(float x, float y, bool bUtile, bool bVtile) const
	{
		x -= 0.5f/xSize();
		y -= 0.5f/ySize();
		x *= xSize();
		y *= ySize();

		int ix = x;
		int iy = y;
		ix = std::min( std::max(ix, 0), xSize()-1);
		iy = std::min( std::max(iy, 0), ySize()-1);

		return (*this)[iy][ix];
	}

	template <class T> 
	T matrixMN<T>::interpolation(float x, float y, bool bUtile, bool bVtile) const
	{
		x -= 0.5f/xSize();
		y -= 0.5f/ySize();
		int iu0, iu1, iv0, iv1;
		float w00, w01, w10, w11;
		ed::interpolation(x, y, xSize(), ySize(), 
			iu0, iu1, iv0, iv1, 
			w00, w01, w10, w11,
			bUtile, bVtile);
		T res = (T)(
			(*this)[iv0][iu0]*w00 + 
			(*this)[iv1][iu0]*w01 + 
			(*this)[iv0][iu1]*w10 + 				
			(*this)[iv1][iu1]*w11);
		return res;
	}
	
	template <class T> 
	T matrixMN<T>::bilinear_interpolation(float x, float y, bool bUtile, bool bVtile) const
	{
		x -= 0.5f/xSize();
		y -= 0.5f/ySize();
		int iu0, iu1, iv0, iv1;
		float w00, w01, w10, w11;
		ed::bilinear_interpolation(x, y, xSize(), ySize(), 
			iu0, iu1, iv0, iv1, 
			w00, w01, w10, w11,
			bUtile, bVtile);
		T res = 
			(*this)[iv0][iu0]*w00 + 
			(*this)[iv1][iu0]*w01 + 
			(*this)[iv0][iu1]*w10 + 				
			(*this)[iv1][iu1]*w11;
		return res;
	}

	/**
	 * Специализация шаблонного метода для osg::Vec4ub, чтоб не терять точность
	 */
	template <> 
	inline osg::Vec4ub matrixMN<osg::Vec4ub>::bilinear_interpolation(float x, float y, bool bUtile, bool bVtile) const
	{
		x -= 0.5f / xSize();
		y -= 0.5f / ySize();
		
		int iu0, iu1, iv0, iv1;
		float w00, w01, w10, w11;

		ed::bilinear_interpolation(x, y, xSize(), ySize(), 
			iu0, iu1, iv0, iv1, 
			w00, w01, w10, w11,
			bUtile, bVtile);

		const osg::Vec4ub& v00 = (*this)[iv0][iu0];
		const osg::Vec4ub& v01 = (*this)[iv1][iu0];
		const osg::Vec4ub& v10 = (*this)[iv0][iu1];
		const osg::Vec4ub& v11 = (*this)[iv1][iu1];

		// Чтоб не терять точность сначала все складываем во флотах и потом только один раз кастим к unsigned char
		unsigned char r = (unsigned char)(w00 * v00.r() + w01 * v01.r() + w10 * v10.r() + w11 * v11.r());
		unsigned char g = (unsigned char)(w00 * v00.g() + w01 * v01.g() + w10 * v10.g() + w11 * v11.g());
		unsigned char b = (unsigned char)(w00 * v00.b() + w01 * v01.b() + w10 * v10.b() + w11 * v11.b());
		unsigned char a = (unsigned char)(w00 * v00.a() + w01 * v01.a() + w10 * v10.a() + w11 * v11.a());

		return osg::Vec4ub(r, g, b, a);
	}

	template <class T> 
	T matrixMN<T>::cubic_interpolation(float x, float y, bool bUtile, bool bVtile) const
	{
		x -= 0.5f/xSize();
		y -= 0.5f/ySize();

		int iu[4], iv[4];
		float w[4][4];
		ed::cubic_interpolation(
			"catmull-rom", 
			x, y, xSize(), ySize(), 
			iu, iv,
			w, bUtile, bVtile);
		T res;
		for(int x=0; x<4; x++)
		{
			for(int y=0; y<4; y++)
			{
				int U = iu[x];
				int V = iv[y];
				T val = (*this)[V][U];

				if(x==0 && y==0)
					res = val*w[x][y];
				else
					res += val*w[x][y];
			}
		}
		return res;
	}
	template <class T> 
	osg::Vec2f matrixMN<T>::gradient(float x, float y, bool bUtile, bool bVtile)
	{
		osg::Vec2f gradient(0, 0);
		float dx = 0.5f/xSize();
		float dy = 0.5f/ySize();

		T v = interpolation(x, y, bUtile, bVtile);
		T vx = interpolation(x-dx, y, bUtile, bVtile);
		T vX = interpolation(x+dx, y, bUtile, bVtile);
		T vy = interpolation(x, y-dy, bUtile, bVtile);
		T vY = interpolation(x, y+dy, bUtile, bVtile);
		T Dx = (vX-vx);
		T Dy = (vY-vy);
		gradient.x() = Dx;
		gradient.y() = Dy;
		gradient.normalize();
		return gradient;
	}

	// окрестности точки x y 
	template <class T> 
	void matrixMN<T>::getNeibs( int x, int y, ed::vector<osg::Vec2i>& neibs)
	{
		int w = this->sizeX();
		int h = this->sizeY();

		neibs.clear();
		for(int i=-1; i<2; i++)
		{
			if(x+i<0 || x+i>=w )
				continue;
			for(int j=-1; j<2; j++)
			{
				if(i==0 && j==0)
					continue;
				if(y+j<0 || y+j>=h )
					continue;

				neibs.push_back( osg::Vec2i(x+i, y+j) );
			}
		}

	}
	// окрестности точки x y крестом
	template <class T> 
	void matrixMN<T>::getNeibsCrost( int x, int y, ed::vector<osg::Vec2i>& neibs)
	{
		int w = this->sizeX();
		int h = this->sizeY();

		neibs.clear();
		for(int i=-1; i<2; i++)
		{
			if(x+i<0 || x+i>=w )
				continue;
			for(int j=-1; j<2; j++)
			{
				if(i==0 && j==0)
					continue;
				if(y+j<0 || y+j>=h )
					continue;
				if( abs(j)+abs(i) >1 )
					continue;

				neibs.push_back( osg::Vec2i(x+i, y+j) );
			}
		}

	}
	template <class T> 
	inline T matrixMN<T>::zeroValue(){return T();}
	template <> 
	inline float matrixMN<float>::zeroValue(){return 0;}
	template <> 
	inline int matrixMN<int>::zeroValue(){return 0;}
	template <> 
	inline uint32_t matrixMN<uint32_t>::zeroValue(){return 0;}
	template <> 
	inline double matrixMN<double>::zeroValue(){return 0;}


	// calc one mip level
	template <class T> 
	void matrixMN<T>::filter( ed::matrixMN<T>& mfilter) const
	{
		int w = this->sizeX();
		int h = this->sizeY();
		w/=2;
		h/=2;
		mfilter.init(w, h);

		for(int y=0; y<h; y++)
		{
			for(int x=0; x<w; x++)
			{
				T v = zeroValue();
				v += (*this)[y*2+0][x*2+0];
				v += (*this)[y*2+0][x*2+1];
				v += (*this)[y*2+1][x*2+0];
				v += (*this)[y*2+1][x*2+1];
				v *= 0.25f;
				mfilter[y][x] = v;
			}
		}
	}
	template <class T> 
	void matrixMN<T>::filter()
	{
		ed::matrixMN<T> mfilter;
		this->filter(mfilter);
		*this = mfilter;
	}

}

/**
 * Write/Load components to/from a stream.
 */
template<class T, class Stream> inline
Stream& operator >> (Stream& out, ed::matrixMN<T>& v)
{
	if(out.getMode() == Stream::READ)
	{
		int x, y;
		ed::vector<T> data;
		out>>x>>y;
		out>>data;
		v.init(y, x);

		int w = v.sizeX();
		int h = v.sizeY();

		for(int y=0; y<h; y++)
		{
			for(int x=0; x<w; x++)
			{
				T color = data[y*w+x];
				v[y][x] = color;
			}
		}
	}
	else
	{
		out>>v.size>>v.rowsize;

		int w = v.sizeX();
		int h = v.sizeY();

		ed::vector<T> data(w*h);
		for(int y=0; y<h; y++)
		{
			for(int x=0; x<w; x++)
			{
				T color = v[y][x];
				data[y*w+x] = color;
			}
		}
		out>>data;
	}
	return out;
}

#endif
