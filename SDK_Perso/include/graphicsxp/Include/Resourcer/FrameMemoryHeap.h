#ifndef _FrameMemoryHeap_h_
#define _FrameMemoryHeap_h_
#include <stddef.h>
#include <assert.h>

#include "graphicsxp.h"

// класс для быстрого выделения памяти сроком на один кадр
// в конце resourcer->Update() вся выделенная память будет уничтожена
// использование: int *i = new(frame_heap) int;
class GRAPHICSXP_API FrameMemoryHeap
{
public:

    FrameMemoryHeap( size_t initsize = 128*1024*1024, size_t initchunks = 4096 );
    ~FrameMemoryHeap();
    
    void * get_memory( size_t n );
    void free_memory( void *ptr, size_t n );
    void reset_memory();
    void release_memory();
    size_t get_max_memory();

	inline void *quick_get_memory(size_t n)
	{
		// align
		n += 0xF;
		n &= ~0xF;

		// data
		char *ptr = m_lastbyte;
		m_lastbyte += n;

		// overflow check
		if(m_lastbyte > m_poolend)
		{
			assert(!"Frame heap overflow");
			return 0;
		}

		return (void*)ptr;
	}

    // stack-like behaviour
    void *getFrame();
    void setFrame(void *p);

private:

    struct free_chunk
    {
        char *m_data;
        size_t m_size;
        free_chunk *m_next, *m_prev;
    };

    void create_pool( size_t bytes );
    void * get_from_pool( size_t n );
    free_chunk * get_chunk( void *ptr, size_t n );
    free_chunk * remove_chunk( free_chunk * chunk );
    void move_left( free_chunk * chunk );
    void move_right( free_chunk * chunk );

    char *m_poolalign, *m_poolbase, *m_lastbyte, *m_poolend;
    size_t m_poolsize;
    
    free_chunk *first_chunk, *last_chunk;

    free_chunk *m_chunks_src;
    size_t      m_first_free_chunk,
                m_chunks_size;

    size_t      min_chunk_size;

    size_t nInitSize,
           nInitChunks;

};

extern GRAPHICSXP_API FrameMemoryHeap frame_heap;

#undef new

// ОСТОРОЖНО: для памяти выделенной таким образом НЕ ДОЛЖЕН вызываться delete
inline void *operator new( size_t n, FrameMemoryHeap &fh )
{
	return fh.quick_get_memory( n );
}

inline void operator delete ( void *ptr, FrameMemoryHeap &fh )
{
}

#endif // _FrameMemoryHeap_h_