#ifndef __TYPES_LIST_H__
#define __TYPES_LIST_H__

namespace CoreMeta
{

class NullType {};

template <class Type, class Tail>
struct TypeList
{
	typedef Type value;
	typedef Tail tail;
};

}

#define TYPE_LIST0() CoreMeta::TypeList<CoreMeta::NullType, CoreMeta::NullType>
#define TYPE_LIST1(t1) CoreMeta::TypeList<t1, CoreMeta::NullType>
#define TYPE_LIST2(t1,t2) CoreMeta::TypeList<t1, TYPE_LIST1(t2) >
#define TYPE_LIST3(t1,t2,t3) CoreMeta::TypeList<t1, TYPE_LIST2(t2,t3) >
#define TYPE_LIST4(t1,t2,t3,t4) CoreMeta::TypeList<t1, TYPE_LIST3(t2,t3,t4) >
#define TYPE_LIST5(t1,t2,t3,t4,t5) CoreMeta::TypeList<t1, TYPE_LIST4(t2,t3,t4,t5) >
#define TYPE_LIST6(t1,t2,t3,t4,t5,t6) CoreMeta::TypeList<t1, TYPE_LIST5(t2,t3,t4,t5,t6) >
#define TYPE_LIST7(t1,t2,t3,t4,t5,t6,t7) CoreMeta::TypeList<t1, TYPE_LIST6(t2,t3,t4,t5,t6,t7) >
#define TYPE_LIST8(t1,t2,t3,t4,t5,t6,t7,t8) CoreMeta::TypeList<t1, TYPE_LIST7(t2,t3,t4,t5,t6,t7,t8) >
#define TYPE_LIST9(t1,t2,t3,t4,t5,t6,t7,t8,t9) CoreMeta::TypeList<t1, TYPE_LIST8(t2,t3,t4,t5,t6,t7,t8,t9) >

#endif