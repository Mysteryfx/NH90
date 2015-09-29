#ifndef FM_LOGGER_H
#define FM_LOGGER_H

#include "Base.h"

#include "ed/string.h"
#include "ed/map.h"

#include "Util/Strings.h"
#include <functional>

namespace EagleFM {
namespace Logger {

class FMBASE_API FmLogger
{
public:
	FmLogger(void);
	~FmLogger(void);

	typedef std::function<ed::string()> LogFunc;

	typedef struct LogCat{
		LogCat() {}
		ed::map<ed::string,LogFunc> values_;
	};

	typedef ed::map<ed::string,LogCat> LogCats;

	void register_(const ed::string& cathegory,
		const ed::string& name, const LogFunc log_func);

	void write(const ed::string& cathegory);

private:

	LogCats log_cathegories_;
	bool first_run_;
};

#ifdef _ED_HIDDEN_DEBUG 
#define FM_D_LOGGER_DECLARE(name) Logger::FmLogger name##LOGGER_;
#else
#define FM_D_LOGGER_DECLARE(name)
#endif

#ifdef _ED_HIDDEN_DEBUG 
#define FM_D_LOGGER_REGISTER(name,cat_n,var_n,format,var) name##LOGGER_.register_(cat_n,var_n,##[this]\
{return ed::string_printf(format,this->##var##); });
#else
#define FM_D_LOGGER_REGISTER(name,cat_n,var_n,format,var)
#endif

#ifdef _ED_HIDDEN_DEBUG 
#define FM_D_LOGGER_REGISTER_LAMBDA(name,cat_n,var_n,format,lambda) name##LOGGER_.register_(cat_n,var_n,lambda);
#else
#define FM_D_LOGGER_REGISTER_LAMBDA(name,cat_n,var_n,format,var)
#endif


#ifdef _ED_HIDDEN_DEBUG 
#define FM_D_LOGGER_WRITE(name,cat_n)  name##LOGGER_.write(cat_n);
#else
#define FM_D_LOGGER_WRITE(name,cat_n)
#endif

}}

#endif

