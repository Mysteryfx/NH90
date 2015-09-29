#ifndef _ED_Math_My_Matrix_h_
#define _ED_Math_My_Matrix_h_

#include "./_config.h"
#include "ed_string.h"

#include <ed/vector.h>
#include <ed/string.h>
#include <math.h>
#include <assert.h>

template <typename T> class nMatrix;

template <typename T> ED_MATH_EXTERN nMatrix<T> operator *(const nMatrix<T>& Left, T Right);
template <typename T> ED_MATH_EXTERN nMatrix<T> operator *(T Left, const nMatrix<T>& Right);
template <typename T> ED_MATH_EXTERN nMatrix<T> operator *(const nMatrix<T>& Left, const nMatrix<T>& Right);
template <typename T> ED_MATH_EXTERN nMatrix<T> operator +(const nMatrix<T>& Left, const nMatrix<T>& Right);
template <typename T> ED_MATH_EXTERN nMatrix<T> operator -(const nMatrix<T>& Left, const nMatrix<T>& Right);

template <typename T> ED_MATH_EXTERN void print_matrix(const nMatrix<T> & matrix);
template <typename T> ED_MATH_EXTERN void fprint_matrix(const char* file_name, const char* open_method, const char* caption, const nMatrix<T>& matrix, const char* num_template);

template <typename T>
class ED_MATH_EXTERN nMatrix
{
public:
	nMatrix()
	{
	}

	nMatrix(unsigned int size)
	{
		resize(size);
	}

	nMatrix(unsigned int row, unsigned int col)
	{
		resize(row, col);
	}

	nMatrix(unsigned int size, T data[])
	{
		init(size, size, data);
	}

	nMatrix(unsigned int row, unsigned int col, T data[])
	{
		init(row, col, data);
	}

	nMatrix(const nMatrix<T>& Right)
	{
		(*this) = Right;
	}

	void init(unsigned int size, T data[])
	{
		init(size, size, data);
	}

	void init(unsigned int row, unsigned int col, T data[])
	{
		resize(row, col);
		for (unsigned int cnt_row = 0; cnt_row < row; cnt_row++)
		{
			for (unsigned int cnt_col = 0; cnt_col < col; cnt_col++)
				(*this)[cnt_row][cnt_col] = T(data[cnt_row * col + cnt_col]);
		}
	}
	
	ed::vector<T>& operator [](unsigned int i) 
	{ 
		return data[i]; 
	}
	
	const ed::vector<T>& operator [](unsigned int i) const 
	{ 
		return data[i];
	}
	
	void diag(unsigned int size, T data[])
	{
		resize(size, size);
		for (unsigned int cnt = 0; cnt < size; cnt++)
			(*this)[cnt][cnt] = T(data[cnt]);
	}
	
	void diag(unsigned int size, T value)
	{
		resize(size, size);
		for (unsigned int cnt = 0; cnt < size; cnt++)
			(*this)[cnt][cnt] = T(value);
  	}
	
	void cov(unsigned int size, T data[])
	{
		resize(size, size);
		for (unsigned int i = 0; i < size; i++)
		{
			for (unsigned int j = 0; j < size; j++)
				(*this)[i][j] = T(sqrt(data[i] * data[j]));
		}
	}
	
	unsigned int Rows() const {return (unsigned int)data.size();}
	unsigned int Cols() const {return (unsigned int)(!data.empty() ? data[0].size() : 0);}
	
	//
	void clear();
	void null();
	bool isEmpty() const;
	void resize(unsigned int size);
	void resize(unsigned int row, unsigned int col);
	void removeRow(unsigned int row);
	void removeCol(unsigned int col);
	void remove(unsigned int row, unsigned int col);
	void transpose();
	void invertByCofactors();
	void invertByLU();
	
	//
	nMatrix<T> getTranspose() const;
	nMatrix<T> getExtract(unsigned int row, unsigned int col) const;
	double getCofactor(unsigned int row, unsigned int col) const;
	double getDeterminantByLeibniz() const;
	double getDeterminantByLU() const;
	//
	
	nMatrix<T>& operator =(const nMatrix<T>& Right)
	{
		resize(Right.Rows(), Right.Cols());
		for (unsigned int row_it = 0; row_it < Rows(); row_it++)
		{
			for (unsigned int col_it = 0; col_it < Cols(); col_it++)
				(*this)[row_it][col_it] = T(Right[row_it][col_it]);
		}

		return *(this);
	}
	
