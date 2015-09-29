/**
 * @brief Linear predictor.
 * @author Dmitry S. Baikov
 */
#ifndef _ED_Math_Half_h_
#define _ED_Math_Half_h_

#include "./_config.h"


namespace Math {


ED_MATH_EXTERN unsigned short float2half(float f);
ED_MATH_EXTERN float half2float(unsigned short h);

/*
class Half {
	unsigned short m_value;

public:
		Half() {}
		Half(float f) { m_value = float2half(f); }

	operator float() const { return half2float(m_value); }

	unsigned short bits() const { return m_value; }

};
*/

}

#endif /* _ED_Math_Half_h_ */
