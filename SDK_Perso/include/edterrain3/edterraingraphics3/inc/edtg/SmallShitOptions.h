#ifndef EDTG_SmallShitOptions_H_
#define EDTG_SmallShitOptions_H_

namespace edtg
{

//Small shit
struct SmallShitOptions
{

	struct File
	{
		CopyBreakpoint copybreakpoint;

		// имя blk файла
		ed::string name;

		struct Layer
		{
			float squareSize;		// размер квадрата в метрах
			float scale;			// масштаб, позиция вертекса из blk умножается на него
		};

		ed::vector<Layer> layers;

		// параметры для неравномерного появления
		float slope;
		float slopeDelta;

		// имя 3D текстуры
		ed::string texFileName;
	};

	ed::vector<File> files;

	// разрешение текстуры
	int heightMapSize;

	// референсная дальность, метров (для которой настроены squareSize)
	float referenceDimention;
	// текущая дальность, метров (от настроек и раствора камеры)
//	float dimention;
	// плотность 1-столько сколько в
	float dencity;


	// Расчитывается в рантайме. Костыль чтоб не переделывать криворучкость
	float maxSquareSize;
};

}

#endif