#pragma once
#include "Sources.h"
#include "BoutonDeuxPosition.h"
#include "VoyantTroisEtat.h"


namespace cockpit {
	namespace NH_90 {
		typedef enum etat_des_elements_electriques_externe {
			element_panne							= 0,
			element_non_branche						= 0,
			element_branche_non_utilise				= 60,
			element_branche_utilise					= 30,
			element_branche_utilise_non_prioritaire	= 100
		};
		typedef enum type_alim_externe {
			dcepu_alimentation,
			acepu_alimentation
		};
		class SourcesExternes :
			public Sources
		{
		public:
			SourcesExternes();
			SourcesExternes(double nominalVolt);
			SourcesExternes(double nominalVolt, etat_des_elements_electriques_externe etat, type_alim_externe type, SystemeElectrique* systeme_maitre, element::BoutonDeuxPosition bouton, element::VoyantTroisEtat voyant);
			~SourcesExternes();

			/* virtual etat_des_elements_electrique getEtat() override;
			* Donne la correspondance de l'"état de l'équipement électrique externe" en "état des éléments électriques de base".
			*/
			virtual etat_des_elements_electrique getEtat() override;
			/* virtual etat_des_elements_electriques_externe getEtatExterne();
			* Donne l'état de l'équipement électrique externe.
			*/
			virtual etat_des_elements_electriques_externe getEtatExterne();
			/* virtual type_alim_externe getType();
			* Donne le type de source externe.
			*/
			virtual type_alim_externe getType();
			/* void setEtatExterne(etat_des_elements_electriques_externe etat_externe);
			* Donne une valeur aux différents états (etat_externe et etat en faisant la correspondance entre les deux type de donné).
			*/
			void setEtatExterne(etat_des_elements_electriques_externe etat_externe);
			/* void ElementElectriqueDeBase::setOnOff(bool status) :
			* Si l'état n'est pas etat_panne :
			*		- Affecte la valeur etat_actif si status true,
			*		- Affecte la valeur etat_stand_by si status false.
			*/
			virtual void setOnOff(bool status) override;

			/* void branche();
			* Branche l'équipement externe.
			*/
			void branche();
			/* void debranche();
			* Débranche l'équipement externe.
			*/
			void debranche();
			/* void userChangeEtat(float value) :
			* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
			*/
			void userChangeEtat(float value);

			/* bool isUsed();
			* Demande au systeme_maitre si l'équipement est utilisé.
			*/
			bool isUsed();
			/* bool isOnOff() :
			* Retourne true si l'équipement est on.
			*/
			virtual bool isOnOff() override;
			/* bool ElementElectriqueDeBase::isAlive() :
			* Retourne true si l'équipement externe est branché et pas en panne, peu importe son utilisation.
			*/
			virtual bool isAlive() override;
			/* int get_etat_externe();
			* Donne l'état de la source externe sous forme d'entier.
			*/
			int get_etat_externe();
			/* void update();
			* Mets à jour l'état de la source externe (selon ordre de priorité).
			*/
			virtual void update();
			/* void teste_voyant();
			* Affiche l'état de test du voyant.
			*/
			void teste_voyant() { voyant.test_voyant(); }

		protected:
			type_alim_externe type;									//Type de source externe (DC ou AC).
			etat_des_elements_electriques_externe etat_externe;		//Etat de l'équipement électrique externe.

			element::BoutonDeuxPosition bouton;						//Bouton On/Off de la source externe.
			element::VoyantTroisEtat voyant;						//Voyant qui affiche l'état de la source externe.

			/* virtual void checkingUsage();
			* Vérifie le mode de fonction de la source externe.
			*/
			virtual void checkingUsage();

		};
	}
}