#ifndef DXRendererAPI_H
#define DXRendererAPI_H

#ifdef DXRENDERER_EXPORTS
#define DXRENDERER_API __declspec(dllexport)
#else
#define DXRENDERER_API __declspec(dllimport)
#endif

#endif // DXRendererAPI_H