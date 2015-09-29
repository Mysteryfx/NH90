#ifndef __VFS_BINARY_FILE_H__
#define	__VFS_BINARY_FILE_H__

#include "io/IBinaryFile.h"

#include "config.h"

#ifndef ED_CORE_EXPORT
#define ED_CORE_EXPORT ED_DLL_EXPORT
#endif

struct PHYSFS_File;

namespace io {

/// Easy to use wrapper around FILE to make cross platform data reading easy.
class VFSBinaryFile : public IBinaryFile{
protected:
	/// File handler
	PHYSFS_File *_f;

	/// Name of file (useful to show in error messages)
	ed::string _fileName;
	
	VFSBinaryFile(const ed::string &file);

	/// Close file.
	ED_CORE_EXPORT ~VFSBinaryFile();
public:
	/// Seek to specified position in file.
	ED_CORE_EXPORT void seek(uint64_t pos);
	
	/// Returns name of file.
	ED_CORE_EXPORT const ed::string& getName()const;
	
	/// Returns offset from file start.
	ED_CORE_EXPORT uint64_t getOffset()const;
};

class VFSROBinaryFile : public VFSBinaryFile{
public:
	/// Open file for reading only
	/// throws io::Exception on failure
	ED_CORE_EXPORT VFSROBinaryFile(const ed::string &file);

	/// Read raw bytes from file.
	ED_CORE_EXPORT void read(void *buf, uint64_t size);
	
	/// Write raw bytes to file.
	/// Doesn't have effect.
	void write(const void *buf, uint64_t size){}
};

class VFSWOBinaryFile : public VFSBinaryFile{
public:
	/// Open file for reading only
	/// throws io::Exception on failure
	ED_CORE_EXPORT VFSWOBinaryFile(const ed::string &file);

	/// Read raw bytes from file.
	void read(void *buf, uint64_t size){}
	
	/// Write raw bytes to file.
	ED_CORE_EXPORT void write(const void *buf, uint64_t size);
};

}

#endif

