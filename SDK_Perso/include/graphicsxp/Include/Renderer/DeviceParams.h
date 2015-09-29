#ifndef DeviceParams_H
#define DeviceParams_H

#include <ed/string.h>
#include <ed/list.h>

#pragma pack (8)
namespace Graphics
{

// Дескриптор графического режима
struct GraphicsMode
{
	int width, height;
	unsigned bpp;
    unsigned maxdepth;
	int frequency;
};

typedef ed::list<GraphicsMode> modeList;

// Дескриптор адаптера
struct AdapterDescriptor
{
	ed::string name;
    unsigned id;
	modeList modes;
    bool CanDoWindowed;
};

typedef ed::list<AdapterDescriptor> adapterList;

// Дескриптор графической системы компьютера
struct SystemDescriptor
{
	adapterList adapters;
};

}
#pragma pack ()

#endif // DeviceParams_H