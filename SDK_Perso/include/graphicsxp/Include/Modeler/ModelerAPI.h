#ifndef ModelerAPI_H
#define ModelerAPI_H

#ifdef MODELER_EXPORTS
#define MODELER_API __declspec(dllexport)
#else
#define MODELER_API __declspec(dllimport)
#endif

#endif // ModelerAPI_H