#pragma once
#include "Sources.h"
#include "BoutonDeuxPosition.h"
#include "VoyantDeuxEtat.h"

using namespace cockpit::element;

namespace cockpit {
	class ccMainPanel;
	namespace NH_90 {
		class SystemeElectrique;
		enum switch_mode;
		class Batterie :
			public Sources
		{
		public:
			Batterie();
			~Batterie();
			/* Batterie(int i_voltNominal, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre, cockpit::element::BoutonDeuxPosition bouton, cockpit::element::VoyantDeuxEtat voyant);
			* Instancie une batterie avec un bouton et un voyant.
			*/
			Batterie(int i_voltNominal, etat_des_elements_electrique etat, /*double temperature,*/ SystemeElectrique* systeme_maitre, BoutonDeuxPosition bouton, VoyantDeuxEtat voyant);

			/* void setEtat(etat_des_elements_electrique etat);
			* Modifie l'état de la batterie.
			*/
			virtual void setEtat(etat_des_elements_electrique etat) override;
			/* cockpit::NH_90::switch_mode getSwitchedMode() :
			* Retourne le mode d'utilisation de la batterie.
			*/
			switch_mode getSwitchedMode();
			/* bool isMcoCut() :
			* Retourne true si la manette MCO qui gouverne le système électrique à été coupé.
			*/
			bool isMcoCut();
			/* void update() :
			* Cette fonction va agire directement sur la durée de vie de la batterie.
			*/
			virtual void update();
			/* void charge(double voltCharge) :
			* Cette fonction dit à la batterie qu'elle est en charge.
			*/
			void charge(double voltCharge);
			/* bool isInCharge() :
			* Cette fonction dit si la batterie est en recharge.
			*/
			bool isInCharge();
			/* void userChangeEtat(float value) :
			* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
			*/
			virtual void userChangeEtat(float value);
			/* bool isOnOff() :
			* Fonctionnement différent de ce qui était initialement prévue.
			* Retourne toujours true si etat_actif et false sinon.
			*/
			virtual bool isOnOff();
			/* void afficheEtat();
			* Affiche toutes les informations du verrou.
			*/
			void afficheEtat();
			/* void teste_voyant();
			* Affiche l'état de test du voyant.
			*/
			void teste_voyant() { voyant.test_voyant(static_cast<int>(enum_voyant_batterie::test_voyant_batterie)); }

		private:
			//double temperature;										//Température interne de la batterie.
			static const int lifetime = 10 * 60;						//Duré de vie d'une batterie sur une utilisation en mode normal continu (10 minutes). En mode stand by > 4h et en mode data = 1h30
			double life;												//Vie de la batterie.
			bool inCharge;												//Information selon laquelle la batterie se recharge.
			double voltCharge;											//Voltage d'entréeé de la recharge batterie.

			BoutonDeuxPosition bouton;									//Bouton On/Off de la batterie.
			VoyantDeuxEtat voyant;										//Voyant qui affiche l'état de la batterie.

			/* void initBatterie(double temperature, etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre) :
			* Initialisation des propriétés de la batterie.
			*/
			void initBatterie(/*double temperature,*/ etat_des_elements_electrique etat, SystemeElectrique* systeme_maitre);
		};
	}
}