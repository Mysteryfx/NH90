#ifndef __GUIDED_CLUSTER_H__
#define __GUIDED_CLUSTER_H__

#include "simulation_block_disc.h"

#include "cx_interpolator.h"

#include "WeaponBlocks.h"
#include "viObjectNode.h"
#include "wMovingObject.h"

#ifndef NO_REAL_LOCKON
#include "lPointOnSurface.h"
#include "edTerrain/PointOnSurfaceAdapter.h"
#include "Randomizer/RandomObject.h"
#endif

class dbg_cone;
class dbg_limited_line;
class dbg_limited_line;

class wAmmunition;
class wGuidedClusterDescriptor;
class wGuidedClusterPart;

typedef ed::vector< std::pair<bool, wGuidedClusterPart*> > GuidedClusterPartsVector;

enum TYPE_OF_CLUSTER_BOMB_PART
{
	SKEET,
	SUBMUNITION,
	OPENED_DISPENSER,
	PANEL,
	NONGUIDED_CLUSTER,
};

class WEAPON_BLOCKS_API wGuidedCluster : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wGuidedCluster, wSimulationBlockDisc, wGuidedClusterDescriptor);
public:
#ifndef NO_REAL_LOCKON
	static void init_randoms(int offset);
	static void serialize_randoms(Serializer& s);

	static RandomObject& getRandom() { return rndNormal_; }
#endif

public:
	wGuidedCluster(const wGuidedClusterDescriptor* desc, wSimulationSystem* system, Lua::Loader* runtime_cfg);
	virtual ~wGuidedCluster();

	virtual double simulate(double time);

	wAmmunition* getParent();
	const wGuidedClusterDescriptor* getDesc() const;

	bool isFirstCasetteOpened;
	bool isSecondCasetteOpened;

protected:
	void clearParts();
	bool checkIsDead();
	void spawnSubmunitions_(const Math::Vec3d& pos, 
							const Math::Vec3d& vel, 
							const Math::Rot3d& par_rot, 
							const Math::Rot3d& init_rot, 
							const Math::Vec3d& omega, 
							const Math::Vec3d* initial_pos);

	void spawnSkeets_(const Math::Vec3d& pos, 
						const Math::Vec3d& vel, 
						const Math::Rot3d& par_rot, 
						const Math::Rot3d& init_rot, 
						const Math::Vec3d& omega, 
						const Math::Vec3d* initial_pos);

	void spawnPanels(const Math::Vec3d& pos, 
		const Math::Vec3d& vel, 
		const Math::Rot3d& par_rot, 
		const Math::Rot3d& init_rot, 
		const Math::Vec3d& omega, 
		const Math::Vec3d* initial_pos);

	void spawnOpenedDispenser_(const Math::Vec3d& pos, 
		const Math::Vec3d& vel, 
		const Math::Rot3d& par_rot, 
		const Math::Rot3d& init_rot, 
		const Math::Vec3d& omega, 
		const Math::Vec3d* initial_pos);

	bool testGroundCollision_(const Math::Vec3d& prev_pos, 
							  const Math::Vec3d& pos, 
							  const Math::Vec3d& vel, 
							  bool			     effect,
							  TYPE_OF_CLUSTER_BOMB_PART type_);

	bool isBLU108Alive(); // Вспомогательная функция, если кассеты BLU живы, наводим камеру только на них

	GuidedClusterPartsVector parts_;

#ifndef NO_REAL_LOCKON
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface point_;		// Collision handling
#else 
	edterrain::PointOnSurfaceAdapter point_;
#endif
#endif

	INPUT_WIRES(3, (
		WIRE(double, rho_),		// Все бомблеты на одной высоте
		WIRE(double, M_),
		WIRE(Math::Vec3d, wind_)
		))

		OUTPUT_WIRES(9, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_),
		WIRE(Math::Rot3d, rot_),
		WIRE(Math::Vec3d, omega_),

		WIRE(Math::Vec3d, colPos_),
		WIRE(Math::Vec3d, colNorm_),
		WIRE(int, colObj_),
		WIRE(Math::Vec3d, colVel_),
		WIRE(int, objPartName_)
		))

	INPUT_PORT_OBJ_PTR(wAmmunition, owner_);

	OUTPUT_PORT(bool, explode_);
	OUTPUT_PORT(bool, died_);

#ifndef NO_REAL_LOCKON
	static RandomObject rndNormal_;
#endif
};


class WEAPON_BLOCKS_API wGuidedClusterDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wGuidedCluster)

