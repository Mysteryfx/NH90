#ifndef __avRadioMeasureEquipment__
#define __avRadioMeasureEquipment__

#include "wRadio.h"

#include "Physic/wRadioReceiver.h"
#include "Physic/wRadioSignal.h"
#include "Utilities/Parameter.h"
#ifdef DEBUG_TEXT
#include "Utilities/Printer.h"
#endif DEBUG_TEXT


struct lua_State;

class WRADIO_API avRadioMeasureEquipment
{
public:
	struct Measurement;

	class Functor
	{
	public:
		virtual ~Functor() {;}
		virtual void operator()(wRadioReceiver & radioReceiver, Measurement & measurement) const = 0;
	};

	struct CheckSignal
	{
		CheckSignal(wRadioSignals signal, double SNR) : signals_(signal), SNR_(SNR) {;}
		bool operator()(const wRadioReceiver & radioReceiver, const wRadioTransmitter * pRadioTransmitter, double E) const;
	private:
		unsigned int	signals_;
		double			SNR_;
	};

	class WRADIO_API TrueBearing : public Functor
	{
	public:
		TrueBearing(const CheckSignal & checkSignal) : checkSignal_(checkSignal) {;}
		virtual void operator()(wRadioReceiver & radioReceiver, Measurement & measurement) const;
	private:
		CheckSignal checkSignal_;
	};

	class WRADIO_API MagneticBearing : public Functor
	{
	public:
		MagneticBearing(const CheckSignal & checkSignal) : checkSignal_(checkSignal) {;}
		virtual void operator()(wRadioReceiver & radioReceiver, Measurement & measurement) const;
	private:
		CheckSignal checkSignal_;
	};

	class WRADIO_API Range : public Functor
	{
	public:
		Range(const CheckSignal & checkSignal) : checkSignal_(checkSignal) {;}
		virtual void operator()(wRadioReceiver & radioReceiver, Measurement & measurement) const;
	private:
		CheckSignal checkSignal_;
	};

	class WRADIO_API BackAzimuth : public Functor
	{
	public:
		BackAzimuth(const CheckSignal & checkSignal) : checkSignal_(checkSignal) {;}
		virtual void operator()(wRadioReceiver & radioReceiver, Measurement & measurement) const;
	private:
		CheckSignal checkSignal_;
	};

	class WRADIO_API BackElevation : public Functor
	{
	public:
		BackElevation(const CheckSignal & checkSignal) : checkSignal_(checkSignal) {;}
		virtual void operator()(wRadioReceiver & radioReceiver, Measurement & measurement) const;
	private:
		CheckSignal checkSignal_;
	};

	class WRADIO_API Simple : public Functor
	{
	public:
		Simple() : checkSignal_(0, 0.0f) {;}
		Simple(const CheckSignal & checkSignal) : checkSignal_(checkSignal) {;}
		virtual void operator()(wRadioReceiver & radioReceiver, Measurement & measurement) const;
	private:
		CheckSignal checkSignal_;
	};

	struct Measurement
	{
		Measurement();
		void update(wRadioReceiver & radioReceiver);
		Functor *		functor;
		Parameter		value;
#ifdef DEBUG_TEXT
		ed::string		format;
		double			coeff;
#endif
	};
	typedef ed::map<int, Measurement> Measurements;

	struct Receiver
	{
		typedef ed::vector<unsigned int > MeasurementNums;
		WRADIO_API Receiver();
		void update(Measurements & measurements);
		wRadioReceiver	radioReceiver;
		MeasurementNums	measurementNums;
	};
	typedef ed::map<int, Receiver> Receivers;
public:
	avRadioMeasureEquipment();
	~avRadioMeasureEquipment();
#ifdef DEBUG_TEXT
	void setName(ed::string & name_);
#endif
	void setOnOff(bool onOff_);
	Receiver & addReceiver(unsigned int recNum_, unsigned int measurementNum_, Functor * functorIn);
	Receiver & addReceiver(unsigned int recNum_);
	Measurement & addMeasurement(unsigned int measurementNum_, unsigned int recNum_, Functor * functorIn);
	Receivers & getReceivers();
	const Receivers & getReceivers() const;
	virtual void update();
	const Parameter & getMeasurement(unsigned int measurementNum) const;
#ifdef DEBUG_TEXT
	virtual size_t print(char * buff, size_t size) const;
	Common::Printer *		getPrinter() { return &printer; }
	ed::string		name;
#endif
private:
	Receivers		receivers;
	Measurements	measurements;
#ifdef DEBUG_TEXT
	Common::ObjectPrinter<avRadioMeasureEquipment, 2000> printer;
#endif
};

#endif