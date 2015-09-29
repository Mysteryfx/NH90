#ifndef _ED_LUA_H_
#define _ED_LUA_H_

#include "./_config.h"
#include "ed_defs.h"

#ifdef lua_hpp
#error Previously included "lua.hpp" detected! Please, remove.
#endif

EXTERN_C_BEGIN

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

// prevent include
#define lua_hpp


/**
 * Create lua state.
 * Replaces lua_open(), lua_newstate(), luaL_newstate()
 */
ED_CORE_EXTERN
struct lua_State* ED_lua_open(const char *name);

/**
 * Close Lua state.
 * Completely replaces lua_close().
 * Dumps usage statistics.
 */
ED_CORE_EXTERN
void ED_lua_close(struct lua_State *);

/**
 * Load Lua file.
 * Completely replaces luaL_loadfile().
 */
ED_CORE_EXTERN
int ED_lua_loadfile(struct lua_State *L, const char *file);

/**
 * Exec Lua file.
 * Completely replaces luaL_dofile().
 */
ED_CORE_EXTERN
int ED_lua_dofile(struct lua_State *L, const char *file);

/* =================================== */

/**
 * Open base libs: _G and coroutine
 * print() - redirected to ed_log(state name)
 * dofile, loadfile routed via VFS
 */
ED_CORE_EXTERN
int ED_luaopen_base(struct lua_State *L);

/**
 * Open package library.
 * require(), package.loaders[2]() routed via VFS
 */
ED_CORE_EXTERN
int ED_luaopen_package(struct lua_State *L);

/**
 * Open IO library (VFS)
 */
ED_CORE_EXTERN
int ED_luaopen_io(struct lua_State *L);

/**
 * Open OS and LFS libraries routed to VFS.
 */
ED_CORE_EXTERN
int ED_luaopen_os(struct lua_State *L);

/**
 * Opens debug library.
 * Removes debug.getregistry(), debug.getupvalue(), debug.setupvalue()
 */
ED_CORE_EXTERN
int ED_luaopen_debug(struct lua_State *L);

/**
 * Open all std libs + LFS
 */
ED_CORE_EXTERN
void ED_lua_openlibs(struct lua_State *L);

/**
 *Copy table to other state
 */
ED_CORE_EXTERN
void ED_lua_copyindex(lua_State * dst,lua_State * src, int src_index);

enum ED_LUA_SandboxLevel {
    /**
     * No sandboxing.
     * Full system access.
     */
    ED_LUA_SANDBOX_OFF,

    /**
     * Light sandboxing. Allows:
     * reads from app folder, write folder
     * writes, removes from write folder
     *
     */
    ED_LUA_SANDBOX_RELAXED,

    /**
     * Full sandboxing. No io, lfs, os, debug. (Default setting).
     */
    ED_LUA_SANDBOX_STRICT,
};

ED_CORE_EXTERN
void ED_lua_setsandbox(lua_State *L, int level);

ED_CORE_EXTERN
int ED_lua_getsandbox(lua_State *L);

/* =================================== */

#ifndef _ED_LUA_INTERNALS_
// Hook lua.h
#undef lua_open
#ifdef __FUNCTION__
#define lua_open() ED_lua_open(__FUNCTION__)
#else
#define lua_open() ED_lua_open(__FILE__)
#endif

#define lua_newstate(a, u)  lua_open()
#define luaL_newstate()  lua_open()

#define lua_close(s) ED_lua_close(s)

#define lua_call(L, n, r) (use_lua_pcall[])

#define luaL_loadfile(L, f) ED_lua_loadfile(L, f)

#undef luaL_dofile
#define luaL_dofile(L, filename) \
    (ED_lua_loadfile(L, filename) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define luaopen_base(L) ED_luaopen_base(L)
#define luaopen_package(L) ED_luaopen_package(L)
#define luaopen_io(L) ED_luaopen_io(L)
#define luaopen_os(L) ED_luaopen_os(L)
#define luaopen_debug(L) ED_luaopen_debug(L)

#define luaL_openlibs(L) ED_lua_openlibs(L)
#endif

EXTERN_C_END

#endif /* _ED_LUA_H_ */
