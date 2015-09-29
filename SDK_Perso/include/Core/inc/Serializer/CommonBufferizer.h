#ifndef CommonBufferizer_H
#define CommonBufferizer_H

#include "CommonSerializer.h"

namespace Common {

class CommonBufferizer : public CommonSerializer
{
protected:
	void *buffer;
	unsigned m_size, m_max_size;

    virtual void            serialize(void *data, size_t size);

public:
	// фабрика для обхода проблемы выравнивания
	ED_CORE_EXTERN static CommonBufferizer * Create();

	ED_CORE_EXTERN CommonBufferizer();
	ED_CORE_EXTERN ~CommonBufferizer();

	// начать - окончить сериализацию
	ED_CORE_EXTERN void Open(Mode mode, void *mem, unsigned max_size);
	ED_CORE_EXTERN void Close(unsigned &size);
};

} // namespace Common;

#endif // CommonBufferizer_H