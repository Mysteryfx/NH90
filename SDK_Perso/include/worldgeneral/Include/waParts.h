//                    file waParts.h
#pragma once

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "woLAConst.h" // из-за drawArgsMax
#include "Randomizer\RandomObject.h"
#include "GraphicEffect.h"
#include "GraphicEffectManager.h"
#include "Randomizer\NoiseGen.h"
#include "WorldMail.h"

class waParts;

struct PartsExtData
{
	ObjectID		objectID;
	ed::string      shapeName;
	cPosition	    pos;
	wsType F_Type;
};

struct PartsExtStatus
{
	cVector	    pos;
	cVector	    rot_Angle;
};


class wPartsMail
{ 
public:
	wPartsMail();
	virtual ~wPartsMail();

	typedef GenMessage<wPartsExtMsgID+0, Mail::CreateMsg, PartsExtData> MsgCreate;
	typedef GenMessage<wPartsExtMsgID+1, Mail::CreateMsg, ObjectID> MsgDestroy;
	typedef GenMessage<wPartsExtMsgID+2, Mail::Reliable, ObjectID, PartsExtStatus> MsgStatus;

	bool    createMessage(const PartsExtData & data, Mail::Message & msg);
	void	destroy(const waParts* obj);
};

namespace Mail
{
	Stream& operator >> (Stream& stream, PartsExtData & param);
	Stream& operator >> (Stream& stream, PartsExtStatus & param);
}


class WORLDGENERAL_API waPartsActivity : public wTime
{
public:
	waPartsActivity(waParts *p, wModelTime dt);
	virtual ~waPartsActivity();
	virtual void NextEvent();

	waParts	*boss;
};

#define IID_IwaParts Common::identify<waParts>()

class WORLDGENERAL_API waParts : public MovingObject // Свободно падающие объекты-эффекты
{
public:
	static RandomObject	randomOrderly;	// Собственный итератор в нормальной последовательности
	static RandomObject	randomPhase;
	static NoiseGen		noiseGen;

	static void		Create(const cPosition& pos_,
						   const cVector& V_,
						   const wsType & F_Type,
						   float _vy_hold = -850.0f,
						   const Graphics::DParamList*	drawArgs_ = 0, 
						   float smoke = 0.0f,
						   unsigned livery_id = ~0,//INVALID_LIVERY_HANDLE = ~0
						   const char * model_name = nullptr,
						   bool IsNetObject = false);

	static waParts* Create(const PartsExtData & data);

	waParts();
	~waParts(void);

	void SetNetPart();
	void DestroyNetPart();
	virtual bool        netGetCreate(Mail::Message&) const;
	virtual bool		netSendUpdate(int dest, int prio, int& cookie);

	static waParts *    netCreate(const PartsExtData & data);
	void netChangeStatus(const PartsExtStatus& newStatus);
	void netDestroy();

	void			setPart(const cPosition& pos_,
							const cVector& V_,
							const wsType & F_Type,
							float _vy_hold,
							const Graphics::DParamList* drawArgs_,
							float smoke,
							unsigned livery_id,
							const char * model_name = nullptr);
	virtual void	serialize(Serializer &serializer); 

	cPosition&		Position(wModelTime t);
	virtual dVector		GetPoint(wModelTime t);
	virtual dVector		GetPoint()
	{
		return GetPoint(wTime::GetModelTime());
	}
	const Graphics::DParamList& GetDrawArguments() { return drawArgs; }
	const gBox&		LocalBox(wModelTime t)	{	return box;}

	wtPointer		parLife;


protected:

    void			Init(const char * model_name = nullptr,const Graphics::DParamList * args = nullptr);
	virtual void	Status_Quo(wModelTime);	   // состояние в момент t
	virtual void	Status_Quo_Phantom(wModelTime);	   // состояние в момент t
	void			SpaceRegister(wModelTime dt); 
	void			setShape(const char * model_name = nullptr,const Graphics::DParamList * args = nullptr); // установить форму
	bool			isFonar() const;  //проверить, фонарь ли
	void			removeVariousEffects(); //делает undock для various_effects и очищает его
	
	void            putLandingShape(cPoint&, cVector);  //оставить на земле обломок
	/*получить вектор отклонения относительно свобоной траектории падения*/
	cVector			getDeviation(float phase, float scale);

	bool			bIsFonar; //флаг, является ли объект фонарем, чтобы не дергать постоянно isFonar()
	Graphics::DParamList	drawArgs; // Хранилище для аргументов рисования
	wModelTime		UpdateTime; // засечка времени обновления (введено для анимации) 
	cVector         Ordme;      // вектор угловой скорости.
    float           vy_hold;    // Установившаяся вертикальная скорость
	float           Ay;         // Вертикальное ускорение
	float           Ag;         // Горизонтальное ускорение 
	cVector         nv;         // единичный вектор заданного направления			
	cPosition		pos0;		// начальное положение
	dVector			pos_p0;
	float			vg0;		// начальная горизонтальная скорость
	float			vh0;		// начальная вертикальная скорость 
	float			tg;			// время ускоренного движения в горизонте 
	float			th;			// время ускоренного движения по вертикали 
	float			H_hold;		// высота начала движения с vy_hold
	wModelTime		LTime0;		// мировое время в начале жизни. 

	gBox			box;
	wModelTime		Life_Time;	// время жизни
	gePointer		gSmoke;     // дымный хвост за обломком
	//EffectHolderPtr pFire;
	ed::vector<gePointer> various_effects;
	float			smoke;		// величина дымного хвоста 
	cPosition       fire_pos;   // величина пламени

	cVector			p_loc;      //лок.точка геом.центра

	cVector			devStart;	//стартовое смещение от траектории
	float			devFadeIn;  
	float			rndPhase;	//рандомное смещение фазы для noiseGen
	float			devScale;	//масштаб отклонения от траектории

	PartsExtData m_sNetPartsData;
	PartsExtStatus m_sNetPartsStatus;
	PartsExtStatus m_sNetPartsStatus_Prev;
	cVector	m_NetV;
	cVector	m_NetW;
	wModelTime	m_prevNetUpdateTime;
private:
	friend class wPartsMail;
	static wPartsMail	mail;

	bool isShell() const 
	{
		return  obj_type.Level2() == wsType_Free_Fall && 
				obj_type.Level3() == wsType_ShellCartridge;
	};

	bool m_bNetPhantom;

};