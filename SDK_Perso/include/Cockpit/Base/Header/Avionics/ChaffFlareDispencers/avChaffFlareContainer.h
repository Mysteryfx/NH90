#pragma once

#include "Avionics\avBreakable.h"


namespace cockpit
{


enum DISPENCER_EXP_TYPES
{
	UNKNOWN = 0,
	CHAFF,
	FLARE,
	BOTH
};

class COCKPITBASE_API avChaffFlareContainer : public avBreakable
{
public:
		avChaffFlareContainer();
		virtual ~avChaffFlareContainer();

		void initialize(int num_of_container);
		void clear();

		void DropChaff(int count, int number_of_contatiner);
		void DropFlare(int count, int number_of_contatiner);

		void dropSnar(unsigned char Type, int count_, int number_of_contatiner);
		int  getSnar(unsigned char Type); // returns Snar count

		void LoadChaff(int chaff_){chaff = chaff_;}
		void LoadFlare(int flare_){flare = flare_;}
		const int &getFlare()const{return flare;}
		const int &getChaff()const{return chaff;}

private:

		int chaff;
		int flare;

		//int number;
};


}
