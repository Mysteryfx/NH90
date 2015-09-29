namespace edterrain
{


inline unsigned int PointOnSurfaceAdapter::getTypeSurface() const
{
	const int typeMeshLand = 0;  // �����
	const int typeMeshSea = 1;  // ����
	const int typeMeshRoad = 2;  // ����������
	const int typeMeshBeach = 3;  // ��������� �����
	const int typeMeshRiver = 4;  // ����
	const int typeMeshTown = 5;  // ������
	const int typeMeshLake = 7;  // �����
	const int typeMeshIsland = 8;  // �������
	const int typeMeshRailway = 9;  // �.������
	const int typeMeshChannel = 15; // ������
	const int typeMeshForest = 16; // Forest
	const int typeMeshGrass = 17; // Grass
	const int typeMeshStone = 18; // Stone
	const int typeMeshConstruction = 19; // ������ ����������� (������, ������, ...)
	const int typeMeshLine = 20; // �������������� ����� �� �����
	const int typeMeshMGRS = 21; // MGRS ����� �� �����
	const int typeMeshField = 22; // ����
	const int typeMeshRunway = 23; // �������� (���)
	const int typeMeshPlant = 24; // ��������
	const int typeMeshELT = 25; // ��� (��� �����)
	const int typeMeshBuilding = 26; // ��������
	const int typeMeshIsoline = 27; // ����� ����� ��� �����
	const int typeMeshRock = 28; // �����
	const int typeMeshTrees = 29; // ������� �� ���������
	const int typeMeshLamp = 30; // ��������
	const int typeMeshShelf = 31; // ����� (����������� ���)
	const int typeMeshSurfaceDetails = 32; // ������ �����

	const int typeMeshUndef = 33; // �� ����� 33 � ���� � ������ ������� �����
	const int typeMeshMAX = 34;

	static int semanticByMask[32] =
	{
		typeMeshLand,		// ST_LAND   = 0x0001,
		typeMeshUndef,		// 0x0002
		typeMeshRunway,		// ST_RUNWAY = 0x0004,

		typeMeshSea,		// ST_SEA    = 0x0010,
		typeMeshRiver,		// ST_RIVER  = 0x0020,
		typeMeshLake,		// ST_LAKE   = 0x0040,

		typeMeshRoad,		// ST_ROAD   = 0x0100,
		typeMeshRailway,	// ST_RAILWAY= 0x0200,
		typeMeshTown,		// ST_TOWN   = 0x0400,
		typeMeshField,		// ST_FIELD  = 0x0800,

		typeMeshBeach,		// ST_BEACH  = 0x1000,
		typeMeshUndef,		// 0x2000
		typeMeshUndef,		// 0x4000
		typeMeshUndef,		// 0x8000

		typeMeshUndef,		// 0x10000
		typeMeshUndef,		// 0x20000
		typeMeshUndef,		// 0x40000
		typeMeshUndef,		// 0x80000
		typeMeshUndef,		// 0x100000
		typeMeshUndef,		// 0x200000
		typeMeshUndef,		// 0x400000
		typeMeshUndef,		// 0x800000

		typeMeshUndef,		// 0x1000000
		typeMeshUndef,		// 0x2000000
		typeMeshUndef,		// 0x4000000
		typeMeshUndef,		// 0x8000000
		typeMeshUndef,		// 0x10000000
		typeMeshUndef,		// 0x20000000
		typeMeshUndef,		// 0x40000000
		typeMeshUndef,		// 0x80000000
	};

	uint64_t mask = this->types();

	if (mask == 0)
		return ST_LAND;

	// ������ �������� ���
	int i = 0;
	for (; mask != 0; i++, mask = mask >> 1)
	{
		if (mask & 1)
			break;
	}
	return semanticByMask[i];
}


} // namespace edterrain
