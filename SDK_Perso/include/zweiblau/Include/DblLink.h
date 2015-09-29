#ifndef _DblLink_h_
#define _DblLink_h_

//темплейт реализует дважды-связанный список
//с произвольным доступом. Обращаться к элементам
//можно по индексу, либо по GetNext.
//
//Характерен тем, что не тратит время на поиск
//свободного места при добавлении
//нового элемента, независимо от количества
//и последовательности предыдущих операций
//добавления/удаления. НЕ УМЕЕТ (и не будет)
//расширяться динамически.

//
//(работоспособность проверялась циклом
// из 100,000 операций Add/Delete по рандомному
// индексу). По проблемам использования - обращайтесь.
// Т. Иванов
//
template <class T>

//типа контейнер для элемента дважды-связанного списка
class ListHolder
{
    public:
        T objectum;
        ListHolder* prev;
        ListHolder* next;
        BOOL exist;
       
//
        ListHolder();
        ~ListHolder();
};

//-------------------------------
template <class T>

ListHolder<T>::ListHolder()
{
    
    prev=0;
    next=0;

    exist=FALSE;
}

//-------------------------------
template <class T>

ListHolder<T>::~ListHolder()
{
    
    prev=0;
    next=0;
    exist=FALSE;
}

//-------------------------------
template <class T>

//темплейт реализует дважды-связанный список
//с произвольным доступом. 


class KidList
{

typedef ListHolder<T> Lg;

DWORD max_count;
DWORD elem_count;
DWORD real_count;
Lg first_elem;
Lg* myMem;
Lg* lpFree_start;	//points to first free element
Lg* lpInits_end;	//points to first un-inited element
Lg* lpGlobal_tail;  //points to last element
Lg* lpLocal_tail;	//points to last meaningful (occupied) element
Lg* cur_ptr;

public:
KidList();
~KidList();
void init(DWORD count);  //list size is fixed to "count" and cannot be expanded on the fly.
void uninit();
DWORD Add(T* lpobject); //adds element and returns its index
void Del(DWORD index);  //delete by index
void DelCurrent();      //delete by current position of internal iterator
DWORD GetCount() {return real_count;}  //returns count of occupied elements
void ResetPtr() {cur_ptr=&first_elem;} //resets internal iterator to list.begin
T* GetNext(); //Get element and shift internal iterator to next
T* Get(DWORD index); //get by index

};
///////////////////////////////////


template <class T>

KidList<T>::KidList()
{
max_count=0;
elem_count=0;
real_count=0;
myMem=0;
lpFree_start=myMem;
lpInits_end=myMem;
first_elem.next=myMem;
first_elem.prev=0;
lpGlobal_tail=&first_elem;
lpLocal_tail=&first_elem;
}
//////
template <class T>

void KidList<T>::init(DWORD count)
{
max_count=count;
elem_count=0;
real_count=0;
myMem=new Lg[count+1];
lpFree_start=myMem;
lpInits_end=myMem;
first_elem.next=myMem;
first_elem.prev=0;
lpGlobal_tail=&first_elem;
lpLocal_tail=&first_elem;

memset((void*)myMem,0,count*sizeof(Lg));
}
template <class T>

void KidList<T>::uninit()
{
max_count=0;
elem_count=0;
real_count=0;
delete[] myMem;
myMem=0;
lpFree_start=myMem;
lpInits_end=myMem;
first_elem.next=myMem;
first_elem.prev=0;
lpGlobal_tail=&first_elem;
lpLocal_tail=&first_elem;
}
///////
template <class T>
KidList<T>::~KidList()
{
	if (myMem!=0) delete[] myMem;
}

/////
template <class T>
T* KidList<T>::GetNext()
{
	if (cur_ptr==lpInits_end) return 0;
    cur_ptr=cur_ptr->next;
    if (cur_ptr==lpInits_end) return 0;
	Lg*  reax=cur_ptr;
	return &(reax->objectum);
}

