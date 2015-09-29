/* 
 * ed_sound_api.h
 *
 * Eagle Dynamics' Sound Engine
 * Application Programming Interface
 *
 * Copyright (c) Eagle Dynamics
 * Written by Dmitry Baikov
 */
#ifndef _ed_sound_api_h_
#define _ed_sound_api_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "ed_sound_params.h"

typedef int SND_Context_id;
typedef int SND_Host_id;
typedef int SND_Source_id;

#define SND_CONTEXT_ID_NONE (-1)
#define SND_HOST_ID_NONE (-1)
#define SND_SOURCE_ID_NONE (-1)

// 0.0 for linear scale
// -inf for dB scale
#define SND_ZERO_GAIN 0.0

struct SND_ListenerParams;
struct SND_HostParams;
struct SND_SourceParams;

/* --- Context API --- */

void SND_set_listener(SND_Context_id ctx, const struct SND_ListenerParams *params);


/* --- Host API --- */

SND_Host_id SND_add_host(SND_Context_id cid, const char *name, const struct SND_HostParams *params);
void SND_host_update(SND_Host_id hid, const struct SND_HostParams *params);
void SND_host_del(SND_Host_id hid);


/* --- Source API --- */

SND_Source_id SND_add_source(SND_Host_id hid, const char* proto, const struct SND_SourceParams *params);
SND_Source_id SND_add_source_alt(SND_Host_id hid, const char* proto, const char* alt_proto, const struct SND_SourceParams *params);
void SND_source_play(SND_Source_id src, const struct SND_PlayParams *play, const struct SND_SourceParams *params);
void SND_source_update(SND_Source_id src, const struct SND_SourceParams *params);
void SND_source_stop(SND_Source_id src);
void SND_source_del(SND_Source_id src);
int SND_source_is_playing(SND_Source_id src);

/**
 * Link sources, so play/update/stop to either one in a linked chain affects all.
 * Only one of sources may be already linked.
 * @return 1 on success, 0 on fail (invalid sources OR both already linked)
 */
int SND_source_link(SND_Source_id src1, SND_Source_id src2);

/**
 * Unlink source from it's chain.
 * @return 1 on success, 0 on fail (was not linked or invalid source id)
 */
int SND_source_unlink(SND_Source_id src);


/* -------------------
 * Live streaming API.
 */

/**
 * Push audio buffer to live playing source, each call appends data to source play queue.
 * @return number of frames consumed.
 * @note currently ignores source linking.
 */
unsigned SND_source_live_push(SND_Source_id src,
                                unsigned samplerate, unsigned channels,
                                unsigned frames, const short* data);

/**
 * Get current playing time (in seconds) of the live source.
 */
double SND_source_live_time(SND_Source_id src);


/* --- THE END --- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ed_sound_api_h_ */
