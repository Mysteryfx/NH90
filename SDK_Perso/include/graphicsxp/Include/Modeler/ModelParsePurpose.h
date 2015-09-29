#ifndef MODEL_PARSER_PURPOSE_H
#define MODEL_PARSER_PURPOSE_H

namespace Graphics
{

// цель обхода
enum ModelParsePurpose
{
	mpRender			= 1<<0,	// отрисовка
	mpIntersect			= 1<<1,	// поиск пересечения с отрезком
	mpSegment			= 1<<2,	// поиск характерных отрезков и пересечений с ними
	mpConnector			= 1<<3,	// поиск коннекторов
	mpEffect			= 1<<4	// сбор объемных эффектов по модели
};

} // namespace Graphics

#endif
