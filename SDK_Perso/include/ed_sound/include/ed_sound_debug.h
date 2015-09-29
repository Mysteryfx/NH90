#ifndef _ed_sound_debug_h_
#define _ed_sound_debug_h_

#ifdef __cplusplus
extern "C" {
#endif

/* internal debug/readback API */

SND_Context_id SND_context_next(SND_Context_id cid);

SND_Host_id SND_host_next(SND_Context_id ctx, SND_Host_id hid);
const char* SND_host_name(SND_Host_id hid);
const struct SND_HostParams* SND_host_params(SND_Host_id hid);
int SND_host_voices(SND_Host_id hid);
double SND_host_radius(SND_Host_id hid);

#ifdef _ED_QTREE_H
ed_qtree_ref SND_context_qtree(SND_Context_id cid);
#endif

//const struct SND_SourceParams* SND_source_params(SND_Source_id sid);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ed_sound_debug_h_ */
