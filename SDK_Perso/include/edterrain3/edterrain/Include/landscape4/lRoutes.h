#ifndef LANDSCAPE4_LROUTES_H
#define LANDSCAPE4_LROUTES_H

#include "../landscape3_config.h"
#include "edTerrain.h"

#include "landscape4/lRoad.h"
#include "landscape4/lDataFile.h"
#include "landscape4/SpatialTree.h"

namespace roads4
{
	/**
	 * Маршрут движения транспорта
	 */
	struct lRoute
	{
		roads4::lRoad road;			//Дорога, по которой движется транспорт
		
		ed::string vehicleType;	//Тип транспорта

		double fragmentDuration;	//Длительность фрагмента
	};

	/**
	 * Маршруты движения
	 */
	struct lRoutesFile : public landscape4::lDataFile
	{
		/**
		 * Ссылка на фрагмент 
		 */
		struct FragmentRef
		{
			uint32_t route;			//Маршрут движения, в массиве lRoutesFile::routes
			uint32_t fragment;		//Номер фрагмента
		};

		landscape4::SpatialTree<FragmentRef> spatial;	//Ссылки на фрагменты маршрутов

		ed::vector<roads4::lRoute> routes;				//Маршруты движения транспорта

	public:
		virtual ed::string getType() {return "landscape4::lRoutesFile";}
		virtual bool copyFrom(lDataFile* src){*this = *(lRoutesFile*)src;return true;};

		EDTERRAIN_API bool save(const char* filename);
		EDTERRAIN_API bool save(const ed::string& filename);
		EDTERRAIN_API bool load(io::MmfStream& stream);
		EDTERRAIN_API bool load(const char* filename);
		EDTERRAIN_API bool load(const ed::string& filename);

	private:
		bool serialize(io::MmfStream& stream);
	};

}


#endif
