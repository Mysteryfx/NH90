#ifndef _ed_sound_capture_h_
#define _ed_sound_capture_h_

#ifdef __cplusplus
extern "C" {
#endif

enum SND_Format {
	SND_FORMAT_MONO = 1,
	SND_FORMAT_STEREO = 2
};

int SND_capture_init(enum SND_Format fmt);
void SND_capture_exit();

unsigned SND_capture_get_samplerate();

typedef void (*SND_capture_callback)(void *buffer, unsigned nframes, void *userdata);

int SND_capture_start_file(const char *filename);
int SND_capture_start_callback(SND_capture_callback cb, void *userdata);
void SND_capture_stop();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ed_sound_capture_h_ */