	nMatrix<T>& operator *=(T Right)
	{
		(*this) = (*this) * Right;
		return (*this);
	}
	
	nMatrix<T>& operator *=(const nMatrix<T>& Right)
	{
		(*this) = (*this) * Right;
		return (*this);
	}
	
	nMatrix<T>& operator +=(const nMatrix<T>& Right)
	{
		(*this) = (*this) + Right;
		return (*this);
	}
	
	nMatrix<T>& operator -=(const nMatrix<T>& Right)
	{
		(*this) = (*this) - Right;
		return (*this);
	}

	friend ED_MATH_EXTERN nMatrix<T> operator *<>(const nMatrix<T>& Left, T Right);
	friend ED_MATH_EXTERN nMatrix<T> operator *<>(T Left, const nMatrix<T>& Right);
	friend ED_MATH_EXTERN nMatrix<T> operator *<>(const nMatrix<T>& Left, const nMatrix<T>& Right);
	friend ED_MATH_EXTERN nMatrix<T> operator +<>(const nMatrix<T>& Left, const nMatrix<T>& Right);
	friend ED_MATH_EXTERN nMatrix<T> operator -<>(const nMatrix<T>& Left, const nMatrix<T>& Right);

private:
	ed::vector< ed::vector<T> > data;
};

template <typename T> 
inline void nMatrix<T>::clear()
{
	data.clear();
}

template <typename T> 
inline void nMatrix<T>::null()
{
	for (unsigned int row_it = 0; row_it < Rows(); row_it++)
	{
		for (unsigned int col_it = 0; col_it < Cols(); col_it++)
			(*this)[col_it][row_it] = T(0);
	}
}


template <typename T> 
inline bool nMatrix<T>::isEmpty() const
{
	return data.empty();
}

template <typename T> 
inline void nMatrix<T>::resize(unsigned int size)
{
	resize(size, size);
}

template <typename T> 
inline void nMatrix<T>::resize(unsigned int row, unsigned int col)
{
	data.resize(row);
	for(unsigned int i = 0; i < Rows(); i++)
	{
		data[i].resize(col);
	}
	//for (iterator it = begin(); it != end(); it++)
	//	it->resize(col);
}

template <typename T> 
inline void nMatrix<T>::removeRow(unsigned int row)
{
	if (isEmpty() || row < 0 || row >= Rows())
		return;

	data.erase(data.begin() + row);
}

template <typename T> 
inline void nMatrix<T>::removeCol(unsigned int col)
{
	if (isEmpty() || col < 0 || col >= Cols())
		return;

	for(unsigned int i = 0; i < Rows(); i++)
	{
		data[i].erase(data[i].begin() + col);
	}
	//for (iterator it = begin();	it != end(); it++)
	//	it->erase(it->begin() + col);
}

template <typename T> 
inline void nMatrix<T>::remove(unsigned int row, unsigned int col)
{
	if (isEmpty())
		return;

	removeRow(row);
	removeCol(col);
}

template <typename T> 
inline void nMatrix<T>::transpose()
{
	nMatrix<T> result = getTranspose();
	(*this) = result;
}

template <typename T> 
inline void nMatrix<T>::invertByCofactors()
{
	getInverseByCofactors(*this);
}

template <typename T> 
inline void nMatrix<T>::invertByLU()
{
	getInverseByLU(*this);
}

template <typename T> 
inline nMatrix<T> nMatrix<T>::getTranspose() const
{
	nMatrix<T> result;
	result.resize(Cols(), Rows());
	for (unsigned int row_it = 0; row_it < Rows(); row_it++)
	{
		for (unsigned int col_it = 0; col_it < Cols(); col_it++)
			result[col_it][row_it] = (*this)[row_it][col_it];
	}

	return result;
}

template <typename T> 
inline nMatrix<T> nMatrix<T>::getExtract(unsigned int row, unsigned int col) const
{
	nMatrix<T> result = (*this);
	result.remove(row, col);
	return result;
}

template <typename T>
inline double nMatrix<T>::getCofactor(unsigned int row, unsigned int col) const
{
	if (isEmpty())
		return 0.0;

	if (Cols() != Rows())
	{
		assert(0);
		return 0.0;
	}

	if (Cols() < 2)
		return 0.0;

	double Minor = getExtract(row, col).getDeterminantByLeibniz();
	int sign = fmod(double(row + col + 1), 2.0) == 0.0 ? -1 : 1;
	return Minor * sign;
}

