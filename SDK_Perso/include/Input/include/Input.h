#pragma once

#include <ed/string.h>
#include <ed/vector.h>
#include <ed/unordered_set.h>

#include "InputExports.h"
#include "InputAction.h"
#include "InputKeyAction.h"

struct InputFilterInfo;
class ForceFeedback;

class INPUT_API Input
{
public:
	enum DeviceTypes
	{
		DT_UNKNOWN,
		DT_KEYBOARD,
		DT_MOUSE,
		DT_JOYSTICK,
		DT_TRACKIR,
		DT_HEADTRACKER,
	};

	class FFInfo
	{
	public:
		FFInfo() : trimmer(0), shake(0), swapAxes(false), invertX(false), invertY(false)
		{
		}

		ed::string deviceName;
		double trimmer;
		double shake;
		bool swapAxes;
		bool invertX;
		bool invertY;
	};

	// получить список устройств
	void getDevices(ed::vector<ed::string>& devices);

	// получить тип устройства
	DeviceTypes getDeviceType(const ed::string& deviceName);

	// получить список поддерживаемых устройством кнопок и осей
	void getDeviceKeysAxes(const ed::string& device, ed::vector<int>& keys, ed::vector<int>& axes);

	// установит номер джойстика
	void setJoystickNumber(const ed::string& joystick, int number);

	// получить номер джойстика по его имени
	// если устройство не джойстик, возвращает -1
	int getJoystickNumber(const ed::string& joystick);

	// устанавливает для устройства полную синхронизацию
	void setFullSync(const ed::string& deviceName, const ed::string& layerName, bool fullSync);

	// возвращает для джойстика полную синхронизацию
	// false если устройство и/или слой не найдены
	bool getFullSync(const ed::string& deviceName, const ed::string& layerName);

	// задает префикс для имен слоев, содержаших раскладку для юнита
	void setUnitMarker(const ed::string& marker);

	// возвращает префикс для имен слоев, содержаших раскладку для юнита
	const ed::string& getUnitMarker();

	// создать слой ввода
	// для нормальной работы инпута должен быть создан хотя бы один слой и помещен в стек
	void createLayer(const ed::string& layerName);

	// удалить слой ввода и освободить память
	void deleteLayer(const ed::string& layerName);

	// проверка существования слоя с именем layerName
	bool getLayerExists(const ed::string& layerName);

	// делает слой ввода дефолтным
	void setDefaultLayer(const ed::string& layerName);

	// добавляет дефолтный слой ввода в стек
	void setDefaultLayerTop();

	// удаляет дефолтный слой из стека
	void removeDefaultLayer();

	// делает слой верхним с стеке
	void setTopLayer(const ed::string& layerName);

	// возвращает имя верхнего слоя,
	// если стек пустой, то 0
	const ed::string* getTopLayer();

	// удаляет слой из стека
	void removeLayerFromStack(const ed::string& layerName);

	// возвращает стек слоев
	// верхний слой - первый элемент массива
	void getLayerStack(ed::vector<ed::string>& layers);

	// устанавливает стек слоев
	// слои должны быть в том порядке,
	// в котором они возвращаются функцией getLayersStack()
	void setLayerStack(const ed::vector<ed::string>& layers);

	// очищает стек слоев
	void clearLayerStack();

	// возвращает список всех загруженных слоев
	// это не стек слоев! 
	// загруженные слои могут отсутствовать в стеке
	void getLoadedLayers(ed::vector<ed::string>& layers);

	// удаление(полное уничтожение!!!) всех слоев ввода
	void removeAllLayers();

	// добавить модификатор типа Shift или Caps Lock
	void addReformer(const ed::string& layerName, int reformer, bool isSwitch);

	// удалить модификатор
	void removeReformer(const ed::string& layerName, int reformer);

	// удалить все модификаторы
	void clearReformers(const ed::string& layerName);

	// добавить клавиатурную комбинацию
	void addKeyCombo(const ed::string& layerName, int key, const ed::vector<int>& reformers,const InputKeyAction & action);

	// добавить комбинацию для оси
	void addAxisCombo(const ed::string& layerName, int axis, const ed::vector<int>& reformers,const InputAxisAction & action, const InputFilterInfo& fi);

	// собирает входные воздействия со всех устройств, интерпретирует их и 
	// возвращает связанные с ними команды. Команды будут добавлены в конец actions.  
	void process(ed::vector<InputAction>& actions);

	// возвращает максимально возможное значение для осей
	// для нормализации значение оси, полученное с устройства, 
	// должно делиться на это число
	static int getAxesValueMax();

	// возвращает число, гарантированно большее чем getAxesValueMax()
	static int getNotAxesValue();

	// определяет, является ли значение значением оси
	static bool isAxesValue(int value);

	// должна вызываться при получении окном фокуса
	void activate(); 

	// должна вызываться при потере окном фокуса
	void deactivate(); 

	// возвращает ForceFeedback интерфейс для устройств, его имеющих
	ForceFeedback* getForceFeedback(const ed::string& deviceName);

	// установить параметры ForceFeedback
	void setForceFeedback(const ed::string& layerName, const FFInfo& info);

	// выключить клавиатуру
	void banKeyboard(bool ban);

	// выключить мышь
	void banMouse(const ed::unordered_set<int>& bannedEvents);

	// установить путь к headtracker dll 
	void setHeadTrackerDllPath(const ed::string& path);

	// получить путь к headtracker dll 
	const ed::string& getHeadTrackerDllPath();

	// сбрасывает состояние устройств 
	// и для всех нажатых кнопок и осей генерирует команды
	void flush();

	// симуляция нажатия на кнопки клавиатуры
	// значения для кнопок брать в InputDefs.h
	void addOuterKey(int key, bool pressed);

	// начальная инициализация
    static void initialize(void* hWnd);

	// освобождение памяти
	static void uninitialize();

private:
	Input();
	~Input();

	bool turnLayerForceFeedbackOn(const ed::string& layerName);
	bool turnLayerForceFeedbackOff(const ed::string& layerName);
	void turnTopLayerForceFeedbackOn();

	void releaseKeyboardKeys();
	void releaseMouseKeys();
};

extern INPUT_API Input* getGlobalInput();

