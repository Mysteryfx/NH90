#ifndef LANDSCAPE4_lReferenceFile_H
#define LANDSCAPE4_lReferenceFile_H

#include "edterrain.h"
#include "landscape3/lMaterial.h"
#include "landscape4/lGeometry.h"
#include "landscape4/lDataFile.h"
#include "edModel/IModel.h"

#ifdef EDGE
	#include "Core/io/STLSerializer.h"
#else
	#include "io/STLSerializer.h"
#endif

namespace landscape4
{

class lReferenceFile : public lDataFile
{
public:
	// дистанции
	struct Distances
	{
		bool bOn;
		double nearDistance, nearBlendDistance; 
		double farDistance, farBlendDistance;
		EDTERRAIN_API Distances();

		// Поправить средние дистанции
		EDTERRAIN_API Distances getValid()const;
	};
	enum enDistancePass
	{
		DP_MAIN=0,		// дистанции для стандартной камеры
		DP_SHADOWS,		// дистанции для прохода теней
		DP_REFLECTION,	// дистанции для прохода отражений
		DP_MAX,			// 
	};

	// Референсы
	// Одно имя может быть представлено несколько раз с разными уровнями (levelmask)
	struct Reference
	{
		// Используется для инстансирования
		bool usedForInstancing;
		int bufferSizeInstancing;
		// имя референса
		ed::string name;
		// имя лода, может быть ==""
		ed::string lodname;
		// дистанции для разных проходов
		Distances distances[DP_MAX];
		// box
		osg::BoundingBoxf fullbox;
		// материал
		material3::Material material;
		// геометрия
		geometry4::GeometrySource geometry;
		// параметры для компут материала
		ed::map<ed::string, ed::string> computeParameters;
		// семантика для компут материала
		ed::string computeSemantic;

		// Reference
		EDTERRAIN_API Reference();

		EDTERRAIN_API ed::string getDescription()const;

		EDTERRAIN_API double getNearDistance()const;
		EDTERRAIN_API double getFarDistance()const;
	};

	ed::vector<Reference> references;

	// После загрузки:
	struct ReferenceObject
	{
		ed::string name;
		ed::vector<int> lods;		// индексы в списке references
		osg::BoundingBoxf box;
		model::IModel* model;		// загруженая модель
		ReferenceObject();
	};
	ed::vector<ReferenceObject> referenceObjects;
	// Список по именам (lwr)
	ed::map<ed::string, int> referenceObjectsNames;

public:
	EDTERRAIN_API lReferenceFile();
	EDTERRAIN_API virtual ~lReferenceFile();

	virtual ed::string getType() { return "landscape4::lReferenceFile"; }
	virtual bool copyFrom(lDataFile* src){*this = *(lReferenceFile*)src;return true;};

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool save(const ed::string& filename);
	EDTERRAIN_API bool load(io::MmfStream& stream);

private:

	bool serialize(io::MmfStream& stream);

	void loadReferenceObjects();

private:
	struct ModelImpl : public model::IModel
	{
		lReferenceFile* file;
		ReferenceObject* refObject;

	public:
		ModelImpl( lReferenceFile* file, ReferenceObject* refObject);
		virtual ~ModelImpl();

	public:
		/// Returns the number of transform nodes in model.
		virtual unsigned int getNumTransformNodes()const;	
		/// Returns current state of model loading.
		virtual model::LoadState getLoadState()const;
		/// Returns model name, which is also its hash key.
		/// Can't return NULL.
		virtual const char* getName()const;

		/// Returns bounding box of model.
		virtual const osg::BoundingBox& getBoundingBox()const;

		/// Returns light box of model. If model doesn't have light sources returns invalid box.
		virtual const osg::BoundingBox& getLightingBox()const;

		/// Returns number of model arguments.
		virtual unsigned int getNumArguments()const;

		/// Returns the number of connectors in model.
		virtual unsigned int getNumConnectors()const;

