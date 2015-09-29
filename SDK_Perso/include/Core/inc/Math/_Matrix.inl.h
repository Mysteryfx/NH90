#ifndef _ED_Math_Matrix_h_
#error Matrix.inl.h should not be included directly
#endif


// implementation
#ifdef ED_MATH_IMPLEMENT_MATRIX

template<int N, typename T>
Matrix<N,T>& Matrix<N,T>::set(T val)
{
	for(int i=0; i<N; ++i)
		for(int j=0; j<N; ++j)
			m[i][j] = i==j ? val : T(0);
	return *this;
}

template<int N, class T>
Matrix<N,T>& Matrix<N,T>::transpose()
{
	for(int i=1; i<N; ++i)
	{
		for(int j=0; j<i; ++j)
		{
			T tmp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = tmp;
		}
	}
	return *this;
}

template<int N, typename T>
ED_MATH_EXTERN void ED_MATH_API transpose(Matrix<N,T>& r, const Matrix<N,T>& m)
{
	for(int i=0; i<N; ++i)
		for(int j=0; j<N; ++j)
			r[j][i] = m[i][j];
}

// matrix by matrix
template<int N, typename T>
ED_MATH_EXTERN void ED_MATH_API multiply(Matrix<N,T>& r, const Matrix<N,T>& m1, const Matrix<N,T>& m2)
{
	for(int i=0; i<N; ++i)
	{
		for(int j=0; j<N; ++j)
		{
			r[i][j] = m1[i][0] * m2[0][j];
			for(int k=1; k<N; ++k)
				r[i][j] += m1[i][k] * m2[k][j];
		}
	}
}

template<typename T>
ED_MATH_EXTERN void ED_MATH_API invert(Matrix<2,T>& r, const Matrix<2,T>& m)
{
	// TBI
}

template<typename T>
ED_MATH_EXTERN void ED_MATH_API invert(Matrix<3,T>& r, const Matrix<3,T>& m)
{
	// TBI
}

template<typename T>
ED_MATH_EXTERN void ED_MATH_API invert(Matrix<4,T>& r, const Matrix<4,T>& m)
{
	// TBI
}

// invert orthogonal matrix
template<int N, typename T>
ED_MATH_EXTERN void ED_MATH_API invertOrtho(Matrix<N,T>& r, const Matrix<N,T>& m)
{
	for(int i=0; i<N-1; ++i)
	{
		for(int j=0; j<N-1; ++j)
			r[j][i] = m[i][j];
		r[i][N-1] = T(0);
		r[N-1][i] = T(0);
	}

	for(int i=0; i<N-1; ++i)
	{
		T l2 = length2( r.m[i] );
		for(int j=0; j<N-1; ++j)
			r[i][j] /= l2;
	}

	r.col(N-1) = -(r * Vector<N,T>(m.col(N-1)));
	r.m[N-1][N-1] = T(1);
}

#endif
