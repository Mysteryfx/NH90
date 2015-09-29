#ifndef _avSNS_h
#define _avSNS_h

#include "Avionics/avDevice.h"
#include "Utilities/avDate.h"
#include "wFloat.h"

#include "Math/Interpolator.h"
#include "Utilities/GaussProcess.h"
#include "Utilities/KalmanFilter.h"
#include "Utilities/ccPolarPosition.h"
#include "Physic/wRadioAntenna.h"
#include "Satellite.h"

#ifdef _ED_HIDDEN_DEBUG 
//#define SNS_DEBUG_PARSING
#endif

#ifdef SNS_DEBUG_PARSING
#include "Renderer/DbgGraph.h"
#endif

class waSatellite;
struct AlmanacGPS;

namespace cockpit
{

enum enum_SNS_Mode
{
	SNS_MODE_OFF,
	SNS_MODE_WARMING,
	SNS_MODE_ON,
};

enum enum_GPS_Using_Frequencies
{
	FREQ_L1,
	FREQ_BOTH
};

enum enum_GPS_Range_Measure_Method
{
	BY_CODE,
	BY_CODE_AND_CARRIER,
};

enum enum_Sat_System
{
	SAT_SYS_NONE = -1,
	SAT_SYS_GLONASS,
	SAT_SYS_GPS,
	SAT_SYS_BOTH
};

enum enum_GPS_Mode
{
	SAT_MODE_NOTAVAILABLE, // недостаточное количество спутников в поле видимости
	SAT_MODE_2D,
	SAT_MODE_3D
};

enum enum_GPS_receiver_start_method
{
	HOT_START,
	WARM_START,
	COLD_START,
	START_MAX
};

enum enum_satellites_qty_type
{
	SATELLITES_IN_BASE,
	TRACKING_SATELLITES,
	USED_SATELLITES,
	READY_SATELLITES,
	SATELLITES_QTY_TYPES_MAX
};

struct avSatelliteInfo
{
public:
	avSatelliteInfo();
	void start_tracking(int channel_num, wModelTime time);
	void stop_tracking(wModelTime time);
	waSatellite * getSatellite() const;
public:
	unsigned int	ID;
	unsigned int	type;
	unsigned int	sat_num;
	AlmanacGPS		almanac;
	bool			health;
	//
	int				channel;
	wModelTime		lost_time;
	wModelTime		acquisition_time;
};

typedef ed::map<unsigned int, avSatelliteInfo> avSatelliteInfoMap;

struct avSNSchannelStatus
{
public:
	avSNSchannelStatus();
	bool			free() const {return satellite_info == NULL;}
	dVector			getSatWGS84Pos() const;
	dVector			getSatWGS84Vel() const;
public:
	float			pseudo_range_accuracy;
	float			SNR;
	float			SNRs;
	avSatelliteInfo *satellite_info;
	bool			used;
	PolarPosition	local_polar_coord;
};

typedef ed::vector<avSNSchannelStatus> avSNSchannelsStatus;

struct avSNSchannel : public avSNSchannelStatus
{
public:
	avSNSchannel();
	void			set(const avSNSchannelStatus & channel_status, int channel_num, wModelTime time);
	void			update(const avSNSchannelStatus & channel_status, wModelTime time);
	void			clear(wModelTime time);
	double			getPseudoRange(dVector & selfPos, dVector * satWGS84Pos_ = NULL) const;
	float			getSNR() const;
public:
	GaussProcess	psedoRangeErrorProcess;
	GaussProcess	SNRProcess;
	wModelTime		last_update_time;
	bool			ready;
};

typedef ed::vector<avSNSchannel> avSNSchannels;

struct avSNSSettings
{
public:
	typedef ed::vector<unsigned int> ExcludedSatellitesID;
public:
	avSNSSettings();
	void clear();
	bool check_exclude(unsigned int satID) const;
public:
	unsigned char			ellipsoid;
	float					min_elev_angle;
	float					PDOP_limit;
	ExcludedSatellitesID	excludedSatellitesID;
};

struct COCKPITBASE_API avSatelliteSystemStatus : public avSNSSettings
{
public:
	avSatelliteSystemStatus(){ clear();}
	void clear();
	void clear_status();
	void clearDOPs();
	float getCEP() const;
	float getVerMediumAccuracy() const;
public:
	//спутники
	unsigned char	satellite_system_mode;
	unsigned int	satellite_count;
	unsigned int	use_satellite_count;
	unsigned int	ready_satellite_count;
	bool			hybrid_sat_group;
	//UERE
	double			pseudo_range_accuracy;
	//геометрический фактор
	float			GDOP;
	float			PDOP;
	float			HDOP;
	float			VDOP;
	float			TDOP;
};

class  avSNS_update_status_timer: public avDevice_BasicTimer
{
public:
    avSNS_update_status_timer();
	avSNS_update_status_timer(avDevice* _parent, wModelTime _dtime) : avDevice_BasicTimer(_parent, _dtime) {;}
	void NextEvent();
};

class avSNS_common_timer : public avDevice_BasicTimer
{
public:
    avSNS_common_timer();
	avSNS_common_timer(avDevice* _parent, wModelTime _update_dtime, wModelTime _extrapolation_dtime);
	void NextEvent();
	wModelTime getUpdateDt() const {return dtime * extrapolations_per_update;}
	//bool is_extrapolation() const {return extrapolations_counter < extrapolations_per_update;}
private:
	int extrapolations_per_update;
	int extrapolations_counter;
};

class COCKPITBASE_API avSNS :   public avDevice
{
public:
	avSNS(void);
	virtual ~avSNS(void);
	//avDevice interface begin
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script);
	virtual void serialize(Serializer& serializer);
	virtual void SetCommand(int command, float value = 0);
	virtual void update();

