#pragma once
#include "WorldGeneral.h"
#include "ed/vector.h"

namespace Lua
{
	class Config;
}

typedef ed::vector<float> data_type;

struct WORLDGENERAL_API data_type_polynom
{
	double a;
	double b;
	double c;
	double d;

	double  get(double x_in) const
	{
		return  a +
				b * x_in + 
				c * x_in * x_in + 
				d * x_in * x_in * x_in;
	}
};

class WORLDGENERAL_API data_1d_table
{
	data_type   x;
	data_type   values;
	data_type_polynom polynom;
	bool			  polynom_valid;
	double			  polynom_x_gain;
	inline size_t vindex(size_t x_index) const
	{
		assert(x_index < x.size());
		return x_index;
	}
public:
	data_1d_table()
	{
		polynom_valid = false;
	}
	float  get(float x_value) const; // interpolation
	void   read(Lua::Config & config,const char * x_name	 = "x",
									 const char * table_name = "values");
	bool   valid() const { return polynom_valid || !values.empty(); }
};


class WORLDGENERAL_API data_2d_table
{
	struct polynomial_form_data 
	{  
		data_type_polynom y_form_a;
		data_type_polynom y_form_b;
		data_type_polynom y_form_c;
		data_type_polynom y_form_d;

		double  get(double x_in,double y_in) const
		{
			data_type_polynom x_form;
			x_form.a = y_form_a.get(y_in);
			x_form.b = y_form_b.get(y_in);
			x_form.c = y_form_c.get(y_in);
			x_form.d = y_form_d.get(y_in);
			return x_form.get(x_in);
		}
	};

	data_type   x;
	data_type   y;
	data_type   values;
	polynomial_form_data polynom;
	bool				 polynom_valid;
	double				 polynom_y_gain;
	double			     polynom_x_gain;

	inline size_t vindex(size_t x_index,size_t y_index) const
	{
		assert(x_index < x.size());
		return y_index * x.size() + x_index;
	}
public:
	data_2d_table()
	{
		polynom_valid	= false;
		polynom_y_gain  = 1.0;
		polynom_x_gain  = 1.0;
	}
	float  get(float x_value, float y_value) const; // interpolation
	void   read(Lua::Config & config,const char * x_name = "x",
							 	 	 const char * y_name = "y",
									 const char * table_name = "values");
	bool   valid() const { return polynom_valid || !values.empty(); }
};

struct WORLDGENERAL_API engine_SFM
{
	data_2d_table p_max;
	data_2d_table p_afterburner;
	data_2d_table ce_max;
	data_2d_table ce_afterburner;
	data_1d_table ce_throttle_responce;

	data_1d_table rpm_acceleration_time_factor;
	data_1d_table rpm_deceleration_time_factor;

	data_1d_table rpm_throttle_responce;
	data_1d_table thrust_rpm_responce;


	bool valid() const
	{
		return p_max.valid() &&
			   ce_max.valid() &&
			   ce_throttle_responce.valid();
	}
};



enum engine_type_enum
{
	E_TURBOJET,
	E_TURBOJET_AB,
	E_PISTON,
	E_TURBOPROP,
	E_TURBOFAN,
	E_TURBOSHAFT
};

struct WORLDGENERAL_API SFM_info
{
	struct	aerodynamics_info
	{
		struct tab_entry
		{
			float     Max1; 
			float     cx0;
			float     Cyal;
			float     Bpol;
			float     Bpol4;
			float     Omx_max;
			float     Alfa_dop;
			float     Cy_max;
		};
		float	kjx;
		float	kjz;
		float	Czbe;
		float	cx_gear;
		float	cx_flap;
		float	cy_flap;
		float	cx_brk;
		float   Cy0;
		float   Mzalfa;
		float   Mzalfadt;

		ed::vector<tab_entry> table;
	};


	struct engine_info
	{
		struct tab_entry
		{
			float Max1;
			float pm; 
			float pf;
		};
		unsigned	typeng;
		float   Nmg;
		float   MinRUD;
		float   MaxRUD;
		float   MaksRUD;
		float   ForsRUD;
		float   cemax;
		float   cefor;
		float	dcx_eng;
		float	hMaxEng;
		float	dpdh_f;
		float	dpdh_m;

		float			k_adiab_1;
		float			k_adiab_2;
		float			MAX_Manifold_P_1;
		float			MAX_Manifold_P_2;
		float			MAX_Manifold_P_3;
		float			k_after_cool;
		float			Displ;
		float			k_Eps;
		float			Stroke;
		float			V_pist_0;
		float			Nu_0;
		float			Nu_1;
		float			Nu_2;
		float			N_indic_0;
		float			N_fr_0;
		float			N_fr_1;
		float			Init_Mom;
		float			D_prop;
		float			MOI_prop;
		float			k_gearbox;
		float			P_oil;
		float			k_boost;
		float			k_cfug;
		float			k_oil;
		float			k_piston;
		float			k_reg;
		float			k_vel;


		engine_SFM	 	extended_table;

		ed::vector<tab_entry> table;
	};
	aerodynamics_info aerodynamics;
	engine_info       engine;
};
