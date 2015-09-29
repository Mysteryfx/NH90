
// Нет требования на квадратность геометрии
// Нет ограничений на вертикальные стенки
//Не используется, не переносить. Виталик
class lFragment : public lGeometrySource
{
	// Флаги семантик (у одного тр-ка может быть несколько?)
	ed::vector<uint32_t> semantics;

	// Поверхность
	GeometryItem land;

	// Куски
	ed::vector< GeometryItem> parts;

	// BsdTreeItem
	struct BsdTreeItem
	{
		Edge32 edge;		// вертексы
		int left;			// Если <0 это индекс тр-ка
		int	right;			// Если <0 это индекс тр-ка
		float maxheight;	// максимальная высота вертексов 
		float minheight;	// минимальная высота вертексов 
	};
	std::mmfvector< BsdTreeItem> bsdtree;

	// 

};

