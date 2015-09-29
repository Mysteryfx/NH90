#ifndef ED_MODEL_ILIGHT_H
#define ED_MODEL_ILIGHT_H

namespace model{

/// Describes light source.
class ILight {
protected:
public:
	virtual ~ILight(){}

	/// Returns position of light source.
	/// \param modelPos holds position of model.
	virtual osg::Matrixd getPosition(const osg::Matrixd &modelPos,const Arguments &args,const osg::Matrixd *bones,unsigned int nBones)const = 0;
	/// Returns position of light source.
	/// \param modelPos holds position of model.
	virtual void getPosition(const osg::Matrixd &modelPos,const Arguments &args,const osg::Matrixd *bones,unsigned int nBones,osg::Matrixd &pos)const = 0;
	
	/// Returns visibility of light source.
	virtual bool getVisibility(const Arguments &args)const = 0;

	/// Returns color of light.
	virtual const osg::Vec3f& getColor()const = 0;

	/// Returns brightness of light.
	virtual float getBrightness()const = 0;

	/// Returns distance of light.
	virtual float getDistance()const = 0;

	/// Returns type of light source.
	virtual light::LightType getType()const = 0;

	/// Returns inner angle (in radians) of spot light cone or direct light inner cylinder radius.
	virtual float getPhi()const = 0;
	
	/// Returns outer angle (in radians) of spot light cone or direct light outer cylinder radius.
	virtual float getTheta()const = 0;

	/// Returns texture of light.
	virtual Texture2dProperties& getTexture() = 0;

	/// Returns texture of light.
	virtual const Texture2dProperties& getTexture()const = 0;

	/// Applies model instance arguments to all light properties.
	virtual void apply(const Arguments &args)const = 0;
};
}
#endif
