#ifndef __MISSION_STORAGE_H__
#define __MISSION_STORAGE_H__

#include "WorldGeneral.h"

#include "minizip/zip.h"
#include "minizip/unzip.h"

#include <ed/vector.h>
#include <ed/string.h>
#include <ed/set.h>

namespace Mission
{

class MissionReader
{
public:
	MissionReader();
	~MissionReader();

	void open(const ed::string& filename);		

	// Считать 
	bool hasChunk(const ed::string& name) const;
	int	 getChunkCount();
	const ed::string& getChunkName(int index);

    int  getChunkSize(const ed::string& name);
    // Буффер должен быть соответствующей длины
	void readChunk(const ed::string& name, char* buf);
    void readChunkToFile(const ed::string& name, const ed::string& filename);

    int  getChunkSize(int index);
	void readChunk(int index, char* buf);
    void readChunkToFile(int index, const ed::string& filename);

protected:
	int getChunkIndex_(const ed::string& name, bool throwEx) const;
	void readChunksList_();

    void openChunk_(int index);
    void closeChunk_();

protected:
	unzFile file_; 
    // Name/size pair for each chunk
    ed::vector<std::pair<ed::string, int> > chunks_;
};

class MissionWriter
{
public:
	MissionWriter();
	~MissionWriter();

	void open(const ed::string& filename);
	void close();		// RAII тут не работает, потому что zipClose может вернуть ошибку, а бросать исключения в дескрипторе - плохо

    // Create chunk from in-memory data
	void createChunk(const ed::string& name, const char* data, int len);
    // Create chunk from file
    void createChunkFromFile(const ed::string& name, const ed::string& filename);

protected:
	zipFile file_;
	ed::set<ed::string> chunks_;
};

}

#endif