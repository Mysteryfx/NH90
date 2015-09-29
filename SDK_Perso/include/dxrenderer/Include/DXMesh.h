#ifndef _DXMesh_h_
#define _DXMesh_h_

#include <D3D9.h>
#include "Renderer\CustomGeometry.h"
#include "DXRendererAPI.h"
#include "DXGeometry.h"
#include "rwrappers/RenderUserAPI.h"

namespace Graphics
{

class DXRENDERER_API DXMesh :	public DXGeometry,
								public CustomGeometry
{
    friend class DXRenderer;
    
protected:

	/*
	struct VBStream 
	{
		ed::string				semantic;
		render::stream_handle_t	handle;
		render::ELEMENTTYPE_ENUM type;		
		void *data;
		unsigned size;
	};*/

    
    bool initialized;
	bool error_load;
    
    int LoadStep;
    bool m_screenSpace;
    bool m_bHasAlpha;
    DWORD dwNumVertices;
    DWORD dwNumIndices;
    DWORD NumPrims;
	render::PRIMTYPE_ENUM  DxPrimType;
    DWORD VertexStride;

	//bounds
    Box box;
	
	BYTE* VertSrc;
	BYTE* IndSrc;
	DWORD IndexSize;
    
    Vector3 LocalCenter;
    float   LocalBoundRadius;
    
    // Custom geometry interface
    
    void SetPrimitiveType(PrimitiveType ptype);
    void SetScreenSpace  (bool isScreenSpace);

    void SetVertices     (const VertexList& vertices);
    void SetIndices      (const IndexList& indices);
    void SetNormals      (const VertexList& normals);
    void SetTexCoords    (const VertexList& texCoords,const int index = 0);
    void SetColors       (const ColorList& colors);

	virtual void SetVertices(VertexList&& vertices);
	virtual void SetIndices(IndexList&& indices);
	virtual void SetNormals(VertexList&& normals);
	virtual void SetTexCoords(VertexList&& texCoords,const int index = 0);
	virtual void SetColors(ColorList&& colors);
    
    PrimitiveType       GetPrimitiveType();
    bool                IsScreenSpace();
    const VertexList &  GetVertices();
    const IndexList  &  GetIndices();
    const VertexList &  GetNormals();
    const VertexList &  GetTexCoords(const int index = 0);
    const ColorList  &  GetColors();

	virtual VertexList&& MoveVertices();
	virtual IndexList&& MoveIndices();
	virtual VertexList&& MoveNormals();
	virtual VertexList&& MoveTexCoords(const int index = 0);
	virtual ColorList&& MoveColors();

    bool  HasAlpha() { return m_bHasAlpha; }

    VertexList  m_vertices;
    IndexList   m_indices;
    VertexList  m_normals;
    ColorList   m_colors;
    VertexList  m_texCoords;

	// previously it was in DXGeometry
	::render::VertexBuffer _vBuf;
	::render::IndexBuffer _iBuf;

	::render::stream_handle_t _positionStream;
	::render::stream_handle_t _normalStream;
	::render::stream_handle_t _diffuseStream;
	::render::stream_handle_t _texCoordStream;

    void LoadCustomGeometry();
    void FreeCustomGeometry();
    void CalcBoundSphere();

    void FreeVertices();
    void FreeIndices();

    bool m_bCustomGeometry;
    
public:
    static Resource *Create(ResourceType type, const ed::string& className);
    
    DXMesh();
    ~DXMesh();
    
    virtual void DrawGeometry(RenderObject *robject);
    
    // габариты
    virtual const Box& GetBoundingBox();
    virtual float GetRadius()           {return LocalBoundRadius;}
    virtual const Vector3& GetCenter()  {return LocalCenter;}
	
	// статистика
	virtual unsigned GetPolygonCount();
	
	// детальная проверка столкновений
	// пересечение с отрезком [a,b] при параметрах params
	// может вернуть параметр t на отрезке и нормаль n в точке пересечения
	virtual bool IntersectSegment(const Vector3& a, const Vector3& b, const DParamList& params, 
		float *t = 0, Vector3 *n = 0);
    
    // сортировка и оптимизация
    virtual GeometryKey GetKey();
    
    virtual bool CreateByName(const ed::string& resname);
    
    // Создать ресурс из коллекции ресурсов (например элементы геометрии в модели)
    // не все ресурсы обязаны поддерживать эту возможность
    //virtual bool CreateFromCollection(ResourceCollection *collection, const ed::string& resname, void *data = 0);
    
    // Общие характеристики ресурса
    
    // тип ресурса
    //virtual ResourceType Type();
    
    // имя ресурса (уникально среди ресурсов данного типа)
    //virtual const ed::string& Name();
    
    // имя класса ресурса
    virtual const ed::string& ClassName();
    
    // вернуть строку с характеристиками ресурса (для отладки)
    virtual ed::string GetDebugString();
    
    // управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
    // загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
    // на шаге 0 не должно быть захвачено никаких аппаратных ресурсов
    
    // получить приоритет ресурса
    virtual float GetLoadPriority();
    
    // получить текущий шаг загрузки
    virtual int GetLoadStep();
    
    // получить количество шагов для полной загрузки ресурса
    virtual int GetLoadStepCount();
    
    // загрузить ресурс до шага step
    virtual void LoadToStep(int step);
    
    // освободить ресурс до шага step
    virtual void FreeToStep(int step);
    
    // индекс кадра последнего обращения, менеджер ресурсов будет стараться 
    // освобить не используемые ресурсы
    
    // если кто-то использует данный ресурс - необходимо вызвать эту функцию
    virtual void UsedThisFrame();
    
    // получить индекс кадра последнего обращения
    //virtual unsigned GetLastFrame();
    
    // Функции для отслеживания определенных моментов (т.е. Call-back)
    // требуют дополнительной регистрации
    //virtual void Update() {};
    
    // сериализация
    virtual void serialize(Serializer& serializer) {};
    
    // загрузка данных
    virtual void LoadData(void *data, unsigned maxsize);
    
    // редактирование
    virtual CustomGeometry *QueryCustomGeometry();
    
//    virtual DWORD GetFVF(){return FVF;}

	void ApplyTransform(const Position3& transform);
    
	render::PRIMTYPE_ENUM GetD3DPrimType() {return DxPrimType;}
	void CopyVertexBuffer(void *outbuff, const Position3 *pos = 0);
	void CopyIndexBuffer(void *outbuff, int offset = 0);
	int GetVertexCount() {return dwNumVertices;}
	int GetIndexCount() {return dwNumIndices;}

	int GetVertexStride() {return VertexStride;};
    
	virtual bool SupportsUnload();
};

} // namespace Graphics

#endif
