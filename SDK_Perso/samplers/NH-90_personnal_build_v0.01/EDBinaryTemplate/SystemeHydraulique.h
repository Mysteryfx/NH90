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
			* Le système électrique informe le système hydraulique que son état évolue.
			*/
			void electriciteChange(bool electricite);
			/* void electriciteOn()
			* Le système hydraulique conserve l'information selon laquelle le système électrique est on.
			*/
			void electriciteOn();
			/* void electriciteOff()
			* Le système hydraulique conserve l'information selon laquelle le système électrique est off.
			*/
			void electriciteOff();

			/* void elementAChangerEtat(element::type_element_mfx type)
			* Un élément informe le système qu'il à changer d'état.
			* On retrouve les éléments à vérifié par le type de celui-ci.
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
			//Avant et Après SOV
			const int arg_mfd_hyd_c12				= 871; //Circuit avant SOV1 & TRSOV
			const int arg_mfd_hyd_c11				= 863; //Circuit après SOV1
			const int arg_mfd_hyd_t1				= 861; //Circuit après TRSOV
			//Réservoir
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
			//Avant et Après SOV
			const int arg_mfd_hyd_c21				= 872; //Circuit avant SOV2 et UtilSOV
			const int arg_mfd_hyd_c22				= 873; //Circuit après UtilSOV
			const int arg_mfd_hyd_t2				= 862; //Circuit après SOV2
			//Réservoir
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
			* Vérification des SOV. Signifie qu'une sov à changé d'état.
			*/
			void verificationSov();
			/* void verificationTuyau()
			* Vérification des tuyaux. Signifie q'un tuyau à changé d'état.
			*/
			void verificationTuyau();
			/* void verificationPompe()
			* Vérification des pompes. Signifie q'une pompe à changé d'état.
			*/
			void verificationPompe();
			/* void verificationVoyant()
			* Vérification des voyants.
			*/
			void verificationVoyant();
			/* void verifieTout()
			* Vérification de tous les éléments.
			*/
			void verifieTout();

		};
	}
}