// Adjugate matrix == transpose of matrix of cofactors
// Cofactor of a square matrix is (-1) ^ (i + j) times the corresponding minor
template <typename T>
inline bool getInverseByCofactors(nMatrix<T>& A)
{
	if (A.Rows() < 2)
		return true; // ?
	if(A.Cols() != A.Rows())
	{
		assert(0);
		return true;
	}
	
	double det = A.getDeterminantByLeibniz();
	if (det == 0.0)
		return false;
	
	nMatrix<double> Result;
	Result.resize(A.Rows());

	for (unsigned int row_it = 0; row_it < A.Rows(); row_it++)
	{
		for (unsigned int col_it = 0; col_it < A.Cols(); col_it++)
			Result[col_it][row_it] = T(A.getCofactor(row_it, col_it) / det);
	}

	A = Result;

	return true;
}

// ED, 2008
// This is an adaptation of c++ 'rmatrixinverse' routine from ALGLIB project
//
// Inversion of a general matrix.
//
// Input parameters:
//     A   -   matrix. Array whose indexes range within [0..N-1, 0..N-1].
//
// Output parameters:
//     A   -   inverse of matrix A.
//             Array whose indexes range within [0..N-1, 0..N-1].
//
// Result:
//     True, if the matrix is not singular.
//     False, if the matrix is singular.
//
// -- ALGLIB --
//    Copyright 2005 by Bochkanov Sergey
template <typename T>
inline bool getInverseByLU(nMatrix<T>& A)
{
	ed::vector<unsigned int> Pivots;
	getLUdecomposition(A, Pivots);
    return getInverseFromLU(A, Pivots);
}

// ED, 2008
// This is an adaptation of c++ 'rmatrixluinverse' routine from ALGLIB project
//
// Inversion of a matrix given by its LU decomposition.
//
// Input parameters:
//     A       -   LU decomposition of the matrix (output of RMatrixLU subroutine).
//     Pivots  -   table of permutations which were made during the LU decomposition
//                (the output of RMatrixLU subroutine).
//
// Output parameters:
//     A       -   inverse of matrix A.
//                 Array whose indexes range within [0..N-1, 0..N-1].
//
// Result:
//     True, if the matrix is not singular.
//     False, if the matrix is singular.
//
// -- LAPACK routine (version 3.0) --
//    Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
//    Courant Institute, Argonne National Lab, and Rice University
//    February 29, 1992
template <typename T>
inline bool getInverseFromLU(nMatrix<T>& A, const ed::vector<unsigned int> & Pivots)
{
	ed::vector<double> work;
    unsigned int i, j, jp, i_;
    double v;

    // Quick return if possible
	if (A.Rows() == 0)
        return true;

	work.resize(A.Rows());
    
    // Form inv(U)
    if (!getInverseTriangular(A, true, false))
        return false;
    
    // Solve the equation inv(A)*L = inv(U) for inv(A).
    for (j = A.Rows() - 1; j >= 0; j--)
    {
        // Copy current column of L to WORK and replace with zeros.
        for (i = j + 1; i <= A.Rows() - 1; i++)
        {
            work[i] = A[i][j];
            A[i][j] = 0;
        }
        
        // Compute current column of inv(A).
        if (j < A.Rows() - 1)
        {
            for (i = 0; i <= A.Rows() - 1; i++)
            {
				v = 0.0;
				// dot product
				for (i_ = 0; i_ < A.Rows() - j - 1; i_++)
					v += A[i][j + 1 + i_] * work[j + 1 + i_];

                A[i][j] = A[i][j] - v;
            }
        }
		if(j == 0)
		{
			break;
		}
    }
    
    // Apply column interchanges.
    for (j = A.Rows() - 2; j >= 0; j--)
    {
        jp = Pivots[j];
        if (jp != j)
        {
			for (i_ = 0; i_ < A.Rows(); i_++)
				std::swap(A[i_][j], A[i_][jp]);
        }
		if(j == 0)
		{
			break;
		}
    }

    return true;
}

