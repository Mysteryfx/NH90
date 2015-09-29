#ifndef __WMM_BINARYFILE_H__
#define __WMM_BINARYFILE_H__

/************************************************************************/
/* Classes for reading of world magnetic model grids files               */
/************************************************************************/

#include "ed_vfs.hpp"

namespace magvar 
{
	class WMM_Binary_File : public VFS::File
	{
	public:

		struct WMM_File_Data
		{
			double latitude_;
			double longitude_;
			double declination_;
			double inclination_;
		};

		WMM_Binary_File(const char* name);
		virtual bool seek_throuth(int record_number);
		/* record_number - count of the records to pass through */
		virtual bool read(WMM_File_Data* data, unsigned int count);
	};
}
#endif