public:
	virtual ~wGuidedClusterDescriptor ();

	virtual void serialize(wBlockSerializer& ser);

	virtual wGuidedCluster* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(int, count);					   // Количество бомблетов
	DECL_PARAM(int, effect_count);			   // Сколько рисуется эффектов

	// FM
	DECL_PARAM(double, dt);					   // Шаг времени
	
	DECL_PARAM(double, mass);				   // BLU-108 mass
	
	DECL_PARAM(double, empty_dispenser_mass);
	DECL_PARAM(double, empty_dispenser_L);
	DECL_PARAM(double, empty_dispenser_S);
	DECL_PARAM(double, empty_dispenser_I);
	DECL_PARAM(double, empty_dispenser_Ma);
	DECL_PARAM(double, empty_dispenser_Mw);
	
	DECL_PARAM(double, panel_mass);
	DECL_PARAM(double, panel_L);
	DECL_PARAM(double, panel_S);
	DECL_PARAM(double, panel_I);
	DECL_PARAM(double, panel_Ma);
	DECL_PARAM(double, panel_Mw);

	DECL_PARAM(double, skeet_mass);             // skeet mass
	DECL_PARAM(double, skeet_L);
	DECL_PARAM(double, skeet_S);
	DECL_PARAM(double, skeet_I);
	DECL_PARAM(double, skeet_Ma);
	DECL_PARAM(double, skeet_Mw);

	DECL_PARAM(double, skeetInitVel);
	DECL_PARAM(double, submunitionInitVel);
	
	DECL_PARAM(double, caliber);			   // Калибр, мм
	DECL_PARAM(double, L);					   // Характеристическая длина
	DECL_PARAM(double, I);					   // Moment of inertia along lateral axis
	DECL_PARAM(double, Ix);					   // Moment of inertia along longitudinal axis
	DECL_PARAM(double, Ma);					   // Градиент коэффициента момента по углу атаки
	DECL_PARAM(double, Mw);					   // Градиент коэффициента момента по угловой скорости
	
	DECL_PARAM(double, MaWChute);
	DECL_PARAM(double, MwWChute);

	DECL_PARAM(double[5], cxCoeff);			   // Исходные коэффициенты для Cx
	DECL_PARAM(double, mainChute_cx);		   // Сх парашюта
		
	DECL_PARAM(double, mainChuteOpenSpeed);    // animation units per second. Full animation range - 0..1.
	DECL_PARAM(double, chuteDiameter);
	DECL_PARAM(double, extrChuteMax);          // Coefficient for animation
	DECL_PARAM(double, engine_force);		   // Импульс для движков BLU-108
	DECL_PARAM(double, engineWorkTime);		   // seconds
	DECL_PARAM(double, rotationMoment);		   // rotation moment provided by the engine
	DECL_PARAM(double, rotationVelMin);		   // minimum submunition rotation velocity before skeets will be released, revolutions per minute
	DECL_PARAM(double, drop_skeet_height);	   // Высота начала разброса скитов 
	DECL_PARAM(double, skeetsLaunchMaxG);	   // Maximum g to launch skeets
	DECL_PARAM(double, skeetSrchStartTime);
	DECL_PARAM(double, skeetMaxSearchRange);   // meters
	DECL_PARAM(double, skeetHalfApertSize);
	DECL_PARAM(double, skeetSpeenUpTime);	   // seconds
	
	// Release time delays for each group of BLU-108
	DECL_PARAM(double, timeDelayFirstGroup);
	DECL_PARAM(double, timeDelaySecondGroup);

	CxInterpolator Cx;
	double Sm;

	DECL_PARAM(double, wind_time);
	DECL_PARAM(double, wind_sigma);
	DECL_PARAM(double, impulse_sigma);
	DECL_PARAM(double, moment_sigma); 

	DECL_PARAM(ed::string, model_name);
	DECL_PARAM(ed::string, skeet_model);
	DECL_PARAM(ed::string, opened_dispenser_w_submunitions_model);
	DECL_PARAM(ed::string, empty_dispenser_model);
	DECL_PARAM(ed::string, dispenser_part_panel1_model);
	DECL_PARAM(ed::string, dispenser_part_panel2_model);
	DECL_PARAM(ed::string, dispenser_part_panel3_model);
	DECL_PARAM(ed::string, EFP_model); // explosively formed penetrator
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////Bomblets//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class WEAPON_BLOCKS_API wGuidedClusterPart : public viObjectNode
{
public:
			
	wGuidedClusterPart();
	~wGuidedClusterPart();

	void initSubmunition(wGuidedCluster* owner, 
						 const Math::Vec3d& pos, 
						 const Math::Vec3d& vel, 
						 const Math::Rot3d& rot, 
						 const Math::Vec3d& omega, 
						 bool visible);

	void initSkeet(wGuidedCluster* owner, 
					const Math::Vec3d& pos, 
					const Math::Vec3d& vel, 
					const Math::Rot3d& rot, 
					const Math::Vec3d& omega, 
					bool visible);

	void initOpenedDispenser(wGuidedCluster* owner, 
							 const Math::Vec3d& pos, 
							 const Math::Vec3d& vel, 
							 const Math::Rot3d& rot, 
							 const Math::Vec3d& omega, 
							 bool visible);

	void initPanel(int number, wGuidedCluster* owner, 
		const Math::Vec3d& pos, 
		const Math::Vec3d& vel, 
		const Math::Rot3d& rot, 
		const Math::Vec3d& omega, 
		bool visible);

	void simulate(const wGuidedClusterDescriptor* desc, double t, double dt, double rho, double Mach, const Math::Vec3d& wind);

	void SimulateSubmunition(const wGuidedClusterDescriptor* desc, double t, double dt, double rho, double Mach, const Math::Vec3d& wind);
	void SimulateBombPart(const wGuidedClusterDescriptor* desc, double t, double dt, double rho, double Mach, const Math::Vec3d& wind);

	/////Расчет сил и моментов и всего такого прочего для контейнеров BLU108 и подобных/////
	Math::Vec3d CalcSubmunitionForce(
		const Math::Vec3d& x,
		const Math::Vec3d& y,
		const Math::Vec3d& z,
		const Math::Vec3d& velw,
		double rho,
		double Mach,
		const wGuidedClusterDescriptor* desc,
		double t);

	Math::Vec3d CalcSubmunitionTorque(
		double velw_len,
		double rho,
		double alpha, double beta,
		const wGuidedClusterDescriptor* desc, 
		double t,
		double dt);

	void CalcAlphaBeta(double &alpha_, 
		double &beta_, 
		const Math::Vec3d& wind, 
		const Math::Vec3d& vel, 
		const Math::Rot3d& rotation_);

	void DrawSubmunitionChute(double mainChuteDragFactor_,
		double mainChuteDragFactorMax_, double extrChuteMax,
		double alpha,
		double beta);
	/////////////////////////////////////////////////////////////////	

	void SetEmptyDispenserModel(wGuidedCluster* owner);

	virtual const Graphics::DParamList& GetDrawArguments(){ return args; };

	const Math::Vec3d& getPosition() const { return pos_; }
	const Math::Vec3d& getPrevPosition() const { return prevPos_; }
	const Math::Vec3d& getVelocity() const { return vel_; }
	const Math::Vec3d& getTargetPos()const;
	const Math::Rot3d& getRotation()const{ return rot_; };
	const Math::Vec3d& getOmega()const{ return omega_; };

	// for viObjectNode
	cPosition& Position(double t);
	dVector GetPoint();

	bool			   GetLaunchFlag() {return toReleaseSkeets;}
	void			   SetLaunchedFlag() {skeetsReleased = true; 
										  toReleaseSkeets = false;}

	double lastUpdateTime_;
	double lastRequestTime_;

	bool	isFired;
	bool	isEngineWorking;
	bool	isCollided;
	bool	Suiside;

	wGuidedCluster* owner_;
	Math::Vec3d pos_, prevPos_;
	Math::Vec3d vel_;
	Math::Rot3d rot_;
	Math::Vec3d omega_;

	Graphics::DParamList    args;

	/////////////////////////////ДЛЯ КАСCЕТЫ С БОМБЛЕТАМИ///////////////
	double		currentChuteDiameter;
	double      skeetsReleaseSeqStartTime, skeetLaunchTime;
	bool		toReleaseSkeets;     // Команда на рождение бомблетов
	bool		skeetsReleased;	     // Признак того, что бомблеты рождены

	///////////////////////////////ПАРАМЕТРЫ ЦЕЛИ//////////////////////
	void getTargetPosition(cPosition& selfPos,
		double maxSearchRange, double halfAperture, double speenUpTime, double t);
	Math::Vec3d TargetPosition;
	bool		hasTarget;
	ed::vector< std::pair<woPointer, wGuidedClusterPart*> > TargetList;

	int PanelNumber;

	TYPE_OF_CLUSTER_BOMB_PART type_of_part;

	friend class wGuidedCluster;

protected:
	
	double addForceY_, addForceZ_, addForceX_;	// Additional force for dispersion
	cPosition position_;
	dPoint dpos_;
	Math::Vec3d init_fire_pos;
	bool firstStep;
#ifndef NO_REAL_LOCKON
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface Position_point;
#else 
	edterrain::PointOnSurfaceAdapter Position_point;
#endif
#endif

public:
 	virtual void Parse(Graphics::ModelParser& parser,
 		const Position3& pos,
 		Graphics::effectState* effects);
 	////////////////////////////////
 
 	dbg_limited_line*	selfVelDrw_;
 
 	////////////////////////////////
};

#endif
