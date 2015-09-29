#ifndef	__MAPOFFILE_H_
#define	__MAPOFFILE_H_

#include "ExpImpSpec.h"

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR MapOfFile
{
private:
	char*			fileMap;
	int				fileSize;

public:
	MapOfFile(const char* fileName,  const char* inPartPath);
	virtual ~MapOfFile();

    char* const		getFileMap() { return fileMap; }
    const int		getFileSize() { return fileSize; }
};

USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ed::string get_right_path(const ed::string& filename_);
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ed::string get_right_filename(const ed::string& filename_);

#endif	//	__MAPOFFILE_H__
