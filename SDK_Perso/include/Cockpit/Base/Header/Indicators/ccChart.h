#ifndef _CC_CHART_H
#define _CC_CHART_H
#include "Indicators/ccIndicator.h"
#include "Box.h"
#include "osg/Vec2f"
#include "IcChart.h"
#include <ed/string.h>
#include "Utilities/ccTriggerSystem.h"

namespace cockpit 
{
	class ceSimpleLineObject;

	class ChartChannel
	{
	public:
		ChartChannel()
		{
			bias_  = 0;
            scale_ = 1.0;
		};
		~ChartChannel(){};

		bool empty() const { return data.size() < 2; }

		void insertPoint(const osg::Vec2f & pnt);

		unsigned formLine(const Box & view_area,
						  float x_size,
						  float y_size);
		const ed::vector<Vector3> & getCachedVertices() const { return cached_vertices; }

		const osg::Vec2f & getMin() const { return Min_;};
		const osg::Vec2f & getMax() const { return Max_;};

		void			   setBias(float ordinata_bias)   { bias_  = ordinata_bias; }
        void               setScale(float ordinata_scale) { scale_ = ordinata_scale; }
		void			   trimToEffectiveRange(float range);

        void  setLegend(const char * legend)
        {
            if (legend)
                legend_ = legend;
            else if (!legend_.empty())
                legend_.clear();
        }
        const ed::string & getLegend() const { return legend_; }
	public:
		ed::vector<osg::Vec2f>		data;
		ed::vector<Vector3>			cached_vertices;
		osg::Vec2f					Min_;
		osg::Vec2f					Max_;
		float						bias_;
        float						scale_;
        ed::string					legend_;

		triggers::double_param_proxy	mapped_param_;

		//new params
		ed::vector<float> params_; /*0 - scale, 1 - param1*/
		ChartParamFunc param_func_;
	};

	class ccChart : public IcChart,
                    public ccIndicator
	{
	public:
		ccChart();
		virtual ~ccChart();
		void	 control() { check_auto_mapped_channels(); }
		void	 check_auto_mapped_channels();
		void	 initialize(avDevice*, unsigned char, const ed::string&);
        virtual void     setOrdinataLimits(float min_value,float max_value)
        {
            view_area_.min.y = min_value;
            view_area_.max.y = max_value;
        }
        virtual void     setAbscissaLimits(float min_value,float max_value);

        virtual void setModelTimer(float span);
        virtual void setRealTimer(float span);

        virtual void     setAbscissaViewSpan(float span)
        {
            view_area_.max.x = view_area_.min.x + span;
        }


        virtual void     setChartScaleAndBias(unsigned channel,float ordinata_bias,float ordinata_scale);

        virtual void	 insertPoint(unsigned channel,float abscissa,float ordinata,const char * legend);

		virtual void	 insertParamForUpdate(unsigned channel,const char * cockpit_param_name);

		virtual void registerLambdaForUpdate(const ed::string& lambda_name, const ChartParamFunc param_f);
		virtual void unregisterLambdaForUpdate(const ed::string& lambda_name);
		virtual void insertLParamForUpdate(unsigned channel, const ed::string& lambda_name, const ed::string& param_name, ed::vector<float> params);

        virtual void     setAbscissaMarkGridstep(float step);
        virtual void     setOrdinataMarkGridstep(float step);

        virtual void     setEnable(bool onOff)
        {
            isEnabled_ = onOff;
        }
        virtual bool isEnabled() const
        {
            return isEnabled_;
        }
        double  getActualAbscissa() const;
		// controllers
		virtual void create_elements_controllers_map();
	private:
        static void show(Element* element , ccDrawable* parent, const DParamList& args);
		static void chart_channel(Element* element , ccDrawable* parent, const DParamList& args);
		static void zero_line_position(Element* element , ccDrawable* parent, const DParamList& args);
		static void ordinata_mark_line(Element* element , ccDrawable* parent, const DParamList& args);
		static void abscissa_mark_line(Element* element , ccDrawable* parent, const DParamList& args);
        static void abscissa_mark_text(Element* element , ccDrawable* parent, const DParamList& args);
	    static void real_time_scale_update(Element* element , ccDrawable* parent, const DParamList& args);
        static void channel_legend (Element* element , ccDrawable* parent, const DParamList& args);

		unsigned formLine(unsigned channel,ceSimpleLineObject * elm);

		float  calc_draw_ordinata(float val);
		float  calc_draw_abscissa(float val);

		float  calc_ordinata_from_draw(float val);
		float  calc_abscissa_from_draw(float val);

        void   timeScaleAdjust(double current_time);

        void   trimToEffectiveRange(float range);

        double getAbscissaSpan() const { return view_area_.max.x - view_area_.min.x; }


        int getFirstFreeChannel();
				
		Box view_area_;

        bool isModelTimer_;
        bool isRealTimer_;
        bool isEnabled_;
        osg::Vec2f gridStep_;


		static const unsigned channels_count =  16;

		ChartChannel channels_[channels_count];

		struct ChartFunction
		{
			ChartParamFunc func_;
			ed::vector<int> channels_;
		};
		ed::unordered_map<ed::string,ChartFunction> Lambdas_; 
	};
}
#endif
