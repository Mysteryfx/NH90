#pragma once
#include "ElementHydraulique_MFX.h"
#include "Tuyau_MFX.h"

namespace cockpit {
	namespace hydraulique {
		/** class RobinetSOV_MFX : public element::ElementHydraulique_MFX
		* Un robinet SOV est un élément hydraulique qui permet de couper la pression au tuyau qui y est rattaché.
		* C'est un élément de secours. 
		* Il est rattaché à deux tuyaux. 
		* Il récupère la pression du premier et la transfert au second dans la mesure ou le robinet n'est pas fermé.
		*/
		class RobinetSOV_MFX :
			public element::ElementHydraulique_MFX
		{
		public:
			RobinetSOV_MFX(MFXDevice *systeme_maitre, float updateTime, float temperature = 20.0f, float pression = 1.0f, element::etat_des_elements_de_base_mfx etat_robinet = element::etat_des_elements_de_base_mfx::etat_inactif_mfx);
			~RobinetSOV_MFX();

			/* void avantRobinet(element::ElementHydraulique_MFX *element)
			* Attaché un tuyau avant le robinet.
			* Le robinet transfert la pression venant de ce tuyau au tuyau attaché après.
			*/
			void avantRobinet(hydraulique::Tuyau_MFX *tuyau);
			/* void apresRobinet(element::ElementHydraulique_MFX *element)
			* Attaché un tuyau après le robinet.
			* Le robinet transfert donc la pression venant ddu tuyau avant à celui attaché après.
			*/
			void apresRobinet(hydraulique::Tuyau_MFX *tuyau);
			/* void fermer()
			* Ferme le robinet SOV pour couper l'accès d'huile aux éléments précédent. Utilisé en cas de fuite.
			*/
			void fermer() { ouvert = false; brancheApres->coupurePression(); }
			/* void ouvrir()
			* Ouvre le robinet SOV pour laisser passer l'huile et ainsi donné de la pression aux éléments précédent.
			*/
			void ouvrir() { ouvert = true; brancheApres->ouverturePression(); }
			/* bool estOuvert()
			* Dis si la sov est ouverte.
			*/
			bool estOuvert() { return ouvert; }

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
			/* void fuite()
			* Demande à l'élément antérieur de faire la simulation d'une fuite.
			* Cette demande doit-être effectué par un élément précédent celui-ci qui serais en panne.
			*/
			virtual void fuite();
			/* void changementPression(float nouvelle_pression)
			* Comme les éléments hydraulique sont connecté les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en même temps.
			*/
			virtual void changementPression(float nouvelle_pression) { setPression(nouvelle_pression); }
			/* float getPression()
			* Donne la pression de l'élément.
			*/
			virtual float getPression() override { return (ouvert ? ElementHydraulique_MFX::getPression() : 0.0f); }

		private:
			hydraulique::Tuyau_MFX *brancheAvant, *brancheApres;	//Les tuyaux branché avant et après le robinet SOV.
			bool ouvert;											//Etat du robinet SOV (différent des états des éléments de base - ici dis si le robinet est oouvert ou fermé).

			/* void setPression(float pression)
			* Modifie la pression de l'élément.
			*/
			virtual void setPression(float pression) override;
		};
	}
}