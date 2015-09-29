#ifndef MaterialFlags_H
#define MaterialFlags_H

typedef unsigned MaterialFlags;

// Флаги материала
const MaterialFlags mfTransparent		= 0x0001;	// Прозрачный ли материал (есть ли дырки). Не используется.
const MaterialFlags mfSortByDistance	= 0x0002;	// Требует сортировки по дистанции. Иначе - по ключу.
const MaterialFlags mfShadowCaster		= 0x0004;	// Отбрасывает тени.
const MaterialFlags mfLighted			= 0x0010;	// Освещаемый материал.
const MaterialFlags mfHalo				= 0x0020;	// Это хало источника света. Гораздо хуже затуманивается.
const MaterialFlags mfOnlay				= 0x0040;	// Часть поверхности - должно рисоваться вместе с ней.
const MaterialFlags mfWarmAir			= 0x0080;	// Теплый воздух - рисуется после всего
const MaterialFlags mfZNudged			= 0x0100;	// Отодвинуть назад при сортировке по дальности (чтобы рисовался раньше)
const MaterialFlags mfAirfield			= 0x0200;	/// If material is used to draw airfields. Yes it's dirty hack. See RenderParserImpl::DrawAirfields().
const MaterialFlags mfCloud				= 0x0400;	// старые облака

const MaterialFlags mfAll				= 0xffff;

#endif // MaterialFlags_H
