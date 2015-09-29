#ifndef _OFFSHORE_SCENE_H__
#define _OFFSHORE_SCENE_H__

#include "../offshore_config.h"

#include <ed/string.h>
#include <ed/set.h>

#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/Vec4f>
#include <osg/Vec4d>
#include <osg/Matrixd>
#include <osg/BoundingBox>
#include <osg/Rect>

#include <functional>

////////////////////////////////////////////////////////////////////////////////
//
// Структуры для связи с утилитами и независимости кода от пакета моделирования
//
// TreeNode - дерево нод
// нода TreeNode может ссылаться на один из примитивов (трансформ, меш и т.д.)
// примитивы хранятся в отдельных списках transforms, meshes и т.д.
//
namespace offshore
{
	struct Scene;
	struct Error;
	struct Instance;

	// тип ноды
	enum enType
	{
		NONE      = 0,
		TRANSFORM = 1,
		MESH      = 2,
		SPLINE    = 3,
		POINTS    = 4,
		SKINMESH  = 5,
		BONES     = 6,
		INSTANCE  = 7,
		LIGHT     = 8,
		CAMERA    = 9,
	};

	// нода
	// для организации иерархии
	struct TreeNode
	{
		int32_t type;					// тип элемента (трансформ/меш/кривая)
		int32_t index;					// номер в соотв.списке
		ed::string nodename;			// имя
		ed::string nodetype;			// тип ноды "kTransform", "kjoint", etc
		int32_t parent;					// папа
		ed::list<int32_t> childs;		// дети
		uint64_t pMaxNode;				// id ноды в 3dsMax
		uint64_t pMaxParentNode;		// id парент ноды в 3dsMax
		// mitka 08/12/2009
		ed::map< ed::string, ed::string> params;	// параметры ноды (например wirecolor)

		OFFSHORE_API TreeNode();
		OFFSHORE_API TreeNode(int32_t type, int32_t index, const char* nodename, const char* nodetype, int32_t parent = -1, uint64_t pMaxNode = 0	);
		OFFSHORE_API TreeNode(int32_t type, int32_t index, const ed::string& nodename = "", const ed::string& nodetype = "", int32_t parent = -1, uint64_t pMaxNode = 0	);

		/**
		 * Удалить из списка детей ноду с индексом nodeIndex если таковая имеется
		 * @param nodeIndex идекс ноды, которую надо удалить
		 */
		OFFSHORE_API void removeChild(int32_t nodeIndex);
	};

	// реальный индекс для зацикленого массива
	inline int cycle(int v, int size);

	// трансформ
	struct Transform
	{
		int32_t treenode;
		osg::Matrixd m;
	};

	struct Light
	{
		int32_t treenode;

		enum enType
		{
			OMNI        = 0,
			SPOT        = 1,
			DIRECTIONAL = 2,
		};

		int8_t type;

		osg::Vec4f color;

		// omin light settings
		float omniAttenuation;

		// spot light settings
		//...

		// directional light settings
		//...
	};


	struct Camera
	{
		enum enType
		{
			PERSP = 0,
			ORTHO = 1
		};
		enum enFovType
		{
			WIDTH    = 0,
			HEIGHT   = 1,
			DIAGONAL = 2
		};

		int32_t treenode;
	
		int8_t type; // enType

		int8_t fovType; // enFovType
		double fov; // в градусах

		double aspect;

		double nearClipPlane;
		double farClipPlane;

		bool isViewport;
		
		Camera();
	};

	// Для меша:
	// треугольник
	struct Face
	{
		Face(){};
		Face(uint32_t v1, uint32_t v2, uint32_t v3)
		{
			v[0] = v1;
			v[1] = v2;
			v[2] = v3;
		}

		uint32_t v[3];

	public:
		//================================================================================
		// containsEdge
		//    Если полигон содержит ребро [a, b], то возвращает его индекс в полигоне.
		//    Считаем, что вершины полигона завёрнуты по часовой стрелке, и если полигон
		//    содержит обратное ребро [b, a], то clockwise будет false
		//================================================================================
		OFFSHORE_API int32_t containsEdge(int32_t a, int32_t b, bool *clockwise = NULL) const;
	};

	////////////////////////////////////////////
	// Ребро
	struct Edge32
	{
		unsigned long v1, v2;
		Edge32()
		{
			this->v1 = 0xFFFFFFFF;
			this->v2 = 0xFFFFFFFF;
		}
		Edge32(unsigned long v1, unsigned long v2)
		{
			if(v1>v2) std::swap(v1, v2);
			this->v1 = v1;
			this->v2 = v2;
		}
		bool isValid() const
		{
			if( this->v1 != 0xFFFFFFFF && this->v2 != 0xFFFFFFFF)
				return true;
			return false;
		}
		bool operator <(const Edge32& edge) const
		{
			if(v1<edge.v1) return true;
			if(v1>edge.v1) return false;
			return v2<edge.v2;
		}
		bool operator ==(const Edge32& edge) const
		{
			if(v1!=edge.v1) return false;
			return v2==edge.v2;
		}
		int opposite(int v) const
		{
			if( v==v1) return v2;
			if( v==v2) return v1;
			return -1;
		}
	};

	// полигон
	struct Polygon
	{
		ed::vector<int32_t> indicies;
		ed::vector<Face> faces;			// индексы вершин в полигоне
		Polygon() {};
		Polygon(int nIndicies, int nFaces) : indicies(nIndicies), faces(nFaces) {};

		OFFSHORE_API bool operator < (const Polygon& other) const;

		OFFSHORE_API bool operator == (const Polygon& other) const;

		//================================================================================
		// isValid
		//    Проверяет корректность индексов фэйсов полигона
		//================================================================================
		OFFSHORE_API bool isValid() const;

		//================================================================================
		// isSingular
		//    Вырожденный ли полигон. (Точка, линия)
		//================================================================================
		OFFSHORE_API bool isSingular() const;

		/**
		 * isSingular - вырожден ли полигон (является точкой или линией)
		 *
		 * @param vertices набор точек, на котором задан полигон
		 * @param delta абсолютная погрешность вычисления длины
		 */
		OFFSHORE_API bool isSingular(const ed::vector<osg::Vec3d>& vertices, double delta = 0.01) const;

		//================================================================================
		// createTriangle
		//================================================================================
		inline static Polygon createTriangle(int32_t i0, int32_t i1, int32_t i2, bool triangulation = true)
		{
			Polygon result(3, 0);

			result.indicies[0] = i0;
			result.indicies[1] = i1;
			result.indicies[2] = i2;

			if (triangulation)
				result.faces.push_back(Face(0, 1, 2));

			return result;
		}

		//================================================================================
		// createQuad
		//
		//   i0----i1  Создаёт полигон-квад.
		//   | \    |  С триангуляцией если выставлен флаг triangulation
		//   |  \   |
		//   |   \  |
		//   |    \ |
		//   i3----i2
		//
		//================================================================================
		inline static Polygon createQuad(int32_t i0, int32_t i1, int32_t i2, int32_t i3, bool triangulation = true)
		{
			Polygon result(4, 0);

			result.indicies[0] = i0;
			result.indicies[1] = i1;
			result.indicies[2] = i2;
			result.indicies[3] = i3;

			if (triangulation)
			{
				result.faces.push_back(Face(0, 1, 2));
				result.faces.push_back(Face(0, 2, 3));
			}

			return result;
		}

		// indicies[v] вершины (v может быть не в диапазоне 0-indicies.size()-1)
		int32_t vert(int v) const {return indicies[offshore::cycle(v, (int)indicies.size())];}

		// Сдвиг индексов вершин полигона 
		OFFSHORE_API void rotate(int _v);

		// полигон из одного тр-ка - use createTriangle instead
		OFFSHORE_API void _setSingleFace(int32_t i0, int32_t i1, int32_t i2);
		// квад - use createQuad instead
		OFFSHORE_API void _setSingleQuad(int32_t i0, int32_t i1, int32_t i2, int32_t i3);

		// ребро по индексу
		OFFSHORE_API offshore::Edge32 edge(int i) const;

		// area
		OFFSHORE_API double getArea2d(const ed::vector<osg::Vec3d>& verts) const;
		OFFSHORE_API double getArea3d(const ed::vector<osg::Vec3d>& verts) const;
		// триангуляция полигона
		OFFSHORE_API bool Triangulation(const osg::Vec3d* verts);

		// бокс треугольника
		OFFSHORE_API osg::BoundingBoxd getBoundingBox(const ed::vector<osg::Vec3d>& verts) const;
		OFFSHORE_API osg::BoundingBoxd getBoundingBox(const ed::vector<osg::Vec3d>& verts, const osg::Matrixd& matrix) const;

		//================================================================================
		// containsVertex
		//    Если полигон содержит вершину с индексом vertexIndex, то возвращает индекс
		//    этой вершины в полигоне, или -1 в противном случае.
		//================================================================================
		OFFSHORE_API int32_t containsVertex(int vertexIndex) const;

