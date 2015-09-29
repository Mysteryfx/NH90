#ifndef RendererAPI_H
#define RendererAPI_H

#ifdef RENDERER_XP_EXPORTS
#define RENDERER_XP_API __declspec(dllexport)
#else
#define RENDERER_XP_API __declspec(dllimport)
#endif

#endif // RendererAPI_H