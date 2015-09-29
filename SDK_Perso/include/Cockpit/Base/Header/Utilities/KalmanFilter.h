#pragma once
#include "CockpitBase.h"
#include "Math/nMatrix.h"

class KalmanFilter
{
public:
	typedef nMatrix<double> KFMatrix;
public:
	KalmanFilter();

	KFMatrix X; //вектор апостериорных оптимальных оценок вектора состояния
	KFMatrix P; //апостериорная ковариационная матрица ошибок фильтрации
	KFMatrix H; //"сигнальная функция"
	KFMatrix F; //фундаментальная (переходная) матрица вектора состояний
	KFMatrix Q; //ковариационная матрица шумов системы
	KFMatrix R; //ковариационная матрица шумов измерений
	void process(const KFMatrix & Z); //такт работы фильтра
	void idle(); //холостой такт фильтра
	KFMatrix get_Xext() const;
	void clear();

#ifdef _ED_HIDDEN_DEBUG 
	bool fprintf_matrix;
#endif
};


inline KalmanFilter::KalmanFilter()
{
#ifdef _ED_HIDDEN_DEBUG 
	fprintf_matrix = false;
#endif
}

inline void KalmanFilter::process(const KFMatrix& Z)
{
#ifdef _ED_HIDDEN_DEBUG 
	ed::string format = "%f\t";
	if (fprintf_matrix)
	{
		KFMatrix void_matrix;
		fprint_matrix("c:\\kalman_filter.log", "wt+", "", void_matrix, format.c_str());  //стираем
		fprint_matrix("c:\\kalman_filter.log", "at+", "Z(k)", Z, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "F(k)", F, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "H(k)", H, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "Q(k)", Q, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "R(k)", R, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "X(k-1)", X, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "P(k-1)", P, format.c_str());
	}
#endif

	//экстраполяция
	KFMatrix Pext = F * P * F.getTranspose() + Q;

#ifdef _ED_HIDDEN_DEBUG 
	if (fprintf_matrix)
		fprint_matrix("c:\\kalman_filter.log", "at+", "Pext(k)", Pext, format.c_str());
#endif

	KFMatrix Xext = F * X;

#ifdef _ED_HIDDEN_DEBUG 
	if (fprintf_matrix)
		fprint_matrix("c:\\kalman_filter.log", "at+", "Xext(k)", Xext, format.c_str());
#endif

	//нахождение весовой матрицы
	KFMatrix temp1 = H * Pext * H.getTranspose() + R;
	getInverseByLU(temp1);
	KFMatrix K = Pext * H.getTranspose() * temp1;

#ifdef _ED_HIDDEN_DEBUG 
	if (fprintf_matrix)
		fprint_matrix("c:\\kalman_filter.log", "at+", "K(k)", K, format.c_str());
	//оценка состояния
	if (fprintf_matrix)
	{
		KFMatrix Yext = H * Xext;
		fprint_matrix("c:\\kalman_filter.log", "at+", "Yext(k)", Yext, format.c_str());
	}
#endif

	X = Xext + K * (Z - H * Xext);

#ifdef _ED_HIDDEN_DEBUG 
	if (fprintf_matrix)
		fprint_matrix("c:\\kalman_filter.log", "at+", "X(k)", X, format.c_str());
#endif

	KFMatrix E;
	E.diag(P.Cols(), 1.0);

#ifdef _ED_HIDDEN_DEBUG 
	if (fprintf_matrix)
		fprint_matrix("c:\\kalman_filter.log", "at+", "E", E, format.c_str());
#endif

	P = (E - K * H) * Pext;

#ifdef _ED_HIDDEN_DEBUG 
	if (fprintf_matrix)
		fprint_matrix("c:\\kalman_filter.log", "at+", "P(k)", P, format.c_str());
#endif
}

inline void KalmanFilter::idle()
{
#ifdef _ED_HIDDEN_DEBUG 
	ed::string format = "%f\t";
	if (fprintf_matrix)
	{
		KFMatrix void_matrix;
		fprint_matrix("c:\\kalman_filter.log", "wt+", "", void_matrix, format.c_str());  //стираем
		fprint_matrix("c:\\kalman_filter.log", "at+", "F(k)", F, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "H(k)", H, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "Q(k)", Q, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "R(k)", R, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "X(k-1)", X, format.c_str());
		fprint_matrix("c:\\kalman_filter.log", "at+", "P(k-1)", P, format.c_str());
	}
#endif

	X = F * X;
	//P = F * P * F.getTranspose() + Q;
#ifdef _ED_HIDDEN_DEBUG 
	if (fprintf_matrix)
	{
		fprint_matrix("c:\\kalman_filter.log", "at+", "X(k+1)", X, format.c_str());  //стираем
	}
#endif
}

inline KalmanFilter::KFMatrix KalmanFilter::get_Xext() const
{
	return F * X;
}

inline void KalmanFilter::clear()
{
	X.clear();
	P.clear();
	H.clear();
	F.clear();
	Q.clear();
	R.clear();
}
