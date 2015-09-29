
#ifndef __INFRAREDVISUALIZER__
#define __INFRAREDVISUALIZER__

#include "Renderer\A2GRadarEnums.h"

#include "smSceneManager_Implement.h"
#include "Resourcer\ResourceManager.h"


typedef enum 
{
	irpHotMode = 0, 
	irpResolution,
	irpMaxCount 
} InfraRedParams;

typedef struct 
{
  ed::vector<woPointer> objList;
} InfraRedSearchTargetData ;

class MFDContainer;

class InfraRedVisualizer 
{
private :
	  SceneManager_Implement* SceneManager;

	  render::FrameBuffer	fbRadarOptical;
      
	  ed::vector<A2GRadarTargetMaterialParams> targets_render_params ; // вектор параметров целей для отрисовки
	  InfraRedSearchTargetData search_target_data ;  //  для запроса поиска целей

public :
	InfraRedVisualizer( SceneManager_Implement* ASceneManager); 
	bool startup();
	void render(MFDContainer &mfdScene, render::FrameBuffer &mfd,  Graphics::DParamList& param_list );
};


#endif /*__INFRAREDVISUALIZER__*/
