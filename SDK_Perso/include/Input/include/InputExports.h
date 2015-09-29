#pragma once
#ifdef INPUT_EXPORTS
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API __declspec(dllimport)
#endif