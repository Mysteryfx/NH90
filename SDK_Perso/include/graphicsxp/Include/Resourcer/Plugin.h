#ifndef Plugin_H
#define Plugin_H

#include <ed/string.h>
#include "graphicsxp.h"

// Интерфейс для плагинов
// Не необходим, но может использоваться для сообщения дополнительной 
// информации о себе и выводе интерфейсов потребителям
class Plugin
{
private:
	ed::string name;
public:
	// регистрация происходит автоматически при создании/уничтожении
	GRAPHICSXP_API Plugin(const char* name);
	GRAPHICSXP_API virtual ~Plugin();

	GRAPHICSXP_API const char* GetName();

	// вызываются вместе с инициализацией/деинициализацией ресурсера
    virtual bool initialize() {return true;}
    virtual void uninitialize() {}
};

#endif // Plugin_H



