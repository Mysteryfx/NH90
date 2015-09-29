#pragma once
#include "Batterie.h"

namespace cockpit {
	namespace NH_90 {
		class GenerateurUrgence :
			public Batterie
		{
		public:
			GenerateurUrgence();
			~GenerateurUrgence();


		};
	}
}