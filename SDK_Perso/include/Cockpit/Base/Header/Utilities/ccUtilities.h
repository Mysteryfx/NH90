#ifndef   _CCUTILITES_H
#define   _CCUTILITES_H

#include "CockpitBase.h"

#include "Modeler/GeometryNode.h"
#include "Renderer/Material.h"
#include "Renderer/Geometry.h"
#include "wTime.h"
#include "cAlgorithm.h"
#include "Plane.h"
#include "AuxMath.h"
#include "Vector4.h"
#include "Utilities/ccStraightLine.h"
#include "dPosition.h"
#include "wFloat.h"
#include "Utilities/ccPolarPosition.h"
#include "IwHumanPayload.h"

#include "ed_log.h"
#define LUADEBUGPRINT(source, script_name, LUA) \
	ED_ERROR("Cockpit: %s: Cannot do Lua file : %s, reason  %s.", source, script_name, lua_tostring(LUA,-1))

#define lua_register_userdata(L,n,p) \
		(lua_pushstring(L, n), \
		 lua_pushlightuserdata(L, p), \
		 lua_settable(L, LUA_REGISTRYINDEX))

class IwHumanSensor;
class IwoLA;
class IwHumanHead;
struct Waypoint;
class wHumanCommunicator;


namespace Graphics{
	class ModelNode;
}
namespace Lua {
	class Config;
}

using namespace Graphics;


namespace cockpit
{

static const dVector gv_iframe = dVector(0.0, -9.81, 0.0);
static const double gs = 9.81;

static const float clk_epsilon = 0.0001f;

template<typename T>
class limits
{
	public:
		limits(const T _min = -1.0,const T _max = 1.0){ min = _min; max = _max;}
		~limits(){};
		limits(const limits &L) { *this = L;};
		limits& operator=(const limits &L)
		{
			if (this != &L)
			{
				max = L.max;
				min = L.min; 
			}
			return *this;
		};
		bool   in_limit      (T in_value) const { return (in_value < max) && (in_value > min);}
		bool   in_limit_eq   (T in_value) const { return (in_value <= max) && (in_value >= min);}

		T to_limit      (T in_value) const { return MIN_MAX(min,in_value,max);};

		/************************************************************************/
		/* "cycle_in_limit" remarks:
			with integral types 'cycle_in_limit' is FASTER than  cycle_in_limit_s  (up to 5 times)
		    with floating point types 'cycle_in_limit' is SLOWER than  cycle_in_limit_s (up to 2 times)
		*/
		/************************************************************************/
		T cycle_in_limit(T in_value) const /*integral types version */
		{ 
			if(in_value < min) 
				return max + (in_value-min) % (max-min);
			else if(in_value > max)
				return (in_value-min) % (max-min) + min;
			return in_value;
		}

		T cycle_in_limit_s(const T val_in) const /*integral types version */
												/*stable reduction to [min ... max),[min...max) */
		{
			float int_p;
			float f = modf(float(val_in - min)/float(max-min),&int_p);

			if(f < 0) f += 1;
			return min + T(floor(f * (max-min) + 0.5));
		}

		char get_dist(T value1, T value2) /*returns distance sight between two values in cycled range*/
		{
			float d1 = value2 - value1;
			float d2 = -SIGN(d1)*((max-min) - fabs(d1));

			if(fabs(d1) > fabs(d2))
				return d2 == 0 ? -SIGN(d1) : SIGN(d2);
			else
				return d1 == 0 ? -SIGN(d2) : SIGN(d1);
		}

		T get_coeff_by_value(T in_value) const { return  (max != min)? (in_value - min)/(max - min):0;};
		T get_value_by_coeff(T coeff)    const { return   min + coeff*(max - min);};

