#ifndef __TIMEMANIPULATION_H__
#define __TIMEMANIPULATION_H__

const unsigned int unitsTimeInSec	= 1;
const unsigned int unitsTimeInMin	= unitsTimeInSec * 60;
const unsigned int unitsTimeInHour	= unitsTimeInMin * 60;
const unsigned int unitsTimeInDay	= unitsTimeInHour * 24;

inline unsigned int getDay (unsigned int time)
{
	return time / unitsTimeInDay;
}
inline unsigned int getHour (unsigned int time)
{
	return (time % unitsTimeInDay) / unitsTimeInHour;
}
inline unsigned int getMin (unsigned int time)
{
	return (time % unitsTimeInHour) / unitsTimeInMin;
}
inline unsigned int getSec (unsigned int time)
{
	return (time % unitsTimeInMin) / unitsTimeInSec;
}
inline unsigned int setDay (unsigned int time, unsigned int day)
{
	return time % unitsTimeInDay + day * unitsTimeInDay;
}
inline unsigned int setHour (unsigned int time, unsigned int hour)
{
	unsigned int dayunitsTime = time / unitsTimeInDay * unitsTimeInDay;
	return time % unitsTimeInHour + hour * unitsTimeInHour + dayunitsTime;
}
inline unsigned int setMin (unsigned int time, unsigned int minute)
{
	unsigned int hourunitsTime = time / unitsTimeInHour * unitsTimeInHour;
	return time % unitsTimeInMin + minute * unitsTimeInMin + hourunitsTime;
}
inline unsigned int setSec (unsigned int time, unsigned int second)
{
	unsigned int minunitsTime = time / unitsTimeInMin * unitsTimeInMin;
	return time % unitsTimeInSec + second * unitsTimeInSec + minunitsTime;
}

#endif