		//================================================================================
		// containsEdge
		//    Если полигон содержит ребро [a, b], то возвращает его индекс в полигоне.
		//    Считаем, что вершины полигона завёрнуты по часовой стрелке, и если полигон
		//    содержит обратное ребро [b, a], то clockwise будет false
		//================================================================================
		OFFSHORE_API int32_t containsEdge(int32_t a, int32_t b, bool *clockwise = NULL) const;

		//================================================================================
		// triangulate
		//    Триангулировать полигон. Работает только для выпуклых полигонов.
		//
		//    vertices - массив вершин, индексы которых содержит полигон
		//================================================================================
		/* DEPRECATED */OFFSHORE_API void triangulateOld(const ed::vector<osg::Vec3d>& vertices);

		/**
		 * Триангуляция полигона
		 * @param vertices массив вершин, индексы которых содержит полигон
		 * @return true если полигон успешно протриангулирован
		 */
		OFFSHORE_API bool triangulate(const ed::vector<osg::Vec3d>& vertices);

		/**
		 * Является ли проекция полигона на OXZ выпуклой
		 * @param vertices массив вершин, индексы которых содержит полигон
		 * @return true если полигон выпуклый
		 */
		OFFSHORE_API bool isConvex(const ed::vector<osg::Vec3d>& vertices) const;

		// Ортогонален ли полигон заданному вектору
		OFFSHORE_API bool isOrthogonal(const ed::vector<osg::Vec3d>& vertices, const osg::Vec3d& orth, double delta = 0.01) const;  

		//================================================================================
		// center
		//    Центр полигона.
		//
		//    vertices - массив вершин, индексы которых содержит полигон
		//    return   - центр полигона. Среднее арифметическое всех его вершин
		//================================================================================
		OFFSHORE_API osg::Vec3d center(const ed::vector<osg::Vec3d> &vertices) const;
		//================================================================================
		// normal
		//    Нормаль полигона
		//
		//    normals - массив нормалей, индексы которых содержит полигон
		//    return   - Нормаль полигона. Среднее всех его нормалей
		//================================================================================
		OFFSHORE_API osg::Vec3d normal(const ed::vector<osg::Vec3d> &normals) const;

		//================================================================================
		// isPointInside
		//    Находится ли точка внутри полигона. 2D проверка, координата y игнорируется
		//
		//    point       - точка.
		//    vertices    - массив с вершинами полигона (в самом полигоне хранятся индексы).
		//    outFace     - номер фэйса полигона в который попала точка, -1 если не попала.
		//    barycentric - барицентрические координаты точки в этом фэйсе.
		//    threshold   - погрешность. Если поставить много, то будут захватываться точки
		//                  которые на самом деле в этот полигон не попадают и наоборот.
		//================================================================================
		OFFSHORE_API bool isPointInside(
				const osg::Vec3d& point,
				const ed::vector<osg::Vec3d>& vertices,
				int& outFace, osg::Vec3d& barycentric, double threshold) const;


		/**
		 *	Возвращает расстояние от полигона до точки в 2d, если точка лежит внутри полигона то вернет 0 
		 *
		 *	@pt			- точка, от которой считаем расстояние 
		 *	@vertices	- хранилище вершин на которые ссылаются индексы полигона
		 *  @ponm(out)	- точка на меше, ближайшая до точки
		 *  @epsilon	- техническая погрешность
		 *
		 *  @return - расстояние до полигона
		 */
		OFFSHORE_API double distanceToPolygon2d(
			const osg::Vec3d& pt, 
			const ed::vector<osg::Vec3d>& verts, 
			int& outFace,
			osg::Vec3d& barycentric,
			double epsilon = 0.01) const;

	};

	// точка на сплайне
	struct PointOnSpline
	{
		// номер отрезка
		int segment;
		// параметр (0-1)
		double parameter;

		PointOnSpline(int segment=0, double parameter=0)
		{
			this->segment = segment;
			this->parameter = parameter;
		}

		bool operator <(const PointOnSpline& pons) const
		{
			if(segment<pons.segment)
				return true;
			if(segment>pons.segment)
				return false;
			return parameter<pons.parameter;
		}
	};

	// процедурный UV set
	struct Manifold
	{
		// Матрица преобразования в UV space
		osg::Matrixd xyzTOuv;
	public:
		Manifold(){}
		Manifold(const osg::Matrixd& xyzTOuv){this->xyzTOuv=xyzTOuv;}
	};

	// набор текстурных координат или какихто других данных для вертекса
	struct uvSet
	{
		ed::string name;
		int32_t stride;
		ed::vector<double> uv;
		ed::vector<Polygon> uvpolygons;

		ed::string manifoldSourceName;		// имя входного параметра для манифолда, пустая строка = mesh.verts
		ed::vector< Manifold> manifolds;

	public:

		uvSet() {}
		uvSet(const ed::string& name, int32_t stride) : name(name), stride(stride) {}
		OFFSHORE_API uvSet(const ed::string& name, const ed::vector<int32_t>& values);
		OFFSHORE_API uvSet(const ed::string& name, const ed::vector<uint32_t>& values);
		OFFSHORE_API uvSet(const ed::string& name, const ed::vector<osg::Vec3d>& values);
		OFFSHORE_API uvSet(const ed::string& name, const ed::vector<osg::Vec2d>& values);
		OFFSHORE_API uvSet(const ed::string& name, const ed::vector<double>& values);

		int size() const {return uv.size() / stride;};
		void set(const char* name, int32_t stride){this->name=name, this->stride=stride;};
		OFFSHORE_API void clear();
		void resize(uint32_t size) { uv.resize(size * stride); }
		void resize(uint32_t size, double val) { uv.resize(size * stride, val); }
		OFFSHORE_API void resize(uint32_t size, const osg::Vec2d& val);
		OFFSHORE_API void resize(uint32_t size, const osg::Vec3d& val);

		void reserve(uint32_t size) { uv.reserve(size * stride); }

		OFFSHORE_API void fill(double val);

		void push_back(const double& val){uv.push_back(val);};
		void push_back(const osg::Vec2d& val){uv.push_back(val.x());uv.push_back(val.y());};
		void push_back(const osg::Vec3d& val){uv.push_back(val.x());uv.push_back(val.y());uv.push_back(val.z());};
		void push_back(const osg::Vec4d& val){uv.push_back(val.x());uv.push_back(val.y());uv.push_back(val.z());uv.push_back(val.w());};
		OFFSHORE_API void push_backArray(const double* pval);

		OFFSHORE_API bool getUV(int32_t v, osg::Vec2d& dst) const;
		OFFSHORE_API bool getUV(int32_t v, osg::Vec3d& dst) const;
		double* getUV(int32_t v) { return &uv[stride * v]; }
		const double* getUV(int32_t v) const { return &uv[stride * v]; }

		OFFSHORE_API bool setUV(int32_t v, double src);
		OFFSHORE_API bool setUV(int32_t v, const osg::Vec2d& src);
		OFFSHORE_API bool setUV(int32_t v, const osg::Vec3d& src);
		OFFSHORE_API bool setUV(int32_t v, const osg::Vec4d& src);
		OFFSHORE_API bool setUVarray(int32_t v, double* src);

		int vertexcount() const { return (int)uv.size() / stride; }
		int polygoncount() const { return (int)uvpolygons.size(); }
		int faceverticescount() const;

		/**
		 * Инвертировать V координату
		 */
		OFFSHORE_API void invertV();

		/**
		 * Каждый элемент умножить на матрицу
		 */
		OFFSHORE_API void transform(const osg::Matrixd& m);

		// uvSet.queryVerts([&](int v, double* val)
		// uvSet.queryPolygons([&](int p, offshore::Polygon& poly)
		// uvSet.queryFaces([&](int p, int f, offshore::Face& face)
		// uvSet.queryPolygonVerts([&](int p, int pv, double* val)
		template <typename Function> void queryVerts(const Function& f);
		template <typename Function> void queryPolygons(const Function& f);
		template <typename Function> void queryFaces(const Function& f);
		template <typename Function> void queryPolygonVerts(const Function& f);

	};


	typedef ed::vector<ed::vector<double> > MeshUVSet;

	// точка на mesh
	struct PointOnMesh
	{
		// полигон
		int polygon;
		// face в полигоне
		int face;
		// Барицентрические координаты
		osg::Vec3d barycentric;

		PointOnMesh(int polygon=0, int face=0, osg::Vec3d barycentric=osg::Vec3d(1, 0, 0))
		{
			this->polygon = polygon;
			this->face = face;
			this->barycentric = barycentric;
		}
	};

	// Mesh
	struct Mesh
	{
		// индекс ноды этого меша в сцене. scene.meshes[scene.Node(mesh.treenode)->index] - будет нашим мешом
		int32_t treenode;

		int32_t material;
		ed::vector<osg::Vec3d> verts;
		ed::vector<osg::Vec3d> vertcolors;
		ed::vector<int32_t> srcvertindicies;
		ed::vector<Polygon> polygons;

		// uv
		ed::vector<uvSet> uvsets;

		// нормали и полигоны нормалей
		ed::vector<osg::Vec3d> normals;
		ed::vector<Polygon> npolygons;