	//avDevice interface end
	virtual void	Start(bool fast = false) {mode = SNS_MODE_ON;}
	virtual void	Stop() {mode = SNS_MODE_OFF;}
	virtual bool	IsOn() const {return mode > SNS_MODE_OFF;}
	virtual bool	IsReady() const {return mode == SNS_MODE_ON;}
	//
	void			set_receiver_ready_flag(bool receiver_ready_);
	bool			get_receiver_ready_flag() const {return receiver_ready;}
	//
	wModelTime		get_time()   const  { return time; }
	double			get_latitude()    const;
	double			get_longitude()   const;
	double			get_height()      const;
	double			get_ground_speed() const  { return sqrt(speed.x * speed.x + speed.z * speed.z);}
	double			get_FPU()   const  {if(get_ground_speed() < 1.39) return 0; double FPU = atan2(speed.x, speed.z); if(FPU < 0) FPU += 6.28; return FPU;}
	Vector3			get_self_position() const {return Vector3(position.x, position.y, position.z);}
	virtual void	extrapolate_coords();

	virtual void	correct(const cVector & position_);

	virtual void	update_status() = 0; //обновление состояния системы
	virtual const	avSatelliteSystemStatus & get_status() const = 0;
	virtual const	avSNSchannelsStatus & get_tracking_satellites() const = 0;
protected:
	void			setTrueCoordAndVel();
    static bool     check_service_provider(int provider_id);

	//параметры
	wModelTime		position_latency;
	//
	unsigned char	mode;
	bool			receiver_ready;
	avDate			date;
	//
	cVector			position; //текущая с учетом экстраполяций между измерениями
	cVector			speed; //текущая с учетом экстраполяций между измерениями
	wModelTime		time;
};

struct gps_channel_sorter: public  std::less<avSNSchannelStatus>
{
	bool operator()(avSNSchannelStatus _Left , avSNSchannelStatus _Right) const;
};

