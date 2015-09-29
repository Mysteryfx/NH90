#ifndef _IC_CHART_H
#define _IC_CHART_H

#ifndef VAL_AND_LGND
#define VAL_AND_LGND(val)  (val),#val
#endif

typedef std::function<float(ed::vector<float>)> ChartParamFunc;

class IcChart 
{
public:
    virtual ~IcChart(){}
    virtual void setOrdinataLimits   (float min_value,float max_value) = 0;
    virtual void setAbscissaLimits   (float min_value,float max_value) = 0;
    virtual void setModelTimer       (float span) = 0;
    virtual void setRealTimer        (float span) = 0;
    virtual void setAbscissaViewSpan (float span) = 0;
    virtual void setAbscissaMarkGridstep(float step) = 0;
    virtual void setOrdinataMarkGridstep(float step) = 0;
    virtual void setEnable(bool onOff) = 0;
    virtual bool isEnabled() const = 0;
    virtual double getActualAbscissa() const = 0;


    virtual void setChartScaleAndBias(unsigned channel,float ordinata_bias,float ordinata_scale) = 0;
    virtual void insertPoint         (unsigned channel,float abscissa     ,float ordinata,const char * legend = 0) = 0;

	virtual void insertParamForUpdate(unsigned channel,const char * cockpit_param_name) = 0;

	virtual void registerLambdaForUpdate(const ed::string& lambda_name, const ChartParamFunc param_f) = 0;
	virtual void unregisterLambdaForUpdate(const ed::string& lambda_name) = 0;
	virtual void insertLParamForUpdate(unsigned channel, const ed::string& lambda_name, const ed::string& param_name, ed::vector<float> params) = 0;
};

class IcChartManager
{
public:
    virtual ~IcChartManager(){}
    virtual  IcChart *getChart(unsigned i) = 0;
};

#endif
