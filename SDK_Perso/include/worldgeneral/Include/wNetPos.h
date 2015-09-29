#ifndef wNetPos_h
#define wNetPos_h


#include "WorldGeneral.h"
#include "cLinear.h"
#include "GraphMath/Quaternion.h"


class WORLDGENERAL_API wNetPos {
public:
		wNetPos();

	void init(float time, const cVector& pos, const cVector& vel);
	void addPos(float current, float measure, const cVector& pos, const cVector& vel);
	void getPos(float time, cVector& pos, cVector& vel)
		{ path.value(time, pos, vel); }

	float lastUpdate() const { return lastTime; }

private:
	class WORLDGENERAL_API Path {
	public:
			Path();
			Path(float t0, const cVector& p0, const cVector& v0,
				float t1, const cVector& p1, const cVector& v1)
				{ init(t0, p0, v0, t1, p1, v1); }

		void init(float t0, const cVector& p0, const cVector& v0,
				float t1, const cVector& p1, const cVector& v1);

		void value(float t, cVector& pos, cVector& vel);

	private:
		cVector c[3];
	};

private:
	Path path;
	float lastTime;
	cVector lastPos, lastVel;

};


class WORLDGENERAL_API wNetRot {
public:
		wNetRot();

	void init(float time, const Quaternion& q);
	void addRot(float current, float measure, const Quaternion& rot);
	void getRot(float t, Quaternion& r)
		{ rot.value(t, r); }

private:
	class WORLDGENERAL_API Rot {
	public:
			Rot()
				{ init(0, Quaternion::One, 1, Quaternion::One); }
			Rot(float t0, const Quaternion& r0, float t1, const Quaternion& r1)
				{ init(t0, r0, t1, r1); }

		void init(float t0, const Quaternion& r0, float t1, const Quaternion& r1);
		void value(float t, Quaternion& r);

	private:
		Quaternion begin, end;
		float t0, inv_dt;
	};

private:
	Rot rot;
	float lastTime;
	Quaternion lastRot;

};


#endif /* wNetPos_h */
