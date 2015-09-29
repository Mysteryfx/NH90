#ifndef __wRadioSignal__
#define __wRadioSignal__

//Each signal belongs to one or more of these types
enum wRadioSignal {
	SIGNAL_NULL				= 0,
	SIGNAL_VOICE_AM			= 1 << 0,
	SIGNAL_VOICE_FM			= 1 << 1,
	SIGNAL_TACAN_BEARING	= 1 << 2,
	SIGNAL_TACAN_RANGE		= 1 << 3,
	SIGNAL_TACAN_RANGE_AA	= 1 << 4,
	SIGNAL_TACAN_X			= 1 << 5,
	SIGNAL_TACAN_Y			= 1 << 6,
	SIGNAL_RSBN_RANGE		= 1 << 7,
	SIGNAL_RSBN_BEARING		= 1 << 8,
	SIGNAL_VOR				= 1 << 9,
	SIGNAL_DVOR				= 1 << 10,
	SIGNAL_DME				= 1 << 11,
	SIGNAL_ILS_LOCALIZER	= 1 << 12,
	SIGNAL_ILS_GLIDESLOPE	= 1 << 13,
	SIGNAL_PRMG_LOCALIZER	= 1 << 14,
	SIGNAL_PRMG_GLIDESLOPE	= 1 << 15,
	SIGNAL_INNER_MARKER		= 1 << 16,		
	SIGNAL_MIDDLE_MARKER	= 1 << 17,
	SIGNAL_OUTER_MARKER		= 1 << 18
};

typedef unsigned long long wRadioSignals;

const wRadioSignals SIGNAL_ALL = std::numeric_limits<wRadioSignals>::max();

#endif __wRadioSignal__