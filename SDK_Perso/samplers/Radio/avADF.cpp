#include "stdafx.h"
#include "Avionics/Radio/avADF.h"

#include <functional>

#include "cLinear.h"

#include "ITerrain.h"

#include "Physic/wRadioReceiver.h"
#include "Physic/wRadioTransmitter.h"

using namespace cockpit;

void avADF::Source::GaussError::reset()
{
	gaussProcessAz.start();
	gaussProcessElev.start();
}

PolarPosition avADF::Source::GaussError::update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt)
{
	float az = gaussProcessAz.process(dt);
	float elev = gaussProcessElev.process(dt);
	return PolarPosition(1.0f, az, elev);
}

avADF::Source::MountainEffect::MountainEffect()
{
	gaussProcessAz.set_Tx(25.0);
	gaussProcessElev.set_Tx(25.0);
}

PolarPosition avADF::Source::MountainEffect::update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt)
{
	wModelTime now = wTime::GetModelTime();
	const cPosition & transmitterPos = pRadioTransmitter->Position();
	const cPosition & selfPos = pRadioReceiver->Position();
	cVector dirToBeaconPoint = transmitterPos.p - selfPos.p;
	const float beaconPointTanElev = dirToBeaconPoint.y / sqrt(dirToBeaconPoint.length2_ZX());
	float maxTanElev = beaconPointTanElev;
	cPoint way[10000];
	const int wayLength = globalLand->Envelope(cPoint(selfPos.p), transmitterPos.p, &(way[0]), 9999);
	for(unsigned int i = 1;
		i < wayLength;
		i++)
	{
		const cVector dirToCurPoint = way[i] - selfPos.p;
		const float tanElev = dirToCurPoint.y / sqrt(dirToCurPoint.length2_ZX());
		maxTanElev = __max(maxTanElev, tanElev);
	}
	if(maxTanElev - beaconPointTanElev > TO_RAD(0.25f))
	{
		const float hRad = __max(1.0f, selfPos.p.y - globalLand->heightSurface(selfPos.p.x, selfPos.p.z));
		const float mountainEffectSx = (TO_RAD(20.0) + TO_RAD(70.0) * __min(1.0f, 300.0f / hRad)) * __min(1.0f, (maxTanElev - beaconPointTanElev) / 0.577f);
		gaussProcessAz.set_sx(mountainEffectSx);
		gaussProcessElev.set_sx(mountainEffectSx);
	}
	else
	{
		gaussProcessAz.set_sx(0.0);
		gaussProcessElev.set_sx(0.0);
	}	
	return GaussError::update(pRadioReceiver, pRadioTransmitter, dt);
}

avADF::Source::NightEffect::NightEffectTable avADF::Source::NightEffect::NightEffectTable::instance;

avADF::Source::NightEffect::NightEffectTable::NightEffectTable()
{
	wModelTime rise_time = 6 * 3600.0;
	wModelTime sunset_time = 23 * 3600.0;
	data.clear();
	data.set_border_type(BORDER_NONE);
	data[fmod(rise_time - 7200.0, 86400.0)]		= TO_RAD(10.0f);
	data[fmod(rise_time, 86400.0)]				= TO_RAD(30.0f);
	data[fmod(rise_time + 7200.0, 86400.0)]		= TO_RAD(0.0f);
	data[fmod(sunset_time - 7200.0, 86400.0)]	= TO_RAD(0.0f);
	data[fmod(sunset_time, 86400.0)]			= TO_RAD(30.0f);
	data[fmod(sunset_time + 7200.0, 86400.0)]	= TO_RAD(10.0f);
	data.setBorders(0.0, 24.0 * 3600.0 - 1.0);
	data.set_border_type(BORDER_CYCLIC);
}

avADF::Source::NightEffect::NightEffect()
{

}

PolarPosition avADF::Source::NightEffect::update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt)
{
	double error = 0.0f;
	const cPosition & selfPos = pRadioReceiver->Position();
	float horDistance = sqrt((pRadioTransmitter->Position().p - selfPos.p).length2_ZX());
	if(horDistance > 100000.0f)
	{
		float distFactor = horDistance < 200000.0f ? (horDistance - 100000.0) / 100000.0 : 1.0f;
		error = NightEffectTable::instance.data.get(wTime::GetAbsoluteModelTime()) * distFactor;
	}	
	gaussProcessAz.set_sx(error);
	gaussProcessElev.set_sx(error);

	return GaussError::update(pRadioReceiver, pRadioTransmitter, dt);
}

