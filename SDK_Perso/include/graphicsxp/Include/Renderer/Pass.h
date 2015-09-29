#ifndef _LOCKON_PASS_H_
#define _LOCKON_PASS_H_

#include "graphicsxp.h"

namespace Graphics {

	class Context;

	class Pass {
	protected:

		int _passType;

	public:

		GRAPHICSXP_API Pass(int type);
		GRAPHICSXP_API ~Pass();

		enum PassType {
			INVALID_PASS		= -1,
			PASS_MAIN			= 0,
			PASS_SHADOWMAP,
//			PASS_FLATSHADOW,
			PASS_ENV,
			PASS_MIRROR,
			PASS_MFD,
			PASS_MAP,
			PASS_MAP_SAT, // map view from satelite
//			PASS_Z_ONLY,
			PASS_IR,
			PASS_OLD_IR, // for full screen IR view
			PASS_CUSTOM, // for debug purposes
			PASS_REFRACTION,
			PASS_BOWWAVE,
			PASS_RADAR,
			ALL_PASSES
		};

		GRAPHICSXP_API void setPassType(int type);
		inline int getPassType() const {return _passType;}

		GRAPHICSXP_API virtual bool begin(Context*);
		GRAPHICSXP_API virtual void end(Context*);

	};

};

#endif
