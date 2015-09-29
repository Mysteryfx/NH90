/**
 * @file Signal/Evoker.h
 * @author Dmitry S. Baikov
 */
#ifndef SIGNAL_EVOKER_H
#define SIGNAL_EVOKER_H


#define _SIGNAL_INTERNALS
#include "./config.h"


namespace Signal {


class SIGNAL_API Evoker {
public:
	typedef void (*Callback)(void* client_info, void* signal_info);
	typedef void (*Destructor)(void* client_info);

		Evoker();
		~Evoker();

	/**
	 * insert callback function
	 */
	void insert( Callback call, void* self, Destructor destroy );

	/**
	 * Remove all appropriate callbacks.
	 * @param caller 0 - ignore caller ptr
	 * @param info_key 0 - ignore info
	 * @param key_size 0 - compare info pointers
	 * @return number of removed entries
	 */
	int remove( Callback call, void* info_key, int key_size);

	/**
	 * Remove all callbacks.
	 */
	void clear() { remove(0, 0, 0); }

	/**
	 * Evoker all callbacks with specified argument
	 */
	void evoke(void* signal_info);

private:
	class Impl;
	Impl* self;

		Evoker( const Evoker& );
	void operator = ( const Evoker& );

}; // class Evoker


} // namespace Signal


#endif /* SIGNAL_EVOKER_H */
