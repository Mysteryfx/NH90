#ifndef _ed_sqlite3_h_
#define _ed_sqlite3_h_

#include "../_config.h"
#include <exception>
#include <ed/string.h>
#include <stdint.h>

struct sqlite3;
struct sqlite3_stmt;

namespace ed {

class SQLite3 {
    sqlite3* _db;
    static void error(int rc, const char* msg=nullptr);
public:
    class Error : public std::runtime_error {
        int _code;
    public:
        ED_CORE_EXPORT Error(int code);

            Error(int code, const char* msg)
                : std::runtime_error(msg), _code(code)
            {}

        int code() const { return _code; }
    };

    class Cursor;

    ED_CORE_EXPORT SQLite3();
    ED_CORE_EXPORT ~SQLite3();
    //ED_CORE_EXPORT SQLite3(const char* db, int flags, const char* vfs=nullptr);

    ED_CORE_EXPORT void exec(const char* sql);
    void exec(const ed::string& sql) { exec(sql.c_str()); }

    friend class Cursor;
}; // class SQLite3

class SQLite3::Cursor {
    sqlite3_stmt* _cur;
    int _bind_index;
    int _column_index;
public:
    Cursor()
        : _cur(nullptr)
        , _bind_index(0)
        , _column_index(0)
    {}

    Cursor(SQLite3& db, const char* sql)
        : _cur(nullptr)
        , _bind_index(0)
        , _column_index(0)
    {
        prepare(db, sql);
    }

    ~Cursor()
    {
        finalize();
    }

    ED_CORE_EXPORT void prepare(SQLite3& db, const char* sql);

    // === i/o
    int next_bind() { return ++_bind_index; }
    int next_column() { return _column_index++; }

    // --- writing
    ED_CORE_EXPORT Cursor& bind(int idx, int val);
    ED_CORE_EXPORT Cursor& bind(int idx, int64_t val);
    ED_CORE_EXPORT Cursor& bind(int idx, double val);
    ED_CORE_EXPORT Cursor& bind(int idx, const char* str, size_t len=-1);
    Cursor& bind(int idx, const ed::string& val) { return bind(idx, val.c_str(), val.size()); }

    // -- reading

    ED_CORE_EXPORT Cursor& column(int idx, int& val);
    ED_CORE_EXPORT Cursor& column(int idx, int64_t& val);
    ED_CORE_EXPORT Cursor& column(int idx, double& val);
    ED_CORE_EXPORT Cursor& column(int idx, ed::string& val);

    // === usage
    ED_CORE_EXPORT bool step();

    ED_CORE_EXPORT void reset();
    ED_CORE_EXPORT void finalize();
};

} // namespace ed

// --- writing
inline ed::SQLite3::Cursor&
operator << (ed::SQLite3::Cursor& cur, int val)
{ return cur.bind(cur.next_bind(), val); }

inline ed::SQLite3::Cursor&
operator << (ed::SQLite3::Cursor& cur, int64_t val)
{ return cur.bind(cur.next_bind(), val); }

inline ed::SQLite3::Cursor&
operator << (ed::SQLite3::Cursor& cur, double val)
{ return cur.bind(cur.next_bind(), val); }

inline ed::SQLite3::Cursor&
operator << (ed::SQLite3::Cursor& cur, const ed::string& val)
{ return cur.bind(cur.next_bind(), val); }

// --- reading
inline ed::SQLite3::Cursor&
operator >> (ed::SQLite3::Cursor& cur, int& val)
{ return cur.column(cur.next_column(), val); }

inline ed::SQLite3::Cursor&
operator >> (ed::SQLite3::Cursor& cur, int64_t& val)
{ return cur.column(cur.next_column(), val); }

inline ed::SQLite3::Cursor&
operator >> (ed::SQLite3::Cursor& cur, double& val)
{ return cur.column(cur.next_column(), val); }

inline ed::SQLite3::Cursor&
operator >> (ed::SQLite3::Cursor& cur, ed::string& val)
{ return cur.column(cur.next_column(), val); }

#endif /* _ed_sqlite3_h_ */
