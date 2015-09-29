#pragma once

#include "Avionics/avDevice.h"

#include "wFloat.h"
#include "Utilities/ccUtilities.h"

#include "FMElectricSystems/Items/MultiThrowSwitch.h"
#include "FMElectricSystems/Items/Wire.h"

namespace Lua
{
    class Config;
}

namespace cockpit
{

class GaugeBiasChunk
{
public:
	GaugeBiasChunk() : min_val(0.0), max_val(0.0), bias(0.0) {};
	
	double min_val, max_val, bias;
};

typedef ed::vector<GaugeBiasChunk> BiasVec;

class COCKPITBASE_API GaugeBias
{
public:
	bool l_read(Lua::Config& config, const char* field);
	bool l_read(Lua::Config& config);

	BiasVec bias;
};

class GaugeAnglesChunk
{
public:
	GaugeAnglesChunk() : val(0.0), angle(0.0) {}
	
	double val, angle;
};

typedef ed::vector<GaugeAnglesChunk> AnglesVec;

class MomentNormaChunk
{
public:
	MomentNormaChunk() : min_angle(0.0), max_angle(0.0), norma(0.0) {};
	
	double min_angle, max_angle, norma;
};

typedef ed::vector<MomentNormaChunk> NormasVec;

class COCKPITBASE_API GaugeOrient
{
public:
	bool l_read(Lua::Config& config);

	AnglesVec angles;
	NormasVec normas;
};

class COCKPITBASE_API gauge
{
public:
	gauge() : value_min(0.0), value_max(Pi_2), error(0.0), failure(false) {value_out.Set(0.0, wTime::GetModelTime());}
	double simulate(double value_in, wModelTime dt) {value_out.Set(0.0, wTime::GetModelTime()); return 0.0;}
	void stop(wModelTime dt);

	double get_value_out() const {return value_out.GetValue(wTime::GetModelTime());}
	double get_value_min() const {return value_min;}
	double get_value_max() const {return value_max;}
	bool get_failure() const {return failure;}
	double get_error() const {return error;}

	void set_value_out(double param_in) {value_out.Set(param_in, wTime::GetModelTime());}
	void set_value_min(double param_in) {value_min = param_in;}
	void set_value_max(double param_in) {value_max = param_in;}
	void set_failure(bool param_in) {failure = param_in;}
	
	virtual bool l_read(Lua::Config& config, const char* field);

protected:
	static const double epsilon;

	wDouble value_out;
	double value_min, value_max;
	bool failure;
	double error;
};

class COCKPITBASE_API gauge_linear : public gauge
{
public:
	gauge_linear() : d_value(0.0) {}
	virtual bool l_read(Lua::Config& config, const char* field);
	double simulate(double value_in, wModelTime dt);
	
	double get_d_value() const {return d_value;}
	void set_d_value(double param_in){d_value = param_in;}

protected:
	double d_value;
};

// В функцию simulateF() можно передавать внешнюю силу
// Две особенности функции:
// 1) Коэффициент, которым можно масштабировать силу, который задается в скрипте.
// 2) Если прибор свое значение увеличивает против часовой стрелки, то этот коэффициент надо указать с минусом.
//
// Ориентация прибора должна задаваться так, чтобы угол увеличивался по часовой стрелке. Ноль -
// это вертикально вверх. минус 90 - вправо, плюс 90 - влево. Например, у типового прибора, вроде напряжения,
// где стрелка ходит по дуге слева направо типичными углами будут: от минус 60 до плюс 60.
// Углы задаются парами - угол ориентации стрелки и соответстующее ему положение стрелки на шкале (в единицах прибора).
// Если шкала линейная, то начальный и конечный углы будут совпадать. Например, для вертикальной шкалы
// углы будут составлять: от -90 до -90. Пример результата действия силы - если переменная сила (тряска)
// действует строго вниз относительно панели приборов, то стрелка ориентированная вертикально будет неподвижна,
// а стрелка ориентированая горизонтально будет иметь макс. амплитуду тряски. Для круговой замкнутой шкалы,
// если у нее, например, нулевое положение строго влево относительно приборной доски, надо указывать углы не "от -90 до -90", а "от -90 до 270".
class COCKPITBASE_API gauge_periodic : public gauge
{
public:
	gauge_periodic() : isLagElement(false), w(0.0), w_max(0.0), T1(0.0), T2(0.0), momentCoeff(1.0) {}
	virtual bool l_read(Lua::Config& config, const char* field);
	double simulate(double value_in, wModelTime dt);
	double simulateF(double value_in, double forceY, double forceZ, wModelTime dt);
	
