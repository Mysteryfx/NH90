namespace render
{
// mirror of SharedParams in RenderContext.hlsl
inline SharedParams::SharedParams()
{
	sunDirectionV.set(1, 1, 1);
	sunDirectionWS.set(1, 1, 1);
	moonDirectionWS.set(1, 1, 1);
	sunColor.set(1, 1, 1, 1);
	moonColor.set(1, 1, 1, 1);
	ambientColor.set(1, 1, 1, 1);

	shadowsPower = 1;
	civilLights = 1;

	fogCoefficients.set(1, 1, 1, 1);
	fogColor.set(1, 1, 1);
	fogDistances.set(1, 1, 1);

	shadowMapFactor = 1;			
	heightmapAvailable = 0;			
	lightmapAvailable = 0;

	heightmapAvailable = 0;			
	lightmapAvailable = 0;			
	cascadeShadowsAvailable = 0;
	atmosphereAvailable = 0;
};
// 
inline void SharedParams::transpose()
{
	cloudShadowsProj.transpose();
	landHeightmapProj.transpose();
	lightMapProj.transpose();
	prevViewOrigin.transpose();
	prevProj.transpose();
	sunV.transpose();
	invSunV.transpose();
}

}//namespace render
