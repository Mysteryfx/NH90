#ifndef _GraphicEffect_h_
#define _GraphicEffect_h_

#include "Modeler\Model.h"
#include "Modeler\ModelInstance.h"
#include "Randomizer\RandomObject.h"
#include "viObject.h"


#define UNDOCK_EFFECT(effect) if((effect)) { (effect)->undock(); effect = nullptr; }

namespace Effects
{

extern RandomObject rndEven;
extern RandomObject rndNormal;

enum GraphicEffectType
{
    eGroundExp          = 0,
    eNapalm             = 1,
    eCluster            = 2,
    eVolume             = 3,
    eSky                = 4,
    eWater              = 5,
    eSimpleGround       = 6,
    eSimpleSky          = 7,
    eSmokeCloud         = 8,
    eSkyDirect          = 9,
    eFireTail           = 10,
    eFireShort          = 11,
    eFireMiddle         = 12,
    eSmokeOneWhite      = 13,
    eSmokeOneWhiteShort = 14,
    eTankShot           = 15,
    eFireShortZero      = 16,
    eFireMiddleZero     = 17,
    eSmokeOneGray       = 18,
    eSmokeOneBlack      = 19,
    eSmokeOneWhiteZero  = 20,
    eGroundExp1         = 21,
    eGroundExp2         = 22,
    eSmokePuff		= 23,
    eRocketStart	= 24,
    eFlash		= 25,
    eSky1		= 26,
    eSky2		= 27,
    eNapalm1		= 28,
    eSimpleGround1	= 29,
    eGunSmoke		= 30,

	eShellOnGround = 31,
	eShellOnObject = 32,
};

enum TypeSmoke
{
    // Следы от ракет. Цвет как у внешнего освещения (от солнца)
    SMALLROCKET    =   0,
    BIGROCKET      =   0,

    // Инверсионный след. Цвет от солнца
    CONTRAIL       =   1,

    // Следы от горящих самолетов и т.д. Цвет темно-серый
    FIRETAIL       =   2, // с пламенем
    BLACKTAIL      =   3, // без пламени

    // Можно задать произвольный цвет
    SIMPLESMOKE    =   4,

	SIMPLESMOKELONG =  5,
	SHELL_SMOKE		=  6,
};

enum TypeDust
{
    CAR = eSmokeOneGray,
    PLANE = eSmokeOneGray
};

enum gePrecType
{
    gptRain,
    gptSnow,
};

struct IGraphicEffectParamBlock;

class GraphicEffect : public viObject,
                      public Graphics::Model,
                      public Graphics::ModelInstance,
                      public LinkHost
{

public:

    virtual ~GraphicEffect()
    {}

    virtual void dock()													= 0;
    virtual void undock()												= 0;
    virtual void setPower( float pw )									= 0;
	virtual void setScale( float scale )								= 0;
    virtual void changeLocalPosition( const cVector   *lpos )			= 0;
	virtual void changeLocalPosition( const Position3 *lpos )			= 0;

	//параметры всего эффекта
	virtual void setParam(int id, float val)							= 0;

	//возвращает блок параметров эмиттера, если он существует
	virtual IGraphicEffectParamBlock* getEmitterParamBlock(int emitterId) { return nullptr; }
};

class IObjectFire
{
public:
	virtual void setPosSource(cPosition *pos) = 0;
};

//заворачиваем левые параметры эмиттера в обертку
class EffectParam
{
public:
	enum EffectParamType
	{
		PT_UNKNOWN,
		PT_FLOAT,
		PT_VEC3F,
		PT_VEC3D,
		PT_VEC4F,
		PT_MATRIXF,
		PT_POSITION,
		PT_CPOSITION,
		PT_DPOSITION,
		PT_CSTRING,//const char*
	};

	EffectParamType type;
	void *data;

public:
	EffectParam(): type(PT_UNKNOWN), data(nullptr) {}
	EffectParam(EffectParamType _type, void *ptr): type(_type), data(ptr) {}
};


struct IGraphicEffectParamBlock
{
	~IGraphicEffectParamBlock(){}
	//установить параметр эффекта по id
	virtual void setParam(int id, const EffectParam &param) = 0;
};


} // namespace Effects


typedef Link< Effects::GraphicEffect > gePointer;

#endif // _GraphicEffect_h_
