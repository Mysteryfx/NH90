/**
 * @file Property.h
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_PROPERTY_H
#define GENUI_PROPERTY_H


namespace UI {


/**
 * Helper class to implement properties (intercept get/set functions in implicit way)
 * example:
 * class Test {
 *     int value;
 * public:
 *     // get-set interface
 *     int getValue() const { return value; }
 *     void setValue(int val) { value = val; }
 *
 *     // property
 *     int value() const { return getValue(); }
 *     Property<Test, int, int> value() { return Property<Test, int, int>(this, getValue, setValue); }
 * };
 *
 * now we can simple write:
 *     Test a, b;
 *     a.value() = b.value();
 * which is equivalent to:
 *     Test a, b;
 *     a.setValue( b.getValue() );
 * and dummy object will be completely optimized away.
 *
 * @todo operator = () should return smth more reasonable than void.
 * may be InType (so , the value would nopt be filtered (bad?) but we'll avoid
 * unnesseccary (i think so) call to getValue().
 * let's test it.
 */
template<class Object, class OutType, class InType>
class Property {
private:
	typedef OutType (Object::*Getter)() const;
	typedef void (Object::*Setter)(InType);

	Object *m_object;
	Getter m_get;
	Setter m_set;

public:
		Property(Object* obj, Getter get, Setter set)
			: m_object(obj), m_get(get), m_set(set) {}

		operator OutType() const { return (m_object->*m_get)(); }
		InType operator = (InType value) { (m_object->*m_set)(value); return value; }

}; // class Property<Object, OutType, InType>


#define _GENUI_PROPERTY(Class, Name, Getter, OutType, Setter, InType) \
	OutType Name () const { return Getter(); } \
	Property<Class, OutType, InType> Name () \
		{ return Property<Class, OutType, InType>(this, Getter, Setter); }


} // namespace UI

#endif /* GENUI_PROPERTY_H */
