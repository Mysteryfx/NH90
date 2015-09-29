#pragma once

enum COMPONENT_TYPE_ENUM
{
	CT_TRANSFORM,
	CT_MODEL,
	CT_LIGHT,
	CT_CAMERA,
	CT_FLIGHT_CONTROLLER,

	COMPONENTS_COUNT
};

class EntityComponent;
typedef ed::vector<EntityComponent*> ComponentStorage;

class Entity
{
	ComponentStorage components;
public:
	~Entity();
	bool	init();
	void	addComponent(EntityComponent *comp);
	EntityComponent *getComponentByType(COMPONENT_TYPE_ENUM type);
};



class EntityComponent
{
	friend class Entity;
protected:
	COMPONENT_TYPE_ENUM type;
	Entity *parentEntity;
	ComponentStorage *storage;//указатель на хранилище в котором лежит компонент, всегда существует

	EntityComponent() {}

public:
	virtual ~EntityComponent() { release(); }
	virtual bool	init() = 0;
	inline COMPONENT_TYPE_ENUM getComponentType() { return type; }
	//вызывается родителем
	virtual void	release() {};

	ComponentStorage *getStorage();

	Entity *getParentEntity() { return parentEntity; }
	//создать компонент. По умолчанию создается в глобальном хранилище, если не указано localStorage
	static EntityComponent *create(COMPONENT_TYPE_ENUM type, ComponentStorage *localStorage = nullptr);
	static ComponentStorage &getGlobalStorage(COMPONENT_TYPE_ENUM type);
};