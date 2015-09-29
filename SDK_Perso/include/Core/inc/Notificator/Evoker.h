#ifndef __Evoker_h__
#define __Evoker_h__

#include <ed/vector.h>

/** 
 * The Evoker class supports notifying with deffered subscribing.
 * To use it you should do the following:
 * <pre><dir>
 * @@ 1. Simple variant:
 * @@ class YourEvoker;
 * @@ class YourListener 
 * @@ { 
 * @@ public: 
 * @@   virtual void anyMethod(YourEvoker *) = 0; 
 * @@ };
 * @@ 
 * @@ class YourEvoker: public Evoker<YourListener, YourEvoker *>
 * @@ {
 * @@ public:
 * @@   void yourMainMethod()
 * @@   {
 * @@       evoke(YourListener::anyMethod);
 * @@   }
 * @@ };
 * @@                       or better for DLL_EXPORT
 * @@ class YourEvoker
 * @@ {
 * @@   Evoker<YourListener, YourEvoker *>  evoker;
 * @@ public:
 * @@   void yourMainMethod()
 * @@   {
 * @@       evoker.evoke(YourListener::anyMethod, this);
 * @@   }
 * @@ };
 * @@ 
 * @@ 2. Advanced variant:
 * @@ class AnyClass;
 * @@ class YourListener 
 * @@ { 
 * @@ public: 
 * @@   virtual void anyMethod(AnyClass) = 0; 
 * @@ };
 * @@ 
 * @@ class YourEvoker
 * @@ {
 * @@   Evoker<YourListener, AnyClass>  evoker;
 * @@ public:
 * @@   void yourMainMethod()
 * @@   {
 * @@       AnyClass  anyClass;
 * @@       evoker.evoke(YourListener::anyMethod, anyClass);
 * @@   }
 * @@ };
 * @@ 
 * @@ 3. Expert variant:
 * @@ class AnyClass;
 * @@ class YourListener: public IUnknown {...}
 * @@ ...  Evoker<YourListener, AnyClass, CComPtr<YourListener> >  evoker;
 * @@ 
 * </pre></dir>
 * @author Alexander Matveev, Dmitry Baikov
 */
template <class T, class Ptr = T *>
class Evoker
{
    typedef typename ed::vector<Ptr> listeners_array;
protected:
	unsigned int        evokedCounter;
	bool                canceled;
	listeners_array    listeners;
	listeners_array    addedListeners;
	listeners_array    removedListeners;

    void    removeAll(typename listeners_array &c, const Ptr & value)
	{
		typename listeners_array::iterator iter = c.begin();
		while (iter != c.end())
		{
			if (*iter == value)
			{
				iter = c.erase(iter);
				continue;
			}
			iter++;
		}
	}

	void remove(typename listeners_array &c, const typename listeners_array& r)
	{
		typename listeners_array::const_iterator iter = r.begin();
		for (; iter != r.end(); iter++)
		{
			removeAll(c, *iter);
		}
	}

	void merge(typename listeners_array &c, const typename listeners_array& r)
	{
		typename listeners_array::const_iterator iter = r.begin();
		for (; iter != r.end(); iter++)
		{
            if ((*iter) != NULL)
			    c.push_back(*iter);
		}
	}
private:
    void onStartEvoke()
    {
        evokedCounter++;
    }
    void onFinishEvoke()
    {
        canceled = false;
        evokedCounter--;
        if (evokedCounter == 0)
        {
            // safe update
            remove(addedListeners, removedListeners);
            remove(listeners, removedListeners);
            merge(listeners, addedListeners);
            removedListeners.clear();
            addedListeners.clear();
        }
    }

    bool checkEachIteration(typename listeners_array::iterator & iter)
    {
        Ptr & listener = *iter;
        // Ptr  самозануляющийся указатель так что в любой момент может быть нулевым,
        // мусор в этом случае можно и нужно убрать сразу 
        if (listener == NULL)
        {
            iter = listeners.erase(iter);
            return false;
        }	
        if (std::find(removedListeners.begin(), removedListeners.end(), listener) != removedListeners.end())
        {
            iter++;
            return false;
        }
        return true;
    }
    
public:
	Evoker()
	{
		evokedCounter = 0;
		canceled = false;
	}
	virtual ~Evoker()	
	{
		listeners.clear();
		addedListeners.clear();
		removedListeners.clear();
	}

    template <class P>
    void evoke(void (T::*f)(P)) { evoke(f, static_cast<P>(this)); }

    template <class P, class A>
	void evoke(void (T::*f)(P), const A& p)
    {
        onStartEvoke();
	    typename ed::vector<Ptr>::iterator iter = listeners.begin();
	    while (iter != listeners.end())
	    {
            if (!checkEachIteration(iter))
                continue;
            Ptr & listener = *iter;
            //////////////////////////////////////////////////////////////////////////
            (listener->*f)(p);
            //////////////////////////////////////////////////////////////////////////
		    if (canceled)
			    break;
			iter++;
	    }
        onFinishEvoke();
    }	

    void evoke(void (T::*f)(void))
    {
        onStartEvoke();
        typename ed::vector<Ptr>::iterator iter = listeners.begin();
        while (iter != listeners.end())
        {
            if (!checkEachIteration(iter))
                continue;
            Ptr & listener = *iter;
            //////////////////////////////////////////////////////////////////////////
            (listener->*f)();
            //////////////////////////////////////////////////////////////////////////
            if (canceled)
                break;
            iter++;
        }
        onFinishEvoke();
    }
    void add(Ptr listener)
    {
	    if (listener == 0)
        {
		    return;
        }
        if (evokedCounter > 0)
        {
    	    addedListeners.push_back(listener);
            removeAll(removedListeners, listener);
            return;
        }
	    listeners.push_back(listener);
    }
	void remove(Ptr listener)
    {
	    if (listener == 0)
        {
		    return;
        }
        if (evokedCounter > 0)
        {
    	    removedListeners.push_back(listener);
            return;
        }
        removeAll(listeners, listener);
    }
	void clear()
	{
		listeners.clear() ;
		addedListeners.clear();
		removedListeners.clear();	
		evokedCounter = 0;
		canceled = false;
	}
	void cancel()	{ canceled = true; }
};

#endif
