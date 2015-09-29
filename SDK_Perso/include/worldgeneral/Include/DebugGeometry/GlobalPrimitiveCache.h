#pragma once
#include <ed/vector.h>
#include "Renderer/DbgGraph.h"
#include "Renderer/RenderText.h"
#include "WorldGeneral.h"

class PrimitiveContainer;

class GlobalPrimitiveCache
{
public:
	WORLDGENERAL_API static	GlobalPrimitiveCache* instance();
	WORLDGENERAL_API void	draw();

	typedef ed::vector<PrimitiveContainer*> Cache;

	WORLDGENERAL_API Cache::size_type			addCache();
	WORLDGENERAL_API void						removeCache(Cache::size_type);

	WORLDGENERAL_API PrimitiveContainer*			setCurrent(Cache::size_type);
	inline PrimitiveContainer*	currentCache() const {return _currentAct;};

private:
	GlobalPrimitiveCache();
	~GlobalPrimitiveCache();
	
	GlobalPrimitiveCache(const GlobalPrimitiveCache&);
	GlobalPrimitiveCache& operator=(const GlobalPrimitiveCache&);
	GlobalPrimitiveCache(GlobalPrimitiveCache&&);
	GlobalPrimitiveCache& operator=(GlobalPrimitiveCache&&);

	dbg_poly_line				_line;
	dbg_circle					_circle;
	Graphics::RenderText		_rendText;

	Cache _cache;
	PrimitiveContainer* _currentAct;
};