	double get_w() const {return w;}
	GaugeBias& get_bias() {return bias;}
	double get_w_max() const {return w_max;}
	double get_T1() const {return T1;}
	double get_T2() const {return T2;}
	double getMomentCoeff() const {return momentCoeff;}
	
	void set_w(double param_in) {w = param_in;}
	void set_w_max(double param_in) {w_max = param_in;}
	void set_T1(double param_in) {T1 = param_in;}
	void set_T2(double param_in) {T2 = param_in;}
	void setMomentCoeff(double param_in) {momentCoeff = param_in;}

protected:
	double findBias(double val);
	double findAngle(double val);
	double findNorma(double angle);
	double calcMoment(double angle, double forceY, double forceZ);
	double simulate(double currVal, double prevVal, wModelTime dt);
	double simulateM(double value_in, double addMoment, wModelTime dt);

	bool isLagElement;
	double w;
	GaugeBias bias;
	double w_max;
	double T1, T2;
	GaugeOrient angles;
	double momentCoeff;
};

class COCKPITBASE_API gauge_limited : public gauge_periodic
{
public:
	gauge_limited() {}
	double simulate(double value_in, wModelTime dt);
	double simulateF(double value_in, double forceY, double forceZ, wModelTime dt);

protected:
	double simulateM(double value_in, double addMoment, wModelTime dt);
};

class COCKPITBASE_API gauge_input_adapter
{
public:
	typedef ed::vector<double> GaugeAdapterValues;

	void l_read(Lua::Config& config, const char* field);

	void set_defalut(const GaugeAdapterValues& input, const GaugeAdapterValues& output);

	double get_animation(double value) const; /*returns in animation units*/
	double get_native(double value) const; /*returns in gauge native units*/

private:
	GaugeAdapterValues input_;
	GaugeAdapterValues output_;
};


// A non-ideal meter susceptible for voltage changes in the bus
inline double electricQuotientMeter(double trueVal, double minVal, double voltageRatio, double K)
{
	//return trueVal + measurRange * (voltageRatio - 1.0) * K;
	return minVal + (trueVal - minVal) * (1.0 + K * (voltageRatio - 1.0));
}

class COCKPITBASE_API gear_handle // анимация крана шасси
{
public:
     gear_handle();
     virtual	~gear_handle(){};
     double      get_animation_value() const { return animation.get_value_out(); }
     bool        get_commanded_pos()   const { return commanded_gear_down; } 

     void        force(bool gear_down);
     void        command(bool gear_down);
     void        connect_electric(EagleFM::Elec::ItemBase & bus,
                                  EagleFM::Elec::Wire    & on_up,
                                  EagleFM::Elec::Wire    & on_down);
     void        update(double dt);
protected:
	wModelTime                  unlock_dpos, 
                                dpos,
                                lock_dpos;
	char                        moving; // -1 = уборка, 0 = не двигается, 1 = выпуск
	bool                        commanded_gear_down;
	gauge_linear                animation;
    EagleFM::Elec::MultiThrowSwitch	selector; //управление уборкой/выпуском шасси

};


template <unsigned N>
class counter_drum
{ 
    class drum_digit
    { 
    public:
        drum_digit()
        { 
            smooth   = false;
            value    = 0;
            basement = 1;
        }
        void set(double in,double eps)
        {
            in = fmod(in,10 * basement);
            if (smooth)
            {
                value = in / basement;
                return;
            }
            double  p_2 = int(in/basement)* basement;
            double  p_1 = in - p_2;
            if (p_1 > basement - eps)
	            value = p_2 / basement + (p_1 - (basement - eps)) / eps;
            else
	            value = p_2 / basement;
        }
        bool     smooth;
        double   value;
        unsigned basement;
    };
public:
    counter_drum() 
    { 
        assert(N);
        for (int i = 1; i < N; ++i)
        { 
            d[i].basement = d[i-1].basement * 10;
        }
        last = 0;
        threshold  = 1;
    };
   ~counter_drum(){};

    void set(double value)
    { 
        if (last == value)
            return;
        for(int decade = 0; decade < N; ++decade)
            d[decade].set(value,threshold);
        last = value;
    } 
    void set_smooth(unsigned decade,bool value)
    { 
        assert(decade < N);
        d[decade].smooth = value;
    } 
    double get_decade(unsigned decade) const 
    {
        assert(decade < N);
        return d[decade].value;
    }
    void set_threshold(double val) { threshold = val;} 
private:
    drum_digit  d[N];
    double      last;
    double      threshold;
};

}
