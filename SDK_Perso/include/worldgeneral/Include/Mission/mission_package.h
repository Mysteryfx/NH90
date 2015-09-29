#ifndef __MISSION_PACKAGE_H__
#define __MISSION_PACKAGE_H__

#include "WorldGeneral.h"
#include <ed/string.h>
#include <ed/map.h>

#include "mission_storage.h"

#include "../../Core/inc/Misc/TempFilesManager.h"

struct lua_State;

namespace Mission
{

class Mission;
class Track;
class ITrackEventFilter;

class WORLDGENERAL_API MissionPackageChunkCheck
{
public:
	virtual bool check(const ed::string & chunk_name) = 0;
	virtual bool finished() const { return false; };
};

class WORLDGENERAL_API MissionPackage
{
protected:
	struct ChunkEntry_
	{
		ed::string filename;   // empty if no file is associated
		bool remove;            // remove associated file in the end?

        bool isDataValid;       // data contains valid info?
        ed::string data;       
	};

public:
	MissionPackage(const ITrackEventFilter* track_filter);
	~MissionPackage();
	
	void reload() { load(filename_); } 
	void load(const ed::string& filename);
	void save(const ed::string& filename);

    // Трека бывает два - из которого читаем и в который пишем
	Track*   getReadTrack()	{ return readTrack_; }
	Track*	 getWriteTrack();

    // Все данные разделяются на именованые куски (то есть файлы)
	bool		hasChunk(const ed::string& name);
    void		addChunk(const ed::string& name, const ed::string& data);
    void        removeChunk(const ed::string& name);
	// Не самая быстрая функция
	ed::vector<ed::string> getChunkNames() const;
	// Прикрепить к мисии файл
	// remove - удалять ли файл после того, как он будет не нужен
	void		addFileChunk(const ed::string& name, const ed::string& filename, bool remove);
	// Закешировать кусок и выдать имя файла
	ed::string getChunkFile(const ed::string& name);
    // Получить данные из соответствующего куска
    ed::string getChunkString(const ed::string& name);

    // Загрузить lua скрипт из соответствующего куска
    // В этом скрипте могут быть вызовы функции doZipFile, которая аналогична
    // dofile, но загружает данные из текущего контейнера
    // Замечание: я не совсем уверен, что ее протестировал
	// env_name - выполнить загруженный блок в энвайронменте с данным именем
    void		loadLuaChunk(const ed::string& name, lua_State* state, const ed::string& pathPrefix = "",const char * env_name = 0);
    void		compileMissionChunkAsLuaChunk(const ed::string& name, lua_State* state,const ed::string& pathPrefix = "");

	const ed::string& getFileName() const { return filename_; }

	void		forEachChunk(MissionPackageChunkCheck & checker);

    void        loadI18N(const ed::string& lang, lua_State* state);
protected:
	void cacheChunk_(ChunkEntry_& e, bool force = false);
    ed::string getChunkData_(ChunkEntry_& e);

protected:
	const ITrackEventFilter* trackFilter_;

	Track*		readTrack_;			// трек для чтения
	Track*		writeTrack_;		// трек для записи

	ed::string filename_;

	typedef ed::map<ed::string, ChunkEntry_> ChunkList_;
	ChunkList_ chunks_;

    CoreUtils::TempFilesManager tempFiles_;
};

}

#endif