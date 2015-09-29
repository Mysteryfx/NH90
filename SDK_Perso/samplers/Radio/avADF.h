#ifndef __avADF__
#define __avADF__

#include "Math/Interpolator.h"

#include "Utilities/GaussProcess.h"
#include "Utilities/ccPolarPosition.h"
#include "../avMechanics.h"

class wRadioReceiver;
class wRadioTransmitter;

namespace cockpit
{

enum ADF_Range { ADF_RANGE_VOID = -1, ADF_RANGE_LF, ADF_RANGE_HF };

class COCKPITBASE_API avADF
{
	class Source
	{
	public:
		class Error
		{
		public:
			virtual ~Error() {;}
			virtual void reset() = 0;
			virtual PolarPosition update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt) = 0;
		};
		class GaussError : public Error
		{
		public:
			virtual void reset();
			virtual PolarPosition update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt);
		protected:
			GaussProcess	gaussProcessAz;
			GaussProcess	gaussProcessElev;
		};
		class MountainEffect : public GaussError
		{
		public:
			MountainEffect();
			virtual PolarPosition update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt);
		};
		class NightEffect : public GaussError
		{
		public:
			NightEffect();
			virtual PolarPosition update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt);
		private:
			class NightEffectTable
			{
			public:
				NightEffectTable();
				static NightEffectTable instance;
				MapInterpolator<wModelTime, float> data;
			};		
		};
		class ShoreLineEffect : public GaussError
		{
		public:
			ShoreLineEffect();
			virtual PolarPosition update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt);
		};
		class Drift  : public GaussError
		{
		public:
			Drift(double limit_);
			virtual PolarPosition update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt);	
		private:
			double limit;
		};
		class Vobbling : public GaussError
		{
		public:
			Vobbling();
			virtual PolarPosition update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt);
		};	
	public:
		Source();
		~Source();
		void initialize(ADF_Range ADF_range);
		void setTransmitter(const wRadioTransmitter * pRadioTransmitterIn);
		inline const wRadioTransmitter * getTransmitter() const { return pRadioTransmitter; }
		void resetErrors();
		cVector getDirection(wRadioReceiver * pRadioReceiver, wModelTime dt);
	private:
		const wRadioTransmitter * pRadioTransmitter;
		typedef ed::vector<Error*> Errors;
		Errors				errors;
	};
public:
	avADF(ADF_Range ADF_range);	
	virtual ~avADF();
	void	initialize(wRadioReceiver * pRadioReceiver_, Lua::Config & config);
	void	setReceiver(wRadioReceiver * pRadioReceiver_) { pRadioReceiver = pRadioReceiver_; }
	void	update(wModelTime dt);
	void	test(float req, wModelTime dt);
	void	release();
	bool	isSignalReceived() const;
	float	getBearing() const { return goniometer.get_value_out(); }		
protected:
	float 	calcBearing(wModelTime dt);
	cVector	calcDirection(wModelTime dt);
	void	moveArrow(float req, wModelTime dt);
	wRadioReceiver * 	pRadioReceiver;		
	enum { SOURCE_MAX = 3 };
	Source				source[SOURCE_MAX];
	gauge_periodic goniometer;
};

}

#endif // __avADF__