		// per polygon data
		ed::vector<uint32_t> smoothgroup;

	public:
		OFFSHORE_API Mesh();

		//================================================================================
		// isValid
		//    Проверяет правильность индексов в меше
		//================================================================================
		OFFSHORE_API bool isValid(ed::vector<offshore::Error>& errors) const;

		// Проверяет соответствие полигонов ув сетов
		OFFSHORE_API bool isValidPolygons() const;

		//================================================================================
		// isNaN
		//    Возвращает true если в меше есть невалидные вертексы 
		//================================================================================
		OFFSHORE_API bool isNaN(ed::vector<offshore::Error>& errors) const;

		// один полигон
		OFFSHORE_API void setSinglePolygon(const ed::vector<osg::Vec3d>& verts);
		OFFSHORE_API void setSinglePolygon(const ed::vector<osg::Vec3d>& verts, const ed::vector<osg::Vec2d>& uvs);
		OFFSHORE_API void setSingleBox(const osg::BoundingBox& box);

		// frustum
		OFFSHORE_API void createFrustum(const osg::Matrixf& matrix);

		// вернет mesh с такимиже uvSet, только все массивы будут пусты
		OFFSHORE_API Mesh getTemplateMesh();

		// удаляет повторяющиеся полигоны
		OFFSHORE_API void removeDublicatedPolygons(ed::vector<int>* remapPolygons=nullptr);

		// удаляет вырожденные полигоны
		// delta - абсолютная погрешность вычисления длины
		OFFSHORE_API void deleteSingularPolygons(double delta, ed::vector<int>* remapPolygons=nullptr);
	public:
		int32_t vertexcount()const{return (int32_t)verts.size();}
		int32_t polygonCount()const{return (int32_t)polygons.size();};
		int32_t faceVerticesCount()const;
		
		// Количество треугольников в меше
		OFFSHORE_API int32_t triCount() const;

	public:
		// удалить отдельные вертексы
		//	  remap			- маппинг вершин в исходном меше на индексы новых вершин
		//	  invertRemap	- маппинг новых вершин на индексы вершин в исходном меше
		OFFSHORE_API int32_t removeIsolatedVerts(ed::map<int, int>* remap=nullptr, ed::map<int, int>* invertRemap=nullptr);

		// Вырожденный треугольник?
		OFFSHORE_API bool isSingularFace(int32_t v0, int32_t v1, int32_t v2);

		/**
		 * Умножить все вершины меша на matrix. Обычно используется для перевода в WorldSpace
		 */
		OFFSHORE_API void multiply(const osg::Matrixd &matrix);

		/**
		 * Получить uv-сет по имени
		 * @param name имя uv-сета
		 * @return поинтер на uv-cет либо NULL если uv-сета с таким именем нет
		 */
		OFFSHORE_API uvSet* getUvSet(const char* name);
		OFFSHORE_API uvSet* getUvSet(const ed::string& name);
		OFFSHORE_API const uvSet* getUvSet(const char* name) const;
		OFFSHORE_API const uvSet* getUvSet(const ed::string& name) const;
		OFFSHORE_API int getUvSetIndex(const ed::string& name) const;

		/**
		 * Добавить uv-cет с заданным именем и страйдом
		 * @param name имя uv-сета
		 * @param stride страйд
		 * @return поинтер на созданный uv-сет, либо NULL если такой uv-cет уже существовал, но c иным stride
		 */
		OFFSHORE_API uvSet* addUvSet(const char* name, int32_t stride);

		/**
		 * Добавить uv-сет с заданным именем и значением uv (страйд 1)
		 * @param name имя uv-сета
		 * @param value значение uv-вершины
		 * @return поинтер на созданный uv-сет, либо NULL если такой uv-cет уже существовал, но c иным stride
		 */
		OFFSHORE_API uvSet* addFilledUvSet(const char* name, double value);
		OFFSHORE_API uvSet* addFilledUvSet(const char* name, const osg::Vec2d& value);
		OFFSHORE_API uvSet* addFilledUvSet(const char* name, const osg::Vec3d& value);

		OFFSHORE_API uvSet* addUvSet(const ed::string& name, const ed::vector<int32_t>& values);
		OFFSHORE_API uvSet* addUvSet(const ed::string& name, const ed::vector<uint32_t>& values);
		OFFSHORE_API uvSet* addUvSet(const ed::string& name, const ed::vector<osg::Vec3d>& values);
		OFFSHORE_API uvSet* addUvSet(const ed::string& name, const ed::vector<osg::Vec2d>& values);
		OFFSHORE_API uvSet* addUvSet(const ed::string& name, const ed::vector<double>& values);

		/**
		 * Добавить uv-сет с заданным именем и значением uv (страйд 4)
		 * @param name имя uv-сета
		 * @param value значение uv-вершины
		 * @return поинтер на созданный uv-сет, либо NULL если такой uv-cет уже существовал, но c иным stride
		 */
		OFFSHORE_API uvSet* addFilledUvSet(const char* name, const osg::Vec4d& value);

		// remove uv set by name
		OFFSHORE_API void removeUvSet(const char* name);

		/**
		* Копировать мапинг со всех текстурных каналов другой мешки
		* Путем вертикального проецирования
		* transform переводит вертексы из пространства srcmesh в this
		*/
		OFFSHORE_API void copyMappingFrom(offshore::Mesh& srcmesh, const osg::Matrixd& transform);
		/**
		 * Копировать мапинг с текстурного канала другой мешки
		 * Путем вертикального проецирования
		 * transform переводит вертексы из пространства srcmesh в this
		 */
		OFFSHORE_API void copyMappingFrom(offshore::Mesh& srcmesh, offshore::uvSet& srcuvset, offshore::uvSet& dstuvset, const osg::Matrixd& transform);

		// применить маниволды: посчитать UV
		OFFSHORE_API void applyManifolds(int stride);

		/**
		 * Проверяет, совместимы ли "виды" мешей, то есть можно ли их объединить в один
		 * @param mesh меш с которым проверям на совместимость
		 * @param fields что проверять. "P" - вершины и полигоны, "N" - нормали и нполигоны, 
		 * "S" - смусгруппы + имена uv-сетов. Пустой массив если надо проверить все
		 * @return совместимы ли меши
		 */
		OFFSHORE_API bool match(const offshore::Mesh& mesh, const ed::vector<ed::string>& fields = ed::vector<ed::string>()) const;

		/**
		 * Добавляет меш meshToAdd к исходному. Предварительно проверяет совместимы ли меши
		 * @param mesh меш который добавляем.
		 * @param fields что добавлять. "P" - вершины и полигоны, "N" - нормали и нполигоны, 
		 * "S" - смусгруппы + имена uv-сетов. Пустой массив если надо добавить все
		 * @return false если меши не совместимы, true иначе
		 */
		OFFSHORE_API bool append(const Mesh &meshToAdd, const ed::vector<ed::string>& fields = ed::vector<ed::string>(), ed::vector<int>* addedVerts=nullptr, ed::vector<int>* addedPolys=nullptr);

		/**
		 * Добавить к мешу много мешей. Предварительно проверяет совместимы ли меши
		 * @param scene сцена с мешами.
		 * @param meshNodeIndicies индексы нод мешей в сцене.
		 * @param fields что добавлять. "P" - вершины и полигоны, "N" - нормали и нполигоны, 
		 * "S" - смусгруппы + имена uv-сетов. Пустой массив если надо добавить все
		 * @return false если меши не совместимы, true иначе
		 */
		OFFSHORE_API bool appendMany(offshore::Scene& scene, ed::vector<int32_t>& meshNodeIndicies, const ed::vector<ed::string>& fields = ed::vector<ed::string>());

		/**
		 * Возвращает true если меш пустой (нет вершин, полигонов, увсетов и т.п.)
		 */
		OFFSHORE_API bool isEmpty();

		/**
		 * Очистить меш
		 */
		OFFSHORE_API void clear();
		OFFSHORE_API void clearAndFreeMemory();
		OFFSHORE_API void swap(offshore::Mesh& other);

		// ????
		OFFSHORE_API void CalculateBariCoord(const ed::vector<int> &polygonsFlag,
			const ed::vector<osg::Vec3d> &verts,
			MeshUVSet &res);
		OFFSHORE_API void CalculateBariCoord(const ed::vector<osg::Vec3d> &verts,
			MeshUVSet &res);

		// построить список ребер
		// ed::list<int> это индексы полигонов которым принадлежит ребро
		OFFSHORE_API void BuildEdgeList(
			ed::map<Edge32, ed::list<int> >& edges,
			const ed::vector<uint32_t>* selectedpolygons=NULL
			)const;
		// 
		// Получить все "открытые" рёбра меша
		OFFSHORE_API ed::vector<offshore::Edge32> getOpenEdges() const;
		//================================================================================
		// buildEdgeList
		//    Всё рёбра меша и индексы полигонов в которых они лежат
		//================================================================================
		OFFSHORE_API void buildEdgeList(ed::map<offshore::Edge32, ed::vector<int32_t> >& edges) const;
		OFFSHORE_API void buildEdgeList(ed::vector<std::pair<offshore::Edge32, ed::vector<int32_t> > >& edges) const;

