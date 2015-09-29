#ifndef ResourceType_H
#define ResourceType_H

// Типы ресурсов
enum ResourceType
{
	rtNone,				/// отсутствие ресурса
	rtTexture,			/// текстуры
	rtVolumeTexture,	/// volume textures
	rtMaterial,			/// материалы (логический ресурс)
	rtGeometry,			/// элементы геометрии
	rtFont,				/// шрифт
	rtEffect,			/// объемные эффекты
	rtModel,			/// модели объектов
	rtModelHeader,		/// часть модели, которой оперирует renderer
	rtRenderState,		/// Блок атрибутов отрисовки 
	rtDataSource,		/// Источник данных
	rtLiveryManager,	/// livery manager

	rtLast = 20,	/// зарезервированно 20 типов ресурсов
};

#endif // ResourceType_H
