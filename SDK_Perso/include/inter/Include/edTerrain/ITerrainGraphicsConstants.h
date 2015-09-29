#pragma once
#include <stdint.h>

namespace render 
{
	////////////////////////////////////
	// 
	// пространство
	// 
	enum enSpace
	{
		SP_WORLDSPACE = 0,
		SP_WORLDSPACEORIGIN = 1,
		SP_CAMERASPACE = 2
	};

	////////////////////////////////////
	// 
	// тип проекции камеры
	// 
	enum enProjection
	{
		PR_PERSP=0, 
		PR_ORTHO=1,
		PR_CUSTOM=2
	};

	////////////////////////////////////
	// 
	// шейдинг модели
	// 
	enum enShadingModel
	{
		SHM_UNKNOWN				= -1,			// UNKNOWN
		SHM_FINALCOLOR			= 0, // проход финального цвета
		SHM_DEFERREDSHADING		= 1, // deferred
		SHM_TRANSPARENT			= 2, // прозрачные объекты
		SHM_TRANSPARENTMAP		= 3, // в таргет прозрачных объектов
		SHM_FLATSHADOWS			= 4, // плоские тени
		SHM_SHADOWMAP			= 5, // shadow map
		SHM_SHELFMAP			= 6, // в таргет дна
		SHM_WAVEMAP				= 7, // в таргет прибоя
		SHM_HEIGHTMAP			= 8, // в таргет высот и типа поверхности
		SHM_LIGHTMAP			= 9, // в таргет перспективной освещенки
		SHM_ENVIRONMENTMAP		= 10, // в кубический таргет окружения
		SHM_MAPTEX				= 11, // рендер карты (спутниковый вид)
		SHM_MAPALT				= 12, // рендер карты (с высотами)
		SHM_MAP					= 13, // рендер карты (политическая)
		SHM_REFLECTIONMAP		= 14, // в таргет отражений
		SHM_COMPUTE				= 15, // расчет буферов для инстансера
		SHM_DEFERRED_LIGHTS		= 16, // применение освещения в DS
		// Служебные
		SHM_DEPTH_OF_FIELD		= 17, // depth-of-field
		SHM_OCCLUSION			= 18, // occlusion
		SHM_CASCADED_SHADOWMAP	= 19, 
		SHM_RADAR = 20,
		SHM_MAX
	};
	inline const char* getShadingModelName(enShadingModel sm);

	////////////////////////////////////
	// 
	// маски шейдинг моделей
	// 
	enum enShadingModelMask
	{
		SHMM_FINALCOLOR			= 1<<SHM_FINALCOLOR, // проход финального цвета
		SHMM_DEFERREDSHADING	= 1<<SHM_DEFERREDSHADING, // deferred
		SHMM_TRANSPARENT		= 1<<SHM_TRANSPARENT, // прозрачные объекты
		SHMM_TRANSPARENTMAP		= 1<<SHM_TRANSPARENTMAP, // в таргет прозрачных объектов
		SHMM_FLATSHADOWS		= 1<<SHM_FLATSHADOWS, // плоские тени
		SHMM_SHADOWMAP			= 1<<SHM_SHADOWMAP, // shadow map
		SHMM_SHELFMAP			= 1<<SHM_SHELFMAP, // в таргет дна
		SHMM_WAVEMAP			= 1<<SHM_WAVEMAP, // в таргет прибоя
		SHMM_HEIGHTMAP			= 1<<SHM_HEIGHTMAP, // в таргет высот и типа поверхности
		SHMM_LIGHTMAP			= 1<<SHM_LIGHTMAP, // в таргет перспективной освещенки
		SHMM_ENVIRONMENTMAP		= 1<<SHM_ENVIRONMENTMAP, // в кубический таргет окружения
		SHMM_MAPTEX				= 1<<SHM_MAPTEX, // рендер карты (спутниковый вид)
		SHMM_MAPALT				= 1<<SHM_MAPALT, // рендер карты (с высотами)
		SHMM_MAP				= 1<<SHM_MAP, // рендер карты (политическая)
		SHMM_REFLECTIONMAP		= 1<<SHM_REFLECTIONMAP, // в таргет отражений
		SHMM_COMPUTE			= 1<<SHM_COMPUTE, // расчет буферов для инстансера
		SHMM_DEFERRED_LIGHTS	= 1<<SHM_DEFERRED_LIGHTS, // применение освещения в DS
		// Служебные
		SHMM_DEPTH_OF_FIELD		= 1<<SHM_DEPTH_OF_FIELD, // depth-of-field
		SHMM_OCCLUSION			= 1<<SHM_OCCLUSION, // occlusion
		SHMM_CASCADED_SHADOWMAP = 1 << SHM_CASCADED_SHADOWMAP, 
		SHMM_RADAR				= 1 << SHM_RADAR, 
	};
	inline ed::string getShadingModelsName(uint32_t mask);

	////////////////////////////////////
	// 
	// Id общей текстуры
	// 
	enum class SharedTextureHandle
	{
		Unknown = -1,
		CloudShadowsMap = 0,
		HeightMap = 1, 
		SkyMap = 2, 
		LightMap = 3,
		ReflectionMap = 4,
		EnvironmentCube = 5,
		WaveMap = 6,
		FFTMap = 7,
		AtmosphereTransmittance = 8,
		AtmosphereIrradiance = 9,
		AtmosphereInscatter = 10,
		SkyTex = 11,
		CascadeShadowMap = 12,
		ShadowMapDepth = 13,
		ShadowMapColor = 14,
	};
	// по имени получить SharedTextureHandle
	// Хендл валиден все время
	inline SharedTextureHandle resolveSharedTextureName(const ed::string& name); 


	////////////////////////////////////
	// 
	// Id общего Structured буфера
	// 
	enum SharedStructuredBufferHandle
	{		
		Unknown = -1,
		AmbientCube = 0,

		elementsCount
	};
	// по имени получить SharedStructuredBufferHandle
	// Хендл валиден все время
	inline SharedStructuredBufferHandle resolveSharedStructuredBufferName(const ed::string& name);


	////////////////////////////////////
	// 
	// Id выборки
	// 
	typedef uint32_t SampleHandle;
	static const SampleHandle INVALID_SAMPLE_HANDLE = (SampleHandle)~0;
}