		//================================================================================
		// buildBordersList
		//    Собрать все открытые ребра меша в ломаные
		//================================================================================
		OFFSHORE_API void buildBordersList(ed::list<ed::vector<int32_t> > &borders, const ed::vector<uint32_t>* selectedpolygons = NULL) const;
		// Старая версия, которая может делать восьмерки
		OFFSHORE_API void buildBordersListOld(ed::list<ed::vector<int32_t> > &borders, const ed::vector<uint32_t>* selectedpolygons=NULL) const;

		// Список изолированых вертексов
		OFFSHORE_API void buildIsolatedVerts(ed::vector<int32_t>& verts) const;

		//================================================================================
		// getPolygonsOfEdge
		//    Вернет все полигоны в которые входит ребро
		//================================================================================
		OFFSHORE_API ed::vector<uint32_t> getPolygonsOfEdge(const Edge32& edge);

		struct PolygonsOfEdgeResult
		{
			int32_t polygon;		// индекс полигона
			int32_t edgeIndex;		// ребро полигона
			bool clockwise;			// отношение к запрошеному ребру
		};
		OFFSHORE_API void getPolygonsOfEdge(const Edge32& edge, ed::vector<PolygonsOfEdgeResult>& result);

		// Список связности полигонов
		// ed::list<int32_t> - индексы полигонов соседей
		OFFSHORE_API void buildPolygonNeighbourList(ed::vector< ed::list<int32_t> > &neighbours) const;

		/**
		 * Построить список соседних полигонов для каждого полигона
		 * Соседними считаются полигоны, у которых хотя бы одна общая точка
		 */
		OFFSHORE_API bool buildPolygonNeighbourListByVertices(ed::vector<ed::list<int32_t> >& neighbours);

		//================================================================================
		// buildNormals
		//    Строит нормали к мешу
		//================================================================================
		OFFSHORE_API void buildNormals();

		//================================================================================
		// calcManifoldForUvSet
		//    Вычисляет манифолд по текстурным координатам
		//================================================================================
		OFFSHORE_API offshore::Manifold calcManifoldForUvSet(const offshore::uvSet& uvset, double& error);

		// Перестраивает увсет на использование манифолдов
		OFFSHORE_API void buildManifoldsForUvSet(offshore::uvSet& uvset);

		//================================================================================
		// createSphere
		//    Строит сферу по радиусу и центру
		//================================================================================
		OFFSHORE_API offshore::Mesh createSphere(const osg::Vec3d& center, double radius);

		//================================================================================
		// createBox
		//    Строит прямоугольный параллелепипед
		//================================================================================
		OFFSHORE_API void createBox(const osg::Vec3d& center = osg::Vec3d(), double sizeX=1, double sizeY=1, double sizeZ=1);
		OFFSHORE_API void createBox(const osg::BoundingBox& box);

		//================================================================================
		// isPointInside
		//    Находится ли точка внутри меша. Проверяется в плоскости OXZ. y игнорируется!
		//
		//    point      - точка
		//    outPolygon - номер полигона, в котором находится точка
		//    outFace    - номер треугольника в данном полигоне, в который попала точка
		//    bariCoords - барицентрические координаты точки в этом треугольнике
		//    return     - true, если точка находится внутри меша.
		//================================================================================
		OFFSHORE_API bool isPointInside(const osg::Vec3d &point, int *outPolygon = NULL, int *outFace = NULL, osg::Vec3d *bariCoords = NULL, ed::vector<uint32_t>* selectedpolygons=NULL) const;

		bool isPointInside(const osg::Vec3d &point, int &outPolygon, int &outFace, osg::Vec3d &bariCoords) const
		{
			return this->isPointInside(point, &outPolygon, &outFace, &bariCoords);
		}
		bool isPointInside(const osg::Vec3d &point, offshore::PointOnMesh& pointOnMesh) const
		{
			return this->isPointInside(point, &pointOnMesh.polygon, &pointOnMesh.face, &pointOnMesh.barycentric);
		}
		
		//================================================================================
		// getVertexTriangles
		//    Получить все треугольники которым принадлежит вертекс
		//================================================================================
		struct VertexTriangle
		{
			int polygon;
			int vertindexInPolygon;
			int face;
			int vertindexInFace;
		};

		OFFSHORE_API void getVertexTriangles(int v, ed::list<VertexTriangle>& res);

		//================================================================================
		// getBoundingBox
		//    Получить BoundingBox меша
		//================================================================================
		OFFSHORE_API osg::BoundingBox getBoundingBox() const;

		//================================================================================
		// getBoundingBox
		//    Получить BoundingBox меша в заданном пространстве
		//================================================================================
		OFFSHORE_API osg::BoundingBox getBoundingBox(const osg::Matrixd &matrix) const;

		//================================================================================
		// getRect
		//    Получить Rect меша
		//================================================================================
		OFFSHORE_API osg::Rect getRect() const;

		//================================================================================
		// getRect
		//    Получить Rect меша в заданном пространстве
		//================================================================================
		OFFSHORE_API osg::Rect getRect(const osg::Matrixd &matrix) const;

		//================================================================================
		// center
		//    Центр меша - среднее арифметическое всех его вертексов
		//================================================================================
		OFFSHORE_API osg::Vec3d center() const;

		/**
		 * Получить центр меша в заданном пространстве
		 * @param matrix матрица перехода в заданное пространство
		 */
		OFFSHORE_API osg::Vec3d center(const osg::Matrixd& matrix) const;

		//================================================================================
		// extractPolygons
		//    Сделать новый меш из отдельных полигонов текущего меша
		//
		//    polygonIndicies     - индексы полигонов из которых надо собрать новый меш
		//    outMesh             - меш в который запишется результат, все текущие его данные
		//                          будут утеряны
		//    extractVertices     - выгружать в новый меш вершины исходного
		//    extractNormals      - выгружать в новый меш нормали исходного
		//    extractUV           - выгружать в новый меш uv исходного
		//    extractSmoothGroups - выгружать в новый меш группы сглаживания исходного
		//	  vertexRemap		  - маппинг новых вершин на индексы вершин в исходном меше
		//	  invertVertexRemap	  - маппинг вершин в исходном меше на индексы новых вершин
		//    return              - новый меш
		//================================================================================
		OFFSHORE_API void extractPolygons(const ed::vector<int32_t> &polygonIndicies, offshore::Mesh &outMesh,
			bool extractVertices = true, bool extractNormals = true, bool extractUV = true, bool extractSmoothGroups = true,
			ed::vector<int32_t>* vertexRemap=nullptr, ed::vector<int32_t>* invertVertexRemap=nullptr) const;

		/** 
		 * Извлекает изолированные меши, меши считаются изолированными, если они не имеют общих ребер, но могут иметь общие
		 * вершины
		 * @isolatedMeshes(out) - результирующие меши
		 * @isolatorPolys(opt)  - индексы полигонов изоляторов, при их наличии изолированные меши будут определяться так
		 *						как будто этих полигонов в исходной мешке нет.
		 */
		OFFSHORE_API void extractIsolatedMeshes(ed::vector<offshore::Mesh>& isolatedMeshes, ed::set<int>* isolatorPolys = nullptr);

		/** 
		 * Извлекает индексы полигонов изолированных мешей, меши считаются изолированными, если они не имеют общих ребер,
		 * но могут иметь общие вершины
		 * @isolatedParts(out) - индексы полигонов изолированных мешей
		 * @isolatorPolys(opt) - индексы полигонов изоляторов, при их наличии изолированные меши будут определяться так
		 *						 как будто этих полигонов в исходной мешке нет.
		 */
		OFFSHORE_API void extractIsolatedParts(ed::vector<ed::vector<int> >& isolatedParts, ed::set<int>* isolatorPolys = nullptr) const;


		/**
		 * Возвращает полигоны, полученные заливкой от указанных начальных полигонов
		 * @filledPolys(in-out) - залитые полигоны
		 * @edges - ребра в меше
		 * @shouldFillPolygon - заливать ли i-ый полигон. Если не указано, то заливаться будут все возможные к заливке полигоны
		                        [&](int p, int prevp)->bool{}  p - запрашиваемый полигон, pprev - от какого переход
		  */
		OFFSHORE_API void floodFillPolygons(
			ed::vector<int>& filledPolys,
			const ed::map<offshore::Edge32, ed::vector<int32_t> >& edges,
			std::function<bool(int, int)>* shouldFillPolygon = nullptr) const;

		OFFSHORE_API bool _append(const Mesh &added);

		// Добавляет один вертекс в меш. Для нормализованых мешей (когда число вертексов и текстурных вертексов однаково)
		// Без проверок
		OFFSHORE_API int appendVertex(offshore::Mesh& srcmesh, int srcv);

		//================================================================================
		// getUvSetNumberByName
		//    получить индекс uvSet'а в массиве по его имени
		//================================================================================
		OFFSHORE_API int32_t getUvSetNumberByName(const char *name) const;

