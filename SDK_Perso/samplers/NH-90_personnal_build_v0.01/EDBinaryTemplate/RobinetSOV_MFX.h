#pragma once
#include "ElementHydraulique_MFX.h"
#include "Tuyau_MFX.h"

namespace cockpit {
	namespace hydraulique {
		/** class RobinetSOV_MFX : public element::ElementHydraulique_MFX
		* Un robinet SOV est un �l�ment hydraulique qui permet de couper la pression au tuyau qui y est rattach�.
		* C'est un �l�ment de secours. 
		* Il est rattach� � deux tuyaux. 
		* Il r�cup�re la pression du premier et la transfert au second dans la mesure ou le robinet n'est pas ferm�.
		*/
		class RobinetSOV_MFX :
			public element::ElementHydraulique_MFX
		{
		public:
			RobinetSOV_MFX(MFXDevice *systeme_maitre, float updateTime, float temperature = 20.0f, float pression = 1.0f, element::etat_des_elements_de_base_mfx etat_robinet = element::etat_des_elements_de_base_mfx::etat_inactif_mfx);
			~RobinetSOV_MFX();

			/* void avantRobinet(element::ElementHydraulique_MFX *element)
			* Attach� un tuyau avant le robinet.
			* Le robinet transfert la pression venant de ce tuyau au tuyau attach� apr�s.
			*/
			void avantRobinet(hydraulique::Tuyau_MFX *tuyau);
			/* void apresRobinet(element::ElementHydraulique_MFX *element)
			* Attach� un tuyau apr�s le robinet.
			* Le robinet transfert donc la pression venant ddu tuyau avant � celui attach� apr�s.
			*/
			void apresRobinet(hydraulique::Tuyau_MFX *tuyau);
			/* void fermer()
			* Ferme le robinet SOV pour couper l'acc�s d'huile aux �l�ments pr�c�dent. Utilis� en cas de fuite.
			*/
			void fermer() { ouvert = false; brancheApres->coupurePression(); }
			/* void ouvrir()
			* Ouvre le robinet SOV pour laisser passer l'huile et ainsi donn� de la pression aux �l�ments pr�c�dent.
			*/
			void ouvrir() { ouvert = true; brancheApres->ouverturePression(); }
			/* bool estOuvert()
			* Dis si la sov est ouverte.
			*/
			bool estOuvert() { return ouvert; }

			/* void demarrage()
			* Lors du d�marrage d'une pompe hydraulique quelcquonque.
			*/
			virtual void demarrage();
			/* void arret()
			* Proc�dure d'arr�t d'un �l�ment hydraulique.
			*/
			virtual void arret();
			/* void restauration(int ancienne_etat)
			* Proc�dure de restauration des �l�ments de base.
			* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
			*/
			virtual void restauration(int ancienne_etat);
			/* void recuperation()
			* Proc�dure de r�cup�ration des �l�ments de base.
			*/
			virtual void recuperation() {}
			/* void panne()
			* Proc�dure de mise en panne des �l�ments de base.
			*/
			virtual void panne();
			/* void fuite()
			* Demande � l'�l�ment ant�rieur de faire la simulation d'une fuite.
			* Cette demande doit-�tre effectu� par un �l�ment pr�c�dent celui-ci qui serais en panne.
			*/
			virtual void fuite();
			/* void changementPression(float nouvelle_pression)
			* Comme les �l�ments hydraulique sont connect� les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en m�me temps.
			*/
			virtual void changementPression(float nouvelle_pression) { setPression(nouvelle_pression); }
			/* float getPression()
			* Donne la pression de l'�l�ment.
			*/
			virtual float getPression() override { return (ouvert ? ElementHydraulique_MFX::getPression() : 0.0f); }

		private:
			hydraulique::Tuyau_MFX *brancheAvant, *brancheApres;	//Les tuyaux branch� avant et apr�s le robinet SOV.
			bool ouvert;											//Etat du robinet SOV (diff�rent des �tats des �l�ments de base - ici dis si le robinet est oouvert ou ferm�).

			/* void setPression(float pression)
			* Modifie la pression de l'�l�ment.
			*/
			virtual void setPression(float pression) override;
		};
	}
}