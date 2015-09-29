#ifndef ED_STOPWATCH_H
#define ED_STOPWATCH_H

#include <ed/string.h>
#include <ed_time.h>

#include "../_config.h"
#ifndef ED_CORE_EXPORT
	#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif

namespace ed
{
	/**
	 * Helper class for measuring time
	 */
	class Stopwatch
	{
	public:

		/// Construct object and calls method start.
		inline Stopwatch():_start(ED_get_time()){}

		/// Starts counting.
		inline void start(){_start = ED_get_time();}

		/// Returns start time in seconds since Epoch.
		inline double getStartTime()const{return _start;}

		/// Returns current time in seconds since Epoch.
		/// It's just wrapper on get_time().
		inline double getCurrentTime()const{return ED_get_time();}

		/// Returns time in seconds since last call of start method.
		inline double get()const{return ED_get_time() - _start;}

		/**
		 * Get pretty text representation of elapsed time like:
		 *   "1 minute 10 seconds"
		 *   "5 minutes 1 second"
		 *   "1 hour 30 minutes 45 seconds"
		 * and if milliseconds is true:
		 *   "1 minure 10.345 seconds"
		 */
		ED_CORE_EXPORT ed::string prettyText(bool milliseconds = false) const;

	private:
		double _start;
	};
}

#endif