		//================================================================================
		// triangulateAllPoly
		//    триангулировать все полигоны (для отладки, чтоб в максе видеть)
		//================================================================================
		OFFSHORE_API void triangulateAllPoly();

		//================================================================================
		// расчет нормали полигона и треугольника
		//================================================================================
		OFFSHORE_API osg::Vec3d calcPolygonNormal(offshore::Polygon& p);
		OFFSHORE_API osg::Vec3d calcFaceNormal(offshore::Face& f);

		//================================================================================
		// Центральная точка полигона и треугольника
		//================================================================================
		OFFSHORE_API osg::Vec3d calcPolygonCentroid(offshore::Polygon& p);
		OFFSHORE_API osg::Vec3d calcFaceCentroid(offshore::Face& f);

		//================================================================================
		// найти полигоны в которых участвует вертекс
		//================================================================================
		OFFSHORE_API void findPolygonsOfVertex(int vert, ed::vector<int>& polygons);

		/**
		 * Построить список полигонов для каждого вертекса меша
		 */
		OFFSHORE_API void buildPolygonsOfVerticesList(ed::vector<ed::list<int32_t> >& list) const;

		// сортировать список полигонов вертекса CCW
		OFFSHORE_API bool sortPolygonsOfVert(int v, ed::list<int32_t>& list, ed::list<int>* edges) const;

		//Построить tangent space по uvset
		//T - массив векторов касательной для каждой точки из uvset, выходной параметр
		//B - массив бинормалей для каждой точки из uvset, выходной параметр
		OFFSHORE_API void buildTangentSpace(const uvSet& uvset, ed::vector<osg::Vec3d>& T, ed::vector<osg::Vec3d>& B);

		// Построить tangent space по uvset для единственного фейса
		// (in)  polyIndex - индекс полигона в меше
		// (in)  faceIndex - индекс фейса в полигоне
		// (in)  uvset - uvset, по которому строим тангент спэйс
		// (out) T - искомый тангент
		// (out) B - искомая бинормаль		
		// (out) uvOrigin - позиция точки в пространстве меша соответствующая нулю uv(0, 0);
		OFFSHORE_API void buildTangentSpace(int32_t polyIndex, int32_t faceIndex, const uvSet& uvset, osg::Vec3d& T, osg::Vec3d& B, osg::Vec3d& uvOrigin) const;

		// Собрать все треугольники в один список
		OFFSHORE_API void assembleFaces(ed::vector<offshore::Face>& faces) const;
		
		/**
		 * Найти точки пересения меша с отрезком
		 * @param a первая точка отрезка
		 * @param b вторая точка отрезка
		 * @param points точки пересечения
		 * @param threshold погрешность вычисления
		 * @param returns true если есть точки пересечения
		 */
		OFFSHORE_API bool segIntersection(const osg::Vec3d& a, const osg::Vec3d& b, ed::vector<osg::Vec3d>& points, double threshold = 0.001);
		OFFSHORE_API bool segIntersection(const osg::Vec3d& a, const osg::Vec3d& b, ed::vector<offshore::PointOnMesh>& points, double threshold = 0.001);

		/**
		 * Получить 2D площадь меша. В плоскости OXZ
		 */
		OFFSHORE_API double getArea2d() const;

		// Получить площадь полигона меша
		OFFSHORE_API double getPolyArea2d(int32_t polyIndex) const;
		OFFSHORE_API double getPolyArea3d(int32_t polyIndex) const;

		// данные по барицентрическим координатам
		OFFSHORE_API osg::Vec3d getPoint(const offshore::PointOnMesh& ponm) const;
		OFFSHORE_API osg::Vec3d getNormal(const offshore::PointOnMesh& ponm) const;
		OFFSHORE_API double* getUV(const offshore::PointOnMesh& ponm, const offshore::uvSet* uvset, double* buffer) const;

		/**
		 * Сохранить меш в файл. Сохраняет сцену с одним этим мешом
		 */
		OFFSHORE_API bool save(const char* filename, const char* nodename="");
		OFFSHORE_API bool save(const ed::string& filename, const ed::string& nodeName="");

		/**
		 * Выкинуть из меша полигоны по маске
		 * mask[i] - выкидывать ли i-ый полигон
		 * версия принимающая вектор uint8_t вместо bool нужна, т.к. вектор bool не потокобезопасен

		 remap: новые полигоны -> старые полигоны
		 */
		OFFSHORE_API void filter(const ed::vector<bool>& mask, ed::vector<int>* remap=nullptr);
		OFFSHORE_API void filter(const ed::vector<uint8_t>& mask, ed::vector<int>* remap=nullptr);
		OFFSHORE_API void filter(const ed::set<int>& polygons, ed::vector<int>* remap=nullptr);

		OFFSHORE_API void filterVerts(const ed::vector<uint8_t>& mask, ed::vector<int>* remap=nullptr, ed::vector<int>* invertRemap=nullptr);

		// Перевод матрицы из пространства World в Пространство текстурных координат
		OFFSHORE_API bool toUVSpace(const osg::Matrixd& transformW, const offshore::uvSet& uvSet, osg::Matrixd& transformUV) const;

		// Перевод матрицы из пространства текстурных координат в World
		// ponm - PointOnMesh для точки
		OFFSHORE_API bool fromUVSpace(const osg::Matrixd& transformUV, const offshore::uvSet& uvSet, osg::Matrixd& transformW, offshore::PointOnMesh* ponm=0) const;

		// Склеить вертексы и убрать лишние полигоны
		OFFSHORE_API void weld(double weld3d, double weld2d = 0.0);

		// фиксирует граничные точки, склеить вертексы и убрать лишние полигоны
		OFFSHORE_API void weldExcludeBorders( double weld3d, double weld2d /*= 0.0*/, ed::map<int, int>* pold_new_verts=nullptr);

		// Только склеить вертексы
		OFFSHORE_API void weldOnly(double weld3d, double weld2d = 0.0);

		// linear subdivision
		OFFSHORE_API void subdivLinear();
		OFFSHORE_API void subdivLinear(size_t stepsCount);

		// mesh.queryVerts([&](int v, osg::Vec3d& pt)
		template <typename Function> void queryVerts(const Function& f);
		// mesh.queryPolygons([&](int p, offshore::Polygon& poly)
		template <typename Function> void queryPolygons(const Function& f);
		// mesh.queryPolygons([&](int p, const offshore::Polygon& poly)
		template <typename Function> void queryPolygons(const Function& f) const;
		// mesh.queryFaces([&](int p, int f, offshore::Face& face)
		template <typename Function> void queryFaces(const Function& f);
		// mesh.queryPolygonVerts([&](int p, int pv, osg::Vec3d& pt)
		template <typename Function> void queryPolygonVerts(const Function& f);
		// mesh.queryPolygonEdges([&](int p, int pv1, int pv2, osg::Vec3d& pt1, osg::Vec3d& pt2)
		template <typename Function> void queryPolygonEdges(const Function& f);
	};

	// Mesh с привязкой к костям
	struct SkinMesh : public Mesh
	{
		struct Joint
		{
			// имя кости
			ed::string jointname;
			osg::Matrixd pos;
			osg::Matrixd invbindpos;
			// веса вертексов
			ed::vector<float> vertsweights;
		};
		// кости
		ed::vector<Joint> joints;
	};

	// Spline
	struct Spline
	{
		OFFSHORE_API Spline();

		int32_t treenode;
		int32_t material;
		ed::vector<osg::Vec3d> verts;
		// тангенты для точек (необязательные параметры)
		ed::vector<osg::Vec3d> tangents_forward;
		ed::vector<osg::Vec3d> tangents_backward;
	public:
		int32_t vertexcount()const{return (int32_t)verts.size();}
		int32_t segmentscount()const{return std::max((int32_t)verts.size()-1, 0);}

		bool valid() const;

		// построить сплайн по точкам меша
		OFFSHORE_API void createFromMeshVerts(const Mesh& mesh, const ed::vector<int32_t>& border);

		// построить stripe
		OFFSHORE_API void makeStripe(
			double widht,	// ширина
			double u_min, double u_max, // вилка по u
			double v_tile_lenght,	// длина текстуры (метров)
			ed::vector<osg::Vec3d>& stripe_verts,
			ed::vector<osg::Vec2d>& stripe_uv,
			ed::vector<int>& stripe_quads,
			ed::vector<int>& stripe_quads_uv
			) const;

		//================================================================================
		// multiply
		//    Умножаем все вершины сплайна на matrix. Обычно используется для перевода
		//    в WorldSpace
		//================================================================================
		OFFSHORE_API void multiply(const osg::Matrixd &matrix);

		//================================================================================
		// tangent
		//    Касательная для сегмента
		//================================================================================
		OFFSHORE_API osg::Vec3d tangent(int seg) const ;
		OFFSHORE_API osg::Vec3d tangent(int seg, const osg::Matrixd& transform) const;

		//================================================================================
		// tangent
		//    Касательная для вертекса
		//================================================================================
		OFFSHORE_API osg::Vec3d tangentForVert(int v) const;
		OFFSHORE_API osg::Vec3d normalForVert(int v) const;

