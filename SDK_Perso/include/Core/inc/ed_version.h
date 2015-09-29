#ifndef _ED_VERSION_H_
#define _ED_VERSION_H_

#include "./_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns product version string.
 * Example: 1.2.0.3456
 */
ED_CORE_EXTERN
const char* ED_get_app_version();

/**
 * Writes product version string of given module to \param version.
 * \param maxLength holds max. size of \param version including `\0' character.
 */
ED_CORE_EXTERN
void ED_get_module_version(const wchar_t *moduleFileName, char *version, unsigned int maxLength);

/**
 * Returns CPU architecture
 * Example: x86_64 or x86
 */
ED_CORE_EXTERN
const char* ED_get_cpu_architecture();

/**
 * Returns OS version string.
 * Example: 6.1.7100
 */
ED_CORE_EXTERN
const char* ED_get_os_version();

/**
 * Returns RFC2616-compliant full version string.
 * Example: DCS/1.2.0.3456 (x86_64; Windows/6.1.7100)
 *
 * Setting CURL User Agent:
 *   char user_agent[128];
 *   ED_snprintf(user_agent, sizeof(user_agent), "%s %s", ED_get_full_version(), curl_version());
 *   curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
 */
ED_CORE_EXTERN
const char* ED_get_full_version();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_VERSION_H_ */
