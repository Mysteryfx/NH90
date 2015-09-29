#ifndef _CC_CHART_MANAGER_H
#define _CC_CHART_MANAGER_H
#include "IcChart.h"

class ccChartManager : public IcChartManager
{
    ed::vector<IcChart*> charts;
public:
    static ccChartManager & instance() 
    {
        static ccChartManager mng;
        return mng;
    }
    IcChart *getChart(unsigned i)
    {
        if (i < charts.size())
            return charts[i];
        return 0;
    }
    void addChart(IcChart *chart)
    {
        charts.push_back(chart);
    }
    void removeChart(IcChart *chart)
    {
        ed::vector<IcChart*>::iterator it = charts.begin();
        while(it != charts.end())
        {
            if (*it == chart)
            {
                 it = charts.erase(it);
                 return;
            }
            ++it;
        }
    }       
};

#endif