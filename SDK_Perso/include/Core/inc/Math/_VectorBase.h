#ifndef _ED_Math_VectorBase_h_
#define _ED_Math_VectorBase_h_


namespace Math {

template<int N, class T> struct _VectorBase;

template<class T> struct _VectorBase<1,T> {
	union {
		T x;
		T v[1];
	};
};

template<class T> struct _VectorBase<2,T> {
	union {
		struct { T x, y; };
		T v[2];
	};
};

template<class T> struct _VectorBase<3,T> {
	union {
		struct { T x, y, z; };
		T v[3];
	};
};

template<class T> struct _VectorBase<4,T> {
	union {
		struct { T x, y, z, w; };
		T v[4];
	};
};

}

#endif /* _ED_Math_VectorBase_h_ */
