/* 
 * ed_sound_ctl.h
 *
 * Eagle Dynamics' Sound Engine
 * Setup and Control Programming Interface
 *
 * Copyright (c) Eagle Dynamics
 * Written by Dmitry Baikov
 */
#ifndef _ed_sound_ctl_h_
#define _ed_sound_ctl_h_

#ifdef __cplusplus
extern "C" {
#endif

/* --- Init API --- */
enum SND_Init {
	SND_INIT_DEFAULT      = 0x00,
	SND_INIT_NUM_CONTEXTS = 0x01,
	SND_INIT_PRELOAD      = 0x02,
	SND_INIT_CACHE_TIME   = 0x04,
    SND_INIT_DEBUG_LEVEL  = 0x08,
};

enum SND_InitPreload {
    SND_INIT_PRELOAD_NO_WAVES = 0,
    SND_INIT_PRELOAD_TRIGGER_WAVES = 1,
    SND_INIT_PRELOAD_ALL_WAVES = 2,

    // load-all mode
    SND_INIT_PRELOAD_EVERYTHING = SND_INIT_PRELOAD_ALL_WAVES
};

struct SND_InitParams {
	int fields;

	/**
	 * Number of contexts.
	 */
	unsigned int num_contexts;

	/**
	 * Preload control.
	 * 0 - load only active sounds.
	 * 1 - load active and preload 'trigger=true' sounds only.
	 * 2 - preload all sounds for created sources (default).
	 * OR with SND_INIT_PRELOAD_PROTO to preload all sdefs (not sounds).
	 */
	int   preload;

	/**
	 * Time (in seconds) to keep unused sounds loaded (default = 30).
	 */
	float cache_time;

    /**
     * Debug messages level (0 - only important errors, 1 - all errors).
     */
    int debug_level;
};

int  SND_init(const struct SND_InitParams *init);
void SND_exit();

int SND_add_wave_path(const char* path);
int SND_del_wave_path(const char* path);

int SND_add_proto_path(const char* path);
int SND_del_proto_path(const char* path);

/* --- Device enumeration API --- */

struct SND_DeviceInfo {
    char system_id[256];
    char user_name[256];
    unsigned int channels;
};

unsigned SND_device_get_count(); // Backend-dependent
int SND_device_get_info(unsigned int id, struct SND_DeviceInfo*); // Backend-dependent

/* --- Control API --- */
enum SND_Start {
    SND_START_SAMPLE_RATE    = 0x01,
    SND_START_BUFFER_SIZE    = 0x02,
    SND_START_DEVICE_NAME    = 0x04,
    SND_START_NUM_CHANNELS   = 0x08,
    SND_START_CHANNEL_MASK   = 0x10
    //SND_START_SPEAKER_CONFIG = 0x20,
};

struct SND_StartParams {
    int fields;

    unsigned sample_rate;
    unsigned buffer_size;
    const char *device_name;
    unsigned num_channels;
    unsigned channel_mask;
    //const char *speaker_config;
};

int SND_start(const struct SND_StartParams *start); // Backend-dependent
void SND_stop(); // Backend-dependent

void SND_process(); // apply updates

int SND_offline_start(unsigned samplerate/*const struct SND_StartParams *start*/);
void SND_offline_render(float *out, unsigned frame_count);

void SND_context_set2d(SND_Context_id ctx, int is2d);
void SND_context_reset(SND_Context_id ctx);

/**
 * Set speed of sound.
 * @param height_speed_pairs pointer to an array of (altitude, speed) pairs ordered by increasing altitude.
 * @param number of points (pairs)
 */
void SND_set_speed_of_sound(SND_Context_id ctx, unsigned num_points, const float *height_speed_pairs);


/* --- THE END --- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ed_sound_ctl_h_ */
