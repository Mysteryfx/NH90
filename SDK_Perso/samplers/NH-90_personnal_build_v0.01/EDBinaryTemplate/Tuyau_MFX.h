#pragma once
#include "ElementHydraulique_MFX.h"
#include <vector>

namespace cockpit {
	namespace hydraulique {
		/** class Tuyau_MFX : public element::ElementHydraulique_MFX
		* Un tuyau du syst�me hydraulique est un �l�ment qui permet de transf�rer de l'huile d'un �l�ment � un autre, c'est donc une passerelle. 
		* Un tuyau ne produit rien mais transmet ce qu'on lui donne. La pression ainsi transf�rer est la pression venant de l'�l�ment qui le pr�c�de.
		* Un tuyau en panne produit une fuite.
		******************************************************* 
		* Un tuyau poss�de un capteur de pression.
		* Celui-ci donne des informations notamment au MFD.
		*/
		class Tuyau_MFX :
			public element::ElementHydraulique_MFX
		{
		public:
			Tuyau_MFX(MFXDevice *systeme_maitre, float updateTime, float temperature_courante = 20.0f, float pression_courante = 1.0f, element::etat_des_elements_de_base_mfx etat_element_hydraulique = element::etat_des_elements_de_base_mfx::etat_inactif_mfx);
			~Tuyau_MFX();

			/* void avantTuyau(element::ElementHydraulique_MFX *element)
			* Attach� un �l�ment avant le tuyau.
			* Le tuyau transfert la pression venant de cet �l�ment � l'�l�ment attach� apr�s.
			*/
			void avantTuyau(element::ElementHydraulique_MFX *element);
			/* void apresTuyau(element::ElementHydraulique_MFX *element)
			* Attach� un �l�ment apr�s le tuyau.
			* Le tuyau transfert donc la pression venant de l'�l�ment attach� avant � celui attach� apr�s.
			*/
			void apresTuyau(element::ElementHydraulique_MFX *element);
			/* void coupurePression()
			* Coupure de la pression dans le tuyau. Instruction venant d'un robinet SOV.
			* Pression descendue � 0 et influence sur les �l�ments pr�c�dant.
			*/
			virtual void coupurePression();
			/* void ouverturePression()
			* Ouverture de la pression dans le tuyau. Instruction venant d'un robinet SOV.
			* Pression r�cup�rer depuis l'/les �l�ments ant�rieur.
			*/
			virtual void ouverturePression();
			/* void fuite()
			* Demande � l'�l�ment ant�rieur de faire la simulation d'une fuite.
			* Cette demande doit-�tre effectu� par un �l�ment pr�c�dent celui-ci qui serais en panne.
			*/
			virtual void fuite();
			/* void changementPression(float nouvelle_pression)
			* Comme les �l�ments hydraulique sont connect� les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en m�me temps.
			*/
			virtual void changementPression(float nouvelle_pression);

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

		protected:
			std::vector<element::ElementHydraulique_MFX*> brancheAvant, brancheApres;	//Element rattach� au tuyau. Peuvent-�tre Pompe, Robinet SOV, Verin, etc...

			/* float getPressionAvant()
			* Donne la plus haute pression des �l�ments branch� en amont du tuyau.
			*/
			float getPressionAvant();
			/* float getPompePression()
			* Donne la pression dans les pompes.
			*/
			float getPompePression();

			/* void setPression(float pression)
			* Modifie la pression de l'�l�ment.
			*/
			virtual void setPression(float pression) override;

		};
	}
}