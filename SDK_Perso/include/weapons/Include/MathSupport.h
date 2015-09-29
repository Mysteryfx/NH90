#pragma once

#include "Weapons.h"
#include "cLinear.h"
#include "cAlgorithm.h"
#include "cPosition.h"

const float vscShell = 860.0f;

struct targeting_info
{
	targeting_info(){target_H = target_distance = FinDirWeapon = FimDirWeapon = 0.0f;};

	float target_H;
	float target_distance;
	float FimDirWeapon;
	float FinDirWeapon;
    float Fi_y_delta;
	float Fi_z_delta;
};

// фильтр первого порядка
//
// tau = 1/3 = dt/K
class msupp_filter_FO
{
public:
	msupp_filter_FO();

	void    setK(float Kn);
	float   step(float UtIn);
	cVector step(cVector UtIn);
    void	reset();

	friend Serializer& operator << (Serializer& serializer, msupp_filter_FO& filter);
private:
	float fUtPrev, K;
	cVector vUtPrev;
};

class WEAPONS_API msupp_aiming
{
public:
	static cVector msupp_calc_weapon_to_point(bool update_pnt, 
												cVector target_vec, 
												cVector target_V, 
												cPosition gun_pos, 
												cPosition plane_pos, 
												targeting_info* targeting_data, 
												cVector V, 
												cVector vWind, 
												bool& flag_aiming, 
												bool& flag_in_angles, 
												bool& flag_shoot, 
												cVector& prev_Tang_aiming, 
												cVector& prev_dPsi_aiming,
												float Skill,
												float vel_out = 860.0f, 
												float t_acc = 0.0f, 
												float Fi_z_gun = 0.0f);

	
	static void msupp_calc_gunpod_to_point(bool update_pnt, 
											  cPosition gun_pos, 
											  cPosition plane_pos, 
											  targeting_info* targeting_data, 
											  cVector V, 
											  cVector vWind);

	static void msupp_calc_back_cannon_to_point(cPosition target_pos, cVector target_vel, float target_converg_vel, cPosition gun_pos, cPosition plane_pos, targeting_info* targeting_data, cVector V, cVector vWind);
	static cVector msupp_calc_cannon_to_point(cPosition target_pos, cVector target_vel, float target_converg_vel, cVector N_tgt, cPosition gun_pos, cPosition plane_pos, targeting_info* targeting_data, cVector V, cVector vWind, bool& flag_in_angles);
};

