#pragma once
struct smViewport;
#include "Visualizer.h"

namespace render
{
	class FrameBuffer;
}

namespace OculusRift
{
	bool VISUALIZER_API is_active();
	bool VISUALIZER_API initialize();
	void VISUALIZER_API destroy();
	void VISUALIZER_API set_eyes(smViewport & left,smViewport & right);
	void VISUALIZER_API get_display_rect(int & x,int & y,int & w,int & h);
	void VISUALIZER_API process_head_tracker();
	void VISUALIZER_API reset_head_tracker();

	//postprocessing
	void				  init_post_processing();
	void				  push_framebuffer();
	void				  pop_framebuffer_and_apply();
	void				  bake_framebuffer();
	void				  on_eye_render_begin(unsigned eye, smViewport & viewport);
	void				  on_eye_render_end  (unsigned eye, smViewport & viewport);
	void				  on_end_frame  ();
	void				  on_renderer_init();

}