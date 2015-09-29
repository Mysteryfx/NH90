inline vi::LandObject::LandObject(const vi::Object& obj)
{
	const Base<viObject>* x = &obj;
	cast(*x);
}
inline bool vi::LandObject::isValid()
{
	return vi::Base<lLandObject>::isValid();
}
inline wShape* vi::LandObject::Type() const
{
	return instance()->Type();
}
