#ifndef __BINARY_FILE_H__
#define	__BINARY_FILE_H__

#include "IBinaryFile.h"
#include "File.h"

#include "config.h"

#ifndef ED_CORE_EXPORT
#define ED_CORE_EXPORT ED_DLL_EXPORT
#endif


namespace io {

	
/// Easy to use wrapper around FILE to make cross platform data reading easy.
class BinaryFile : public IBinaryFile{
private:
	/// File handler
	File *file;

	/// Name of file (useful to show in error messages)
	ed::string fileName;
	
	/// Current offset from file start.
	File::off_t offset;
	
public:
	/// Open file for reading or writing
	/// throws io::Exception on failure
	ED_CORE_EXPORT BinaryFile(const ed::string &file, File::Mode mode, bool createDirs=false);
	
	/// Close file.
	ED_CORE_EXPORT ~BinaryFile();
	
public:
	/// Seek to specified position in file.
	ED_CORE_EXPORT void seek(uint64_t pos);
	
	/// Read raw bytes from file.
	ED_CORE_EXPORT void read(void *buf, uint64_t size);
	
	/// Write raw bytes to file.
	ED_CORE_EXPORT void write(const void *buf, uint64_t size);
	
	/// Returns name of file.
	virtual const ed::string& getName() const { return fileName; }
	
	/// Returns offset from file start.
	uint64_t getOffset() const { return offset; }
};


/// Strips file name from path.
ED_CORE_EXPORT ed::string getFilePath(const ed::string &fileName);
}


#endif