		//================================================================================
		// getBoundingBox
		//    Получить BoundingBox сплайна
		//================================================================================
		OFFSHORE_API osg::BoundingBox getBoundingBox() const;

		//================================================================================
		// getBoundingBox
		//    Получить BoundingBox сплайна в заданном пространстве
		//================================================================================
		OFFSHORE_API osg::BoundingBox getBoundingBox(const osg::Matrixd &matrix) const;

		//================================================================================
		// getRect
		//    Получить Rect сплайна
		//================================================================================
		OFFSHORE_API osg::Rect getRect() const;

		//================================================================================
		// getRect
		//    Получить Rect сплайна в заданном пространстве
		//================================================================================
		OFFSHORE_API osg::Rect getRect(const osg::Matrixd &matrix) const;

		//================================================================================
		// getLength
		//    Получить Length сплайна
		//================================================================================
		OFFSHORE_API double getLength2d() const;

		OFFSHORE_API double getLength() const;

		// возвращает длины от начала сплайна до каждой вершины
		OFFSHORE_API ed::vector<double> getLengthForAllVerts() const;

		// Dump
		OFFSHORE_API void Dump(FILE* );

		// удалить вырожденые сегменты
		OFFSHORE_API int32_t removeSingularSegments();

		// start Point On Spline
		PointOnSpline startPointOnSpline()const{return PointOnSpline(0, 0);};
		// end Point On Spline
		PointOnSpline endPointOnSpline()const{return PointOnSpline(vertexcount()-2, 1);};

		//Найти ближайшую к point точку на сплайне
		//maxDistance - если расстояние от точки до сплайна превышает maxDistance, то точку не ищем, segment вернет -1
		//segment - номер сегмента, которому принадлежит точка, начинается с нуля и равен номеру начальной точки сегмента, out
		//param - параметр точки на сегменте [0; 1], out
		//distance - расстояние до ближайшей точки, out
		OFFSHORE_API osg::Vec3d findClosestPoint(const osg::Vec3d& point, int& segment, double& param, double& distance) const;
					 osg::Vec3d findClosestPoint(const osg::Vec3d& point, PointOnSpline& pointOnSpline, double& distance) const;
		//Найти ближайшую к point точку на сплайне, расстояние считает по плоскости
		OFFSHORE_API osg::Vec3d findClosestPoint2d(const osg::Vec3d& point, int& segment, double& param, double& distance, PointOnSpline* start=NULL, PointOnSpline* end=NULL) const;
		             osg::Vec3d findClosestPoint2d(const osg::Vec3d& point, PointOnSpline& pointOnSpline, double& distance, PointOnSpline* start=NULL, PointOnSpline* end=NULL) const;

		// Вырезать из сплайна кусок
		OFFSHORE_API void cut(const PointOnSpline& start, const PointOnSpline& end, offshore::Spline& result) const;
		
		//КОШМАР: Разрезать сплайн на 2
		//front - кусок сплайна от начала до точки разреза
		//back - кусок сплайна от точки разреза до конца
		//segment - номер сегмента
		//point - точка, которую добавляем
		//delta - минимальная длина, после которой добавляем точку
		OFFSHORE_API void cut(offshore::Spline& front, offshore::Spline& back, int segment, const osg::Vec3d& point, double delta = 0.1) const;
		
		/**
		 * Разрезать сплайн на два сплайна
		 * @param front часть сплайна до разреза
		 * @param back часть сплайна после разреза
		 * @param length длина части сплайна до разреза
		 * @param delta минимальная длина, после которой добавляем точку
		 * @return true, если сплайн успешно разрезан
		 */
		OFFSHORE_API bool cut(offshore::Spline& front, offshore::Spline& back, double length, double delta = 0.1) const;

		// Разворачивает сплайн, а точнее индексы его вершин
		OFFSHORE_API void reverse();

		// точка на сплайне
		OFFSHORE_API osg::Vec3d getPoint(const PointOnSpline& pointOnSpline) const;
		// тангент точки на сплайне
		OFFSHORE_API osg::Vec3d getTangent(const PointOnSpline& pointOnSpline) const;
		// нормаль точки на сплайне
		OFFSHORE_API osg::Vec3d getNormal(const PointOnSpline& pointOnSpline) const;
		
		// Сдвинуть точку на сплайне
		OFFSHORE_API osg::Vec3d movePointOnSpline(PointOnSpline& pointOnSpline, double lenght) const;
		OFFSHORE_API bool movePointOnSpline2d(PointOnSpline& pointOnSpline, double lenght) const;

		// Применение subdivision
		OFFSHORE_API void subdiv(int steps, offshore::Spline& dst) const;

		/**
		 * Получить ребра сплайна, которые совпадают с ребрами другого сплайна
		 * @otherSpline  - сплайн с которым мы сравниваем этот в поиске общих ребер
		 * @commonEdges(out) - результат в виде:
		 *		индекс общего ребра в этом сплайне => 
		 *		(индекс общего ребра в другом сплайне , направленны ли тангенты совпадающих ребер в одну сторону)
		 * @epsilon - техническая погрешность
		 * @return true, если общие ребра есть
		 */
		OFFSHORE_API bool getCommonEdges(
			const offshore::Spline& otherSpline, 
			ed::map<int32_t, std::pair<int32_t,bool> >& commonEdges,
			double epsilon = 0.01) const;

		// mesh.queryVerts([&](int v, osg::Vec3d& pt)
		template <typename Function> void queryVerts(const Function& f);
		// mesh.querySegments([&](int v, osg::Vec3d& pt1, osg::Vec3d& pt2)
		template <typename Function> void querySegments(const Function& f);
	};

	// Points
	struct Points
	{
		int32_t treenode;
		int32_t material;
		ed::vector<osg::Vec3d> verts;

		int32_t vertexcount() const { return (int32_t)verts.size(); }

		//================================================================================
		// multiply
		//    Умножаем все вертексы на matrix.
		//================================================================================
		OFFSHORE_API void multiply(const osg::Matrixd &matrix);
	};

	struct Error
	{
		ed::string text;
		osg::Vec3d position;
		uint32_t color;
		float size;

		Error() { color = 0xFF; size = 600; }

		//================================================================================
		// Error
		//  Метка ошибки
		//  text	- текст ошибки, пожалуйста используйте шаблон "ТИП_ОШИБКИ:ТЕКСТ_ОШИБКИ"
		//  color	- цвет метки, 0хFF - по умолчанию красный (ошибка)
		//						  0хFFFF - желтый (Предупреждение)
		//	size	- размер метки(треугольника)
		//================================================================================
		Error(const char* text, const osg::Vec3d& position = osg::Vec3d(), float size = 600)
		{
			this->text = "Error: " + ed::string(text);
			this->position = position;
			this->color = 0xFF;
			this->size = size;
		}

		//================================================================================
		// Error
		//  Метка ошибки
		//  text	- текст ошибки, пожалуйста используйте шаблон "ТИП_ОШИБКИ:ТЕКСТ_ОШИБКИ"
		//  color	- цвет метки, 0хFF - по умолчанию красный (ошибка)
		//						  0хFFFF - желтый (Предупреждение)
		//	size	- размер метки(треугольника)
		//================================================================================
		Error(const ed::string& text, const osg::Vec3d& position = osg::Vec3d(), float size = 600)
		{
			this->text = "Error: " + text;
			this->position = position;
			this->color = 0xFF;
			this->size = size;
		}

		osg::Vec3f getColor()
		{
			return osg::Vec3f((color & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f, ((color >> 16) & 0xFF) / 255.0f);
		}
	};

	enum enNotificationType
	{
		OFFSHORE_ERROR   = 0,
		OFFSHORE_WARNING = 1,
	};

	struct Notification
	{
		ed::string text;
		osg::Vec3d position;
		float size;
		int32_t type;

		Notification()
		{
			this->type     = OFFSHORE_ERROR;
			this->text     = "";
			this->position = osg::Vec3d(0.0, 0.0, 0.0);
			this->size     = 0.0;
		}

		Notification(int type, const ed::string& text, const osg::Vec3d& position = osg::Vec3d(), float size = 600)
		{
			this->type     = type;
			this->text     = text;
			this->position = position;
			this->size     = size;
		}

		Notification(const offshore::Error& error)
		{
			this->type     = OFFSHORE_ERROR;
			this->text     = error.text;
			this->position = error.position;
			this->size     = error.size;
		}
	};

	struct Warning : public Notification
	{
		Warning(const ed::string& text, const osg::Vec3d& position = osg::Vec3d(), float size = 600) : Notification(OFFSHORE_WARNING, text, position, size) {}
	};

	inline void notificationsToErrors(const ed::vector<Notification>& notifications, ed::vector<Error>& errors)
	{
		for (size_t i = 0; i < notifications.size(); i++)
		{
			if (notifications[i].type == offshore::OFFSHORE_ERROR)
				errors.push_back(offshore::Error(notifications[i].text, notifications[i].position));
		}
	}

