#ifndef __I18N_H__
#define __I18N_H__


#ifdef I18N_EXPORTS
#define I18N_API __declspec(dllexport)
#else
#define I18N_API __declspec(dllimport)
#endif


/// Initialize internationalization library
I18N_API void initI18n();

/// Returns code of current language
I18N_API const char* getLanguage();

/// Returns code of current country
I18N_API const char* getCountry();

/// Returns name of localized resource
I18N_API void getLocalizedFileName(const char *name, char *buff, int buffSize, const char *prefix = 0);

I18N_API void getLocalizedFolderName(const char *name, char *buff, int buffSize);

#endif
