#ifndef __EDGE_IBINARY_FILE_H__
#define	__EDGE_IBINARY_FILE_H__

#include <stdint.h>

#include <ed/string.h>
#include "osg/Matrixf"
#include "osg/Matrixd"
#include "config.h"

#ifndef ED_CORE_EXPORT
#define ED_CORE_EXPORT ED_DLL_EXPORT
#endif

namespace io {
	
/// Interface to work with files.
class IBinaryFile
{
public:
	/// Close file.
	virtual ~IBinaryFile(){}
	
public:
	/// Read raw bytes from file.
	virtual void read(void *buf, uint64_t size)=0;
	
	/// Write raw bytes to file.
	virtual void write(const void *buf, uint64_t size)=0;

	/// Seek to specified position in file.
	virtual void seek(uint64_t pos)=0;

	/// Returns offset from file start.
	virtual uint64_t getOffset()const=0;

	/// Returns name of data source. Useful for debug.
	virtual const ed::string& getName()const=0;
	
	/// Read unsigned byte from file.
	ED_CORE_EXPORT uint8_t readUint8();
	
	/// Read unsigned 2-bytes integer from file.
	ED_CORE_EXPORT uint16_t readUint16();
	
	/// Read unsigned 4-bytes integer from file.
	ED_CORE_EXPORT uint32_t readUint32();
	
	/// Read unsigned 8-bytes integer from file.
	ED_CORE_EXPORT uint64_t readUint64();

	/// Read 4-bytes float from file
	ED_CORE_EXPORT float readFloat();
	
	/// Read 8-bytes double from file
	ED_CORE_EXPORT double readDouble();
	
	/// Read string of specified size
	ED_CORE_EXPORT ed::string readString(const unsigned int size);
	
	/// Read string of specified size to \param buf.
	/// Warning! buf must have enough size to read string and completing '\0'.
	ED_CORE_EXPORT void readString(const unsigned int size,char *buf);
	
	/// Read vector
	ED_CORE_EXPORT osg::Vec2f readVec2f();
	
	/// Read vector
	ED_CORE_EXPORT osg::Vec2d readVec2d();

	/// Read vector
	ED_CORE_EXPORT osg::Vec3f readVec3f();
	
	/// Read vector
	ED_CORE_EXPORT osg::Vec3d readVec3d();

	/// Read vector
	ED_CORE_EXPORT osg::Vec4f readVec4f();
	
	/// Read vector
	ED_CORE_EXPORT osg::Vec4d readVec4d();

	/// Read quaternion
	ED_CORE_EXPORT osg::Quatf readQuatf();
	/// Read quaternion
	ED_CORE_EXPORT osg::Quatd readQuatd();

	/// Read matrix
	ED_CORE_EXPORT osg::Matrixf readMatrixf();

	/// Read matrix
	ED_CORE_EXPORT osg::Matrixd readMatrixd();

	/// Write unsigned byte to file.
	ED_CORE_EXPORT void writeUint8(uint8_t v);
	
	/// Write unsigned 2-bytes integer to file.
	ED_CORE_EXPORT void writeUint16(uint16_t v);
	
	/// Write unsigned 4-bytes integer to file.
	ED_CORE_EXPORT void writeUint32(uint32_t v);
	
	/// Write unsigned 8-bytes integer to file.
	ED_CORE_EXPORT void writeUint64(uint64_t v);
	
	/// Write float to file.
	ED_CORE_EXPORT void writeFloat(float v);
	
	/// Write double to file.
	ED_CORE_EXPORT void writeDouble(double v);
	
	/// Write string of fixed size to file.
	ED_CORE_EXPORT void writeString(const ed::string &v);

	/// Write vector to file.
	ED_CORE_EXPORT void writeVec2f(const osg::Vec2f &v);

	/// Write vector to file.
	ED_CORE_EXPORT void writeVec2d(const osg::Vec2d &v);

	/// Write vector to file.
	ED_CORE_EXPORT void writeVec3f(const osg::Vec3f &v);

	/// Write vector to file.
	ED_CORE_EXPORT void writeVec3d(const osg::Vec3d &v);

	/// Write vector to file.
	ED_CORE_EXPORT void writeVec4f(const osg::Vec4f &v);

	/// Write vector to file.
	ED_CORE_EXPORT void writeVec4d(const osg::Vec4d &v);

	/// Write quaternion to file.
	ED_CORE_EXPORT void writeQuatf(const osg::Quatf &v);
	/// Write quaternion to file.
	ED_CORE_EXPORT void writeQuatd(const osg::Quatd &v);

	/// Write matrix to file.
	ED_CORE_EXPORT void writeMatrixf(const osg::Matrixf &v);

	/// Write matrix to file.
	ED_CORE_EXPORT void writeMatrixd(const osg::Matrixd &v);

	/// Read ed::string object.
	ED_CORE_EXPORT ed::string readSTLString();

	/// Write ed::string object.
	ED_CORE_EXPORT void writeSTLString(const ed::string &v);
};

}

#endif
