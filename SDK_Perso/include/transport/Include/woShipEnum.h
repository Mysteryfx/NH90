// file woShipEnum.h

#ifndef __woShipEnum_h_
#define __woShipEnum_h_

typedef enum ShipTaskMotion 
{ 
	shipMotion = 1,	
	shipWaiting,		
	shipFlood			
};

typedef enum ShipConnector
{
	pointNose = 1,
	pointStern,
	pointFireBack,
	pointFireCenter,
	pointFireFront
};

#endif