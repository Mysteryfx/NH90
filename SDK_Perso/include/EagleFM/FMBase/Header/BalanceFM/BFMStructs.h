#pragma once
#include <math.h>
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4     0.785398163397448309616
#endif
//структура синусов углов Эйлера , для совмещения с классом SMat
struct TSinus
{
	double Teta, Gamma, Psi;
}
;
//структура косинусов углов Эйлера , для совмещения с классом SMat
struct TCosinus
{
	double Teta, Gamma, Psi;
};


//Структура 3D вектор в любой системе координат
typedef struct SVECTOR
{
  double x,y,z;
  SVECTOR(){this->x = 0 ; this->y = 0  ;this->z = 0 ; }
  SVECTOR(double x , double y , double z){this->x = x ; this->y = y  ;this->z = z ; }
  //сложение вычитание
  SVECTOR operator + (const SVECTOR vec)
  {
	  SVECTOR res(this->x + vec.x , this->y + vec.y , this->z + vec.z);
	  return res;
  }

  SVECTOR operator - (const SVECTOR vec)
  {
	  SVECTOR res(this->x - vec.x , this->y - vec.y , this->z - vec.z);
	  return res;
  }

  void operator += (const SVECTOR vec)
  {
	  this->x += vec.x;
	  this->y += vec.y;
	  this->z += vec.z;
  }
  void operator -= (const SVECTOR vec)
  {
	  this->x -= vec.x;
	  this->y -= vec.y;
	  this->z -= vec.z;
  }

  
  //вектор проекции на вектор
  SVECTOR Proj(SVECTOR & vec)
  {
      double len = (this->x * vec.x + this->y * vec.y + this->z * vec.z) / vec.len();
      return SVECTOR(vec.x * len , vec.y * len , vec.z * len);
  }
  
  double len()
  {
      return sqrt(x * x + y * y + z * z);
  }
  //векторное умножение
  SVECTOR operator * (const  SVECTOR vec)
  {
	  SVECTOR res_(0,0,0);
	  res_.x = this->y * vec.z - this->z * vec.y;
	  res_.y = this->z * vec.x - this->x * vec.z;
	  res_.z = this->x * vec.y - this->y * vec.x;
	  return res_;
  }
  //векторное умножение
  void operator *= (const  SVECTOR vec)
  {
	  SVECTOR res(this->x,this->y,this->z);
	  res = res * vec;
	  this->x  = res.x;
	  this->y  = res.y;
	  this->z  = res.z;
  }
  


}
SVECTOR;

//Структура 3D вектор в любой системе координат
struct SVec
{
  double x,y,z;
  SVec(){this->x = 0 ; this->y = 0  ;this->z = 0 ; }
  SVec(double x , double y , double z){this->x = x ; this->y = y  ;this->z = z ; }
  //сложение вычитание
  void Init(double x , double y , double z){this->x = x ; this->y = y  ;this->z = z ; }
  SVec operator + (const SVec &vec)
  {
	  SVec res(this->x + vec.x , this->y + vec.y , this->z + vec.z);
	  return res;
  }

  SVec operator - (const SVec &vec)
  {
	  SVec res(this->x - vec.x , this->y - vec.y , this->z - vec.z);
	  return res;
  }

  void operator += (const SVec &vec)
  {
	  this->x += vec.x;
	  this->y += vec.y;
	  this->z += vec.z;
  }
  void operator -= (const SVec &vec)
  {
	  this->x -= vec.x;
	  this->y -= vec.y;
	  this->z -= vec.z;
  }

  
  //вектор проекции на вектор
  SVec Proj(SVec* vec)
  {
      double len1 = (this->x * vec->x + this->y * vec->y + this->z * vec->z) / vec->len();
      return SVec(vec->x * len1 , vec->y * len1 , vec->z * len1);
  }
  
  double len()
  {
      return sqrt(x * x + y * y + z * z);
  }
  //векторное умножение
  SVec operator * (const  SVec &vec)
  {
	  SVec res_(0,0,0);
	  res_.x = this->y * vec.z - this->z * vec.y;
	  res_.y = this->z * vec.x - this->x * vec.z;
	  res_.z = this->x * vec.y - this->y * vec.x;
	  return res_;
  }
  //векторное умножение
  void operator *= (const  SVec &vec)
  {
	  SVec res(this->x,this->y,this->z);
	  res = res * vec;
	  this->x  = res.x;
	  this->y  = res.y;
	  this->z  = res.z;
  }
  
  SVec norm()
  {
      double l = len();
      if(l == 0) return SVec(0,0,0);
      return SVec(x/l,y/l,z/l);
  }

}
;
//Структура которая подается на каждом шаге интегрирования

struct TStateVector  
{
	double X,Y,Z;		// координаты вертолета
	double Teta,Psi,Gamma;	// углы тангажа, рысканья и крена
	double Vx,Vy,Vz;		// скорости в связанной с.к.
	double Wx,Wy,Wz;		// угловые скорости
	SVec Xg()
	{
		return SVec(X,Y,Z);
	}
	SVec EA()
	{
		return SVec(Teta,Psi,Gamma);
	}
	SVec Vb()
	{
		return SVec(Vx,Vy,Vz);
	}
	SVec Wb()
	{
		return SVec( Wx,Wy,Wz);
	}
}
;

struct SMat
{

	double xx, xy, xz;
	double yx, yy, yz;
	double zx, zy, zz;


