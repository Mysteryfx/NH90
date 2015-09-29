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
			* Pour cr�er un ElementHydraulique_MFX, il faut :
			*			- float temperature, la temperature de l'�l�ment - en fonction de la temp�rature la pression augmente ou diminue diff�remment;
			*			- type_element_mfx type_element_hydraulique, le type de l'�l�ment hydraulique (pompe_mfx, pompe_electrique_mfx, etc...);
			*			- etat_des_elements_de_base_mfx etat_element_hydraulique, l'�tat de l'�l�ment (etat_actif_mfx, etat_inactif_mfx, etc...);
			*			- float pression, la pression hydraulique - peut-�tre � son �tat maximale d�s le d�but de la partie.
			*/
			ElementHydraulique_MFX(MFXDevice *systeme_maitre, float temperature_courante = 20.0f, float pression_courante = 1.0f, type_element_mfx type_element_hydraulique = type_element_mfx::hydraulique_mfx, etat_des_elements_de_base_mfx etat_element_hydraulique = etat_des_elements_de_base_mfx::etat_inactif_mfx, float updateTime = 0.2f);
			~ElementHydraulique_MFX();

			/* float getTemperature()
			* Donne la temp�rature de l'�l�ment.
			*/
			virtual float getTemperature();
			/* float getPression()
			* Donne la pression de l'�l�ment.
			*/
			virtual float getPression();
			/* float getTemperatureNominal()
			* Donne la temp�rature nominal.
			*/
			float getTemperatureNominal();
			/* float getPressionNominal()
			* Donne la pression nominal.
			*/
			float getPressionNominal();

			/* void demarrage()
			* Proc�dure de d�marrage d'un �l�ment hydraulique.
			*/
			virtual void demarrage() = 0;
			/* void arret()
			* Proc�dure d'arr�t d'un �l�ment hydraulique.
			*/
			virtual void arret();
			/* void restauration(int ancienne_etat)
			* Proc�dure de restauration des �l�ments hydraulique.
			* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
			*/
			virtual void restauration(int ancienne_etat) = 0;
			/* void recuperation()
			* Proc�dure de r�cup�ration des �l�ments hydraulique.
			*/
			virtual void recuperation() = 0;
			/* void panne()
			* Proc�dure de mise en panne des �l�ments hydraulique.
			*/
			virtual void panne() = 0;
			/* void fuite()
			* Demande � l'�l�ment ant�rieur de faire la simulation d'une fuite.
			* Cette demande doit-�tre effectu� par un �l�ment pr�c�dent celui-ci qui serais en panne.
			*/
			virtual void fuite() = 0;
			/* void changementPression(float nouvelle_pression)
			* Comme les �l�ments hydraulique sont connect� les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en m�me temps.
			*/
			virtual void changementPression(float nouvelle_pression) = 0;

			static const float ElementHydraulique_MFX::PRESSIONMARCHE;

		protected:
			float pression_courante;								//Pr�ssion actuelle de l'�l�ment hydraulique (en bar).
			float temperature_courante;								//Temp�rature actuelle de l'�l�ment hydraulique (en �C).

			/* void setTemperature(float temperature)
			* Modifie la temp�rature de l'�l�ment.
			*/
			virtual void setTemperature(float temperature);
			/* void setPression(float pression)
			* Modifie la pression de l'�l�ment.
			*/
			virtual void setPression(float pression);

		private:
			float temperature_nominal;			//Temp�rature nominal de l'�l�ment hydraulique.
			float pression_nominal;				//Pression nominal de l'�l�ment hydraulique.

		};
	}
}