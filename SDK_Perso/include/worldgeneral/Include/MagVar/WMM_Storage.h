#ifndef __WMM_STORAGE_H__
#define __WMM_STORAGE_H__

#include "WMM_Definitions.h"

#include <bitset>
#include <ed/vector.h>
#include <ed/string.h>

namespace magvar 
{
	class WMM_Storage
	{
	public:
		virtual WMM_Data& get_element (int longitude, int latitude) = 0;
	protected:
		WMM_Data NULL_data_;
	};

	class WMM_Simple_Storage : public WMM_Storage
	{
	public:
		WMM_Simple_Storage(const char* file_name );
		~WMM_Simple_Storage();

		virtual WMM_Data& get_element (int longitude, int latitude );

	private:
		WMM_Data matrix_[MAX_LATITUDE + 1 ][ MAX_LONGITUDE + 1 ];
	};

	class WMM_Parted_Storage : public WMM_Storage
	{
	public:
		WMM_Parted_Storage(const char* file_name );
		~WMM_Parted_Storage();
		virtual WMM_Data& get_element (int longitude, int latitude);

	private:

		typedef WMM_Data Cell;

		bool load_part (int part_number);
		//	bool let_througt (std::ifstream& file, int num_lines);
		ed::string file_name_;
		std::bitset<PARTS_COUNT> loaded_parts_;
		ed::vector<Cell**> cells_;
	};
}

#endif