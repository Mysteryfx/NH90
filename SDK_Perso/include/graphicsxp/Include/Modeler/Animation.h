#ifndef Animation_H
#define Animation_H

#include "Common.h"
#include "Renderer\DynamicParams.h"

// for explicit template instantiation only
#include "vector.h"
#include "quaternion.h"
#include "math_serializer.h"
#include "lua/Config.h"

#include <ed/map.h>

namespace Graphics
{

// тип интерполяции
enum AnimInterpolation
{
	aiConst,	// константное значение до следующего ключа
	aiLinear,	// линейная интерполяция до следующего ключа
};

// фаза анимации (в секундах)
typedef float AnimPhase;

// абстрактный интерфейс для любой анимации
class AnimationBase
{
public:
	virtual ~AnimationBase(){}

	// общая информация
	virtual AnimPhase GetStart() = 0;
	virtual AnimPhase GetFinish() = 0;

	virtual void ClearKeys() = 0;
	virtual int KeyCount() = 0;

	// индекс динамического параметра
	virtual void SetParamIndex(int param) = 0;
	virtual int GetParamIndex() = 0;
};

// Анимация по ключам
template <class ValueType>
class Animation : public Common::Unknown<Serializable>, public AnimationBase
{
public:
	// анимационный ключ
	struct Key
	{
		AnimPhase			phase;
		AnimInterpolation	itype;
		ValueType			value;

		Key() {}
		
		Key(AnimPhase _phase, AnimInterpolation _itype, ValueType _value) :
			phase(_phase), itype(_itype), value(_value) {}
	};
	
	Animation(){
		setIdentifier(typeid(*this).name());
		AddRef();
	}

	// общая информация
	inline AnimPhase GetStart();
	inline AnimPhase GetFinish();

	// операции над ключами
	inline void AddKey(AnimPhase phase, AnimInterpolation itype, ValueType value);
	inline Key *GetPrevKey(AnimPhase phase);
	inline Key *GetNextKey(AnimPhase phase);
	inline void DeleteKey(Key *key);
	inline void ClearKeys();
	inline int KeyCount();

	// индекс динамического параметра
	inline void SetParamIndex(int param);
	inline int GetParamIndex();

	// получить значение
	inline ValueType GetValue(AnimPhase phase);
	inline ValueType GetValue(const DParamList& params);
	
	// максимум/минимум	(для векторов - поэлементно)
	inline ValueType GetMaximum();
	inline ValueType GetMinimum();

	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);
	bool	loadFromConfig(Lua::Config &cfg, const char *name, AnimInterpolation aiType = aiLinear);
	//вернуть текстовую lua таблицу с ключами и их значениями
	const char *printKeys();

	inline void getAnimationVector(ed::vector<Key>& animationVector);

	const ed::map<AnimPhase, Key> &getKeys() const
	{
		return keys;
	}

	ed::map<AnimPhase, Key> &getKeys()
	{
		return keys;
	}
	
protected:
	// список ключей анимации, включая первый и последний
	typedef ed::map<AnimPhase, Key>  keyList;
	keyList keys;
	
	// номер параметра, от которого зависит анимация
	int param_index;

	virtual ValueType Interpolate(const Key& prev, const Key& next, float ratio);
};

typedef Animation<int>			AnimationInt;
typedef Animation<float>		AnimationFloat;
typedef Animation<Vector3>		AnimationVector;

class AnimationQuat : public Animation<Quaternion>
{
    inline Quaternion Interpolate(const Key& prev, const Key& next, float ratio);
};

#include "Animation.inl"

} // namespace Graphics

template <class ValueType>
Serializer &operator<<(Serializer &serializer, typename Graphics::Animation<ValueType>::Key& value);

// non-member operators

template <class ValueType>
Serializer &operator<<(Serializer &serializer, typename Graphics::Animation<ValueType>::Key& value)
{
	return serializer << value.phase << (int &)value.itype << value.value;
}

inline Serializer &operator<<(Serializer &serializer, Graphics::Animation<int>::Key& value)
{
	return serializer << value.phase << (int &)value.itype << value.value;
}

inline Serializer &operator<<(Serializer &serializer, Graphics::Animation<float>::Key& value)
{
	return serializer << value.phase << (int &)value.itype << value.value;
}

inline Serializer &operator<<(Serializer &serializer, Graphics::Animation<Vector3>::Key& value)
{
	return serializer << value.phase << (int &)value.itype << value.value;
}

inline Serializer &operator<<(Serializer &serializer, Graphics::Animation<Quaternion>::Key& value)
{
	return serializer << value.phase << (int &)value.itype << value.value;
}

#endif // Animation_H
