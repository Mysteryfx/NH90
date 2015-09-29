#pragma once

namespace cockpit{
	namespace NH_90 {
		class SystemeElectrique;
	}
	namespace ecran {
		typedef enum mfd_type {
			alt,
			bat,
			acb,
			egen,
			emerb,
			essb,
			dbat,
			sbmb,
			tru,
			dcb,
			dcsb
		};
		class MFD
		{
		public:
			MFD();
			~MFD();
			MFD(std::string name, int argument, mfd_type type, cockpit::NH_90::SystemeElectrique* electrique_systeme);
			MFD(std::string name, int argument, mfd_type type, cockpit::NH_90::SystemeElectrique* electrique_systeme, MFD* secours, int argumen_systeme_secour);

			void setSecour(int argument_secour, MFD* secour);

			virtual void change(float value) = 0;

			virtual float getValue() = 0;
			bool comparaison(float value);
			std::string getName();
			int getArgument();
			mfd_type getType();

		protected:
			int argument;
			std::string name;
			mfd_type type;
			int value;

			MFD* secours;
			int argumen_systeme_secour;

			cockpit::NH_90::SystemeElectrique* electrique_systeme;
		};
	}
}