avADF::Source::ShoreLineEffect::ShoreLineEffect()
{
	
}

PolarPosition avADF::Source::ShoreLineEffect::update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt)
{
	return GaussError::update(pRadioReceiver, pRadioTransmitter, dt);
}

avADF::Source::Drift::Drift(double limit_) : limit(limit_)
{

}

PolarPosition avADF::Source::Drift::update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt)
{
	double Esum = sqrt(pRadioReceiver->getSumE2());
	float SNR = Esum / (pRadioReceiver->getNoiseLevel() * 0.1f);
	double error = 0.0;
	if(SNR < 1000.0)
	{
		error = 3.14 * exp(-0.289 * SNR);
	}
	error = std::min(error, limit);
	gaussProcessAz.set_sx(error);
	gaussProcessElev.set_sx(error);
	return GaussError::update(pRadioReceiver, pRadioTransmitter, dt);
}

avADF::Source::Vobbling::Vobbling()
{

}

PolarPosition avADF::Source::Vobbling::update(wRadioReceiver * pRadioReceiver, const wRadioTransmitter * pRadioTransmitter, wModelTime dt)
{
	double Esum = sqrt(pRadioReceiver->getSumE2());
	float SNR = Esum / (pRadioReceiver->getNoiseLevel() * 0.1f);
	double error = 0.0;
	if(SNR < 1000.0)
	{
		error = 3.14 * exp(-0.40943 * SNR);
	}
	{
		const cPosition & selfPos = pRadioReceiver->Position();
		const cVector dirToBeaconsPoint = pRadioTransmitter->Position().p - selfPos.p;
		const float distToBeaconsPoint = dirToBeaconsPoint.length();
		if(distToBeaconsPoint < 150.0f)
		{
			error += 3.14159f * (1.0f - distToBeaconsPoint / 150.0f);
		}
	}
	gaussProcessAz.set_sx(error);
	gaussProcessElev.set_sx(error);
	return GaussError::update(pRadioReceiver, pRadioTransmitter, dt);
}

avADF::Source::Source() : pRadioTransmitter(NULL)
{

}

avADF::Source::~Source()
{
	for(Errors::iterator itE = errors.begin();
		itE != errors.end();
		itE++)
	{
		delete (*itE);
	}
	errors.clear();
}

void avADF::Source::initialize(ADF_Range ADF_range)
{
	assert(errors.empty());
	assert(ADF_range != ADF_RANGE_VOID);
	if(ADF_range == ADF_RANGE_LF)
	{
		errors.reserve(5);
		errors.push_back(new MountainEffect());
		errors.push_back(new NightEffect());
		errors.push_back(new ShoreLineEffect());
		errors.push_back(new Drift(TO_RAD(5.0f)));
		errors.push_back(new Vobbling());	
	}
	else if(ADF_range == ADF_RANGE_HF)
	{
	}
}

void avADF::Source::setTransmitter(const wRadioTransmitter * pRadioTransmitterIn)
{
	pRadioTransmitter = pRadioTransmitterIn;	
	resetErrors();
}

void avADF::Source::resetErrors()
{
	std::for_each(errors.begin(), errors.end(), std::mem_fun_t<void, Error>(&Error::reset));
}

cVector avADF::Source::getDirection(wRadioReceiver * pRadioReceiver, wModelTime dt)
{
	assert(pRadioTransmitter != NULL);
	cVector dir = (pRadioTransmitter->Position().p - pRadioReceiver->Position().p).ort();
	PolarPosition polarDir(1.0f, atan2(dir.z, dir.x), atan2(dir.y, sqrt(dir.length2_ZX())));
	PolarPosition errorPolarDir;
	for(Errors::iterator itE = errors.begin();
		itE != errors.end();
		itE++)
	{
		errorPolarDir += (*itE)->update(pRadioReceiver, pRadioTransmitter, dt);
	}
	polarDir += errorPolarDir;
	return cVector(cos(polarDir.azimuth) * cos(polarDir.elevation), sin(polarDir.elevation), sin(polarDir.azimuth) * cos(polarDir.elevation));
}

avADF::avADF(ADF_Range ADF_range) : pRadioReceiver(NULL)
{
	for(int i = 0; i < SOURCE_MAX; i++)
	{
		source[i].initialize(ADF_range);
	}
}

avADF::~avADF()
{

}

void avADF::initialize(wRadioReceiver* pRadioReceiver_, Lua::Config& config)
{
	pRadioReceiver = pRadioReceiver_;
	
	goniometer.l_read(config, "goniometer");
}