		/// Returns connector by its index.  If there is no if index > getNumConnectors() returns NULL.
		virtual const model::IConnector* getConnector(unsigned int index)const;

		/// Returns connector by its name. If there is no connector with such name returns NULL.
		virtual const model::IConnector* getConnectorByName(const char* name)const;

		/// Returns the number of collision nodes of model.
		/// Note: it doesn't take into account segment nodes.
		virtual unsigned int getNumCollisionShells()const;
		/// Returns shell node by index given by \param i.
		/// \param i must be less than the shell nodes number.
		/// Note: it doesn't take into account segment nodes.
		virtual const model::ICollisionShell* getCollisionShell(unsigned int i)const;

		/// Returns the number of lights in model.
		virtual unsigned int getNumLights()const{return 0;}

		/// Returns light by its index.
		virtual const model::ILight* getLight(unsigned int index)const{return nullptr;}

		/// Returns the number of bones in model.
		virtual unsigned int getNumBones()const;

		/// Returns id of bone or model::INVALID_BONE_ID if there is no bone with given name.
		virtual unsigned int getBoneIdByName(const char *name)const;
	
		/// Returns name of bone node or NULL if given id is invalid.
		virtual const char* getBoneName(unsigned int id)const;
	
		/// Returns matrix of bone or zero matrix if given id is invalid.
		virtual const osg::Matrixd& getBoneMatrix(unsigned int id)const;

		/// Means than model will be used in current frame. So it must start loading.
		virtual void needIt();	

		/// This method forces model to load immediately.
		virtual void forceLoad();

		/// Tests intersection with segment.
		/// \param p returns position of intersection point.
		/// \param n returns normal in intersection point.
		/// Returns name of intersected collision shell or NULL if there was no intersection with any collision shell of model.
		virtual const char* testWithSegment(const osg::Vec3d &a, const osg::Vec3d &b, double &t, osg::Vec3d &n, const osg::Matrixd &modelPos, const model::IModelParams *params);

	};

};

};

template <class S> S& operator >>(S& stream, landscape4::lReferenceFile::Distances& distances)
{
	uint32_t version = 0;
	stream >> version;
	
	if (version >= 0)
	{
		stream >> distances.bOn;
		stream >> distances.nearDistance;
		stream >> distances.nearBlendDistance;
		stream >> distances.farDistance;
		stream >> distances.farBlendDistance;
	}
	return stream;
}

template <class S> S& operator >>(S& stream, landscape4::lReferenceFile::Reference& reference)
{
	uint32_t version = 7;
	stream >> version;

	double nearDistance=0, farDistance=0, farBlendDistance=0, nearBlendDistance=0;

	if (version >= 0)
	{
		stream >> reference.name;
		stream >> nearDistance;					// маска level
		stream >> farDistance;
		stream >> reference.fullbox;
		stream >> reference.material;
		stream >> reference.geometry;
		if(stream.getMode()==io::Stream::READ)
		{
			farBlendDistance = farDistance;
			nearBlendDistance = nearDistance;
		}
	}
	if (version >= 1)
	{
		stream >> farBlendDistance;
	}
	if (version >= 2)
	{
		stream >> nearBlendDistance;
	}
	if (version >= 3)
	{
		stream >> reference.usedForInstancing;
	}
	if (version >= 4)
	{
		stream >> reference.computeParameters;
	}
	if (version >= 5)
	{
		stream >> reference.computeSemantic;
	}	
	if (version >= 6)
	{
		stream >> reference.lodname;
		stream >> reference.distances[landscape4::lReferenceFile::DP_MAIN];
		stream >> reference.distances[landscape4::lReferenceFile::DP_SHADOWS];
		stream >> reference.distances[landscape4::lReferenceFile::DP_REFLECTION];
	}
	if (version >= 7)
	{
		stream >> reference.bufferSizeInstancing;
	}
	return stream;
}

#endif