// ED, 2008
// This is an adaptation of c++ 'rmatrixtrinverse' routine from ALGLIB project
//
// Triangular matrix inversion
//
// The subroutine inverts the following types of matrices:
//     * upper triangular
//     * upper triangular with unit diagonal
//     * lower triangular
//     * lower triangular with unit diagonal
//
// In case of an upper (lower) triangular matrix,  the  inverse  matrix  will
// also be upper (lower) triangular, and after the end of the algorithm,  the
// inverse matrix replaces the source matrix. The elements  below (above) the
// main diagonal are not changed by the algorithm.
//
// If  the matrix  has a unit diagonal, the inverse matrix also  has  a  unit
// diagonal, and the diagonal elements are not passed to the algorithm.
//
// Input parameters:
//     A       -   matrix.
//                 Array whose indexes range within [0..N-1, 0..N-1].
//     IsUpper -   True, if the matrix is upper triangular.
//     IsUnitTriang
//             -   True, if the matrix has a unit diagonal.
//
// Output parameters:
//     A       -   inverse matrix (if the problem is not degenerate).
//
// Result:
//     True, if the matrix is not singular.
//     False, if the matrix is singular.
//
// -- LAPACK routine (version 3.0) --
//    Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
//    Courant Institute, Argonne National Lab, and Rice University
//    February 29, 1992
template <typename T>
inline bool getInverseTriangular(nMatrix<T> & A, bool IsUpper, bool IsUnitTriang)
{
    bool NoUnit;
    unsigned int i, j, i_;
    double v, ajj;
	ed::vector<double> t;
	t.resize(A.Rows());
    
    // Test the input parameters.
    NoUnit = !IsUnitTriang;
    if (IsUpper)
    {
        // Compute inverse of upper triangular matrix.
        for (j = 0; j <= A.Rows() - 1; j++)
        {
            if (NoUnit)
            {
                if (A[j][j] == 0)
                    return false;

                A[j][j] = 1 / A[j][j];
                ajj = -A[j][j];
            }
            else
                ajj = -1;
            
            // Compute elements 1:j-1 of j-th column.
            if (j > 0)
            {
				for (i_ = 0; i_ < j; i_++)
					t[i_] = A[i_][j];

                for (i = 0; i <= j - 1; i++)
                {
					v = 0.0;
                    if (i < j - 1)
					{
						// dot product
						for (i_ = 0; i_ < j - i - 1; i_++)
							v += A[i][i + 1 + i_] * t[i + 1 + i_];
					}
                    
					if (NoUnit)
                        A[i][j] = v + A[i][i] * t[i];
                    else
                        A[i][j] = v + t[i];
                }

				for (i_ = 0; i_ < j; i_++)
					A[i_][j] *= ajj;
            }
        }
    }
    else
    {
        // Compute inverse of lower triangular matrix.
        for (j = A.Rows() - 1; j >= 0; j--)
        {
            if (NoUnit)
            {
                if (A[j][j] == 0)
                    return false;

                A[j][j] = 1 / A[j][j];
                ajj = -A[j][j];
            }
            else
                ajj = -1;

            if (j < A.Rows() - 1)
            {
                // Compute elements j+1:n of j-th column.
				for (i_ = j + 1; i_ < A.Rows(); i_++)
					t[i_] = A[i_][j];

                for (i = j + 1; i <= A.Rows() - 1; i++)
                {
					v = 0.0;
                    if (i > j + 1)
					{
						// dot product
						for (i_ = 0; i_ < i - j - 1; i_++)
							v += A[i][j + 1 + i_] * t[j + 1 + i_];
					}
                    
                    if (NoUnit)
                        A[i][j] = v + A[i][i] * t[i];
                    else
                        A[i][j] = v + t[i];
                }

				for (i_ = j + 1; i_ < A.Rows(); i_++)
					A[i_][j] *= ajj;
            }
        }
    }

    return true;
}

template <typename T>
inline double nMatrix<T>::getDeterminantByLeibniz() const
{
	if (isEmpty() || Cols() != Rows())
		return 0.0;

	if (Rows() == 1)
	{
		return (*this)[0][0];
	}
	else if (Rows() == 2)
	{
		return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
	}
	else
	{
		double result = 0.0;
		for (unsigned int cnt = 0; cnt < Rows(); cnt++)
		{
			int sign = fmod(double(cnt), 2.0) == 0 ? 1 : -1;
			result += sign * (*this)[0][cnt] * getExtract(0, cnt).getDeterminantByLeibniz();
		}

		return result;
	}
}

