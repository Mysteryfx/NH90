#pragma once
#include <stdint.h>

namespace render 
{
	////////////////////////////////////
	// 
	// ������������
	// 
	enum enSpace
	{
		SP_WORLDSPACE = 0,
		SP_WORLDSPACEORIGIN = 1,
		SP_CAMERASPACE = 2
	};

	////////////////////////////////////
	// 
	// ��� �������� ������
	// 
	enum enProjection
	{
		PR_PERSP=0, 
		PR_ORTHO=1,
		PR_CUSTOM=2
	};

	////////////////////////////////////
	// 
	// ������� ������
	// 
	enum enShadingModel
	{
		SHM_UNKNOWN				= -1,			// UNKNOWN
		SHM_FINALCOLOR			= 0, // ������ ���������� �����
		SHM_DEFERREDSHADING		= 1, // deferred
		SHM_TRANSPARENT			= 2, // ���������� �������
		SHM_TRANSPARENTMAP		= 3, // � ������ ���������� ��������
		SHM_FLATSHADOWS			= 4, // ������� ����
		SHM_SHADOWMAP			= 5, // shadow map
		SHM_SHELFMAP			= 6, // � ������ ���
		SHM_WAVEMAP				= 7, // � ������ ������
		SHM_HEIGHTMAP			= 8, // � ������ ����� � ���� �����������
		SHM_LIGHTMAP			= 9, // � ������ ������������� ���������
		SHM_ENVIRONMENTMAP		= 10, // � ���������� ������ ���������
		SHM_MAPTEX				= 11, // ������ ����� (����������� ���)
		SHM_MAPALT				= 12, // ������ ����� (� ��������)
		SHM_MAP					= 13, // ������ ����� (������������)
		SHM_REFLECTIONMAP		= 14, // � ������ ���������
		SHM_COMPUTE				= 15, // ������ ������� ��� ����������
		SHM_DEFERRED_LIGHTS		= 16, // ���������� ��������� � DS
		// ���������
		SHM_DEPTH_OF_FIELD		= 17, // depth-of-field
		SHM_OCCLUSION			= 18, // occlusion
		SHM_CASCADED_SHADOWMAP	= 19, 
		SHM_RADAR = 20,
		SHM_MAX
	};
	inline const char* getShadingModelName(enShadingModel sm);

	////////////////////////////////////
	// 
	// ����� ������� �������
	// 
	enum enShadingModelMask
	{
		SHMM_FINALCOLOR			= 1<<SHM_FINALCOLOR, // ������ ���������� �����
		SHMM_DEFERREDSHADING	= 1<<SHM_DEFERREDSHADING, // deferred
		SHMM_TRANSPARENT		= 1<<SHM_TRANSPARENT, // ���������� �������
		SHMM_TRANSPARENTMAP		= 1<<SHM_TRANSPARENTMAP, // � ������ ���������� ��������
		SHMM_FLATSHADOWS		= 1<<SHM_FLATSHADOWS, // ������� ����
		SHMM_SHADOWMAP			= 1<<SHM_SHADOWMAP, // shadow map
		SHMM_SHELFMAP			= 1<<SHM_SHELFMAP, // � ������ ���
		SHMM_WAVEMAP			= 1<<SHM_WAVEMAP, // � ������ ������
		SHMM_HEIGHTMAP			= 1<<SHM_HEIGHTMAP, // � ������ ����� � ���� �����������
		SHMM_LIGHTMAP			= 1<<SHM_LIGHTMAP, // � ������ ������������� ���������
		SHMM_ENVIRONMENTMAP		= 1<<SHM_ENVIRONMENTMAP, // � ���������� ������ ���������
		SHMM_MAPTEX				= 1<<SHM_MAPTEX, // ������ ����� (����������� ���)
		SHMM_MAPALT				= 1<<SHM_MAPALT, // ������ ����� (� ��������)
		SHMM_MAP				= 1<<SHM_MAP, // ������ ����� (������������)
		SHMM_REFLECTIONMAP		= 1<<SHM_REFLECTIONMAP, // � ������ ���������
		SHMM_COMPUTE			= 1<<SHM_COMPUTE, // ������ ������� ��� ����������
		SHMM_DEFERRED_LIGHTS	= 1<<SHM_DEFERRED_LIGHTS, // ���������� ��������� � DS
		// ���������
		SHMM_DEPTH_OF_FIELD		= 1<<SHM_DEPTH_OF_FIELD, // depth-of-field
		SHMM_OCCLUSION			= 1<<SHM_OCCLUSION, // occlusion
		SHMM_CASCADED_SHADOWMAP = 1 << SHM_CASCADED_SHADOWMAP, 
		SHMM_RADAR				= 1 << SHM_RADAR, 
	};
	inline ed::string getShadingModelsName(uint32_t mask);

	////////////////////////////////////
	// 
	// Id ����� ��������
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
	// �� ����� �������� SharedTextureHandle
	// ����� ������� ��� �����
	inline SharedTextureHandle resolveSharedTextureName(const ed::string& name); 


	////////////////////////////////////
	// 
	// Id ������ Structured ������
	// 
	enum SharedStructuredBufferHandle
	{		
		Unknown = -1,
		AmbientCube = 0,

		elementsCount
	};
	// �� ����� �������� SharedStructuredBufferHandle
	// ����� ������� ��� �����
	inline SharedStructuredBufferHandle resolveSharedStructuredBufferName(const ed::string& name);


	////////////////////////////////////
	// 
	// Id �������
	// 
	typedef uint32_t SampleHandle;
	static const SampleHandle INVALID_SAMPLE_HANDLE = (SampleHandle)~0;
}