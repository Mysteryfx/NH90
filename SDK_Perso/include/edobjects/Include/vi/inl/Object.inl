inline vi::Object::Object(viObject* instancedobject)
{
	this->reftype = INSTANCEDOBJECT;
	this->ref.instancedobject = instancedobject;
}
inline vi::Object::Object(vi::StaticObject::STORAGE storage, uint32_t id)
{
	this->reftype = STATICOBJECT;
	this->ref.staticobject.storage = storage;
	this->ref.staticobject.id = id;
}


inline ObjectID vi::Object::ID()
{
	return this->instance()->ID();
}

inline cPosition &vi::Object::Position(wModelTime t)
{
	return this->instance()->Position(t);
}
inline const gBox &vi::Object::LocalBox(wModelTime t)
{
	return this->instance()->LocalBox(t);
}
// парсинг (отрисовка, проверка столкновений и т.д.)
inline void vi::Object::Parse(
	Graphics::ModelParser& parser,
	const Position3& pos,
	Graphics::effectState* effects
	)
{
	this->instance()->Parse(parser,pos, effects);
}


inline int vi::queryObjects( ClipVolume *volume, QueryHandler query, void *data, viFlag flags)
{
	assert( vi::Manager::instance);
	return vi::Manager::instance->queryObjects( volume, query, data, flags);
}

