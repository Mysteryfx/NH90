#ifndef __TEMP_FILES_MANAGER_H__
#define __TEMP_FILES_MANAGER_H__

#include "_config.h"

#include <ed/string.h>
#include <ed/vector.h>

namespace CoreUtils
{
// Позволяет создавать временные файлы, которые будут удалены при уничтожении класса
class ED_CORE_EXPORT TempFilesManager
{
public:
	TempFilesManager();
	~TempFilesManager();

	static ed::string getTempFileName(const ed::string& prefix);

	ed::string newTempFile(const ed::string& prefix);
	void addTempFile(const ed::string& name);

protected:
	ed::vector<ed::string> filesToRemove_;
};

}


#endif