		friend  bool operator > (const T &in_value,const limits &L){return in_value > L.max;};
		friend  bool operator < (const T &in_value,const limits &L){return in_value < L.min;};
		friend  bool operator >=(const T &in_value,const limits &L){return in_value > L.max || L.in_limit(in_value);};
		friend  bool operator <=(const T &in_value,const limits &L){return in_value < L.min || L.in_limit(in_value);};
		friend  bool operator ==(const T &in_value,const limits &L){return  L.in_limit(in_value);};
		friend  bool operator !=(const T &in_value,const limits &L){return !L.in_limit(in_value);};
		bool         operator > (limits &L) const { return max  > L.max && min  < L.min;};
		bool         operator < (limits &L) const { return max  < L.max && min  > L.min;};
		bool         operator ==(limits &L) const { return max == L.max && min == L.min;};
 		bool         operator !=(limits &L) const { return !(*this == L);};
		limits       operator * (T K) 
		{
			limits L = *this;
					L.max *= K;
					L.min *= K; 
			if (L.min > L.max)
			{
				std::swap(L.min,L.max);
			}
			return L;
		};
		limits&     operator *=(T K)
		{
			max *= K;
			min *= K; 
			if (min > max)
			{
				std::swap(min,max);
			}
			return *this;
		};
		limits       operator / (T K) 
		{
			limits L   = *this;
					L.max /= K;
					L.min /= K; 
			if (L.min > L.max)
			{
				std::swap(L.min,L.max);
			}
			return L;
		};
		limits&     operator /=(T K)
		{
			max /= K;
			min /= K; 
			if (min > max)
			{
				std::swap(min,max);
			}
			return *this;
		};

		limits      operator + (T K)  {  return limits(this->min + K,this->max + K); };
		limits&     operator +=(T K)  
		{
			min +=K;
			max +=K;
			return *this;
		};
		limits      operator - (T K)  {  return limits(this->min - K,this->max - K); };
		limits&     operator -=(T K)  
		{
			min -=K;
			max -=K;
			return *this;
		};
		friend  class limits;
		T min;
		T max;

