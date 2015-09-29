inline void SetToMax(Vector3& v1, const Vector3& v2)
{
	v1.x = std::max(v1.x, v2.x);
	v1.y = std::max(v1.y, v2.y);
	v1.z = std::max(v1.z, v2.z);
}

inline void SetToMax(Quaternion& v1, const Quaternion& v2)
{
	v1.v.x = std::max(v1.v.x, v2.v.x);
	v1.v.y = std::max(v1.v.y, v2.v.y);
	v1.v.z = std::max(v1.v.z, v2.v.z);
	v1.w = std::max(v1.w, v2.w);
}

template <class T>
void SetToMax(T& v1, const T& v2)
{
	v1 = std::max(v1, v2);
}

inline void SetToMin(Vector3& v1, const Vector3& v2)
{
	v1.x = std::min(v1.x, v2.x);
	v1.y = std::min(v1.y, v2.y);
	v1.z = std::min(v1.z, v2.z);
}

inline void SetToMin(Quaternion& v1, const Quaternion& v2)
{
	v1.v.x = std::min(v1.v.x, v2.v.x);
	v1.v.y = std::min(v1.v.y, v2.v.y);
	v1.v.z = std::min(v1.v.z, v2.v.z);
	v1.w = std::min(v1.w, v2.w);
}


template <class T>
void SetToMin(T& v1, const T& v2)
{
	v1 = std::min(v1, v2);
}

// Animation class

// общая информация
template <class ValueType>
AnimPhase Animation<ValueType>::GetStart()
{
	if(keys.empty()) return 0;
	else return keys.begin()->second.phase;
}

template <class ValueType>
AnimPhase Animation<ValueType>::GetFinish()
{
	if(keys.empty()) return 0;
	else return keys.rbegin()->second.phase;
}

// операции над ключами
template <class ValueType>
void Animation<ValueType>::AddKey(AnimPhase phase, AnimInterpolation itype, ValueType value)
{
	keys[phase] = Key(phase, itype, value);
}

template <class ValueType>
typename Animation<ValueType>::Key *Animation<ValueType>::GetPrevKey(AnimPhase phase)
{
	if(keys.empty()) return 0;
	keyList::iterator i = keys.upper_bound(phase);
	if(i == keys.begin()) return 0;
	else 
	{
		i--;
		return &i->second;
	}
}

template <class ValueType>
typename Animation<ValueType>::Key *Animation<ValueType>::GetNextKey(AnimPhase phase)
{
	keyList::iterator i = keys.upper_bound(phase);
	if(i == keys.end()) return 0;
	else return &i->second;
}

template <class ValueType>
void Animation<ValueType>::DeleteKey(typename Animation<ValueType>::Key *key)
{
	keyList::iterator i = keys.find(key->phase);
	if(i != keys.end()) keys.erase(i);
}

template <class ValueType>
void Animation<ValueType>::ClearKeys()
{
	keys.clear();
}

template <class ValueType>
int Animation<ValueType>::KeyCount()
{
	return (int)keys.size();
}

// индекс динамического параметра
template <class ValueType>
void Animation<ValueType>::SetParamIndex(int param)
{
	param_index = param;
}

template <class ValueType>
int Animation<ValueType>::GetParamIndex()
{
	return param_index;
}

// получить значение
template <class ValueType>
ValueType Animation<ValueType>::GetValue(AnimPhase phase)
{
	Key *prev = GetPrevKey(phase);
	Key *next = GetNextKey(phase);

	if((prev == 0) && (next == 0)) return ValueType();
	if(prev == 0) return next->value;
	if(next == 0) return prev->value;

	float ratio = (phase - prev->phase)/(next->phase - prev->phase);
	return Interpolate(*prev, *next, ratio);
}

template <class ValueType>
ValueType Animation<ValueType>::GetValue(const DParamList& params)
{
	return GetValue((AnimPhase)params[param_index]);
}