	inline void errorsToNotifications(const ed::vector<Error>& errors, ed::vector<Notification>& notifications)
	{
		for (size_t i = 0; i < errors.size(); i++)
		{
			notifications.push_back(errors[i]);
		}
	}

	// материал
	struct Material
	{
		ed::string materialId;

		// standart
		ed::string semantic;
		ed::string edmaterial;

		// параметры материала
		ed::map<ed::string, ed::string> params;

		uint64_t pMaxMat;				// StdMat в 3dsMax

		Material(): pMaxMat(0) {}
		Material(const ed::string& edmaterial, const ed::string& semantic, const ed::map<ed::string, ed::string> params = ed::map<ed::string, ed::string>()) :
			edmaterial(edmaterial), semantic(semantic), params(params) {}

		OFFSHORE_API ed::string getParam(const ed::string& name);
		OFFSHORE_API void dump(FILE* file);
	};

	// интерфейс для запросов к сцене
	struct ISceneQuerySink
	{
		// для Scene::Query
		virtual void Node(Scene& scene, int32_t nodeIndex, osg::Matrixd& parentTransform) {};

		// трансформ
		virtual void Transform(Scene& scene, int32_t nodeIndex, osg::Matrixd& transform) {};

		// вниз по иерархии
		virtual void NodeIn(Scene& scene, int32_t nodeIndex, osg::Matrixd& transform) {};
		// вверх по иерархии
		virtual void NodeOut(Scene& scene, int32_t nodeIndex, osg::Matrixd& transform) {};
	};

	// анимационная кривая
	struct AnimationCurve
	{
		double startTime, endTime;
		ed::vector<double> keys;
	};

	// 
	struct AnimationCurveMatrixd
	{
		ed::vector<osg::Matrixd> keys;
	};

	// анимационные клипы
	// содержат набор анимированых параметров, для одной или нескольких нод
	struct AnimationClip
	{
		ed::string clipname;
		double duration;

		struct AnimationParams
		{
			// имя параметра -> анимационная кривая
			ed::map<ed::string, AnimationCurve> params;
			// имя трансформа -> анимационная кривая
		};

		// id TreeNode -> список анимированых параметров
		ed::map<int32_t, AnimationParams> nodes;
	};

	// сцена
	struct Scene
	{
		// mitka 17/04/2009
		ed::map< ed::string, ed::string> params;	// параметры сцены (например wirecolor)
		// дерево объектов
		int32_t rootnode;

		ed::vector<TreeNode> allnodes;
		ed::vector<Transform> transforms;

		// сами объекты
		ed::vector<Mesh> meshes;
		ed::vector<Spline> splines;
		ed::vector<Points> points;
		ed::vector<SkinMesh> skinmeshes;

		// отдельно материалы
		ed::vector<Material> materials;

		// отдельно ошибки
		/* deprecated */ed::vector<Error> errors;
		ed::vector<Notification> notifications;

		ed::vector<Instance> instances;

		ed::vector<Light> lights;

		ed::vector<Camera> cameras;
		int32_t activeViewport;

		// анимационные клипы
		// содержат набор анимированых параметров, для одной или нескольких нод
		ed::vector<AnimationClip> clips;

	public:
		OFFSHORE_API Scene();
		OFFSHORE_API void Clear();
		OFFSHORE_API void clearAndFreeMemory();
		OFFSHORE_API void swap(offshore::Scene& other);

		/**
		 * Сбрасывает все данные нод относящиеся к иерархии 3dmax
		 */
		OFFSHORE_API void clear3dsMaxData();
		/**
		 * Сохранить сцену в файл
		 */
		OFFSHORE_API bool save(const char* filename);
		OFFSHORE_API bool save(const ed::string& filename);

		OFFSHORE_API bool SaveNCheck(const char* filename);
		OFFSHORE_API bool SaveNCheck(const ed::string& filename);

		// Рекурсивно помечает ноды типом NONE, у которых есть дети, и они все NONE
		OFFSHORE_API void setNoneIrrelevantNodes(int nodeIndex);

		// Почистить сцену, удалить NONE ноды и связанную с ними информацию
		OFFSHORE_API void cleanUp();

		/**
		 * Загрузить сцену из файла
		 */
		OFFSHORE_API bool load(const char* filename);
		OFFSHORE_API bool load(const ed::string& filename);

		OFFSHORE_API bool LoadNCheck(const char* filename);
		OFFSHORE_API bool LoadNCheck(const ed::string& filename);

		OFFSHORE_API void Update();

		/**
		 * Добавить в сцену корневую ноду, индекс которой будет лежать в scene.rootnode
		 */
		OFFSHORE_API int addRootNode();

		/**
		 * Добавить в сцену-трансформ ноду
		 * @param name имя ноды
		 * @param type тип ноды - "kTransform"
		 * @param parentIndex индекс ноды родителя
		 * @param transform matrix трансформации
		 */
		OFFSHORE_API int addTransformNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const osg::Matrixd& matrix);

		/**
		 * Добавить в сцену сплайн
		 * @param name имя сплайна
		 * @param type тип ноды - "kSpline"
		 * @param parentIndex индекс ноды родителя
		 * @param value сплайн который добавляем
		 */
		OFFSHORE_API Spline& addSplineNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const Spline& value = Spline(), const ed::map<ed::string, ed::string>& params = ed::map<ed::string, ed::string>());

