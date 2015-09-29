#ifndef	__RESOURCEKEEPER_H_
#define	__RESOURCEKEEPER_H_

#include <ed/map.h>
#include <utility>
#include <memory>
#include <assert.h>
#include <ed/sstream.h>

#include "Factory/SharedObject.h"
#include "MapOfFile.h"
#include "ZeroOnExit.h"

inline ed::string extractTitle(const ed::string& fileName);

template <class TParameters>
class ResourceKeeper
{
public:
	typedef Common::SharedPtr<TParameters> ResourcePtr;
	typedef ed::map<ed::string, ResourcePtr> ResourceMap;

protected:
	ResourceMap resources;
	ed::string	readingFile;

	const TParameters* const readResource (const ed::string& resourceName, int preset = 0);
	const TParameters* const readResource (const ed::string& resourceName, 
													const ed::string& source, int preset = 0);
	const TParameters* const readResource (const ed::string& resourceName, 
													const char* source, 
													unsigned int sourceLength, int preset);

public:

	ResourceKeeper();
	virtual ~ResourceKeeper();

	void clear();

	//Эта фунцкия пытается взять ресурс из хранилища ресурсов,
	//если его там нет, то она читает его из файла с именем = recourceName 
	//и заносит в список. Функция возвращает указатель на соответствующий ресурс,
	//хранящийся в хранилище ресурсов
	const TParameters* const getResource   (const ed::string& resourceName, 
											int preset = 0, 
											bool tryToRead = true);
	const TParameters* const getResource   (const ed::string& resourceName, 
											const ed::string& source, 
											int preset = 0, 
											bool tryToRead = true);
	const TParameters* const getResource   (const ed::string& resourceName, 
											const char* source, 
											unsigned int sourceLength, int preset, 
											bool tryToRead = true);
	//Эта функция заполняет в *recourсe те поля, которые можно заполнить 
	//из файла resourceName и возвращает указатель на обновленный ресурс
	//const TParameters* updateResource(TParameters* resource, ed::string resourceName);
	void updateResource(TParameters* prms, const ed::string& resourceName);
	void updateResourceFromString(TParameters* prms, const ed::string& source);
	void updateResourceFromString(TParameters* prms, const char* source, unsigned int sourceLength);

	TParameters* addResource   (const ed::string& resourceName, const ResourcePtr& prms);
	TParameters* addResource   (const ed::string& resourceName, TParameters* prms);
	void deleteResource(const ed::string& resourceName);
	
	const ed::string&	getReadingFile() const {return readingFile;}
	ed::string fileToResourceName(const ed::string& name, int preset);
};

template <class TParameters>
inline ResourceKeeper<TParameters>::ResourceKeeper()
{
}

template <class TParameters>
inline ResourceKeeper<TParameters>::~ResourceKeeper()
{
	clear();
}

template <class TParameters>
inline void ResourceKeeper<TParameters>::clear()
{
	resources.clear();
}

template <class TParameters>
inline const TParameters* const ResourceKeeper<TParameters>::getResource 
						(const ed::string& resourceName, int preset, bool tryToRead)
{
	ResourceMap::iterator resource;
	resource = resources.find(fileToResourceName(resourceName, preset));
	if (resource == resources.end())
	{
		if (tryToRead)
		{
			const TParameters* prms = readResource(resourceName, preset);
			return prms;
		}
		else
		{
			return new TParameters;
		}
	}
	else
	{
		return resource->second;
	}
}

template <class TParameters>
inline const TParameters* const ResourceKeeper<TParameters>::getResource 
		(const ed::string& resourceName, const ed::string& source, 
		 int preset, bool tryToRead)
{
	getResource(resourceName, source.c_str(), source.length(), preset, tryToRead);
}

template <class TParameters>
inline const TParameters* const ResourceKeeper<TParameters>::getResource 
							(const ed::string& resourceName, const char* source, 
							unsigned int sourceLength, int preset,
							bool tryToRead)
{
	ResourceMap::iterator resource;
	resource = resources.find(resourceName);
	if (resource == resources.end())
	{
		if (tryToRead)
		{
			return readResource(resourceName, source, sourceLength, preset);
		}
		else
		{
			return new TParameters;
		}
	}
	else
	{
		return resource->second;
	}
}

template <class TParameters>
inline void ResourceKeeper<TParameters>::updateResource 
										(TParameters* prms, const ed::string& resourceName)
{
	MapOfFile fileMap(resourceName.c_str(), get_right_path(resourceName).c_str());
	updateResourceFromString(prms, fileMap.getFileMap(), fileMap.getFileSize());
}

template <class TParameters>
inline void ResourceKeeper<TParameters>::updateResourceFromString
										(TParameters* prms, const ed::string& source)
{
	updateResourceFromString(prms, source.c_str(), source.length());
}

template <class TParameters>
inline void ResourceKeeper<TParameters>::updateResourceFromString
								(TParameters* prms, const char* source, unsigned int sourceLength)
{
	if (sourceLength != 0)
	{
		prms->read(source, sourceLength);
	}
}

template <class TParameters>
inline const TParameters* const ResourceKeeper<TParameters>::readResource 
												(const ed::string& resourceName, int preset)
{
	MapOfFile fileMap(resourceName.c_str(), get_right_path(resourceName).c_str());
	ed::string str = fileToResourceName(resourceName, preset);
	readingFile = str;
	ZeroOnExitStr zero(readingFile);
	const TParameters* prms = readResource(str, fileMap.getFileMap(), 
											fileMap.getFileSize(), preset);
	return prms;
}

template <class TParameters>
inline const TParameters* const ResourceKeeper<TParameters>::readResource 
					(const ed::string& resourceName, const ed::string& source, int preset)
{
	return readResource(resourceName, source.c_str(), source.length(), preset);
}

template <class TParameters>
inline const TParameters* const ResourceKeeper<TParameters>::readResource 
(const ed::string& resourceName, const char* source, unsigned int sourceLength, int preset)
{
	TParameters* prms  = (preset == 0 ? (new TParameters()) : (new TParameters(preset)) );
	updateResourceFromString(prms, source, sourceLength);
	
	ResourcePtr thePtr;
	//thePtr.Attach(prms);
	thePtr = prms;
	prms->Release();
	return addResource(resourceName, thePtr);
}

template <class TParameters>
inline ed::string ResourceKeeper<TParameters>::fileToResourceName(const ed::string& name, int preset)
{
	ed::ostringstream stream;
	stream  << extractTitle(name)
			<< "_" 
			<< preset;
	return stream.str();
}


template <class TParameters>
inline TParameters* ResourceKeeper<TParameters>::addResource(const ed::string& name,
							const typename ResourceKeeper<TParameters>::ResourcePtr& ptr)
{
	ptr->setIdentifier(Common::identify<TParameters>());
	//ptr->setName(name);
	std::pair<ResourceMap::iterator, bool> res = resources.insert(ResourceMap::value_type(name, ptr));
#ifndef DEMO_VERSION
	assert( res.second );
#endif DEMO_VERSION
	return res.first->second;
}

template <class TParameters>
inline TParameters* ResourceKeeper<TParameters>::addResource(const ed::string& name,
							TParameters* ptr)
{
	ResourcePtr prms;
	//prms.Attach(ptr);
	prms = ptr;
	ptr->Release();
	return addResource(name, prms);
}

template <class TParameters>
inline void ResourceKeeper<TParameters>::deleteResource(const ed::string& resourceName)
{
	resources.erase(resourceName);
}

#endif	//	__RESOURCEKEEPER_H__
