#ifndef _ZweiBlauAPI_h_
#define _ZweiBlauAPI_h_

#ifdef ZWEIBLAU_EXPORTS
#define ZWEIBLAU_API __declspec(dllexport)
#else
#define ZWEIBLAU_API __declspec(dllimport)
#endif

#endif