// ED, 2008
// This is an adaptation of c++ 'rmatrixludet' routine from ALGLIB project
//
// Determinant calculation of the matrix given by its LU decomposition.
//
// Input parameters:
//     A       -   LU decomposition of the matrix (output of
//                 RMatrixLU subroutine).
//     Pivots  -   table of permutations which were made during
//                 the LU decomposition.
//                 Output of RMatrixLU subroutine.
//
// Result: matrix determinant.
//
// -- ALGLIB --
//    Copyright 2005 by Bochkanov Sergey
template <typename T>
inline double getDeterminantFromLU(const nMatrix<T>& A, const ed::vector<unsigned int> & Pivots)
{
    double result;
    unsigned int i;
	int s;

    result = 1.0;
    s = 1;
	for (i = 0; i <= A.Rows() - 1; i++)
    {
        result *= A[i][i];
        if (Pivots[i] != i)
            s = -s;
    }

    return result * s;
}

// ED, 2008
// This is an adaptation of c++ 'rmatrixdet' routine from ALGLIB project
//
// Calculation of the determinant of a general matrix
//
// Input parameters:
//     A       -   matrix, array[0..N-1, 0..N-1]
//
// Result: determinant of matrix A.
//
// -- ALGLIB --
//    Copyright 2005 by Bochkanov Sergey
template <typename T>
inline double nMatrix<T>::getDeterminantByLU() const
{
	nMatrix<double> A = (*this);
    ed::vector<unsigned int> Pivots;

	getLUdecomposition(A, Pivots);
    return getDeterminantFromLU(A, Pivots);
}

// ED, 2008
// This is an adaptation of c++ 'rmatrixlu' routine from ALGLIB project
//
// LU decomposition of a general matrix of size MxN
//
// The subroutine calculates the LU decomposition of a rectangular general
// matrix with partial pivoting (with row permutations).
//
// Input parameters:
//    A   -   matrix A whose indexes range within [0..M-1, 0..N-1].
//
// Output parameters:
//    A   -   matrices L and U in compact form (see below).
//            Array whose indexes range within [0..M-1, 0..N-1].
//    Pivots - permutation matrix in compact form (see below).
//            Array whose index ranges within [0..Min(M-1,N-1)].
//
// Matrix A is represented as A = P * L * U, where P is a permutation matrix,
// matrix L - lower triangular (or lower trapezoid, if M>N) matrix,
// U - upper triangular (or upper trapezoid, if M<N) matrix.
//
// Let M be equal to 4 and N be equal to 3:
//
//                    (  1          )   ( U11 U12 U13  )
// A = P1 * P2 * P3 * ( L21  1      ) * (     U22 U23  )
//                    ( L31 L32  1  )   (         U33  )
//                    ( L41 L42 L43 )
//
// Matrix L has size MxMin(M,N), matrix U has size Min(M,N)xN, matrix P(i) is
// a permutation of the identity matrix of size MxM with numbers I and Pivots[I].
//
// The algorithm returns array Pivots and the following matrix which replaces
// matrix A and contains matrices L and U in compact form (the example applies
// to M=4, N=3).
//
// ( U11 U12 U13 )
// ( L21 U22 U23 )
// ( L31 L32 U33 )
// ( L41 L42 L43 )
//
// As we can see, the unit diagonal isn't stored.
//
// -- LAPACK routine (version 3.0) --
//    Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
//    Courant Institute, Argonne National Lab, and Rice University
//    June 30, 1992
template <typename T>
inline void getLUdecomposition(nMatrix<T>& A, ed::vector<unsigned int>& Pivots)
{
	static const unsigned int lunb = 8;

    unsigned int nb = lunb;
    
    // Decide what to use - blocked or unblocked code
	if (A.Cols() <= 1 || __min(A.Rows(), A.Cols()) <= nb || nb == 1)
    {
        // Unblocked code
		getLUdecompositionNB(A, A.Rows(), A.Cols(), Pivots);
    }
    else
    {
		nMatrix<T> B;
		ed::vector<unsigned int> bp;
		unsigned int minmn, i, ip, j, j1, j2, cb, i_;
		double v;

        // Blocked code.
        // First, prepare temporary matrix and indices
		B.resize(A.Rows(), nb);
		Pivots.resize(__min(A.Rows(), A.Cols()));
        minmn = __min(A.Rows(), A.Cols());
        j1 = 0;
        j2 = __min(minmn, nb) - 1;
        
        // Main cycle
        while (j1 < minmn)
        {
            cb = j2 - j1 + 1;
            
            // LU factorization of diagonal and subdiagonal blocks:
            // 1. Copy columns J1..J2 of A to B
            // 2. LU(B)
            // 3. Copy result back to A
            // 4. Copy pivots, apply pivots

			for (i = j1; i <= A.Rows() - 1; i++)
			{
				for (i_ = 0; i_ < cb; i_++)
					B[i - j1][i_] = A[i][j1 + i_];
			}

			getLUdecompositionNB(B, A.Rows() - j1, cb, bp);

			for (i = j1; i <= A.Rows() - 1; i++)
			{
				for (i_ = 0; i_ < j2 - j1 + 1; i_++)
					A[i][j1 + i_] = B[i - j1][i_];
			}

            for (i = 0; i <= cb - 1; i++)
            {
                ip = bp[i];
				Pivots[j1 + i] = j1 + ip;
                if (bp[i] != i)
                {
                    if (j1 != 0)
                    {
                        // Interchange columns 0:J1-1
						for (i_ = 0; i_ < j1; i_++)
							std::swap(A[j1 + i][i_], A[j1 + ip][i_]);
                    }

                    if (j2 < A.Cols() - 1)
                    {
                        // Interchange the rest of the matrix, if needed
						for (i_ = 0; i_ < A.Cols() - j2 - 1; i_++)
							std::swap(A[j1 + i][j2 + 1 + i_], A[j1 + ip][j2 + 1 + i_]);
                    }
                }
            }
            
            // Compute block row of U
            if (j2 < A.Cols() - 1)
            {
                for (i = j1 + 1; i <= j2; i++)
                {
                    for (j = j1; j <= i - 1; j++)
                    {
                        v = A[i][j];
						for (i_ = 0; i_ < A.Cols() - 1 - j2; i_++)
							A[i][j2 + 1 + i_] -= v * A[j][j2 + 1 + i_];
                    }
                }
            }
            
            // Update trailing submatrix
            if (j2 < A.Cols() - 1)
            {
                for (i = j2 + 1; i <= A.Rows() - 1; i++)
                {
                    for (j = j1; j <= j2; j++)
                    {
                        v = A[i][j];
						for (i_ = 0; i_ < A.Cols() - 1 - j2; i_++)
							A[i][j2 + 1 + i_] -= v * A[j][j2 + 1 + i_];
                    }
                }
            }
            
            // Next step
            j1 = j2 + 1;
            j2 = __min(minmn, j1 + nb) - 1;
        }
    }
}

