#ifndef UI_SOUND_H
#define UI_SOUND_H

namespace UI {

class Sound {
protected:
	virtual ~Sound() {}
public:
	virtual void play(const char *sound) = 0;
};

}

#endif /* UI_SOUND_H */
