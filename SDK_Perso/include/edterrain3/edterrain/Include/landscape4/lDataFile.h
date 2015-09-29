#ifndef LANDSCAPE4_LDATA_FILE_H
#define LANDSCAPE4_LDATA_FILE_H

#include <ed/string.h>

namespace io
{
	class MmfStream;
}

namespace landscape4
{
// Базовый класс для всех файлов земли
class lDataFile
{
public:
	virtual ~lDataFile(){};
	virtual ed::string getType() = 0;
	virtual bool load(io::MmfStream& file) = 0;
	virtual bool copyFrom(lDataFile* src) = 0;
};

}


#endif