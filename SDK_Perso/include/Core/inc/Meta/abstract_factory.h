//////////////////////////////////////////////////////////////////////////////////////////
// Эта фабрика написана по следам "Современного проектирования на С++" от Александреску,
// и повторяет то, что описано там, но в упрощенном виде.
// 
// Как этим пользоваться
// 
// Класс AbstractFactory принимает четыре шаблонных аргумента
//   KeyT   - ключ по которому различаются объекты
//   ValueT - класс объектов, которые надо создавать
//   Args   - список типов аргументов, которые передаются в функцию создания объекта
//   ErrorPolicy - политика обработки ошибок
//
// Список типов задается так: TYPE_LISTn(t1, ... tn), где n - это количество типов в 
// списке. Соответственно пустой список выглядит как TYPE_LIST0(), список из двух типов
// выглядит как TYPE_LIST2(int, string), и так далее. Всего поддерживаются списки 
// длиной до четырех.
//
// Регистрация класса в фабрике происходит с помощью функции register_class, которая
// принимает два параметра:
//    key   - ключ класса
//    func  - функтор, который будет вызван для создания объекта данного класса
//
// Функторы - это тоже из Александреску, определены они в файле functor.h. Функтор,
// передаваемый в фабрику должен иметь тип <ValueT*, Args>, то есть возвращать указатель
// на объект нужного класса и принимать аргументы тех типов, которые указаны в типе 
// фабрики. В принципе, если для создания объектов используются статические функции,
// ничего особенного делать не надо, можно просто передать эту функцию в качестве 
// аргумента, она скастится автоматически.
//
// Политика обработки ошибок определяет что делать, если во время работы фабрики произошла
// ошибка. В качестве этого аргумента в шаблон надо передать структуру, которая имеет
// две статических функции - duplicate_class(KeyT) и no_class(KeyT). 
// duplicate_class будет вызвана при попытке зарегистрировать в фабрике два класса с 
// одинаковым именем.
// no_class будет вызвана при попытке создать объект класса, не зарегистрированный в 
// фабрике.
// По умолчанию в обоих случаях бросается исключение.
//
// Пример:
//
//     class TestClass
//     {
//     public:
//         TestClass(int v, const string& v2, double v3)
//         {
//             cout << "Created with " << v << " " << v2 << " " << v3 << endl;
//         }
//     };
// 
//     TestClass* create(int v, const string& v2, double v3)
//     {
//         return new TestClass(v, v2, v3);
//     }
//
//      ...
//
//     struct ErrorPolicy
//     {
//         static void duplicate_class(const ed::string& name)
//         {
//             cout << "Duplicate class: " << name << endl;
//         }
// 
//         static void no_class(const ed::string& name)
//         {
//             cout << "No class: " << name << endl;
//         }
//     };
// 
//     AbstractFactory<string, TestClass*, TYPE_LIST3(int, string, double), ErrorPolicy> factory;
// 
//     factory.register_class("a", create);
//     factory.register_class("a", create);
// 
//     TestClass* o = factory.generate("a", 1, "asdf", 1.5);
//     cout << "o: " << o << endl;
// 
//     TestClass* u = factory.generate("b", 1, "asdf", 1.5);
//     cout << "u: " << u << endl;
//
//  Этот пример выводит:
//     Duplicate class: a
//     Created with 1 asdf 1.5
//     o: 00321230
//     No class: b
//     u: 00000000 
//
//  P.S: Ошибки компиляции могут выглядить весьма занимательно
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ABSTRACT_FACTORY_H__
#define __ABSTRACT_FACTORY_H__

#include "types_list.h"
#include "type_traits.h"
#include "functor.h"

#include "exception.h"

#include <ed/map.h>

namespace CoreMeta
{

SIMPLE_EXCEPTION(FactoryNoClassFoundException);
SIMPLE_EXCEPTION(FactoryDuplicateClassException);

template <class KeyT>
struct FactoryErrorPolicyThrow
{
    typedef typename TypeTraits<KeyT>::parameter_type KeyParamT;

    static void duplicateClass(KeyParamT)
    {
        throw FactoryDuplicateClassException("Duplicate class in factory");
    }

    static void noClass(KeyParamT)
    {
        throw FactoryNoClassFoundException("No class in factory");
    }
};


template <class KeyT, class ValueT, class Args, class ErrorPolicy = FactoryErrorPolicyThrow<KeyT> >
class AbstractFactory 
{
public:
	typedef Functor<ValueT, Args> functor_type;

protected:
	typedef ed::map<KeyT, functor_type> map_type;
	typedef typename TypeTraits<KeyT>::parameter_type KeyParamT;
    
    typedef typename functor_type::Param1 Param1;
    typedef typename functor_type::Param2 Param2;
    typedef typename functor_type::Param3 Param3;
    typedef typename functor_type::Param4 Param4;

protected:
    typename map_type::iterator findGenerator_(KeyParamT key)
    {
    	map_type::iterator i = classes_.find(key);
		if (i == classes_.end())
            ErrorPolicy::noClass(key);

        return i;        
    }
        
public:
    // GENERATORS
	ValueT generate(KeyParamT key)
	{
		map_type::iterator i = findGenerator_(key);
		return i != classes_.end() ? i->second() : 0;
	}
    
    ValueT generate(KeyParamT key, Param1 p1)
	{
		map_type::iterator i = findGenerator_(key);
		return i != classes_.end() ? i->second(p1) : 0;
	}

    ValueT generate(KeyParamT key, Param1 p1, Param2 p2)
	{
		map_type::iterator i = findGenerator_(key);
		return i != classes_.end() ? i->second(p1, p2) : 0;
	}

    ValueT generate(KeyParamT key, Param1 p1, Param2 p2, Param3 p3)
	{
		map_type::iterator i = findGenerator_(key);
		return i != classes_.end() ? i->second(p1, p2, p3) : 0;
	}

    ValueT generate(KeyParamT key, Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{
		map_type::iterator i = findGenerator_(key);
		return i != classes_.end() ? i->second(p1, p2, p3, p4) : 0;
	}
    // END GENERATORS
    
	bool hasClass(KeyParamT key)
	{
		return classes_.count(key)>0;
	}

	void registerClass(KeyParamT key, const functor_type& func)
	{
		if (classes_.find(key) != classes_.end())
            ErrorPolicy::duplicateClass(key);
		else
			classes_.insert(make_pair(key, func));
	}

private:
	map_type classes_;
};

}

#endif
