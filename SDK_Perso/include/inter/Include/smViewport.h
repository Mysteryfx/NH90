#ifndef SM_VIEWPORT_H
#define SM_VIEWPORT_H


// for multi-camera setup (Matrox Triple Head)
struct smViewport
{
	float viewDx;
	float viewDy;    // offset from main view in 'view' (viewDx=1 means next view to the right)
	bool  absoluteAngleShift;
	float aspect;           // аспект данного окна
	int   x;
	int   y;
	int   width; 
	int   height; // viewport
	float eye_point_shift;  // for stereo mode
	float hFOV;
	bool  absoluteFOV;
};

#endif