#pragma once
#include "entity.h"
#include "Modeler/ModelLight.h"

namespace Graphics
{
	class ModelConnector;
	class Model;
}

class ModelComponent;

class TransformComponent : public EntityComponent
{	
	Graphics::ModelConnector	*con;
	TransformComponent*	parent;
	ed::vector< TransformComponent*>	children;
	ed::vector<Graphics::ModelLight>	modelLights;//TODO: чо с ними делать, куда засунуть?

public:
	bool bValid;//актуальны ли матрицы, иначе надо пересчитать матрицы для всего дерева родителей
	ModelComponent *cModel;//модель, чтобы можно было линковаться к коннектору
	osg::Matrixf transform;//мировая матрица после вычисления всех трансформаций и иерархий
	osg::Matrixf localTransform;//локальная матрица

	struct marked_part
	{
		ed::string name;
		Position3	pnt;
	};
	TransformComponent();
	~TransformComponent();

	virtual bool init();

	const char	*getConnectorName();
	bool	attach(TransformComponent *child, const char * attach_point);
	bool	attachTo(TransformComponent *parent, const char * attach_point);
	bool	detach();

	void	validateChildrenPositions();//делаем это принудительно перед отрисовкой один раз
	void	collect_lights(Graphics::effectState * to, const Vector3 & origin);
	void	setTransform(const Position3 &p);
	void	setPosition(const osg::Vec3f &pos);

	void	move(const osg::Vec3f &pos);
	void	scale(const osg::Vec3f &pos);

	void	lookAt(const osg::Vec3f &target);

	void	validatePositionsTree();//пересчитать все узлы дерева

	ed::vector<marked_part> marked_parts;

private:	
	TransformComponent *getRoot();
	void	removeChild(TransformComponent* child);
	bool	setAsChild (TransformComponent* child, const char* attach_point);
};