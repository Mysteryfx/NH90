#ifndef CONSOLE_ARGS_H
#define CONSOLE_ARGS_H

#include <ed/string.h>
#include <ed/map.h>
#include <ed/vector.h>

#include "../_config.h"
#ifndef ED_CORE_EXPORT
	#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif

#include "../ExtException.h"

namespace ed 
{
	/**
	 * Exception that is thrown if something gone wrong with ConsoleArgs
	 */
	class InvalidConsoleArgsException : public ExtException
	{
	public:
		InvalidConsoleArgsException(const ed::string& reason) : ExtException(reason, "", 0) {}
	};

	/**
	 * Helper class for working with console arguments
	 * All examples are provided for console line:
	 * 
	 * application.exe sometext -keyOne 1.3 -flag -i "D:\file1.txt" -keyTwo 10 20 -keyThree "C:\Temp" -i "D:\file2.txt"
	 */
	class ED_CORE_EXPORT ConsoleArgs
	{
		ed::map<ed::string, ed::vector<ed::string> > params;
		ed::vector<ed::string> args;

		/**
		 * Internal function for initializing params map
		 */
		void init();

	public:

		/**
		 * Constructors
		 */
		ConsoleArgs() {}
		ConsoleArgs(int argc, char *argv[]);
		ConsoleArgs(const ed::string& arg0);
		ConsoleArgs(const ed::string& arg0, const ed::string& arg1);
		ConsoleArgs(const ed::string& arg0, const ed::string& arg1, const ed::string& arg2);
		ConsoleArgs(const ed::vector<ed::string>& args);

		void dump() const;
		/**
		 * Returns index of key
		 *
		 *   args.indexOf("-keyOne")  =  2
		 *   args.indexOf("-i")       =  5
		 *   args.indexOf("-keyFour") = -1
		 *
		 * @note key must begin with '-' and be at least 2 symbols length
		 */
		int indexOf(const ed::string& key) const;

		/**
		 * Returns true if specified key was defined in console line
		 *
		 *   args.defined("-keyOne")  = true
		 *   args.defined("-keyFour") = false
		 *
		 * @note key must begin with '-' and be at least 2 symbols length
		 */
		bool defined(const ed::string& key) const;

		/**
		 * Returns console argument by index
		 *
		 *   args[0]   = "application.exe"
		 *   args[1]   = "sometext"
		 *   args[2]   = "-keyOne"
		 *   args[100] = InvalidConsoleArgsException is thrown
		 *
		 * @note if index is out of bounds InvalidConsoleArgsException is thrown
		 */
		ed::string operator [] (int index) const;

		/**
		 * Returns value for specified flag
		 *    
		 *    args["-keyOne"] = "1.3"
		 *    args["-keyTwo"] = "10"
		 *    args["-i"]      = "D:\file1.txt"
		 *    args["-flag"]   = ""
		 *    args["-abc"]    = ""
		 *
		 * @note returns empty string if key does not exists but
		 *       you should use defined() method instead of args[key] != ""
		 */
		ed::string operator [] (const ed::string& key) const;

		/**
		 * Returns all values for specified flag
		 *
		 *    args["-keyOne"] = { "1.3" }
		 *    args["-keyTwo"] = { "10", "20" }
		 *    args["-i"]      = { "D:\file1.txt", "D:\file2.txt" }
		 */
		ed::vector<ed::string> getAllValues(const ed::string& key) const;

		/**
		 * Returns total number of console arguments
		 *
		 *    args.size() = 14
		 */
		size_t size() const;

		/**
		 * Checks if key is defined and returns it's value converted to integer
		 * Sets value to defaultValue if key is not defined
		 * throws InvalidConsoleArgsException if key's value couldn't be converted to integer
		 */
		void getOptionalInt(const ed::string& key, int& value) const;
		void getOptionalInt(const ed::string& key, int& value, int defaultValue) const;

		/**
		 * Same as getOptionalInt for double
		 */
		void getOptionalDouble(const ed::string& key, double& value) const;
		void getOptionalDouble(const ed::string& key, double& value, double defaultValue) const;

		/**
		 * Same as getOptionalInt for string
		 */
		void getOptionalString(const ed::string& key, ed::string& value) const;
		
		/**
		 * Don't use this if you don't know what this method is doing
		 */
		void pop();

		/**
		 * Don't use this if you don't know what this method is doing
		 */
		void removeKey(const ed::string& key);
	};
}

#endif