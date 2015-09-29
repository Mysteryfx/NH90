#ifndef _ed_JsonParser_h_
#define _ed_JsonParser_h_

extern "C"{
#include "yajl/yajl_tree.h"
}

#include "ed/string.h"
#include "ed/vector.h"
#include "exception.h"

#include <stdint.h>

namespace ed {

class JsonError : public Exception {
public:
    JsonError(const ed::string& msg)
        : Exception(msg) {}
};

class JsonNode {
protected:
    yajl_val _node;
public:
    JsonNode() : _node(nullptr) {}
    JsonNode(yajl_val v) : _node(v) {}

    operator bool() const { return _node; }

    yajl_val node() const { return _node; }

    JsonNode find(const char* name, yajl_type t = yajl_t_any)
    {
        const char* path[] = {name, NULL};
        return yajl_tree_get(_node, path, t);
    }

    JsonNode get_value(const char* name, yajl_type t = yajl_t_any)
    {
        const char* path[] = {name, NULL};
        auto res = yajl_tree_get(_node, path, t);
        if (!res)
            throw JsonError(ed::string("Can't get json ") + name);
        return res;
    }

    JsonNode operator[] (const char* field) { return get_value(field); }

    size_t num_items() const
    {
        if (!YAJL_IS_ARRAY(_node))
            return 0;
        return YAJL_GET_ARRAY(_node)->len;
    }

    JsonNode operator[] (size_t item)
    {
        if (item >= num_items())
            throw std::out_of_range("Out of range in JSON array");
        return YAJL_GET_ARRAY(_node)->values[item];
    }

    // ---------------------------------------
    bool try_get(ed::string& val)
    {
        if (!YAJL_IS_STRING(_node))
            return false;
        val = YAJL_GET_STRING(_node);
        return true;
    }

    // ---------------------------------------
    bool try_get(uint64_t& val)
    {
        if (YAJL_IS_NUMBER(_node))
        {
            if (YAJL_GET_INTEGER(_node) < 0)
                return false;
            val = YAJL_GET_INTEGER(_node);
            return true;
        }
        else if (YAJL_IS_STRING(_node))
        {
            try {
                val = std::stoll(YAJL_GET_STRING(_node));
                // may throw std::invalid_argument or std::out_of_range
                return true;
            } catch(const std::logic_error&) {
                return false;
            }
        }
        return false;
    }

    bool try_get(bool& val)
    {
        if (YAJL_IS_TRUE(_node))
            val = true;
        else if (YAJL_IS_FALSE(_node))
            val = false;
        else
            return false;
        return true;
    }

    bool try_get(ed::vector<ed::string>& val)
    {
        if (!YAJL_IS_ARRAY(_node))
        {
            if (YAJL_IS_STRING(_node))
            {
                val.clear();
                val.push_back( YAJL_GET_STRING(_node) );
                return true;
            }
            return false;
        }
        size_t sz = YAJL_GET_ARRAY(_node)->len;
        val.clear();
        val.reserve(sz);
        for (size_t i = 0; i < sz; ++i)
        {
            JsonNode item = YAJL_GET_ARRAY(_node)->values[i];
            val.push_back(item.get<ed::string>());
        }
        return true;
    }


    template<class T>
    T get()
    {
        T res;
        if (!try_get(res))
            throw JsonError("json type error");
        return res;
    }

};

class JsonParser : public JsonNode {
    static yajl_val parse(const ed::string& json_text)
    {
        char err[1024];
        yajl_val root = yajl_tree_parse(json_text.c_str(), err, sizeof(err));
        if (!root)
            throw JsonError(ed::string("JSON error: ") + err);
        return root;
    }

    JsonParser(const JsonParser&);
    void operator=(const JsonParser&);

public:
	JsonParser(const ed::string& json_text)
		: JsonNode(parse(json_text))
	{}

	~JsonParser()
	{
		yajl_tree_free(_node);
	}
};

} // namespace ed

#endif /* _ed_JsonParser_h_ */
