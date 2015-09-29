#ifndef __wCommNetFlags_h__
#define __wCommNetFlags_h__

//Each wMessage is corresponded to one of these flags
enum wCommNetFlag
{
	COMM_NET_FLAG_NULL = 0,
	COMM_NET_FLAG_PLAYER_FLIGHT = 1 << 0,	
	COMM_NET_FLAG_AWACS = 1 << 1,
	COMM_NET_FLAG_TANKER = 1 << 2,
	COMM_NET_FLAG_JTAC = 1 << 3,
	COMM_NET_FLAG_ATC = 1 << 4,
	COMM_NET_FLAG_ALLIED_FLIGHTS = 1 << 5,
	COMM_NET_FLAG_GROUND_CREW_TELEPHONE = 1 << 6,
	COMM_NET_FLAG_GROUND_CREW_DIRECT = 1 << 7,
	COMM_NET_FLAG_BEACONS = 1 << 8,
	COMM_NET_FLAG_BROADCAST = 1 << 9,
	COMM_NET_FLAG_DATALINK = 1 << 10,
	COMM_NET_VOIP = 1 << 11,

	NON_RADIO_COMM_NET_FLAGS =	COMM_NET_FLAG_GROUND_CREW_TELEPHONE | COMM_NET_FLAG_GROUND_CREW_DIRECT
};

typedef unsigned int wCommNetFlags;

#endif __wCommNetFlags_h__