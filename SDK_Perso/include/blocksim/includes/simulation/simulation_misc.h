#ifndef __SIMULATION_MISC_H__
#define __SIMULATION_MISC_H__

template <class T>
void delete_all(T** a, int cnt)
{
	if (a == 0) return;
	for (int i=0; i < cnt; i++)
		delete a[i];

	delete[] a;
}

#endif