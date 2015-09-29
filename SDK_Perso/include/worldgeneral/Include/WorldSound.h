#ifndef _ED_A10_SOUND_H
#define _ED_A10_SOUND_H

#include "WorldGeneral.h"

#include "../../ed_sound/include/ed_sound_api.h"
#include "cPosition.h"
#include "dPosition.h"

namespace Sound {

enum ContextID {
    // main contexts
    UI_CONTEXT,
    HEADPHONES_CONTEXT,
    COCKPIT_CONTEXT,
    WORLD_CONTEXT,

    // supplementary contexts
    COCKPIT2D_CONTEXT,
    WORLD2D_CONTEXT,

    // add new contexts BEFORE this line
    NUM_CONTEXTS
};

typedef SND_PlayParams PlayParams;
typedef SND_SourceParams SourceParams;
typedef SND_HostParams HostParams;

#define NO_COPY(cls) \
    private: cls(const cls&); void operator=(const cls&);

// vector adapters for dcs
inline
void set_vector(double pos[3], const dVector& v)
{
    pos[0] = v.x;
    pos[1] = v.y;
    pos[2] = v.z;
}

inline
void set_vector(double pos[3], const cVector& v)
{
    pos[0] = v.x;
    pos[1] = v.y;
    pos[2] = v.z;
}

WORLDGENERAL_API void set_orientation(double rot[4], const dPosition& pos);
WORLDGENERAL_API void set_orientation(double rot[4], const cPosition& pos);

/**
 * Host wrapper
 */
class Host {
    friend class Source;
    NO_COPY(Host)

    SND_Host_id _id;
public:

        Host() : _id(SND_HOST_ID_NONE) {}
        ~Host() { if (_id != SND_HOST_ID_NONE) destroy(); }

    bool is_created() const { return _id != SND_HOST_ID_NONE; }

    WORLDGENERAL_API void create(ContextID ctx, const char *name, const HostParams *params);
    WORLDGENERAL_API void destroy();
    WORLDGENERAL_API void update(const HostParams *params);
};

/**
 * Source wrapper
 */
class Source {
    NO_COPY(Source)

    SND_Source_id _id;
public:

        Source() : _id(SND_SOURCE_ID_NONE) {}
        ~Source() {}

    bool is_created() const { return _id != SND_SOURCE_ID_NONE; }

    /**
     * Create a source using a 'proto' sdef
     */
    WORLDGENERAL_API void create(Host& host, const char *proto, const SourceParams *params=0);

    /**
     * Create a source using a 'proto' sdef and it doesn't exist, fallback to using 'proto_alternative'
     */
	WORLDGENERAL_API void create_alternative(Host& host, const char *proto, const char *proto_alternative, const SourceParams *params=0);

    /**
     * Destroy the source.
     * NOTE: destruction of a host, implicitly destroys ALL its sources.
     */
    WORLDGENERAL_API void destroy();

    /**
     * Check if the source is in 'playing' state.
     */
    WORLDGENERAL_API bool is_playing() const;

    /**
     * Play (restart) source overriding current PlayParams and SourceParams.
     */
    WORLDGENERAL_API void play(const PlayParams *play=0, const SourceParams *params=0);

    /**
     * Play source without restarting (i.e. play only if is not playing).
     */
    WORLDGENERAL_API bool play_continue();

    /**
     * Play (restart) source without looping (overrides source settings).
     */
    WORLDGENERAL_API void play_once(const SourceParams *params=0);

    /**
     * Update source with SourceParams.
     */
    WORLDGENERAL_API void update(const SourceParams *params);

    /**
     * Start playing or update if is playing.
     */
    WORLDGENERAL_API void play_update(const SourceParams *params);

    /**
     * Stop playing source.
     */
    WORLDGENERAL_API void stop();

    /**
     * Link sources, so play/update/stop to either one in a linked chain affects all.
     * Only one of sources may be already linked.
     * @return true on success, false on fail (invalid sources OR both already linked)
     */
    WORLDGENERAL_API bool link(Source& src);

    /**
     * Unlink source from it's chain.
     * @return true on success, false on fail (was not linked or invalid source id)
     */
    WORLDGENERAL_API bool unlink();

};

#undef NO_COPY

}

#endif /* _ED_A10_SOUND_H */
