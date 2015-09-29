// ProcessorsManager.h

#pragma once

#include "Common.h"
#include "Factory/StandardFactory.h"
#include "Processor.h"
#include "Notificator/ParamEvoker.h"

#define EMPTY_IDENTIFIER_NAME  _T("Empty")

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ProcessorsManager: public System, public Common::FactoryManager
{
public:
    class Listener
    {
    public:
        virtual void    onOpen(Processor *) = 0;
        virtual void    onClose(Processor *) = 0;
    };
private:
	typedef ed::map<Common::Identifier, ProcessorPtr> Processors;

	Processors	alive;
	Processors	dead;
	bool	live;
	void startup(ProcessorPtr processor);
	void shutdown(ProcessorPtr processor);

    ParamEvoker<Listener, Processor>    evoker;

	Common::Identifier      identifierToOpen; //postpone
	bool                    cancelStartup;    //postpone
public:
    ProcessorsManager();
    virtual ~ProcessorsManager();
    virtual bool    process();
    
	void  add(Listener *listener) { evoker.add(listener); }
	void  remove(Listener *listener) { evoker.remove(listener); }
    /** add method registers externally created processor */
    Processor *     add(Processor *);
    /** remove method unregisters externally created processor */
//    void            remove(Processor *);
    /** open method creates and registers new processor using registered factories */
    Processor *     open(const Common::Identifier&);
    /** find method finds the first processor with that identifier */
    Processor *     find(const Common::Identifier&);
    /** close method closes all processors with that identifier */
	void			close(const Common::Identifier&);
    /** close method unregisters and closes processor */
    void			close(Processor *processor) { close(processor->getIdentifier()); }

	void			show(const Common::Identifier&, bool flag);
	void            setIdentifierToOpen(Identifier inIdentifier); //postpone
	Identifier      getIdentifierToOpen();                        //postpone
};

extern USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ProcessorsManager *globalProcessorsManager;
void USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR getProcessorsRegistry(Common::FactoryManager **);

template <class T>
class ProcessorFactory: public Common::StandardFactory<T>
{
public:
    ProcessorFactory(const Identifier &i): StandardFactory(i) {}
    ProcessorFactory() {}
	// Factory must implement IUnknown:
	// TO DO: .. CreateInstance(..) { ... new ...Unknown<T>; }
};
#define REGISTER_PROCESSOR_FACTORY(factory) REGISTER_FACTORY(getProcessorsRegistry, factory);
#define REGISTER_PROCESSOR_FACTORY_EX(factory, name) REGISTER_FACTORY_EX(getProcessorsRegistry, factory, name);

namespace ProcessorsFactory
{

/** 
* The open method checks if requested processor is already exists and 
* if not then creates and return new processor otherwise returns existing
*/

template <class T> inline
T *     open()
{
	if (globalProcessorsManager == 0)
    {
        return 0;
    }
    Processor *processor = dynamic_cast<T *>(globalProcessorsManager->find(Common::identify<T>()));
	if (processor == 0) 
    {
        processor = new T;
		processor->setIdentifier(Common::identify<T>());
		processor = globalProcessorsManager->add(processor);
	}
	return dynamic_cast<T *>(processor);
}

template <class T, class P> inline
T *     openEx(const P &p)
{
	if (globalProcessorsManager == 0)
    {
        return 0;
    }
    globalProcessorsManager->close(Common::identify<T>());
    Processor *processor = new T(p);
	processor->setIdentifier(identify<T>());
	globalProcessorsManager->add(processor);
	return dynamic_cast<T *>(processor);
}
/** 
 * The find method returns existing processor
 */
template <class T> inline
T* find()
{
	if (globalProcessorsManager == 0)
    {
		return 0;
    }
	return dynamic_cast<T *>(globalProcessorsManager->find(Common::identify<T>()));
}

/** 
* The close method checks if requested processor is already exists and 
* if yes then destroys it otherwise does nothing
* @note template functions with void return type & no args
* are incorrectly interpreted by MSVC!
*/
template <class T> inline 
T* close()
{
	if (globalProcessorsManager != 0)
    {
		globalProcessorsManager->close(Common::identify<T>());
    }
	return 0;
}

} // namespace ProcessorsFactory