//Фильтр Калмана в GPS-приёмнике, как он на самом деле есть
class GPSKalmanFilter
{
public:
	struct GPSSatData
	{
		GPSSatData();
		waSatellite* sat;
		dVector satPos;
		//dVector satVel;
		double	pseudorange;
		double	pseudorangeAccuracy; //СКВО
	};
	typedef ed::vector<GPSSatData> GPSSatsData;

public:
	GPSKalmanFilter();
	void start(bool mode3D_);
	void reset() { started = false; }
	void correctPos(const dVector & selfPos);
	void switchMode(bool mode3D_);
	void finish() { kF.X.clear(); kF.P.clear(); started = false;}
	void process(const GPSSatsData & gpsSatsData, wModelTime dt, wModelTime dtExt);
	void idle(wModelTime dtExt);
	bool getMode() const { return mode3D; }
	const dVector & getPos() const { return estimatedPos; }
	const dVector & getVel() const { return estimatedVel; }

private:
	KalmanFilter	        kF;
	dVector					estimatedPos;
	dVector					estimatedVel;
	bool					mode3D;
	bool					started;
};

class COCKPITBASE_API avSNS_GPS_Listener : public avSNS
{ //всё, что относится к любому GPS-приёмнику
public:
	avSNS_GPS_Listener();
	~avSNS_GPS_Listener();
	virtual void	initialize(unsigned char ID, const ed::string& name, const ed::string& script);
	virtual void	post_initialize();
	virtual void	release();
	virtual void	update();

	virtual	void	extrapolate_coords();
	virtual void	correct(const cVector & position_);

	virtual void	Start(bool fast = false);
	virtual void	Stop();
	virtual void	StartWorking(bool fast);

	//настройки
	void			setEllipsoid(unsigned char ellipsoid_);
	void			set_min_elev_angle(float angle_);
	void			set_max_PDOP(float max_pdop_);
	void			exclude_satellite(unsigned int satID);
	bool			check_exclude(unsigned int sat_num, unsigned char sat_sys) const;
	//инф. база по спутникам
	avSatelliteInfo * get_satellite_record(unsigned int num, unsigned int system = SAT_SYS_GPS) const;
	virtual int		get_satellite_common_nunmber(unsigned int num, unsigned int system = SAT_SYS_GPS) const;
	//virtual bool	get_satellite_coordinate(cPoint & pos, dVector & wgs84_pos, unsigned int sat_num, unsigned char sat_system, wModelTime time);
	virtual void	make_prediction(cPoint point, wModelTime time, avSatelliteSystemStatus & predicted_sns_status, avSNSchannelsStatus & predicted_channel_status);
	//общее состояние
	virtual const	avSatelliteSystemStatus & get_status() const { return sns_status;}
	virtual const	avSNSchannelsStatus & get_tracking_satellites() const {return tracking_satellites;}
	virtual int		get_satellites_qty(unsigned char sat_system, enum_satellites_qty_type satellites_qty_type = TRACKING_SATELLITES) const;
	virtual void	update_status();
	virtual void	update_channels();
	bool			is_channel_ready(int channel_num) const {return (channel_num >= 0 && channel_num < channels.size()) ? channels[channel_num].ready : true;}
	//сообщения NMEA-протокола
	void			activate_mesage_update(bool activate_) {message_update_active = activate_;}
	virtual void	update_messages_list();
	//физика
	//virtual	float	get_SNR(float SNR) const;
	virtual	float	get_UERE(waSatellite *sat, float SNR) const;
	//вычисление координат
	virtual void	calc_DOPs(const cPoint & point, avSatelliteSystemStatus & in_sns_status, const avSNSchannelsStatus & in_tracking_satellites, bool prediction);

#ifdef SATELLITE_MOVING_DBG
	wModelTime sat_model_time;
#endif

#ifdef SNS_DEBUG_PARSING
	void init_sats_directions();
	void clear_sats_directions();
	void external_parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);
