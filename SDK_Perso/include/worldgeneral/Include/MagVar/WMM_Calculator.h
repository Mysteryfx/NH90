#ifndef __WMM_Calculator_H__
#define __WMM_Calculator_H__

#include "WMM_Definitions.h"
#include <ed/string.h>

namespace magvar
{
	class  WMM_Calculator 
	{
		friend  float_wmm get_mag_decl(float_wmm latitude, float_wmm longitude);
		friend  float_wmm get_mag_incl(float_wmm latitude, float_wmm longitude);
		friend  WMM_Data get_mag_decl_incl(float_wmm latitude, float_wmm longitude);
		friend  void  initialize(int month, int year);
		friend  void  release();
	public:

		struct FoundData
		{
			ed::string cof_file_name_;
			int current_year_;
			int found_year_;
		};

	private:

		WMM_Calculator(){}

		static void init(int month, int year);		

		static void release();

		static ed::string get_grid_file_name(int month, int year);

		static ed::string get_grid_bin_file_name(int month, int year);

		static ed::string get_grid_txt_file_name(int month, int year);

		static ed::string get_write_dir();

		static ed::string get_cof_dir();

		static void set_wmm_storage(WMM_Storage* new_storage);

		static bool calculate_wmm_grid(const char* file_name, int month, int year);

		static WMM_Storage* storage_;
	};
	/************************************************************************/
	/*  Bilinear interpolation                                                                    
			q3		  q4	
	y2	  --* ------- * --
	y	    |	*	  |
			|	p	  |
			|		  |
	y1	  --* ------- * --
			q1		  q2	

			x1	x 	  x2 

			(x2 - x)            (x - x1)
	f(R1) =  -------- * f(q1) + -------- * f(q2)
			(x2 - x1)           (x2 - x1)

			(x2 - x)            (x - x1)
	f(R2) =  -------- * f(q3) + -------- * f(q4)
			(x2 - x1)           (x2 - x1)

			(y2 - y)           (y - y1)
	f(p) =  -------- * f(R1) + -------- * f(R2)
			(y2 - y1)          (y2 - y1)
	/************************************************************************/
	
	float_wmm bilinear_interpolate_1(float_wmm x1, float_wmm x2, float_wmm y1, float_wmm y2, 
									 float_wmm q1, float_wmm q2, float_wmm q3, float_wmm q4, 
									 float_wmm x,  float_wmm y)
	{
		float_wmm D1 = (x2 - x) / (x2 - x1);
		float_wmm D2 = (x - x1) / (x2 - x1);

		return ((y2 - y) / (y2 - y1)) * ( D1 * q1 + D2 * q2 ) + 
			   ((y - y1) / (y2 - y1)) * ( D1 * q3 + D2 * q4 );
	}

	WMM_Data bilinear_interpolate_2(float_wmm x1, float_wmm x2, float_wmm y1, float_wmm y2, 
									float_wmm q1, float_wmm q2, float_wmm q3, float_wmm q4, 
									float_wmm p1, float_wmm p2, float_wmm p3, float_wmm p4,
									float_wmm x,  float_wmm y)
	{
		float_wmm D1 = (x2 - x) / (x2 - x1);
		float_wmm D2 = (x - x1) / (x2 - x1);
		float_wmm E1 = (y2 - y) / (y2 - y1);
		float_wmm E2 = (y - y1) / (y2 - y1);

		return WMM_Data( E1 * ( D1 * q1 + D2 * q2 ) + E2 * ( D1 * q3 + D2 * q4 ),
							   E1 * ( D1 * p1 + D2 * p2 ) + E2 * ( D1 * p3 + D2 * p4 ));

	}
}
#endif