#pragma once

#include "CockpitBase.h"
#include <ed/string.h>

namespace Lua
{
    class Config;
}
struct lua_State;

namespace cockpit
{
	class COCKPITBASE_API avDate
	{
	public:
		avDate(void);
		avDate(int _day,int _month,int _year);
		avDate(int n_day_of_year,int start_year = 2011);
		~avDate(void);
		int               Get_n_day();// Вычислить номер дня от начала года (1 января == 1)
		void operator()(int n_day_of_year,int start_year = 2011);
        friend bool operator >( const avDate & left_,const avDate & right_ )
        {
            if (left_.Year == right_.Year)
            {
                if (left_.Month == right_.Month)
                {
                    return left_.Day > right_.Day;
                }
                return left_.Month > right_.Month;
            }
            return left_.Year > right_.Year;
        }
        friend bool operator <( const avDate & left_,const avDate & right_ )
        {
            if (left_.Year == right_.Year)
            {
                if (left_.Month == right_.Month)
                {
                    return left_.Day < right_.Day;
                }
                return left_.Month < right_.Month;
            }
            return left_.Year < right_.Year;
        }

		bool      operator ==(const avDate & right_) const;
		avDate &  operator = (const char* date_str);
		avDate &  operator = (const ed::string & date_str);
		avDate    operator + (double days_qty);
        int     getDay()   const { return Day;}
        int     getMonth() const { return Month;}
        int     getYear()  const { return Year;}
		void	set(int dd, int mm, int yy);
		double 	getDateInDays() const;
		double 	getDateTimeInDays() const;
		void	setDateFromDays(double date);
		void	nextDay();

        void    load(Lua::Config & config);
        void    save(lua_State * L) const;

	//static functions
        static void         Load_From_Mission(avDate &date);
        static void         Load_From_Mission(avDate &date,double time);
	protected:
		static int		     Get_Last_Day_of_Month(int month,bool visokos);
	//data 
	private:
		int Day;  //от 1 до 31
		int Month;//от 1 до 12
		int Year;
	//static data
	};
}
