#ifndef __wDatabase__
#define __wDatabase__

#include <assert.h>
#include "exception.h"
#include <ed/sstream.h>

template<typename Desc>
class wDescriptorDatabase
{
public:
	typedef ed::string TypeId;
	//SIMPLE_SUBEXCEPTION(Exception, ::Exception)
	wDescriptorDatabase()
	{
	}
	static wDescriptorDatabase<Desc> & instance()
	{
		static wDescriptorDatabase<Desc> database;
		return database;
	}
	void clear()
	{
		storage_.clear();
	}
	inline Desc & reg(const TypeId & typeId)
	{
		std::pair<Storage::iterator, bool> res = storage_.insert(Storage::value_type(typeId, Desc()));
		assert(res.second);
		return res.first->second;
	}
	inline const Desc & getDesc(const TypeId & typeId) const
	{
		Storage::const_iterator itD = storage_.find(typeId);
		if(itD != storage_.end())
			return itD->second;
		else
			throw Exception(FMT("wDescriptorDatabase::getDesc() failed. Descriptor " << typeId.c_str() << " not found"));
	}
private:
	typedef ed::map<TypeId, Desc> Storage;
	Storage storage_;
};

#endif