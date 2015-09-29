#pragma once

namespace Graphics
{
	/// Deprecated
	// Режимы отрисовки, зависят от приложения
	enum RenderMode
	{
		rmInterface,    // пользовательский интерфейс
		rmMap,          // карта
		rmSimulation,   // основной вид
		rmMFD,          // MFD
		rmMirrors,		// зеркала
		//rmCockpit,
		//rmEnvMap
	};
}