		template<typename T1>
		operator limits<T1>() const{ return limits<T1>(static_cast<T1>(min),static_cast<T1>(max)); }
};


template<>
float limits<float>::cycle_in_limit_s(const float val_in) const /*stable reduction to [min ... max),[min...max) */
{
	float int_p;
	float f = modf((val_in - min)/(max-min),&int_p);

	if(f < 0.f) f += 1.f;
	return min + f * (max-min);
}

template<>
double limits<double>::cycle_in_limit_s(const double val_in) const /* stable reduction to [min ... max),[min...max) */
{
	double int_p;
	double f = modf((val_in - min)/(max-min),&int_p);

	if(f < 0.) f += 1.;
	return min + f * (max-min);
}

#define USE_STABLE_CYCLE_FUNCTION

#ifdef USE_STABLE_CYCLE_FUNCTION

template<>
float limits<float>::cycle_in_limit(float in_value) const
{ 
	return limits<float>::cycle_in_limit_s(in_value);
}

template<>
double limits<double>::cycle_in_limit(double in_value) const
{ 
	return limits<double>::cycle_in_limit_s(in_value);
}

#else

template<>
float limits<float>::cycle_in_limit(float in_value) const
{ 
	if(in_value < min) 
		return max + fmod(in_value-min,max-min);
	else if(in_value > max)
		return fmod(in_value-min,max-min) + min;
	return in_value;
}

template<>
double limits<double>::cycle_in_limit(double in_value) const
{ 
	if(in_value < min) 
		return max + fmod(in_value-min,max-min);
	else if(in_value > max)
		return fmod(in_value-min,max-min) + min;
	return in_value;
}

#endif

typedef limits<float> limits_f;
typedef limits<double> limits_d;
typedef limits<int> limits_i;

/////////////////// end LIMITS //////////////////////////

typedef public wLineInterpolation<PolarPosition> wPolar;

Graphics::Geometry*     GetCMD(Graphics::Model *model, const char *name);
Graphics::GeometryNode* GetGeometryNode(Graphics::Model *model, const char *name);

COCKPITBASE_API Graphics::Material* ConstructNamedMaterial(const Vector4& color,const ed::string& name) ;
COCKPITBASE_API Graphics::Material* ConstructNamedTexMaterial(const Vector4& color,const ed::string& tex,const ed::string& name) ;

void COCKPITBASE_API c_draw_debug_cockpit_box(const Box & box,const Position3 & pos,const Vector4 & color);
void COCKPITBASE_API c_draw_debug_cockpit_frustum(double FOV,const Position3 & pos,const Vector4 & color);
void COCKPITBASE_API c_draw_debug_cockpit_tripod (const Position3 & pos);

// auxiliary lua-to-vector functions
typedef ed::vector<Vector3>        vectorarray;
typedef ed::vector<unsigned short> indexarray;

void COCKPITBASE_API readVectorArray(lua_State *L, int index, vectorarray& verts);
void COCKPITBASE_API readVector(lua_State *L, int index, Vector3& v);
void COCKPITBASE_API readVector4(lua_State *L, int index, Vector4& v);
void COCKPITBASE_API readLimit (lua_State *L, int index, limits_d &lim);

void COCKPITBASE_API read_raw_string(lua_State *L,int index,int size, ... );
template<class T>	
void COCKPITBASE_API read_raw_number (lua_State *L,int index,int size, ... );


void COCKPITBASE_API readIndexArray(lua_State *L, int index, indexarray& inds);

inline bool Blinking(unsigned int period = 0x20)
{
	return ((unsigned int)(wTime::GetModelTime() * 100.0) & period) != 0;
};

inline bool Blinking_direct(unsigned int period = 2)
{
	return (static_cast<unsigned int>(wTime::GetModelTime()) % period) != 0;
};

// Alternative function to allow any possible periods
inline bool isBlink(float period)
{
	unsigned int t = static_cast<unsigned int>(wTime::GetModelTime() / period);
	unsigned int half = t / 2;
	if (half * 2 != t)
		return true;

	return false;
};

Graphics::Model* c_model();


COCKPITBASE_API IwHumanPayload* c_payload();
COCKPITBASE_API IwHumanSensor * c_sensors();
COCKPITBASE_API IwoLA         * c_LA();

COCKPITBASE_API wHumanCommunicator * c_get_communicator();

COCKPITBASE_API VolumeEffect *  c_get_sun();
COCKPITBASE_API float			c_get_scene_luminance(VolumeEffect * sun);
COCKPITBASE_API float			c_get_scene_luminance_max();

inline float c_get_scene_luminance_normed()
{
	float res = c_get_scene_luminance(c_get_sun())/c_get_scene_luminance_max();
	if (res > 1.0f)
		return 1.0f;
	return res;
}



union COCKPITBASE_API fictional_value
{
	float		  val;
	unsigned int composed;
};

float COCKPITBASE_API compose_command(unsigned char device,float value = 0);
void  COCKPITBASE_API decompose_command(unsigned char &device,float &decomposed_value,float incoming_value = 0);

unsigned char COCKPITBASE_API planeIntersect (Plane &plane,Vector3& a, Vector3& b,Vector3& _result);
void		  COCKPITBASE_API pointProjection(Plane &plane,Vector3& point,Vector3& _result);
	
inline Vector3 plane_intersection(Plane* plane, Vector3 p0, Vector3 p1)
{
	Vector3 a = p1 - p0;
	Vector3& n = plane->n;
	float t = (n.x*p0.x - n.y*p0.y - n.z*p0.z - plane->d)/(n.x*a.x + n.y*a.y + n.z*a.z);
	
	return Vector3(p0.x + a.x * t, p0.y + a.y * t, p0.z + a.z * t);
};

// луч выходит из точки (0, 0, 0)
inline Vector3 central_plane_intersection(Plane* plane, Vector3 p)
{
	Vector3& n = plane->n;
	float t = -plane->d/(n.x*p.x + n.y*p.y + n.z*p.z);
	
	return Vector3(p.x * t, p.y * t, p.z * t);
}

inline float flat_length(const Vector3 &v1,const Vector3 &v2)
{
	float res = fsqrt((v1.z - v2.z)*(v1.z - v2.z) + (v1.y - v2.y)*(v1.y - v2.y));
//	assert(res);
	return res;
}

inline void calc_view_angles(const dPosition &pos,const dVector  &T_pos,double &fim,double &fin)
{
	dVector world_direction = T_pos - pos.p;
	dVector local_direction;
	mult_transp(local_direction, pos ,world_direction);
	fim = atan2(local_direction.z, local_direction.x);
	fin = atan2(local_direction.y, sqrt(local_direction.x*local_direction.x + local_direction.z * local_direction.z));
	return;
};

Box COCKPITBASE_API BoundOfOriented(const Position3& pos, const Box& box);

template<class T1, class T2> inline void create_array(const T1& v_in, T2& v_out)
{
	int s = (int)v_in.size();
	v_out.resize(s);

	for (int i_v = 0; i_v < s; i_v++)
		v_out[i_v] = v_in[i_v];
};


/*************************************************************************
Интерполяция Лагранжа по Эйтекену.
*************************************************************************/
template <class _Vector>
double lagrangeinterpolate(const _Vector & x,const _Vector & f,double t)
//double cockpit::lagrangeinterpolate(const _Vector & x, _Vector  f,double t)
{
	double result;
	double p;
	result = 0;
	for(int i=0;i<x.size();i++)
	{
		p=1;
		for(int j=0;j<x.size();j++)
		{
			if(i!=j) p*=(t-x[j])/(x[i]-x[j]);
		}
		p*=f[i];
		result+=p;
	}
	return result;
}

//double lagrangeinterpolate(const DParamList & x, DParamList   f,double t);

/*************************************************************************
Линейная интерполяция
*************************************************************************/
template <class _Vector>
double linearinterpolate(const _Vector& x, const _Vector& f, double t)
{
	for (int i = 0; i < x.size(); i++)
	{
		if (t <= x[i])
		{
			if (i > 0)
			{
				return (f[i]*(t - x[i-1]) + f[i-1]*(x[i]-t))/(x[i]-x[i-1]);
			}
			return f[0];
		}
	}
	if (f.size())
	{
		return f[f.size()-1];
	}
	return 0.0;
}

ed::string COCKPITBASE_API create_guid();

typedef ed::vector< std::pair<double, ed::vector< std::pair<double, double> > > > _2d_table;
typedef ed::vector< std::pair<double, ed::vector< std::pair<double, std::pair<double, double> > > > > _2d_2value_table;

double COCKPITBASE_API bilinear_interpolate(const _2d_table&, double, double);
std::pair<double, double> COCKPITBASE_API bilinear_interpolate_2values(const _2d_2value_table&, double, double);

template void COCKPITBASE_API read_raw_number<double>       (lua_State *L,int index,int size, ... );
template void COCKPITBASE_API read_raw_number<float>        (lua_State *L,int index,int size, ... );
template void COCKPITBASE_API read_raw_number<int>          (lua_State *L,int index,int size, ... );
template void COCKPITBASE_API read_raw_number<unsigned char>(lua_State *L,int index,int size, ... );

int COCKPITBASE_API round(double x);
int COCKPITBASE_API roundf(float x);

char COCKPITBASE_API itoc(int in);

typedef Vector3 RGBvector;

typedef std::pair<unsigned int, cPoint> cPointPair;
typedef ed::map<int,cPoint> cPointMap;
class cPerimeter
{
public:
	cPerimeter() {;}
	static cPointMap getPerimeter(const cPointMap & point_map);
	static float getPerimeterLength(const cPointMap & point_map);
	static float getSquare(const cPointMap & point_map);
};

void		 COCKPITBASE_API read_waypoint(Waypoint & wpt,Lua::Config & config);
unsigned int COCKPITBASE_API read_airdrome_id(Lua::Config & config);
bool		 COCKPITBASE_API function_ref_exist(int & ref_guard,lua_State * Lua_s,const char * func_name);

}

#endif // _CCUTILITES_H
