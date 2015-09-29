#pragma once
#include "ElementHydraulique_MFX.h"
#include <vector>

namespace cockpit {
	namespace hydraulique {
		/** class Tuyau_MFX : public element::ElementHydraulique_MFX
		* Un tuyau du système hydraulique est un élément qui permet de transférer de l'huile d'un élément à un autre, c'est donc une passerelle. 
		* Un tuyau ne produit rien mais transmet ce qu'on lui donne. La pression ainsi transférer est la pression venant de l'élément qui le précède.
		* Un tuyau en panne produit une fuite.
		******************************************************* 
		* Un tuyau possède un capteur de pression.
		* Celui-ci donne des informations notamment au MFD.
		*/
		class Tuyau_MFX :
			public element::ElementHydraulique_MFX
		{
		public:
			Tuyau_MFX(MFXDevice *systeme_maitre, float updateTime, float temperature_courante = 20.0f, float pression_courante = 1.0f, element::etat_des_elements_de_base_mfx etat_element_hydraulique = element::etat_des_elements_de_base_mfx::etat_inactif_mfx);
			~Tuyau_MFX();

			/* void avantTuyau(element::ElementHydraulique_MFX *element)
			* Attaché un élément avant le tuyau.
			* Le tuyau transfert la pression venant de cet élément à l'élément attaché après.
			*/
			void avantTuyau(element::ElementHydraulique_MFX *element);
			/* void apresTuyau(element::ElementHydraulique_MFX *element)
			* Attaché un élément après le tuyau.
			* Le tuyau transfert donc la pression venant de l'élément attaché avant à celui attaché après.
			*/
			void apresTuyau(element::ElementHydraulique_MFX *element);
			/* void coupurePression()
			* Coupure de la pression dans le tuyau. Instruction venant d'un robinet SOV.
			* Pression descendue à 0 et influence sur les éléments précédant.
			*/
			virtual void coupurePression();
			/* void ouverturePression()
			* Ouverture de la pression dans le tuyau. Instruction venant d'un robinet SOV.
			* Pression récupérer depuis l'/les éléments antérieur.
			*/
			virtual void ouverturePression();
			/* void fuite()
			* Demande à l'élément antérieur de faire la simulation d'une fuite.
			* Cette demande doit-être effectué par un élément précédent celui-ci qui serais en panne.
			*/
			virtual void fuite();
			/* void changementPression(float nouvelle_pression)
			* Comme les éléments hydraulique sont connecté les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en même temps.
			*/
			virtual void changementPression(float nouvelle_pression);

			/* void demarrage()
			* Lors du démarrage d'une pompe hydraulique quelcquonque.
			*/
			virtual void demarrage();
			/* void arret()
			* Procédure d'arrêt d'un élément hydraulique.
			*/
			virtual void arret();
			/* void restauration(int ancienne_etat)
			* Procédure de restauration des éléments de base.
			* Remet l'élément à l'ancienne état (avant état de panne).
			*/
			virtual void restauration(int ancienne_etat);
			/* void recuperation()
			* Procédure de récupération des éléments de base.
			*/
			virtual void recuperation() {}
			/* void panne()
			* Procédure de mise en panne des éléments de base.
			*/
			virtual void panne();

		protected:
			std::vector<element::ElementHydraulique_MFX*> brancheAvant, brancheApres;	//Element rattaché au tuyau. Peuvent-être Pompe, Robinet SOV, Verin, etc...

			/* float getPressionAvant()
			* Donne la plus haute pression des éléments branché en amont du tuyau.
			*/
			float getPressionAvant();
			/* float getPompePression()
			* Donne la pression dans les pompes.
			*/
			float getPompePression();

			/* void setPression(float pression)
			* Modifie la pression de l'élément.
			*/
			virtual void setPression(float pression) override;

		};
	}
}