#pragma once

namespace offshore
{
	struct Scene;
}

namespace render 
{

class IDump
{
public:
	virtual FILE* stream(){return stdout;};
	virtual offshore::Scene* offshore() = 0;
	virtual void dump(const char* format, ...) = 0;
};

} //namespace render