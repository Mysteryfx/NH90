#include "stdafx.h"
#include "avSampleSystem.h"
#include "WorldManager.h"

#include "Guns\ammo_supply.h"
#include "Guns\aircraft_gun_mount.h"
#include "Database\database.h"
#include "Shells\shell_descriptors.h"
#include "iwHumanPlane.h"
#include "iWoLA.h"
#include "Icommand.h"

using namespace cockpit::Samples;

//make this class available for factory creation
REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::Samples::avSampleSystem>, avSampleSystem);

avSampleSystem::avSampleSystem()
{
}

avSampleSystem::~avSampleSystem()
{
}

void avSampleSystem::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);

	const wShellDescriptor* shell_desc = nullptr;

	if (!shell_desc)
	{
		shell_desc = wDatabase::instance()->getDescriptor<wShellDescriptor>("weapons.shells.M61_20_HE");
	}

	gun_sight.setShellDesc(shell_desc);

	make_default_activity(0.02); // update() will be called 50 time per second

	listen_command(this, iCommandPlaneFire);
	listen_command(this, iCommandPlaneFireOff);
}

void avSampleSystem::SetCommand(int command, float value /*= 0*/)
{
	switch (command)
	{
	case iCommandPlaneFire:
		avWeaponsCommon::fire_cannon(true);
		break;
	case iCommandPlaneFireOff:
		avWeaponsCommon::fire_cannon(false);
		break;
	default:
		break;
	}
}

void avSampleSystem::update()
{
	update_gun_sight_funnel();
}


void avSampleSystem::update_gun_sight_funnel()
{
	/*
	dVector FunPos;
	wModelTime Time;
	wModelTime dtime;
	cVector V, Vec;
	cVector Om;

	dtime = p_device_timer->GetDeltaTime();

	cPosition humanPosition = get_context()->human_ptr->Position(wTime::GetModelTime());

	Om.x = -get_context()->human_ptr->Omx() * 1.0f * dtime * 4.0f + Om_old.x * (1.0 - dtime * 4.0);
	Om.y = get_context()->human_ptr->Omy() * 0.5f * dtime * 4.0f + Om_old.y * (1.0 - dtime * 4.0);
	Om.z = get_context()->human_ptr->Omz() * 0.5f * dtime * 4.0f + Om_old.z * (1.0 - dtime * 4.0);

	Om_old = Om;

	static float FunnelMinDistance = 250.0;
	static float FunnelMaxDistance = 1500.0;
	static float wingspan = 15.0;

	for (int i = 0; i < 6; i++)
	{
		gun_sight.calculate_funnel(FunnelMinDistance + i / 5.0f * (FunnelMaxDistance - FunnelMinDistance), Time, FunPos);

		Angles[i].x -= Om.x * dtime;
		Angles[i].y -= Om.y * dtime;
		Angles[i].z -= Om.z * dtime;
		Omega[i].push(Om);

		while (Omega[i].size() > Time / dtime)
		{
			Vec = Omega[i].front();
			Angles[i].x += Vec.x * dtime;
			Angles[i].y += Vec.y * dtime;
			Angles[i].z += Vec.z * dtime;
			Omega[i].pop();
		}

		Vec.x = FunPos.x;
		Vec.y = FunPos.y;
		Vec.z = FunPos.z;
		mult_transp(V, humanPosition, Vec);
		FunPos.x = V.x;
		FunPos.y = V.y;
		FunPos.z = V.z;

		// Проверить повороты!!!
		FunPos = FunPos.rotZ(Angles[i].z);
		FunPos = FunPos.rotY(Angles[i].y);
		FunPos = FunPos.rotX(Angles[i].x);

		FunnelPoints[i].x = FunPos.x;
		FunnelPoints[i].y = FunPos.y;
		FunnelPoints[i].z = FunPos.z;
		FunnelPoints[i + 6] = FunnelPoints[i];
		FunnelPoints[i].z -= wingspan / 2.0f;
		FunnelPoints[i + 6].z += wingspan / 2.0f;
	}
	*/
}
