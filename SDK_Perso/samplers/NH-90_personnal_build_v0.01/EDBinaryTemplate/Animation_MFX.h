#pragma once

namespace cockpit {
	class ccMainPanel;
	namespace animation {
		class Animation_MFX
		{
		public:
			/* Animation_MFX(int argument, ccMainPanel * panel)
			* Pour créer une animation, il faut :
			*		- argument (nombre entier), l'argument de l'objet animable dans le cockpit;
			*		- panel (pointer vers ccMainPanel), le panel avec lequel on fera les animations.
			*/
			Animation_MFX(int argument, ccMainPanel * panel);
			~Animation_MFX();

			/* float getValeur()
			* Donne la valeur de l'argument.
			*/
			float getValeur();
			/* int getArgument()
			* Donne l'argument.
			*/
			int getArgument();

			/* void userChangeEtat(float value) :
			* Si le changement d'état vient de l'utilisateur, il faut animé l'action approppriée dans le cockpit.
			*/
			virtual void userChangeEtat(float value) = 0;

		protected:
			/* void setValeur(float valeur)
			* Modifie la valeur de l'argument.
			*/
			void setValeur(float valeur);

		private:
			int argument;				//Argument responsable de l'animation des éléments du cockpit.
			ccMainPanel * panel;		//Panel avec lequel toutes les animations ce feront.

		};
	}
}