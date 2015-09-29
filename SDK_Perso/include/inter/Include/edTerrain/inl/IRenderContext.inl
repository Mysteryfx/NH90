namespace render 
{

inline IRenderContext* IRenderContext::fromMeta(render::MetaContext& metaContext)
{
	void* value = metaContext.getExtension(render::CEX_RENDER_CONTEXT);
	if(!value) return nullptr;
	return (IRenderContext*)value;
}

// по имени получить SharedTextureHandle
// Хендл валиден все время
inline SharedTextureHandle resolveSharedTextureName(const ed::string& name)
{
	if(name=="CloudShadowsMap")
		return SharedTextureHandle::CloudShadowsMap;
	if(name=="HeightMap")
		return SharedTextureHandle::HeightMap;
	if(name=="SkyMap")
		return SharedTextureHandle::SkyMap;
	if(name=="LightMap")
		return SharedTextureHandle::LightMap;
	if(name=="ReflectionMap")
		return SharedTextureHandle::ReflectionMap;
	if(name=="EnvironmentCube")
		return SharedTextureHandle::EnvironmentCube;
	if(name=="WaveMap")
		return SharedTextureHandle::WaveMap;
	if(name=="FFTMap")
		return SharedTextureHandle::FFTMap;
	if(name == "transmittanceTex")
		return SharedTextureHandle::AtmosphereTransmittance;
	if(name == "irradianceTex")
		return SharedTextureHandle::AtmosphereIrradiance;
	if(name == "inscatterTex")
		return SharedTextureHandle::AtmosphereInscatter;
	if(name == "skyTex")
		return SharedTextureHandle::SkyTex;
	if(name == "cascadeShadowMap")
		return SharedTextureHandle::CascadeShadowMap;
	if(name == "shadowMapColor")
		return SharedTextureHandle::ShadowMapColor;
	if(name == "shadowMapDepth")
		return SharedTextureHandle::ShadowMapDepth;

	return SharedTextureHandle::Unknown;
}
// по имени получить SharedStructuredBufferHandle
// Хендл валиден все время
inline SharedStructuredBufferHandle resolveSharedStructuredBufferName(const ed::string& name)
{
	if(name=="AmbientCube")
		return SharedStructuredBufferHandle::AmbientCube;

	return SharedStructuredBufferHandle::Unknown;
}

inline ed::string getShadingModelsName(uint32_t mask)
{
	ed::string name;
	for(int i=0; i<SHM_MAX; i++)
	{
		if( (mask & (1<<i))==0)
			continue;
		if( !name.empty())
			name += ", ";
		name += getShadingModelName(enShadingModel(i));
	}
	return name;
}

inline const char* getShadingModelName(enShadingModel sm)
{
	switch(sm)
	{
		case render::SHM_FINALCOLOR:		return "FINALCOLOR";
		case render::SHM_DEFERREDSHADING:	return "DEFERREDSHADING";
		case render::SHM_TRANSPARENT:		return "TRANSPARENTMAP";
		case render::SHM_TRANSPARENTMAP:	return "TRANSPARENTMAP";
		case render::SHM_FLATSHADOWS:	return "FLATSHADOWS";
		case render::SHM_SHELFMAP:		return "SHELFMAP";
		case render::SHM_WAVEMAP:		return "WAVEMAP";
		case render::SHM_HEIGHTMAP:		return "HEIGHTMAP";
		case render::SHM_LIGHTMAP:		return "LIGHTMAP";
		case render::SHM_ENVIRONMENTMAP:return "ENVIRONMENTMAP";
		case render::SHM_MAPTEX:		return "MAPTEX";
		case render::SHM_MAPALT:		return "MAPALT";
		case render::SHM_MAP:			return "MAP";
		case render::SHM_REFLECTIONMAP: return "REFLECTIONMAP";
		case render::SHM_COMPUTE:		return "COMPUTE";
		case render::SHM_DEPTH_OF_FIELD:		return "DEPTH_OF_FIELD";
		case render::SHM_DEFERRED_LIGHTS:	return "DEFFEREDLIGHTS";
		case render::SHM_MAX:			return "unknown";
		case render::SHM_UNKNOWN:		return "unknown";
#ifndef EDGE
		case render::SHM_CASCADED_SHADOWMAP: return "CASCADED_SHADOWMAP";
#endif
	}
	return "unknown";
}

} //namespace render