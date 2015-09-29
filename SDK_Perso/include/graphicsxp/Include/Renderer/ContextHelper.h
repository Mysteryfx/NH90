#ifndef CONTEXT_HELPER_H
#define CONTEXT_HELPER_H
#include "graphicsxp.h"

#include "rwrappers/renderUserAPI.h"


namespace render 
{

enum CBufType : unsigned int {
	CB_PER_FRAME = 1 << 0,
	CB_PER_VIEW = 1 << 1,
	CB_AMBIENT_MAP = 1 << 2,

	CB_ALL = 0xffffffff
};

enum SBufType : unsigned int {
	SB_CLOUDS_COLOR = 1 << 0,

	SB_ALL = 0xffffffff
};

/// Class to set context's constant and structured buffers in shader
class ContextHelper {
public:
	enum Handles{
		PER_FRAME,
		PER_VIEW,
		AMBIENT_MAP,
		CLOUDS_COLOR,
		CLOUDS_COLOR_ID,

		LAST_HANDLE
	};

public:
	GRAPHICSXP_API ContextHelper();

	GRAPHICSXP_API void init(::render::Shader &shader);

	GRAPHICSXP_API void useCBuffers(unsigned int bufs = CB_ALL);

	GRAPHICSXP_API void useSBuffers(unsigned int bufs);

	GRAPHICSXP_API void setCloudsColorId(int id, int count);

	GRAPHICSXP_API void applyCloudsColor(int id, int count) { useSBuffers(SB_CLOUDS_COLOR); setCloudsColorId(id, count); }

private:
	::render::Shader *shader;
	::render::handle_t _handles[LAST_HANDLE];
};

}

#endif
