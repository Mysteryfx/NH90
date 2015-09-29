model::ProxyManager::~ProxyManager()
{
	for(auto i : managers){
		delete i;
	}
}

void model::ProxyManager::add(Manager *mng)
{
	if(mng == nullptr){
		return;
	}

	for(auto i : managers){
		if(i == mng){
			return;
		}
	}

	managers.push_back();

}

Res* model::ProxyManager::open(const char* name)
{
	for(auto i : managers){
		auto res = i->open(name);
		if(res != nullptr){
			return res;
		}
	}
	return nullptr;
}
