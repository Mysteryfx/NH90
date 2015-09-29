#ifndef __wSource__
#define __wSource__

#include "WRadio.h"

#include "wTime.h"
#include "WorldSound.h"
#include "Comm/wMessage.h"

//Sound scheme is a graph of terminal and non-terminal elements of different types: sources, switchers and gain controllers

namespace Sounder {

class ParentElement;

//Base sound element
class WRADIO_API Element
{
public:
	Element();
	virtual ~Element() {;}
	void clear();
	void setInput(ParentElement * pInputIn);
	ParentElement* getInput() const { return pInput_; }
	void setInputGain(float inputGain_);
	Element & operator >> (ParentElement & element_);
	virtual float getGain() const = 0;
	inline float getResultGain() const { return inputGain_ * getGain(); }
#ifdef DEBUG_TEXT
	ed::string name;
#endif
protected:	
	virtual void  update_() = 0;
	float	inputGain_;
	ParentElement* pInput_;
};

//Terminal sound element, can not have children
typedef Element ChildElement;

//Parent sound element, can have children
class WRADIO_API ParentElement : public Element
{
public:
	void clear();
	void insert(ParentElement * pAfter_);
	void addOutput(ChildElement * pOutput_);
	void removeOutput(ChildElement * pOutput_);
	Element & operator << (Element & element_);
protected:
	virtual void  update_();
private:
	typedef ed::list<ChildElement*> Outputs;
	Outputs	outputs;
};

//Switcher - the non-terminal element
class WRADIO_API Switcher : public ParentElement
{
public:
	Switcher() : on_(false) {;}
	virtual ~Switcher() { clear(); }
	void setOnOff(bool onIn);
	inline bool isOn() const { return on_; }
	virtual float getGain() const { return on_ ? 1.0f : 0.0f; }
private:
	bool on_;
};

//Gain controller - the non-terminal element
class WRADIO_API Regulator  : public ParentElement
{
public:
	Regulator() : gain_(0.0f) {;}
	virtual ~Regulator() { clear(); }
	void setGain(float gainIn);
	virtual float getGain() const { return gain_; }	
private:
	float gain_;
};

//Sound source - the terminal element. The source can generate one specific sound.
class WRADIO_API Source : public ChildElement
{
	Source(const Source & other);
	Source & operator = (const Source & other);
public:
	static const float	gainLimit;
	Source();
	virtual ~Source();
	void			create(Sound::Host * pHostIn, const char * nameIn);
	void			update(float gainIn);
	void			update(const Sound::SourceParams& sourceParams);
	void			play_once(float gainIn);
	bool            isPlaying() const {return source_.is_playing();}

	inline void		update() { update_(); }
	virtual float	getGain() const;
protected:	
	virtual void	update_();
	virtual void	play_(const Sound::SourceParams& sourceParams);
	virtual void	stop_();

	Sound::Source	source_;
	float			gain_;
};

bool WRADIO_API sourcePlayMessage(Sound::Source & source, const wMessagePtr & message, const Sound::SourceParams & sourceParams);

//Message sound source - the terminal element. The source uses wMessage to generate sound.
class WRADIO_API MessageSource : public Source
{
public:
	MessageSource();
	virtual ~MessageSource();
	void			create(Sound::Host * pHostIn);
	void			assign(const wMessagePtr & messageIn);
	void			free();
	inline void		stop() { stop_(); }
protected:
	virtual void	update_();
	virtual void	play_(const Sound::SourceParams& sourceParams);
	virtual void	stop_();
private:
	wMessagePtr		message;
};

//Messages sound source - the terminal element. The source uses several wMessage objects at the same time to generate sound.
//Message pool has limited size.
class WRADIO_API MessagesSource : public ParentElement
{
public:
	struct Element
	{
		Element() {;}
		Element(const wMessagePtr & messageIn)
		{
			assign(messageIn);
		}
		inline void		assign(const wMessagePtr & messageIn)
		{
			message = messageIn;
			messageSource.assign(messageIn);
		}
		inline void		free()
		{
			messageSource.free();
			message = NULL;
		}
		inline bool		isFree() const { return message == NULL; }
		inline void		update(float gainIn) { messageSource.update(gainIn); }
		wMessagePtr		message;
		MessageSource	messageSource;
	};
	MessagesSource(size_t sizeIn); //Initialize the source with the size of the pool
	virtual ~MessagesSource();
	void			create(Sound::Host * pHostIn);
	void			update(const wMessagePtr & message, float gainIn);
	void			stop(const wMessagePtr & message);
	virtual float	getGain() const;
	inline Element*	begin() const { return buff_; }
	inline Element*	end() const { return buff_ + size_; }
private:
	Element *		find_(const wMessagePtr & message);
	Element *		findOrTake_(const wMessagePtr & message);
	Element *		buff_;
	size_t			size_;
};

}

#endif