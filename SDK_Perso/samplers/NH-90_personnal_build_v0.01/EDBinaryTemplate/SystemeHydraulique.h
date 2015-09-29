#pragma once
#include "MFXDevice.h"

#include "Pompe_MFX.h"
#include "Tuyau_MFX.h"
#include "RobinetSOV_MFX.h"
#include "ReservoirHuile_MFX.h"

namespace cockpit {
	namespace NH_90 {
		class SystemeHydraulique :
			public MFXDevice
		{
		public:
			SystemeHydraulique();
			~SystemeHydraulique();

			virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
			virtual void SetCommand(int command, float value = 0);
			virtual void update();

			virtual bool checkAnimationConditions(int arg_num, int command, float& value);

			/* void electriciteChange(bool electricite)
			* Le syst�me �lectrique informe le syst�me hydraulique que son �tat �volue.
			*/
			void electriciteChange(bool electricite);
			/* void electriciteOn()
			* Le syst�me hydraulique conserve l'information selon laquelle le syst�me �lectrique est on.
			*/
			void electriciteOn();
			/* void electriciteOff()
			* Le syst�me hydraulique conserve l'information selon laquelle le syst�me �lectrique est off.
			*/
			void electriciteOff();

			/* void elementAChangerEtat(element::type_element_mfx type)
			* Un �l�ment informe le syst�me qu'il � changer d'�tat.
			* On retrouve les �l�ments � v�rifi� par le type de celui-ci.
			*/
			virtual void elementAChangerEtat(element::type_element_mfx type);

		private:
			hydraulique::Pompe_MFX pompe1, pompe2, pompe4, epompe;
			hydraulique::Tuyau_MFX hyd1_tuyau1, hyd1_tuyau2, hyd1_tuyau3,
								   hyd2_tuyau1, hyd2_tuyau2, hyd2_tuyau3;
			hydraulique::RobinetSOV_MFX sov1, sov2, trsov, utilsov;

			fluide::ReservoirHuile_MFX reservoir1, reservoir2;
			moteur::Moteur_MFX mgb, ragb;

			bool electricite;

			////////////Liste des arguments des boutons et cache dans le cockpit.
			//UtilSOV
			const int arg_cache_util		= 21;
			const int arg_switch_util		= 22;
			//ANCPMP
			const int arg_switch_ancpmp		= 427;
			//Caliper ?
			const int arg_cache_caliper		= 24;
			const int arg_switch_caliper	= 25;
			//Frein du rotor
			const int arg_cache_rotorbreak	= 26;
			const int arg_switch_rotorbreak = 27;
			//Pompe Electrique
			const int arg_cache_epompe		= 28;
			const int arg_switch_epompe		= 428;
			//SOV
			const int arg_cache_sov			= 31;
			const int arg_switch_sov		= 32;

			////////////Liste des arguments du MFD Hydraulique
			////////////HYDRAULIQUE 1
			//Pompe
			const int arg_mfd_hyd_epompe			= 846;
			const int arg_mfd_hyd_epompe_pression	= 843;
			const int arg_mfd_hyd_pompe1			= 847;
			const int arg_mfd_hyd_pompe1_pression	= 842;
			//SOV
			const int arg_mfd_hyd_sov1				= 848;
			const int arg_mfd_hyd_trsov				= 860;
			//Avant et Apr�s SOV
			const int arg_mfd_hyd_c12				= 871; //Circuit avant SOV1 & TRSOV
			const int arg_mfd_hyd_c11				= 863; //Circuit apr�s SOV1
			const int arg_mfd_hyd_t1				= 861; //Circuit apr�s TRSOV
			//R�servoir
			const int arg_mfd_hyd_level1			= 841;
			const int arg_mfd_hyd_arrow1			= 736;
			//Circuit 1
			const int arg_mfd_hyd_leak_1_2			= 840; //Optimisation des arguments. LEAK1 et LEAK2 unis.
			const int arg_mfd_hyd_pression1			= 849;
			const int arg_mfd_hyd_htemperature1		= 845; //HTEMP
			const int arg_mfd_hyd_temperature1		= 844;
			const int arg_mfd_hyd_trans1			= 881;
			const int arg_mfd_hyd_trans2			= 879;
			const int arg_mfd_hyd_trans3			= 880;
			const int arg_mfd_hyd_fcp1				= 876;
			const int arg_mfd_hyd_trp				= 875;
			////////////HYDRAULIQUE 2
			//Pompe
			const int arg_mfd_hyd_pompe2			= 853;
			const int arg_mfd_hyd_pompe2_pression	= 856;
			const int arg_mfd_hyd_pompe4			= 854;
			const int arg_mfd_hyd_pompe4_pression	= 857;
			//SOV
			const int arg_mfd_hyd_sov2				= 858;
			const int arg_mfd_hyd_utsov				= 859;
			//Avant et Apr�s SOV
			const int arg_mfd_hyd_c21				= 872; //Circuit avant SOV2 et UtilSOV
			const int arg_mfd_hyd_c22				= 873; //Circuit apr�s UtilSOV
			const int arg_mfd_hyd_t2				= 862; //Circuit apr�s SOV2
			//R�servoir
			const int arg_mfd_hyd_level2			= 852;
			const int arg_mfd_hyd_arrow2			= 757;
			//Circuit 2
			const int arg_mfd_hyd_pression2			= 874;
			const int arg_mfd_hyd_htemperature2		= 855;
			const int arg_mfd_hyd_temperature2		= 722;
			const int arg_mfd_hyd_trans4			= 881;
			const int arg_mfd_hyd_trans5			= 882;
			const int arg_mfd_hyd_trans6			= 883;
			const int arg_mfd_hyd_fcp2				= 877;
			////////////Circuit commun
			const int arg_mfd_hyd_landing			= 867;

			/* void mfd_ecran(int argument, float valeur)
			* Effectue l'affichage dans le mfd correspondant.
			*/
			void mfd_ecran(int argument, float valeur);

			/* void verificationSov()
			* V�rification des SOV. Signifie qu'une sov � chang� d'�tat.
			*/
			void verificationSov();
			/* void verificationTuyau()
			* V�rification des tuyaux. Signifie q'un tuyau � chang� d'�tat.
			*/
			void verificationTuyau();
			/* void verificationPompe()
			* V�rification des pompes. Signifie q'une pompe � chang� d'�tat.
			*/
			void verificationPompe();
			/* void verificationVoyant()
			* V�rification des voyants.
			*/
			void verificationVoyant();
			/* void verifieTout()
			* V�rification de tous les �l�ments.
			*/
			void verifieTout();

		};
	}
}