#ifndef offshore_remap_h__
#define offshore_remap_h__

#include "offshore/scene.h"


namespace offshore {


class Remap
{
	const ed::vector<int32_t>& vertexRemap;
	ed::map<int, int> remapIndicies;
	ed::map<int, int> invertIndicies;
public:

	OFFSHORE_API Remap(const ed::vector<int32_t>& vertexRemap);

	OFFSHORE_API int remap(int v);

	OFFSHORE_API int invertRemap(int v);

	OFFSHORE_API void remap(ed::vector<int32_t>& indicies);

	OFFSHORE_API void remapUvset(offshore::uvSet& uvset);

	OFFSHORE_API void invertRemapUvset(offshore::uvSet& uvset);

	OFFSHORE_API static void remapUvset(offshore::uvSet& uvset, ed::map<int, int>& vertexRemap);
};


} // namespace offshore


#endif // offshore_remap_h__
