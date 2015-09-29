
inline edtg::TerrainOptions::Weather::Weather()
{
	windDirection.set(1.0f, 0.0f);
	windSpeed = 10.f;
	snowDensity = 0.2f;
};

inline edtg::TerrainOptions::Debug::Debug()
{
	switchoffDrawRoutine = 0;
	switchoffEdm = 0;
	switchoffFetchSurface = 0;
	switchoffFetchUniqueScenes = 0;
	switchoffFetchDistricts = 0;
	switchoffFetchSmokes = 1;
	switchoffRenderLights = 0;
	switchoffFetchLights = 0;
	switchoffRenderLockonTrees = 1;
	switchoffSomething = 0;
	clipmapDebugTextures = 0;
	switchoffClipmapUpdates = 0;
	switchoffVehicle = 0;
	switchoffReference[0] = 0;

	test1 = test2 = test3 = test4 = test5 = 0;
	viewBoxesForSemantic = -1;
	renderMode = 0;
};

inline edtg::TerrainOptions::TerrainOptions()
{
	bSkipParse = false;

	dumpFrame = 0;
	dumpOutput = NULL;
	dumpScene = NULL;

	time = 0;

	bDelayLoadingAvailable = true;
	wireframe = false;
	land_noisemax = 0.5f;
	land_noisemin = 0.1f;

	land_noise1front = 1000.f;
	land_noise1back = 10000.f;
	land_noise1top = 2500.f;
	land_noise1bottom = 2000.f;

	land_detailNoiseHeight = 300.f;
	land_detailNoiseSlope = 0.f;
	land_noise1PerSquare = 5.f;
	land_noise2PerSquare = 150.f;

	// district ranges
	distance.uniqueSceneFarDistance = 20000.f;
	// Минимальная площадь объекта на экране. Если меньше - не рендерим
	distance.minimalSquareOnScreen = 20.f;
	distance.minimalSquareOnScreenFull = 40.f;
	distance.levelformap = -1;
	distance.smallShitDimention = 200;
	distance.smokesSceneFarDistance = 10000;
	// дальность отрисовки лодов для деталей
	for(int i=0; i<sizeof(distance.detailsDistances)/sizeof(float); i++)
		distance.detailsDistances[i] = (256<<i);

	// максимальное число кварталов вокруг камеры
	district.maxDistrictsAround = 10000;
	district.arrangeDistrictDistances = 0;
	// Расстояние дальше которого кварталы уже не рендертся
	district.farDistance = 10000.0f;
	// Максимально дальное расстояние на котором кварталы ещё рендерятся целиком
	district.farFullRenderDistance = 6000.0f;
	// Максимально близкое расстояние на котором кварталы ещё рендерятся целиком
//	district.nearFullRenderDistance = 800.0f;
	// Расстояние ближе которого кварталы уже не рендерятся
	district.nearDistance = 560.0f;
	// Расстояние на котором переключаются лоды
	district.treeslodDistance = 500.0f;
	// Расстояние дальше которого кварталы (деревья) уже не рендертся
	district.treesFarDistance = 3000.0f;
	// Максимально дальное расстояние на котором кварталы (деревья) ещё рендерятся целиком
	district.treesFarFullRenderDistance = 2000.0f;
	// Максимально дальное расстояние на котором кварталы (деревья) ещё не блендятся с текстурой подложки
	district.treesFarFullBlendRenderDistance = 2500.0;
	district.heightFactor = 0.0f;
	district.heightRandomFactor = 0.0f;
	district.lampFarDistance = 5000.0;
	district.splineBlockFarDistance = 3000.0;
	//	district.maxFarDistance = 10000.0f;
	// Настройки квартальных фанарей
	district.lamp.lampOn = 1;
	district.lamp.maxSize = 7.4f;
	district.lamp.staticSize = 4.9f;
	district.lamp.spriteScale = 0.0025f;
	district.lamp.minDistance = 100.0f;
	district.lamp.maxDistance = 3385.0f;
	district.lamp.maxAlphaDistance = 1300.0f;
	district.lamp.minAlphaDistance = 250.0f;
	district.lamp.minAlpha = 0.36f;
	district.lamp.maxAlpha = 0.26f;
	district.lamp.minBrightnessDistance = 0.0f;
	district.lamp.maxBrightnessDistance = 15000.0f;
	district.lamp.dsLightRadius = 80.0f;
	district.lamp.dsLightBrightness = 4.0f;

	district.renderType = TerrainOptions::DISTRICT_RENDER_SIMPLE;

	// Плоские тени
	flatShadow.enabled = true;
	flatShadow.farDistance = district.farDistance;
	flatShadow.fullFarDistance = 0.8f * flatShadow.farDistance;

	//Infrared options
	infrared_landDetail = 0.8f;
	infrared_landDarkness = 1.0f;
	infrared_riverDarkness = 0.7f;
	infrared_roadDarkness = 1.5f;
	infrared_runwayDarkness = 1.7f;

	//Новый шум
	noise_startDistance = 10000.0f;
	noise_endDistance = 100.0f;
	noise_maxBlend = 0.7f;
	noise_scale = 10.0f;

	// Шум для рампы
	rampNoise_power = 0.5f;
	rampNoise_scale = 20.0f;

	smallNoise_startDistance = 100.0;
	smallNoise_endDistance = 1.0;
	smallNoise_maxBlend = 0.5;
	smallNoise_scale = 1000.0;

	// Ближние и дальние расстояния для бленда по-дальности. Отдельные для каждого типа накладных.
	distanceBlend.townNearDistance      = 4000.0;
	distanceBlend.townFarDistance       = 10000.0;
	distanceBlend.townLightNearDistance = 4000.0;
	distanceBlend.townLightFarDistance  = 10000.0;
	distanceBlend.fieldNearDistance     = 4000.0;
	distanceBlend.fieldFarDistance      = 10000.0;
	distanceBlend.waterNearDistance     = 4000.0;
	distanceBlend.waterFarDistance      = 10000.0;
	distanceBlend.shelfNearDistance     = 4000.0;
	distanceBlend.shelfFarDistance      = 10000.0;
	distanceBlend.subforest = osg::Vec2f(4000.0, 10000.0);
	distanceBlend.beach = osg::Vec2f(4000.0, 10000.0);
	distanceBlend.road = osg::Vec2f(4000.0, 10000.0);
	distanceBlend.townClipmap = osg::Vec2f(100.0, 1000.0);

	// Настройка новых фонарей
	lamp31.lampOn = 1;
	lamp31.maxSize = 7.4f;
	lamp31.staticSize = 4.9f;
	lamp31.spriteScale = 0.0025f;
	lamp31.minDistance = 100.0f;
	lamp31.maxDistance = 3385.0f;
	lamp31.maxAlphaDistance = 1300.0f;
	lamp31.minAlphaDistance = 250.0f;
	lamp31.minAlpha = 0.36f;
	lamp31.maxAlpha = 0.26f;
	lamp31.minBrightnessDistance = 0.0f;
	lamp31.maxBrightnessDistance = 15000.0f;

	// Настройка фонарей
	airfieldlamp.lampOn = 1;
	airfieldlamp.maxSize = 5.4f;
	airfieldlamp.staticSize = 2.9f;
	airfieldlamp.spriteScale = 0.0025f;
	airfieldlamp.minDistance = 100.0f;
	airfieldlamp.maxDistance = 3385.0f;
	airfieldlamp.maxAlphaDistance = 1300.0f;
	airfieldlamp.minAlphaDistance = 250.0f;
	airfieldlamp.minAlpha = 0.36f;
	airfieldlamp.maxAlpha = 0.26f;
	airfieldlamp.minBrightnessDistance = 0.0f;
	airfieldlamp.maxBrightnessDistance = 15000.0f;
	//Small shit
	/*/
	TerrainOptions::SmallShitOptions::File file;
	file.name = "grass";
	file.slope = 0.3f;
	file.slopeDelta = 0.2f;
	file.texFileName = "smallShit_grass.dds";
	TerrainOptions::SmallShitOptions::File::Layer layer;
	layer.scale = 1.5f;
	layer.squareSize = 300.0f;
	file.layers.push_back(layer);
	layer.scale = 0.75f;
	layer.squareSize = 150.0f;
	file.layers.push_back(layer);
	smallShit.files.push_back(file);
	smallShit.heightMapSize = 512;
	smallShit.referenceDimention = 300;
	smallShit.dimention = smallShit.referenceDimention;
	/*/


	{
		fan.pos.set(0, 0, 0);
		fan.dir.set(0, 1, 0);
		fan.power = 0;
		fan.radius = 0;
		fan.oscillator = 0.1f;
		fan.frequency = 1.f;
	}
	/*/
	moonDirectionWS.set(0,0,0);
	moonColor.set(0,0,0,0);
	sunDirectionWS.set(0,0,0);
	sunBiasDirectionWS.set(0,0,0);
	sunColor.set(0,0,0,0);
	ambientColor.set(0,0,0,0);
	/*/

	memset(hiddensemantics, 0, sizeof(hiddensemantics));
	memset(hiddenlayer, 0, sizeof(hiddenlayer));
	memset(hiddenlevels, 0, sizeof(hiddenlevels));
	memset(hiddencameras, 0, sizeof(hiddencameras));
	memset(hiddenqueues, 0, sizeof(hiddenqueues));

	heightmap.enabled = false;
	heightmap.resolution = 0;
	heightmap.size = 0;

	surfaceDetails.surfaceBlendStrength = 0.0f;
}
