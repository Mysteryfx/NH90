#ifndef _ED_Date_Time__h_
#define _ED_Date_Time__h_

namespace Math {

inline double getDateInDays(int YY, int MM, int DD)
{
	unsigned int cur_date = (YY * 365 + YY/4);
	for(int m = 1; m < MM; m++)
	{
		switch(m)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				cur_date += 31;
			break;
			case 2:
				if(YY / 4 * 4 == YY)
				{
					cur_date += 29;
				}
				else
				{
					cur_date += 28;
				}
			default:
				cur_date += 30;
		}
	}
	cur_date += DD - 1;	
	return cur_date;	
}

inline int getDayNumInYear(int YY, int MM, int DD)
{
	unsigned int cur_date = 0;
	for(int m = 1; m < MM; m++)
	{
		switch(m)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				cur_date += 31;
			break;
			case 2:
				if(YY / 4 * 4 == YY)
				{
					cur_date += 29;
				}
				else
				{
					cur_date += 28;
				}
			default:
				cur_date += 30;
		}
	}
	cur_date += DD;	
	return cur_date;	
}

inline double getDateTimeInDays(int YY, int MM, int DD, int H, int M, int S)
{
	return Math::getDateInDays(YY,MM,DD) + (H * 3600 + M * 60 + S) / 86400.0;
}

}

#endif /* _ED_Date_Time__h_ */