// максимум/минимум	(для векторов - поэлементно)
template <class ValueType>
ValueType  Animation<ValueType>::GetMaximum()
{
	keyList::iterator it = keys.begin();
	ValueType v = it->second.value;
	for(; it != keys.end(); it++)
	{
		SetToMax(v, it->second.value);
	}
	return v;
}

template <class ValueType>
ValueType  Animation<ValueType>::GetMinimum()
{
	keyList::iterator it = keys.begin();
	ValueType v = it->second.value;
	for(; it != keys.end(); it++)
	{
		SetToMin(v, it->second.value);
	}
	return v;
}

// сохранение/загрузка
template <class ValueType>
void Animation<ValueType>::serialize(Serializer &serializer)
{
	serializer << keys << param_index;
}

template <class ValueType>
ValueType Animation<ValueType>::Interpolate(const Key& prev, const Key& next, float ratio)
{
	if(prev.itype == aiLinear) return (ValueType)(prev.value*(1-ratio) + next.value*ratio);
	else if(prev.itype == aiConst) return prev.value;
	else return ValueType();
}

Quaternion AnimationQuat::Interpolate(const Key& prev, const Key& next, float ratio)
{
    if(prev.itype == aiLinear)
    {
//        return (prev.value*(1-ratio) + next.value*ratio);
        double cosom = prev.value.v * next.value.v + prev.value.w * next.value.w;
		if ((cosom > 0.99999) || (cosom < -0.99999))    // prev == next
            return prev.value;
        double omega = acos(cosom);
        double t0 = sin((1.0 - ratio) * omega);
        double t1 = sin(ratio * omega);
        return (prev.value*t0 + next.value*t1).normed();
    }
    else if(prev.itype == aiConst) return prev.value;
    else return Quaternion();
}

/*
template <typename T> 
	bool sortKeys (const T& elem1, const T& elem2 )
{
	return elem1.phase > elem2.phase;
}
*/


template <class ValueType>
void Animation<ValueType>::getAnimationVector( ed::vector<typename Animation<ValueType>::Key>& animationVector )
{
	for(keyList::iterator it = keys.begin(); it != keys.end(); it++)
	{
		animationVector.push_back(it->second);
	}
	//std::sort(animationVector.begin(), animationVector.end(), sortKeys<Animation<ValueType>::Key>);
}



template <class ValueType>
bool Graphics::Animation<ValueType>::loadFromConfig( Lua::Config &cfg, const char *name, AnimInterpolation aiType )
{
	assert(!"not implemented");
	return false;
}

template <> inline
bool Graphics::Animation<Vector3>::loadFromConfig( Lua::Config &cfg, const char *name, AnimInterpolation aiType )
{
	if(!cfg.open(name))
		return false;

	const unsigned paramsCount = 4;// {key, x,y,z}
	float params[paramsCount];
	unsigned i = 0;
	while(cfg.open(++i))
	{
		for(unsigned int j = 1; j <= paramsCount; ++j){
			if(!cfg.get(j, &params[j - 1]))
			{
				cfg.pop();
				ClearKeys();
				return false;
			}
		}
		AddKey(params[0], aiType, Vector3(params[1], params[2], params[3]));
		cfg.pop();
	}

	cfg.pop();	
	return true;
}


template <class ValueType>
const char *Graphics::Animation<ValueType>::printKeys()
{
	return 0;
}

template <> inline
const char *Graphics::Animation<Vector3>::printKeys()
{
	static char buf[1024];
	int offset = sprintf(buf, "\nAnimation = {\n");
	for(auto it=keys.begin(); it!=keys.end(); ++it)
	{
		const Vector3 &v = it->second.value;
		offset += sprintf(buf+offset, "\t{%1.3f, %1.3f, %1.3f, %1.3f};\n", it->second.phase, v.x, v.y, v.z);
	}
	sprintf(buf+offset, "};");
	return buf;
}