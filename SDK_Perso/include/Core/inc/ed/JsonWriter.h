#ifndef _ed_JsonWriter_h_
#define _ed_JsonWriter_h_

#include "ed/string.h"
#include "ed/vector.h"
#include "ed/unordered_map.h"
#include "osg/Matrixd"
#include "osg/Matrixf"

#include "Util/Strings.h"

extern "C"{
#include "yajl/yajl_gen.h"
}

namespace ed {

class JsonWriter {
    yajl_gen _gen;
public:
    JsonWriter()
    {
        _gen = yajl_gen_alloc(NULL);
        yajl_gen_config(_gen, yajl_gen_beautify, 1);
        yajl_gen_config(_gen, yajl_gen_indent_string, " ");
    }

    ~JsonWriter()
    {
        yajl_gen_free(_gen);
    }

    ed::string get_contents()
    {
        const unsigned char* buf;
        size_t len;
        yajl_gen_get_buf(_gen, &buf, &len);
        return ed::string(reinterpret_cast<const char*>(buf), len);
    }

    void map_open()
    {
        yajl_gen_map_open(_gen);
    }

    void map_close()
    {
        yajl_gen_map_close(_gen);
    }

    void array_open()
    {
        yajl_gen_array_open(_gen);
    }

    void array_close()
    {
        yajl_gen_array_close(_gen);
    }

    void write(const ed::string& str)
    {
        yajl_gen_string(_gen, reinterpret_cast<const unsigned char*>(str.data()), str.size());
    }

    void write(const char *str)
    {
        yajl_gen_string(_gen, reinterpret_cast<const unsigned char*>(str), strlen(str));
    }
    
    void write(double d)
    {
        yajl_gen_double(_gen, d);
    }
    
    void write(int64_t i)
    {
        yajl_gen_integer(_gen, i);
    }

	void write(uint64_t i)
    {
        yajl_gen_integer(_gen, i);
    }

	void write(int32_t i)
    {
        yajl_gen_integer(_gen, i);
    }

	void write(uint32_t i)
    {
        yajl_gen_integer(_gen, i);
    }

    void write(bool b)
    {
        yajl_gen_bool(_gen, b);
    }

    template<typename T> void write_not_empty(const ed::string& key, T value)
    {
		write(key);
        write(value);
    }
}; // class JsonWriter

} // namespace ed

inline
ed::JsonWriter& operator << (ed::JsonWriter& gen, const ed::string& str)
{
    gen.write(str);
    return gen;
}

inline
ed::JsonWriter& operator << (ed::JsonWriter& gen, const osg::Matrixd &m)
{
	gen.map_open();
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			gen.write_not_empty(ed::string_printf("m%d%d", i, j), m(i,j));
		}
	}
    gen.map_close();
    return gen;
}

inline
ed::JsonWriter& operator << (ed::JsonWriter& gen, const osg::Matrixf &m)
{
	gen.map_open();
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			gen.write_not_empty(ed::string_printf("m%d%d", i, j), m(i,j));
		}
	}
    gen.map_close();
    return gen;
}

template<typename T>
ed::JsonWriter& operator << (ed::JsonWriter& gen, T v)
{
    gen.write(v);
    return gen;
}

template<typename T>
ed::JsonWriter& operator << (ed::JsonWriter& gen, const ed::vector<T> &v)
{
	gen.array_open();
	for(const auto &i : v){
	    gen << i;
	}
    gen.array_close();
    return gen;
}

template<typename T>
ed::JsonWriter& operator << (ed::JsonWriter& gen, const ed::unordered_map<ed::string, T> &v)
{
	gen.map_open();
	for(const auto &i : v){
	    gen << i.first;
	    gen << i.second;
	}
    gen.map_close();
    return gen;
}

#endif /* _ed_JsonWriter_h_ */
