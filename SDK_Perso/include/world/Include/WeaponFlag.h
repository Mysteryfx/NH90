WEAPON_FLAG_BEGIN
	WEAPON_FLAG(NoWeapon, 0)

	WEAPON_FLAG(LGB, 1u << 1)
	WEAPON_FLAG(TvGB, 1u << 2)
	WEAPON_FLAG(SNSGB, 1u << 3)

	WEAPON_FLAG(HEBomb, 1u << 4)
	WEAPON_FLAG(Penetrator, 1u << 5)
	WEAPON_FLAG(NapalmBomb, 1u << 6)
	WEAPON_FLAG(FAEBomb, 1u << 7)
	WEAPON_FLAG(ClusterBomb, 1u << 8)
	WEAPON_FLAG(Dispencer, 1u << 9)
	WEAPON_FLAG(CandleBomb, 1u << 10)
	WEAPON_FLAG(ParachuteBomb, 1u << 31)

	WEAPON_FLAG(GuidedBomb, LGB | TvGB | SNSGB)
	WEAPON_FLAG(AnyUnguidedBomb, HEBomb | Penetrator | NapalmBomb | FAEBomb | ClusterBomb | Dispencer | CandleBomb | ParachuteBomb)
	WEAPON_FLAG(AnyBomb, GuidedBomb | AnyUnguidedBomb)

	WEAPON_FLAG(LightRocket, 1u << 11)
	WEAPON_FLAG(MarkerRocket, 1u << 12)
	WEAPON_FLAG(CandleRocket, 1u << 13)
	WEAPON_FLAG(HeavyRocket, 1u << 14)

	WEAPON_FLAG(AnyRocket, LightRocket | HeavyRocket | MarkerRocket | CandleRocket)

	WEAPON_FLAG(AntiRadarMissile, 1u << 15)
	WEAPON_FLAG(AntiShipMissile, 1u << 16)
	WEAPON_FLAG(AntiTankMissile, 1u << 17)
	WEAPON_FLAG(FireAndForgetASM, 1u << 18)
	WEAPON_FLAG(LaserASM, 1u << 19)
	WEAPON_FLAG(TeleASM, 1u << 20)
	WEAPON_FLAG(CruiseMissile, 1u << 21)

	WEAPON_FLAG(GuidedASM, LaserASM | TeleASM)
	WEAPON_FLAG(TacticASM, GuidedASM | FireAndForgetASM)
	WEAPON_FLAG(AnyASM, AntiRadarMissile | AntiShipMissile | AntiTankMissile | FireAndForgetASM | GuidedASM | CruiseMissile)

	WEAPON_FLAG(SRAAM, 1u << 22)
	WEAPON_FLAG(MRAAM, 1u << 23)
	WEAPON_FLAG(LRAAM, 1u << 24)

	WEAPON_FLAG(IR_AAM, 1u << 25)
	WEAPON_FLAG(SAR_AAM, 1u << 26)
	WEAPON_FLAG(AR_AAM, 1u << 27)

	WEAPON_FLAG(AnyAAM, IR_AAM | SAR_AAM | AR_AAM | SRAAM | MRAAM | LRAAM)

	WEAPON_FLAG(AnyMissile, AnyASM | AnyAAM)

	WEAPON_FLAG(AnyAutonomousMissile, IR_AAM | AntiRadarMissile | AntiShipMissile | FireAndForgetASM | CruiseMissile)

	WEAPON_FLAG(GUN_POD, 1u << 28)
	WEAPON_FLAG(BuiltInCannon, 1u << 29)

	WEAPON_FLAG(Cannons, GUN_POD | BuiltInCannon)

	WEAPON_FLAG(AntiRadarMissile2, 1u << 30)

	WEAPON_FLAG(AnyAGWeapon, BuiltInCannon | GUN_POD | AnyBomb | AnyRocket | AnyASM)
	WEAPON_FLAG(AnyAAWeapon, BuiltInCannon | GUN_POD | AnyAAM)

	WEAPON_FLAG(UnguidedWeapon, Cannons | BuiltInCannon | GUN_POD | AnyUnguidedBomb | AnyRocket)
	WEAPON_FLAG(GuidedWeapon, GuidedBomb | AnyASM | AnyAAM)

	WEAPON_FLAG(AnyWeapon, AnyBomb | AnyRocket | AnyMissile | Cannons)

	WEAPON_FLAG(MarkerWeapon, MarkerRocket | CandleRocket | CandleBomb)
	WEAPON_FLAG(ArmWeapon, AnyWeapon ^ MarkerWeapon)

	WEAPON_FLAG(AllWeapon, 0xFFFFFFFF)
WEAPON_FLAG_END