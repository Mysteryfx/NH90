#pragma once
#include "Math/Predictor.h"
#include "Math/RateMeter.h"
#include "cLinear.h"
#include "cPosition.h"
#include "dPosition.h"
#include "GraphMath/vector.h"

class wNetObj
{
public:
	struct UpdData
	{
		UpdData(double time, const dVector& pos,
			const cVector& rot, const cVector& vel);

		unsigned int step;
		dVector pos;
		cVector rot;//yaw, pitch, roll
		cVector vel;	
	};

public:
	wNetObj();
	~wNetObj();

	void init(double time, const UpdData&);
	void reset(double time);

	void addUpdate(const UpdData&);//unsigned int step, unsigned char flags, Mail::Reader data);
	void nextStep(double time);
	
	double getCurrentTime() const;

	dVector getPoint(double time);
	void	getPos(cPosition& outPos, cVector* outVel = 0, cVector* outAcc = 0);
	
private:
	struct State {
		unsigned int step;

		Math::Predictor4 pos[3];
		Math::Predictor3 rot[2][3];

		void init(const dVector& p, const cVector& vel, const cVector& r, const double kp[4], const double kr[3]);
		double update(const dVector &p, const cVector& r);
		void stepBy(int by, const double kp[4], const double kr[3]);

		void stepTo(int to, const double kp[4], const double kr[3])
		{
			int diff = to - step;
			if( diff > 0 )
				stepBy(diff, kp, kr);
		}

		void reconf(const double old_kp[4], const double old_kr[3], const double new_kp[4], const double new_kr[3]);

		void freeze(bool useVel, const double kp[4]);

	private:
		static void get(cVector base[2], const cVector& r);
	};

	Math::RateMeter rate;
	
	State snapshot;
	State predictor;
	State position;

	dPosition pos;
	Vector3 accel, vel;
	cPosition c_pos;

	float lastUpdVelocity;

	bool m_needReset;
	int m_conf;

	double pos_filter[4];
	double rot_filter[3];
	static double ref_pos_filter[4];
	static double ref_rot_filter[3];

	static const double netResetDiff;
	static const int predFreq;

	bool calc_position(int step);
	void step_position(int step);
	void reset_position(int step);

	void get_position();
	void update_filters();
	inline unsigned int posStep() const;

	typedef ed::vector<UpdData> UpdateList;
	UpdateList m_updates;
};