	SMat()//Инициализация еденичной матрицей
	{
		xx=yy=zz=(double)1.0;
		xy=xz=yx=yz=zx=zy=(double)0.0;
	}
	~SMat(){}

 
    void Set(TSinus sin, TCosinus cos)//вычисляем матрицу перехода из связной системы координат в земную
   {
	    xx = (double)(cos.Teta * cos.Psi);
        yx = (double)(sin.Teta);
        zx = (double)(-cos.Teta * sin.Psi);
        xy = (double)(-cos.Gamma * sin.Teta * cos.Psi + sin.Gamma * sin.Psi);
        yy = (double)(cos.Gamma * cos.Teta);
 	    zy = (double)(cos.Gamma * sin.Teta * sin.Psi + sin.Gamma * cos.Psi);
        xz = (double)(sin.Gamma * sin.Teta * cos.Psi + cos.Gamma * sin.Psi);
        yz = (double)(-sin.Gamma * cos.Teta);
        zz = (double)(-sin.Psi * sin.Teta * sin.Gamma + cos.Psi * cos.Gamma);			
   }
        
   void SetInv(TSinus sin, TCosinus cos)//вычисляем матрицу перехода из земной системы координат в связную
   {
        
        xx = (double)(cos.Teta * cos.Psi);
        xy = (double)(sin.Teta);
        xz = (double)(-cos.Teta * sin.Psi);
        yx = (double)(-cos.Gamma * sin.Teta * cos.Psi + sin.Gamma * sin.Psi);
        yy = (double)(cos.Gamma * cos.Teta);
 	    yz = (double)(cos.Gamma * sin.Teta * sin.Psi + sin.Gamma * cos.Psi);
        zx = (double)(sin.Gamma * sin.Teta * cos.Psi + cos.Gamma * sin.Psi);
        zy = (double)(-sin.Gamma * cos.Teta);
        zz = (double)(-sin.Psi * sin.Teta * sin.Gamma + cos.Psi * cos.Gamma);
   }
   
   void Set(double Teta,double Psi,double Gamma)
   {
   
       TSinus sin_;
       TCosinus cos_;
       sin_.Teta = (double)sin(Teta);
       sin_.Gamma = (double)sin(Gamma);
       sin_.Psi = (double)sin(Psi);
       cos_.Teta = (double)cos(Teta);
       cos_.Gamma = (double)cos(Gamma);
       cos_.Psi = (double)cos(Psi);
       Set(sin_,cos_);
   }
   
   void SetInv(double Teta,double Psi,double Gamma)
   {
       TSinus sin_;
       TCosinus cos_;
       sin_.Teta = (double)sin(Teta);
       sin_.Gamma = (double)sin(Gamma);
       sin_.Psi = (double)sin(Psi);
       cos_.Teta = (double)cos(Teta);
       cos_.Gamma = (double)cos(Gamma);
       cos_.Psi = (double)cos(Psi);
      
       SetInv(sin_,cos_);
   }
   
   
   
   SVec operator * (const SVec vec)
   {
	   SVec res_(0,0,0);
       res_.x = (this->xx) * vec.x + (this->xy) * vec.y + (this->xz) * vec.z;
       res_.y = (this->yx) * vec.x + (this->yy) * vec.y + (this->yz) * vec.z;
       res_.z = (this->zx) * vec.x + (this->zy) * vec.y + (this->zz) * vec.z;
       return res_;
   }


} ; 

struct SMatrix
{

	double xx, xy, xz;
	double yx, yy, yz;
	double zx, zy, zz;


	SMatrix()//Инициализация еденичной матрицей
	{
		xx=yy=zz=(double)1.0;
		xy=xz=yx=yz=zx=zy=(double)0.0;
	}
	~SMatrix(){}


    void Set(TSinus sin, TCosinus cos)//вычисляем матрицу перехода из связной системы координат в земную
   {
	    xx = (double)(cos.Teta * cos.Psi);
        yx = (double)(sin.Teta);
        zx = (double)(-cos.Teta * sin.Psi);
        xy = (double)(-cos.Gamma * sin.Teta * cos.Psi + sin.Gamma * sin.Psi);
        yy = (double)(cos.Gamma * cos.Teta);
 	    zy = (double)(cos.Gamma * sin.Teta * sin.Psi + sin.Gamma * cos.Psi);
        xz = (double)(sin.Gamma * sin.Teta * cos.Psi + cos.Gamma * sin.Psi);
        yz = (double)(-sin.Gamma * cos.Teta);
        zz = (double)(-sin.Psi * sin.Teta * sin.Gamma + cos.Psi * cos.Gamma);			
   }
        
   void SetInv(TSinus sin, TCosinus cos)//вычисляем матрицу перехода из земной системы координат в связную
   {
        
        xx = (double)(cos.Teta * cos.Psi);
        xy = (double)(sin.Teta);
        xz = (double)(-cos.Teta * sin.Psi);
        yx = (double)(-cos.Gamma * sin.Teta * cos.Psi + sin.Gamma * sin.Psi);
        yy = (double)(cos.Gamma * cos.Teta);
 	    yz = (double)(cos.Gamma * sin.Teta * sin.Psi + sin.Gamma * cos.Psi);
        zx = (double)(sin.Gamma * sin.Teta * cos.Psi + cos.Gamma * sin.Psi);
        zy = (double)(-sin.Gamma * cos.Teta);
        zz = (double)(-sin.Psi * sin.Teta * sin.Gamma + cos.Psi * cos.Gamma);
   }

   SVECTOR operator * (const SVECTOR vec)
   {
	   SVECTOR res_(0,0,0);
       res_.x = this->xx * vec.x + this->xy * vec.y + this->xz * vec.z;
       res_.y = this->yx * vec.x + this->yy * vec.y + this->yz * vec.z;
       res_.z = this->zx * vec.x + this->zy * vec.y + this->zz * vec.z;
       return res_;
   }


} ; 