template <class T>
T* KidList<T>::Get(DWORD index)
{
   if (index>=max_count) return 0;
   Lg*  resi;

	if (real_count==0) return 0;// 
	resi=&myMem[index];
    if (!resi->exist) return 0;
    return &(resi->objectum);
}
//////
template <class T>
DWORD KidList<T>::Add(T* lpobject)
{
	if (real_count>=max_count) return -1;
	if (elem_count>max_count) assert(FALSE);
	Lg* reax;
	Lg* redi;
	DWORD index;
	reax=lpFree_start;

	if (lpFree_start>=lpInits_end)
	{
        //assert(FALSE);
		lpGlobal_tail=lpFree_start;
		lpFree_start++;
		lpInits_end=lpFree_start;
		elem_count++;
	}
	else
	{
        lpFree_start=lpFree_start->next;
	}
	redi=lpLocal_tail->next;

    
	redi->prev=lpLocal_tail;
	lpLocal_tail=redi;
	redi->next=lpFree_start;
	redi->objectum=*lpobject;
    redi->exist=TRUE;

    index = DWORD(redi - myMem);
	real_count++;
    return index;
}
//////////////////
template <class T>
void KidList<T>::Del(DWORD index)
{
    if (index>=max_count) return;

	Lg*  reax;
	Lg*  rebx;
	Lg*  resi;

	if (real_count==0) return;
	resi=&myMem[index];
    if (!resi->exist) return;
    resi->exist=FALSE;

    reax=lpLocal_tail;
    if (resi==lpLocal_tail) lpLocal_tail=resi->prev;
    if (resi==lpGlobal_tail)
    {
        //assert(FALSE);
        if (lpGlobal_tail==reax) lpFree_start=resi;
        real_count--;
        return; ////////
    }
	reax=resi->prev;
	rebx=resi->next;
	reax->next=rebx;
	rebx->prev=reax;


	
	reax=lpGlobal_tail;
	rebx=reax->next;
	resi->next=rebx;
	reax->next=resi;
	resi->prev=reax;
	lpGlobal_tail=resi;
	if (reax==lpLocal_tail) lpFree_start=resi;

	real_count--;
	if (real_count==0)
	{
		elem_count=0;
real_count=0;
lpFree_start=myMem;
lpInits_end=myMem;
first_elem.next=myMem;
first_elem.prev=0;
lpGlobal_tail=&first_elem;
lpLocal_tail=&first_elem;
	}
}
///
//////////////////
template <class T>
void KidList<T>::DelCurrent()
{
    if (cur_ptr==&first_elem) return;
    if (cur_ptr==lpInits_end) return;


	Lg*  reax;
	Lg*  rebx;
	Lg*  resi;

	if (real_count==0) return;
	resi=cur_ptr;
    if (!resi->exist) return;
    resi->exist=FALSE;
    reax=lpLocal_tail;
    if (resi==lpLocal_tail) lpLocal_tail=resi->prev;
    if (resi==lpGlobal_tail)
    {
        //assert(FALSE);
        if (lpGlobal_tail==reax) lpFree_start=resi;
        real_count--;
        return; ////////
    }
	reax=resi->prev;
	rebx=resi->next;
	reax->next=rebx;
	rebx->prev=reax;


	
	reax=lpGlobal_tail;
	rebx=reax->next;
	resi->next=rebx;
	reax->next=resi;
	resi->prev=reax;
	lpGlobal_tail=resi;
	if (reax==lpLocal_tail) lpFree_start=resi;

	real_count--;
	if (real_count==0)
	{
		elem_count=0;
real_count=0;
lpFree_start=myMem;
lpInits_end=myMem;
first_elem.next=myMem;
first_elem.prev=0;
lpGlobal_tail=&first_elem;
lpLocal_tail=&first_elem;
	}

}
///
#endif
