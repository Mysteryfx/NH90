#pragma once
#include "./ISounderAPI.h"
#include <cassert>

namespace ed {

class SounderRef {
protected:
    SounderID _id;

public:
    SounderRef()
        : _id(nullptr)
    {}

    SounderRef(SounderID id)
        : _id(id)
    {}

    SounderRef(const char* sounderName)
        : _id(ED_SounderAPI()->getByName(sounderName))
    {
    }

    operator SounderID() const
    {
        return _id;
    }

    bool isCreated() const
    {
        return _id != nullptr;
    }

    bool setParam(SounderParamID p, double val)
    {
        return ED_SounderAPI()->setParam(_id, p, val);
    }

    bool sendEvent(SounderEventID ev)
    {
        return ED_SounderAPI()->sendEvent(_id, ev, 0, nullptr);
    }

    bool sendEvent(SounderEventID ev, double arg1)
    {
        return ED_SounderAPI()->sendEvent(_id, ev, 1, &arg1);
    }

    bool sendEvent(SounderEventID ev, double arg1, double arg2)
    {
        double args[2] = { arg1, arg2 };
        return ED_SounderAPI()->sendEvent(_id, ev, 2, args);
    }

    bool sendEvent(SounderEventID ev, double arg1, double arg2, double arg3)
    {
        double args[3] = { arg1, arg2, arg3 };
        return ED_SounderAPI()->sendEvent(_id, ev, 3, args);
    }

    bool sendEvent(SounderEventID ev, double arg1, double arg2, double arg3, double arg4)
    {
        double args[4] = { arg1, arg2, arg3, arg4 };
        return ED_SounderAPI()->sendEvent(_id, ev, 4, args);
    }

    bool sendEvent(SounderEventID ev, double arg1, double arg2, double arg3, double arg4, double arg5)
    {
        double args[5] = { arg1, arg2, arg3, arg4, arg5 };
        return ED_SounderAPI()->sendEvent(_id, ev, 5, args);
    }

    bool sendEvent(SounderEventID ev, double arg1, double arg2, double arg3, double arg4, double arg5, double arg6)
    {
        double args[6] = { arg1, arg2, arg3, arg4, arg5, arg6 };
        return ED_SounderAPI()->sendEvent(_id, ev, 6, args);
    }

}; // class SounderRef

class Sounder : public SounderRef {
    // no copy
    Sounder(const Sounder&);
    void operator=(const Sounder&);
public:
    Sounder() 
		: SounderRef()
	{}

    Sounder(const char* sounderName, const char* sounderResource)
        : SounderRef()
    {
        create(sounderName, sounderResource);
    }

    Sounder(Sounder&& mix)
    {
        //std::swap(_id, mix._id);
        _id = mix._id;
        mix._id = nullptr;
    }

    ~Sounder()
    {
        destroy();
    }

    bool create(const char* sounderName, const char* sounderResource)
    {
        if (_id)
            return false;
        _id = ED_SounderAPI()->create(sounderName, sounderResource);
        return _id != nullptr;
    }

    bool destroy()
    {
        if (_id)
        {
            const auto res = ED_SounderAPI()->destroy(_id);
            _id = nullptr;
            return res;
        }
        return false;
    }

}; // class Sounder


class SounderParam {
    SounderParamID _id;
    //const char* _name;
public:
        SounderParam()
            : _id(nullptr)
        {}

        SounderParam(SounderParamID id)
            : _id(id)
        {}

        void bind(const char* name)
        {
            if (!_id)
                _id = ED_SounderAPI()->bindParam(name);
        }

        operator SounderParamID() const
        {
            assert (_id);
            return _id;
        }

}; // class SounderParam


class SounderEvent {
    SounderEventID _id;
    //const char* _name;
public:
        SounderEvent()
            : _id(nullptr)
        {}

        SounderEvent(SounderEventID id)
            : _id(id)
        {}

        void bind(const char* name)
        {
            if (!_id)
                _id = ED_SounderAPI()->bindEvent(name);
        }

        operator SounderEventID() const
        {
            assert (_id);
            return _id;
        }

}; // class SounderEvent

} // namespace ed