		/**
		 * Добавить в сцену меш
		 * @param name имя меша
		 * @param type тип ноды - "kMesh"
		 * @param parentIndex индекс ноды родителя
		 * @param value меш который добавляем
		 */
		OFFSHORE_API Mesh& addMeshNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const Mesh& value = Mesh(), const ed::map<ed::string, ed::string>& params = ed::map<ed::string, ed::string>());

		/**
		 * Добавить в сцену меш с костями
		 * @param name имя меша
		 * @param type тип ноды - "kSkinMesh"
		 * @param parentIndex индекс ноды родителя
		 * @param value меш который добавляем
		 */
		OFFSHORE_API SkinMesh& addSkinMeshNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const SkinMesh& value = SkinMesh());

		/**
		 * Добавить в сцену ноду точек
		 * @param name имя ноды точек
		 * @param type тип ноды - "kPoints"
		 * @param parentIndex индекс ноды родителя
		 * @param value точки которые добавляем
		 */
		OFFSHORE_API Points& addPointsNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const Points& value = Points());

		/**
		 * Добавить в сцену инстанс-ноду
		 * @param name имя меша
		 * @param type тип ноды - "kInstance"
		 * @param parentIndex индекс ноды родителя
		 * @param instanceName имя инстансируемой сцены из массива scene.instances
		 * @param instanceIndex индекс инстансируемой сцены в массиве scene.instances
		 */
		OFFSHORE_API int addInstanceNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const ed::string& instanceName);
		OFFSHORE_API int addInstanceNode(const ed::string& name, const ed::string& type, int32_t parentIndex, int instanceIndex);

		/**
		 * Добавить в сцену лайт-ноду
		 * @param name имя лайта
		 * @param type тип ноды - "kLight"
		 * @param parentIndex индекс ноды родителя
		 * @param value лайт который добавляем
		 */
		OFFSHORE_API Light& addLightNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const Light& value = Light());
		
		/**
		 * Добавить в сцену камера-ноду
		 * @param name имя камеры
		 * @param type тип ноды - "kCamera"
		 * @param parentIndex индекс ноды родителя
		 * @param value камера, которую добавляем
		 */
		OFFSHORE_API Camera& addCameraNode(const ed::string& name, const ed::string& type, int32_t parentIndex, const Camera& value = Camera());
		
		OFFSHORE_API int copyNode(Scene& sourceScene, int sourceIndex, int parentIndex, const osg::Matrixd& m = osg::Matrixd::identity());
		OFFSHORE_API int copyMeshNode(Scene& sourceScene, int sourceIndex, int parentIndex);
		OFFSHORE_API int copySplineNode(Scene& sourceScene, int sourceIndex, int parentIndex);
		/**
		 * Копировать поддерево сцены со всей иерархией
		 * @param sourceScene сцена поддерево которой будет копироваться
		 * @param sourceIndex индекс рут-ноды поддерева сцены sourceScene. Эта нода тоже будет скопирована
		 * @param parentIndex индекс ноды сцены к которой будет подвешено поддерево
		 * @param sourceIndices новые индексы для нод сцены sourceScene
		 * @param ignoreNONENodes копировать дерево без NONE нод.
		 */
		OFFSHORE_API int addExisting(Scene& sourceScene, int sourceIndex, int parentIndex, ed::map<int, int>* sourceIndices = NULL, bool ignoreNONENodes = false);
		/* deprecated */OFFSHORE_API int AddExisting(Scene& sourceScene, int sourceIndex, int parentIndex);
		
		/**
		 * Скопировать поддерево сцены как в addExisting, но sourceIndex нода скопирована не будет
		 * @param sourceScene сцена поддерево которой будет копироваться
		 * @param sourceIndex индекс рут-ноды поддерева сцены sourceScene. Эта скопирована не будет
		 * @param parentIndex индекс ноды сцены к которой будет подвешены чайлды ноды sourceIndex
		 * @param sourceIndices новые индексы для нод сцены sourceScene
		 * @param ignoreNONENodes копировать дерево без NONE нод.
		 */
		OFFSHORE_API int addExistingExclusive(Scene& sourceScene, int sourceIndex, int parentIndex, ed::map<int, int>* sourceIndices = NULL, bool ignoreNONENodes = false);

		/**
		 * Подставить вместо инстанс-ноды иехархию инстанса. Для rootnode и всех чайдов
		 * @param nodeIndex индекс ноды в сцене которую надо развернуть
		 */
		OFFSHORE_API void inplaceInstances(int rootnode);

		OFFSHORE_API Instance& addInstance(const ed::string& instanceName, const osg::Vec3d& position, float size);
		OFFSHORE_API int findInstanceByName(const ed::string& instanceName);

		OFFSHORE_API int AddMaterial(const Material& material = Material());
		OFFSHORE_API int addMaterial(const Material& material = Material());

		// Устанавливает значение параметра ноды
		OFFSHORE_API void setNodeParam(int nodeIndex, const ed::string& param, const ed::string& value);
		OFFSHORE_API void setNodeParam(int nodeIndex, const ed::string& param, double value);
		OFFSHORE_API void setNodeParams(int nodeIndex, const ed::map<ed::string, ed::string>& params);
		OFFSHORE_API void setNodeEmptyParam(int nodeIndex, const ed::string& param, const ed::string& value);
		OFFSHORE_API void setNodeEmptyParam(int nodeIndex, const ed::string& param, double value);
		// Возвращает значение параметра ноды
		OFFSHORE_API ed::string getNodeParam(int nodeIndex, const ed::string& param) const;
		OFFSHORE_API bool getNodeParam(int nodeIndex, const ed::string& param, ed::string& value) const;
		OFFSHORE_API bool getNodeParam(int nodeIndex, const ed::string& param, double& value) const;
		OFFSHORE_API bool getNodeParam(int nodeIndex, const ed::string& param, int& value) const;
		OFFSHORE_API bool getNodeParam(int nodeIndex, const ed::string& param, bool& value) const;
		OFFSHORE_API void removeNodeParam(int nodeIndex, const ed::string& param);

		OFFSHORE_API const ed::map<ed::string, ed::string>* getNodeParams(int nodeIndex) const;
		OFFSHORE_API void getNodeParams(int nodeIndex, ed::map<ed::string, ed::string>& params) const;

		// "удаляет" ноду
		OFFSHORE_API void SetNoneNode(int nodeIndex, bool bRemoveChildren=false);

	// запросы
	public:
		OFFSHORE_API TreeNode*		    NodeRoot();
		OFFSHORE_API TreeNode*          Node(int nodeIndex);
		OFFSHORE_API const TreeNode*    Node(int nodeIndex) const;
		OFFSHORE_API const ed::string& NodeName(int nodeIndex) const;
		OFFSHORE_API void				setNodeName(int nodeIndex, const ed::string& nodename);
		OFFSHORE_API int			    NodeParent(int nodeIndex) const; 
		OFFSHORE_API enType		        NodeType(int nodeIndex) const;
		OFFSHORE_API int			    NodeShapeIndex(int nodeIndex) const;
		OFFSHORE_API osg::Matrixd       NodeTransform(int nodeIndex, bool bParentTransform=false) const; // трансформ для объекта, если bParentTransform - вернет без учета трансформа ноды nodeIndex
		OFFSHORE_API void				setNodeTransform(int nodeIndex, const osg::Matrixd& m);
		OFFSHORE_API Material*          NodeMaterial(int nodeIndex);
		OFFSHORE_API int				NodeMaterialIndex(int nodeIndex);
		OFFSHORE_API void				setNodeMaterial(int nodeIndex, int material);
		OFFSHORE_API void				NodeColor(int nodeIndex, osg::Vec3f& color);
		OFFSHORE_API ed::list<int32_t>	NodeChilds(int nodeIndex);
		OFFSHORE_API osg::BoundingBox   NodeBoundingBox(int nodeIndex, const osg::Matrixd& matrix = osg::Matrixd());
		OFFSHORE_API ed::vector<osg::Vec3d>* NodeVerts(int nodeIndex);
		OFFSHORE_API ed::map< ed::string, ed::string>& NodeParams(int nodeIndex);
		OFFSHORE_API void setNodeParams(int nodeIndex, ed::map< ed::string, ed::string>& params);
		OFFSHORE_API offshore::Transform* ParentTransform(int nodeindex);

		OFFSHORE_API TreeNode*		getRootNode();
		OFFSHORE_API Material*		getMaterial(int32_t material);
		OFFSHORE_API const Material* getMaterial(int32_t material) const;

		OFFSHORE_API bool query(ISceneQuerySink* sink, int rootNode, bool oldIncorrectImplementation = false);

		OFFSHORE_API bool queryShapesInWorldSpace(ed::vector<int32_t>& shapes, int rootNode = -1);
		OFFSHORE_API bool queryShapes(ed::vector<std::pair<osg::Matrixd, int32_t> >& matricesNshapes, int rootNode = -1);
		OFFSHORE_API bool queryMeshes(ed::vector<std::pair<osg::Matrixd, int32_t> >& matricesNshapes, int rootNode = -1);
		OFFSHORE_API bool querySplines(ed::vector<std::pair<osg::Matrixd, int32_t> >& matricesNshapes, int rootNode = -1);
		OFFSHORE_API bool queryTransforms(ed::vector<std::pair<osg::Matrixd, int32_t> >& matricesNshapes, int rootNode = -1);

		// Метод надо выпилить
		OFFSHORE_API bool QueryShapesOld(ed::vector<std::pair<osg::Matrixd, int32_t> >& matrixNshapes, int rootNode, int reserve = -1);
		OFFSHORE_API TreeNode* FindNodeByName(const ed::string& name);
		OFFSHORE_API int findNodeByName(const ed::string& name);

		// Найти все ноды с именем name
		OFFSHORE_API int findNodesByName(const ed::string& name, ed::vector<int32_t>& nodes)const;
		// Найти первую ноду с именем name
		OFFSHORE_API int findNodeByName(const ed::string& instanceName, int rootindex, bool bViewBranches=false, bool bCaseSensitive=false)const;

		// Можно ли объеденить ноды
		OFFSHORE_API bool isNodesMergable(int node1, int node2) const;

		OFFSHORE_API void Dump(FILE* file, int rootNode = -1);

		OFFSHORE_API bool isNaN(ed::vector<offshore::Error>& errors);

		/**
		 * Получить BoundingBox сцены
		 */
		OFFSHORE_API osg::BoundingBox getBoundingBox();

		/**
		 * Получить количество вершин во всех мешах сцены. В основном нужно для .reserve() в векторах в различных операциях
		 */
		OFFSHORE_API int getVertexCount();

		OFFSHORE_API int errorCount() const;
		OFFSHORE_API int warningCount() const;

		// scene.queryShapes([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node)
		template <typename Function> void queryShapes(const Function& f);
		template <typename Function> void queryShapes(int rootnode, const Function& f);
		// scene.queryMeshes([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node, offshore::Mesh& mesh)
		template <typename Function> void queryMeshes(const Function& f);
		template <typename Function> void queryMeshes(int rootnode, const Function& f);
		// scene.querySplines([&](const osg::Matrixd& transform, int nodeIndex, offshore::TreeNode* node, offshore::Spline& spline)
		template <typename Function> void querySplines(const Function& f);
		template <typename Function> void querySplines(int rootnode, const Function& f);
		// scene.queryTransforms( rootnode, [&](const osg::Matrixd& m, int nodeindex, offshore::TreeNode* node)
		template <typename Function> void queryTransforms(int rootnode, const Function& f);

		// scene.traverseChilds( rootnode, [&](int nodeindex, offshore::TreeNode* node)
		template <typename Function> void traverseChilds(int rootnode, const Function& f);

	private:
		static bool queryRequrcive(offshore::Scene& scene, offshore::ISceneQuerySink* sink, int node, osg::Matrixd parentTransform, bool isRootNode);
	};

	///////////////////////////////////////
	//
	// Reference scene for instances
	// 
	struct Instance
	{
		ed::string name;		// имя
		osg::Vec3d position;	// позиция в максе
		float size;				// размер даммика в максе

		Scene scene;
		Instance():position(0, 0, 0), size(1){};
	};

	struct SceletonAnimation
	{
		// длительность
		double duration;		// в секундах
		// зацикленая анимация
		bool bCycled;

		// кости
		struct Joint
		{
			ed::string name;
			AnimationCurveMatrixd exclusivetransforms;
			AnimationCurveMatrixd inclusivetransforms;
		};
		// 
		ed::vector<Joint> joints;

	public:
		OFFSHORE_API bool Save(const char* filename);
		OFFSHORE_API bool Load(const char* filename);
	protected:
		OFFSHORE_API bool Serialize(io::Stream& s);
	};
};


#include "scene.inl"


#endif