void avADF::update(wModelTime dt)
{
	pRadioReceiver->update();
	float bearing = calcBearing(dt);	
	moveArrow(bearing, dt);
}

inline void avADF::test(float req, wModelTime dt)
{
	moveArrow(req, dt);
}

void avADF::release()
{
	pRadioReceiver = NULL;
}

bool avADF::isSignalReceived() const
{
	float treshold = 10.0 * pRadioReceiver->getNoiseLevel();
	treshold *= treshold;
	return pRadioReceiver->getSumE2() > treshold;
}

cVector avADF::calcDirection(wModelTime dt)
{
	const wRadioReceiver::Transmitters & transmitters = pRadioReceiver->getTransmitters();
	if(transmitters.empty())
	{
		return cVector(0.0f);
	}
	
	struct Signal
	{
		Signal() : pRadioTransmitter(NULL), E2(0.0) {;}
		Signal(const wRadioTransmitter * pRadioTransmitterIn, double E2In) : pRadioTransmitter(pRadioTransmitterIn), E2(E2In) {;}
		const wRadioTransmitter *	pRadioTransmitter;
		double						E2;
	};

	const size_t signalsMax = 10;
	size_t signalQty = 0;
	Signal signals[signalsMax];
	double summE2 = 0.0;
	for(wRadioReceiver::Transmitters::const_iterator itT = transmitters.begin();
		itT != transmitters.end() && signalQty < signalsMax;
		itT++)
	{
		double E2 = itT->second * itT->second;
		Signal signal(itT->first, E2);
		signals[signalQty] = signal;
		summE2 += E2;
		signalQty++;
	}

	struct SignalWeaker : public std::binary_function<Signal, Signal, bool>
	{
		inline bool operator()(const Signal & left, const Signal & right) const
		{
			return left.E2 < right.E2;
		}
	};
	std::sort(signals, signals + signalQty, std::not2(SignalWeaker()));

	const size_t strongestSignalQty = std::min(signalQty, (size_t)SOURCE_MAX);
	
	class FindSourceByTransmitter
	{
	public:
		FindSourceByTransmitter(const wRadioTransmitter * pRadioTransmitterIn) : pRadioTransmitter(pRadioTransmitterIn) {;}
		inline bool operator()(const avADF::Source & source) const
		{
			return pRadioTransmitter == source.getTransmitter();
		}
	private:
		const wRadioTransmitter * pRadioTransmitter;
	};
	
	class FindSignalByTransmitter
	{
	public:
		FindSignalByTransmitter(const wRadioTransmitter * pRadioTransmitter_) : pRadioTransmitter(pRadioTransmitter_) {;}
		inline bool operator()(const Signal & signal)
		{
			return pRadioTransmitter == signal.pRadioTransmitter;
		}
	private:
		const wRadioTransmitter * pRadioTransmitter;
	};
	
	class FindUnusedSource
	{
	public:
		FindUnusedSource(Signal * signalsIn, size_t qtyIn) : signals(signalsIn), qty(qtyIn) {;}
		inline bool operator()(const avADF::Source & source) const
		{
			return std::find_if(signals, signals + qty, FindSignalByTransmitter(source.getTransmitter())) == signals + qty;
		}
	private:
		Signal * signals;
		size_t qty;
	};
	
	cVector dir;
	for(size_t i = 0; i < strongestSignalQty; i++)
	{
		Source * pSource = std::find_if(source, source + SOURCE_MAX, FindSourceByTransmitter(signals[i].pRadioTransmitter));
		if(pSource != source + SOURCE_MAX)
		{
		
		}
		else
		{
			pSource = std::find_if(source, source + SOURCE_MAX, FindUnusedSource(signals, strongestSignalQty));
			assert(pSource != source + SOURCE_MAX);
			if(pSource != source + SOURCE_MAX)
				pSource->setTransmitter(signals[i].pRadioTransmitter);			
		}
		double weight = sqrt(signals[i].E2 / summE2);
		dir += weight * pSource->getDirection(pRadioReceiver, dt);
	}
	return dir;
}

float avADF::calcBearing(wModelTime dt)
{
	cVector dir = calcDirection(dt);
	cVector localDir;
	mult_transp(localDir, pRadioReceiver->Position(), dir);
	return atan2(localDir.z, localDir.x);	
}

inline void avADF::moveArrow(float req, wModelTime dt)
{
	goniometer.simulate(req, dt);
}