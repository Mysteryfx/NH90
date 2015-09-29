#ifndef __EFFECTS_CONFIG_H__
#define __EFFECTS_CONFIG_H__

#ifdef EFFECTS_EXPORTS
#define EFFECTS_API __declspec(dllexport)
#else
#define EFFECTS_API __declspec(dllimport)
#endif

#endif
