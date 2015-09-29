#pragma once

const float Pi = 3.14159265358979f;
const float GradToRad = Pi/180.f;
const float RadToGrad = 180.f/Pi;

// Привести угол к диапазону [min, max)
inline float AlignAngle(float angle, float max, float min)
{
	while(angle>=max)
		angle -= Pi*2.f;
	while(angle<min)
		angle += Pi*2.f;
	return angle;
}