// ED, 2008
// This is an adaptation of c++ 'rmatrixlu2' routine from ALGLIB project
//
// Level 2 BLAS version of RMatrixLU
//
// -- LAPACK routine (version 3.0) --
//    Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
//    Courant Institute, Argonne National Lab, and Rice University
//    June 30, 1992
template <typename T>
inline void getLUdecompositionNB(nMatrix<T>& A, unsigned int m, unsigned int n, ed::vector<unsigned int>& Pivots)
{
    unsigned int i, j, jp, i_;
    ed::vector<T> t1;
    double s;

	Pivots.resize(__min(m, n));
    t1.resize(__max(m, n));
    
    // Quick return if possible
    if (m == 0 || n == 0 )
        return;

    for (j = 0; j <= __min(m - 1, n - 1); j++)
    {
        // Find pivot and test for singularity.
        jp = j;
        for (i = j + 1; i <= m - 1; i++)
        {
            if (fabs(A[i][j]) > fabs(A[jp][j]))
                jp = i;
        }

        Pivots[j] = jp;

        if (A[jp][j] != 0)
        {
            //Apply the interchange to rows
            if (jp != j)
				std::swap(A[j], A[jp]);
            
            //Compute elements J+1:M of J-th column.
            if (j < m)
            {
                jp = j + 1;
                s = 1 / A[j][j];
				for (i_ = jp; i_ < m; i_++)
                    A[i_][j] *= s;
            }
        }

        if (j < __min(m, n) - 1)
        {
            //Update trailing submatrix.
            jp = j + 1;
            for (i = j + 1; i <= m - 1; i++)
            {
                s = A[i][j];
                for (i_ = 0; i_ < n - jp; i_++)
					A[i][jp + i_] -= s * A[j][jp + i_];
            }
        }
    }

}

//outer operators

#ifdef ED_MATH_IMPLEMENT_NMATRIX

