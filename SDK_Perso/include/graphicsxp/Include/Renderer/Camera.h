#ifndef Camera_H
#define Camera_H

#include "graphicsxp.h"
#include "position.h"
#include "box.h"
#include "osg/Vec3f"

#include "ClipVolumes/ClipVolumeCamera.h"
#include "ClipVolumes/ClipVolumeBox.h"

namespace Graphics{

//TODO: Special method returning dxmatrix.
// Дескриптор камеры на уровне Renderer
class Camera{
public:
	enum Type{
		ctPerspective,  // взгляд из точки
		ctParallel,	 // параллельная проекция
	};
private:
	// тип камеры
	Type _type;

	mutable ClipVolumeCamera _perspectiveVolume;
	mutable ClipVolumeBox _ortoVolume;

	mutable ClipVolume *_curVolume;
	// положение камеры
	Position3	_pos;
	
	// углы раствора - в радианах (для перспективной проекции)
	float _hAngle, _vAngle;
    float _aspect;
	float _nearBorder, _farBorder;
	// Позиция камеры в мире
	Vector3		_origin;
//	osg::Vec3d	_originOsg;
	// границы области (для параллельной проекции)
	Box _volume;

	mutable bool _dirty;
public:
	GRAPHICSXP_API Camera(Type type = ctPerspective);
	~Camera(){}

	/// Sets camera type.
	void setType(Type type){_type=type;_dirty=true;}
	/// Returns camera type.
	Type getType()const{return _type;}

	/// Sets camera position.
	void setPos(const Position3 &pos){_pos=pos; _dirty=true; }
	/// Returns camera position.
	const Position3& getPos() const {return _pos;}
	inline osg::Vec3f getOsgPos() const {return osg::Vec3f(_pos.p.x, _pos.p.y, _pos.p.z);}

	/// Edits camera position.
	/// Warning! This method marks camera as dirty.
	Position3& editPos(){_dirty=true; return _pos;}
	
	/// Sets horizontal angle for perspective projection.

    void setHAngleAndAspect(float h_,float aspect)
    {
        _hAngle = h_;
        _aspect = aspect;
        if (_aspect == 1.0)
        {
            _vAngle = _hAngle;
        }
        else
        {
            _vAngle = 2.0f * atan(tanf(0.5f * _hAngle) / _aspect);
        }
        _dirty  = true;
    }
	/// Returns horizontal angle for perspective projection.
	float getHAngle() const {return _hAngle;}
	/// Returns vertical angle for perspective projection.
	float getVAngle() const { return _vAngle;}
    float getAspect() const { return _aspect; } ;

	/// Sets near border.
	void setNearBorder(float a) {_nearBorder=a;_dirty=true;}
	/// Returns near border.
	float getNearBorder()const{return _nearBorder;}
	
	/// Sets near border.
	void setFarBorder(float a) {_farBorder=a;_dirty=true;}
	/// Returns near border.
	float getFarBorder() const {return _farBorder;}

	/// Sets camera origin.
	void setOrigin(const Vector3 &origin){ _origin=origin; _dirty=true; }
	void setOrigin(const dVector &origin){ _origin=Vector3(origin.x, origin.y, origin.z); _dirty=true; }

	/// Returns camera origin.
	const Vector3& getOrigin() const {return _origin;}
	osg::Vec3f getOsgOrigin() const {return osg::Vec3f(_origin.x, _origin.y, _origin.z);}

	/// Sets camera volume for orthograph projection.
	void setVolume(const Box &volume) {_volume=volume;_dirty=true;}
	/// Returns camera volume for orthograph projection.
	const Box& getVolume() const {return _volume;}
	/// Edits camera volume for orthograph projection.
	/// Warning! This method marks camera as dirty.
	Box& editVolume() {_dirty=true; return _volume;}
	
	/// Returns clip volume of camera.
	/// Warning! Don't delete returned object as it managed inside camera!
	GRAPHICSXP_API ClipVolume* getClipVolume()const;
};
}

#endif // Camera_H
