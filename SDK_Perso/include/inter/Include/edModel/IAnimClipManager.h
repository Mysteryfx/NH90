#ifndef IAnimClipManager_H__
#define IAnimClipManager_H__

namespace model
{

class IAnimClip;
class IAnimScript;

// Гард от креша при изменении интерфейса
const double IAnimClipManagerVersion = 0.3;

/// Animation clip manager.
class IAnimClipManager
{
protected:
	virtual ~IAnimClipManager() {}
public:
	/// Stored already loaded AnimClip in hash, so you never load one AnimClip twice.
	/// If there is no such AnimClip on disk returns NULL
	virtual IAnimClip* open(const char* fileName)=0;

	/// Анимационный сценарий
	virtual IAnimScript* openScript(const char* fileName)=0;
};

/*/
inline BlendArg::BlendArg()
{
	clip = 0;
	time = 0;
	weight = 0;
}
/*/

}

#endif
