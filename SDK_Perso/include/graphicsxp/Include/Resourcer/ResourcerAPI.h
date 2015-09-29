#ifndef ResourcerAPI_H
#define ResourcerAPI_H

#ifdef RESOURCER_EXPORTS
#define RESOURCER_API __declspec(dllexport)
#else
#define RESOURCER_API __declspec(dllimport)
#endif

#endif // ResourcerAPI_H