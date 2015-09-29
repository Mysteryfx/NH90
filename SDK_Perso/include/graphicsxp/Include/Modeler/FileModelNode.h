#ifndef FileModelNode_H
#define FileModelNode_H

#include "graphicsxp.h"
#include "ModelNode.h"

namespace Graphics
{

// Узел, которому соответствует файл - модель
class GRAPHICSXP_API FileModelNode : public ModelNode
{
protected:
	// указатель на зарезервированную память
	void *owned_mem;

	// размер части, сохраняемой сериализацией
	unsigned serializer_part;

	// загрузка данных 
	virtual void LoadData(ModelNode *cur);

public:
	FileModelNode(ModelNode *_parent = 0);
	~FileModelNode();

	// загрузка/сохранение из файла
	bool LoadFromFile(const ed::string& fileName);
	bool SaveToFile(const ed::string& fileName);
	
	// загрузка/сохранение в память
	bool ReadFromMemory(void *mem, unsigned &size, unsigned maxsize);
	bool WriteToMemory(void *mem, unsigned &size, unsigned maxsize);

	// вспомогательные функции для загрузки/сохранения
	unsigned GetSerializerPartSize();
	void *GetDataPointer();
	
	// сохранение/загрузка
	virtual void serialize(Serializer &serializer);
};

} // namespace Graphics

#endif // FileModelNode_H