#endif
protected:
	virtual void	check_satellites(const cPoint & point,  const avSNSSettings & sns_settings_, avSNSchannelsStatus & tracking_satellites_, bool prediction = false); //определения состава видимой спутниковой группировки
	virtual void	add_satellite_for_sort(avSNSchannelsStatus & tracking_satellites_, const avSNSchannelStatus & channel_status, bool prediction = false); //добавление спутника в список на сортировку
	virtual void	add_satellite(avSatelliteSystemStatus & sns_status_, avSNSchannels & channels_, const avSNSchannelStatus & channel_status); //добавление спутника в каналы
	virtual void	analyze_status(avSatelliteSystemStatus & sns_status_, avSNSchannelsStatus & tracking_satellites_, bool prediction = false);
	virtual void	clear_status(bool full = false);
	//
	wModelTime						stop_time;
	wModelTime						start_time;
	//параметры приемоизмерителя
	enum_GPS_Using_Frequencies		using_frequencies;
	enum_GPS_Range_Measure_Method	range_measure_method;
	unsigned int					channels_qty;
	ed::map<float, float>			re_acquisition_time;
	//
	avSatelliteInfoMap				satellite_database;
	avSatelliteSystemStatus			sns_status;
	wModelTime						status_update_dt;
	avSNS_update_status_timer*		update_status_timer;
	avSNSchannels					channels;
	avSNSchannelsStatus				tracking_satellites;
	GPSKalmanFilter*				pGPSKalmanFilter;
	bool							terrainLOSEnable;
	//физика
	cVector							antenna_pos;
	wRadioAntenna					antenna;
	bool							GPS_available;
	//
	bool							message_update_active;
private:
	wModelTime						starting_time[START_MAX];
#ifdef SNS_DEBUG_PARSING
	ed::map<int, dbg_line*>		directions_to_sats;
#endif
};

struct gps_glonass_channel_sorter: public  std::less<avSNSchannelStatus>
{
	gps_glonass_channel_sorter()
	{
		primary_system = SAT_SYS_NONE;
	}
	gps_glonass_channel_sorter(unsigned char primary_system_)
	{
		primary_system = primary_system_;
	}
	bool operator()(avSNSchannelStatus _Left , avSNSchannelStatus _Right) const;
	unsigned char primary_system;
};

class COCKPITBASE_API avSNS_GPS_GNSS_Listener : public avSNS_GPS_Listener
{ //выбор систем, особенности реализации модели GG12
public:
	avSNS_GPS_GNSS_Listener() : avSNS_GPS_Listener() {};
	~avSNS_GPS_GNSS_Listener() {};
	void initialize(unsigned char ID, const ed::string& name, const ed::string& script);
	virtual void	Stop();
	//настройки
	void			set_system(unsigned char	  system_);
	unsigned char	get_selected_system() const {return selected_system;}
	void			set_primary_system(unsigned char	  primary_system_);
	unsigned char	get_primary_system() const {return primary_system;}
	//база данных по спутникам
	virtual int		get_satellites_qty(unsigned char sat_system, enum_satellites_qty_type satellites_qty_type = TRACKING_SATELLITES) const;
	virtual int		get_satellite_common_nunmber(unsigned int num, unsigned int system = SAT_SYS_GPS) const;
protected:
	virtual void	add_satellite_for_sort(avSNSchannelsStatus & tracking_satellites_, const avSNSchannelStatus & channel_status, bool prediction = false); //добавление спутника в список на сортировку
	virtual void	analyze_status(avSatelliteSystemStatus & sns_status_, avSNSchannelsStatus & tracking_satellites_, bool prediction = false);
	virtual void	clear_status(bool full = false);
	//вычисление координат
#ifdef SNS_DEBUG_PARSING
	virtual void	calc_DOPs(const cPoint & point, avSatelliteSystemStatus & in_sns_status, const avSNSchannelsStatus & in_tracking_satellites, bool prediction);
#endif
	//
	bool			GLONASS_available;
	//настройки
	unsigned char	selected_system;
	unsigned char	primary_system;
	//сколько чего и каких?
	int				GPS_satellite_max_number;
	int				satellites_qty[2][SATELLITES_QTY_TYPES_MAX];
};
}

#endif // _avSNS_h