template <typename T> ED_MATH_EXTERN
nMatrix<T> operator *(const nMatrix<T>& Left, T Right)
{
	if (Left.isEmpty())
		return Left;

	nMatrix<T> result;
	result.resize(Left.Rows(), Left.Cols());
	for (unsigned int row_it = 0; row_it < Left.Rows(); row_it++)
	{
		for (unsigned int col_it = 0; col_it < Left.Cols(); col_it++)
			result[row_it][col_it] = Left[row_it][col_it] * T(Right);
	}

	return result;
}

template <typename T> ED_MATH_EXTERN
nMatrix<T> operator *(T Left, const nMatrix<T>& Right)
{
	if (Right.isEmpty())
		return Right;

	nMatrix<T> result;
	result.resize(Right.Rows(), Right.Cols());
	for (unsigned int row_it = 0; row_it < Right.Rows(); row_it++)
	{
		for (unsigned int col_it = 0; col_it < Right.Cols(); col_it++)
			result[row_it][col_it] = Right[row_it][col_it] * T(Left);
	}

	return result;
}

template <typename T> ED_MATH_EXTERN
nMatrix<T> operator *(const nMatrix<T>& Left, const nMatrix<T>& Right)
{
	if (Left.isEmpty())
		return Left;
	if (Left.Cols() != Right.Rows())
	{
		assert(0);
		return Left;
	}

	nMatrix<T> result;
	result.resize(Left.Rows(), Right.Cols());
	for (unsigned int row_it = 0; row_it < Left.Rows(); row_it++)
	{
		for (unsigned int col_it = 0; col_it < Right.Cols(); col_it++)
		{
			T value = T(0);
			for (unsigned int col_it2 = 0; col_it2 < Left.Cols(); col_it2++)
				value += Left[row_it][col_it2] * T(Right[col_it2][col_it]);

			result[row_it][col_it] += value;
		}
	}

	return result;
}

template <typename T> ED_MATH_EXTERN
nMatrix<T> operator +(const nMatrix<T>& Left, const nMatrix<T>& Right)
{
	if (Left.isEmpty())
		return Left;
	if (Left.Cols() != Right.Cols() ||
		Left.Rows() != Right.Rows())
	{
		assert(0);
		return Left;
	}

	nMatrix<T> result;
	result.resize(Left.Rows(), Left.Cols());
	for (unsigned int row_it = 0; row_it < Left.Rows(); row_it++)
	{
		for (unsigned int col_it = 0; col_it < Left.Cols(); col_it++)
			result[row_it][col_it] = Left[row_it][col_it] + Right[row_it][col_it];
	}

	return result;
}

template <typename T> ED_MATH_EXTERN
nMatrix<T> operator -(const nMatrix<T>& Left, const nMatrix<T>& Right)
{
	return Left + Right * -1.0;
}

template <typename T> ED_MATH_EXTERN
void print_matrix(const nMatrix<T>& matrix, const char* num_template = "%f ")
{
	printf("\n");
	for (unsigned int row_cnt = 0; row_cnt < matrix.Rows(); row_cnt++)
	{
		char ch[1000];
		int pos = 0;
		for (unsigned int col_cnt = 0; col_cnt < matrix.Cols(); col_cnt++)
			pos += ED_snprintf(ch + pos, 1000 - pos, num_template, float(matrix[row_cnt][col_cnt]));

		ed::string str = ch;
		printf("%s\n", str.c_str());
	}
}

template <typename T> ED_MATH_EXTERN
void fprint_matrix(const char* file_name, const char* open_method, const char* caption, const nMatrix<T>& matrix, const char* num_template = "%f ")
{
	FILE* f = fopen(file_name, open_method);
	if (f)
	{
		fprintf(f, "\n");
		fprintf(f, "%s\n", caption);
		for(unsigned int row_cnt = 0; row_cnt < matrix.Rows(); row_cnt++)
		{
			char ch[1000];
			int pos = 0;
			for (unsigned int col_cnt = 0; col_cnt < matrix.Cols(); col_cnt++)
				pos += ED_snprintf(ch + pos, 1000 - pos, num_template, float(matrix[row_cnt][col_cnt]));

			ed::string str = ch;
			fprintf(f, "%s\n", str.c_str());
		}

		fclose(f);
	}
}

#endif //#ifdef ED_MATH_IMPLEMENT_NMATRIX

#endif //#define _ED_Math_My_Matrix_h_
