#pragma once
#include "ElementMecanique_MFX.h"

namespace cockpit {
	namespace element {

		typedef enum pression_hydraulique_nominal_mfx {
			PRESS_POMPE_HYDRAULIQUE_mfx					= 207,
			PRESS_POMPE_ELECTRIQUE_HYDRAULIQUE_mfx		= 196,
			PRESS_ROBINET_SOV_HYDRAULIQUE_mfx			= 207,
			PRESS_TUYAU_HYDRAULIQUE_mfx					= 207,
			PRESS_VERRIN_HYDRAULIQUE_mfx				= 207,
			PRESS_SYSTEME_HYDRAULIQUE_mfx				= 207,
		};
		typedef enum temperature_hydraulique_nominal_mfx {
			TEMP_POMPE_HYDRAULIQUE_mfx				= 75,
			TEMP_POMPE_ELECTRIQUE_HYDRAULIQUE_mfx	= 75,
			TEMP_ROBINET_SOV_HYDRAULIQUE_mfx		= 75,
			TEMP_TUYAU_HYDRAULIQUE_mfx				= 75,
			TEMP_VERRIN_HYDRAULIQUE_mfx				= 75,
			TEMP_SYSTEME_HYDRAULIQUE_mfx			= 75,
		};

		class ElementHydraulique_MFX :
			public ElementMecanique_MFX
		{
		public:
			/* ElementHydraulique_MFX(float temperature_courante, float pression_courante, type_element_mfx type_element_hydraulique, etat_des_elements_de_base_mfx etat_element_hydraulique)
			* Pour créer un ElementHydraulique_MFX, il faut :
			*			- float temperature, la temperature de l'élément - en fonction de la température la pression augmente ou diminue différemment;
			*			- type_element_mfx type_element_hydraulique, le type de l'élément hydraulique (pompe_mfx, pompe_electrique_mfx, etc...);
			*			- etat_des_elements_de_base_mfx etat_element_hydraulique, l'état de l'élément (etat_actif_mfx, etat_inactif_mfx, etc...);
			*			- float pression, la pression hydraulique - peut-être à son état maximale dès le début de la partie.
			*/
			ElementHydraulique_MFX(MFXDevice *systeme_maitre, float temperature_courante = 20.0f, float pression_courante = 1.0f, type_element_mfx type_element_hydraulique = type_element_mfx::hydraulique_mfx, etat_des_elements_de_base_mfx etat_element_hydraulique = etat_des_elements_de_base_mfx::etat_inactif_mfx, float updateTime = 0.2f);
			~ElementHydraulique_MFX();

			/* float getTemperature()
			* Donne la température de l'élément.
			*/
			virtual float getTemperature();
			/* float getPression()
			* Donne la pression de l'élément.
			*/
			virtual float getPression();
			/* float getTemperatureNominal()
			* Donne la température nominal.
			*/
			float getTemperatureNominal();
			/* float getPressionNominal()
			* Donne la pression nominal.
			*/
			float getPressionNominal();

			/* void demarrage()
			* Procédure de démarrage d'un élément hydraulique.
			*/
			virtual void demarrage() = 0;
			/* void arret()
			* Procédure d'arrêt d'un élément hydraulique.
			*/
			virtual void arret();
			/* void restauration(int ancienne_etat)
			* Procédure de restauration des éléments hydraulique.
			* Remet l'élément à l'ancienne état (avant état de panne).
			*/
			virtual void restauration(int ancienne_etat) = 0;
			/* void recuperation()
			* Procédure de récupération des éléments hydraulique.
			*/
			virtual void recuperation() = 0;
			/* void panne()
			* Procédure de mise en panne des éléments hydraulique.
			*/
			virtual void panne() = 0;
			/* void fuite()
			* Demande à l'élément antérieur de faire la simulation d'une fuite.
			* Cette demande doit-être effectué par un élément précédent celui-ci qui serais en panne.
			*/
			virtual void fuite() = 0;
			/* void changementPression(float nouvelle_pression)
			* Comme les éléments hydraulique sont connecté les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en même temps.
			*/
			virtual void changementPression(float nouvelle_pression) = 0;

			static const float ElementHydraulique_MFX::PRESSIONMARCHE;

		protected:
			float pression_courante;								//Préssion actuelle de l'élément hydraulique (en bar).
			float temperature_courante;								//Température actuelle de l'élément hydraulique (en °C).

			/* void setTemperature(float temperature)
			* Modifie la température de l'élément.
			*/
			virtual void setTemperature(float temperature);
			/* void setPression(float pression)
			* Modifie la pression de l'élément.
			*/
			virtual void setPression(float pression);

		private:
			float temperature_nominal;			//Température nominal de l'élément hydraulique.
			float pression_nominal;				//Pression nominal de l'élément hydraulique.

		};
	}
}