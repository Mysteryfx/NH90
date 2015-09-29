#pragma once
#include <ed/string.h>
#include "Transport.h"

#ifndef REGISTER_CONST
#define REGISTER_CONST(x) extern CarsConstant x;
#endif

namespace CarsConst
{
	class CarsConstant
	{
	public:
		CarsConstant(const ed::string &name);
		inline operator double() {return _value;}
	private:
		double _value;
	};

	//Path following
	//при следовании по пути - допустимый отступ от целевой точки, на котором не действует сила по притяжению к точке
	REGISTER_CONST(folToleranceDist)
	//при следовании по пути - расстояние от целевой точки, при котором принудительно начинаем тормозить, чтобы не перегнать её
	REGISTER_CONST(folBreakingDist)
	//время предсказание позиции в пути, для корректировки силы притягивающей к пути (важно в поворотах)
	REGISTER_CONST(predictedPathTime)
	//расстояние от пути на котором работает сила притягиващая к пути
	REGISTER_CONST(maxOutsidePathDist)
	//при следовании по пути, уменьшает (должен быть <1) отступ от предыдущей маинки в колонне
	//тогда идеальная позиция в формации ставится позади отступа от предыдущей маинки 
	//и выравнивание скорости происходит по виртуальному лидеру
	//TODO может этот коэфициент стоит заменить допуском для выравнивания скорости
	REGISTER_CONST(intervalCoeff)
	//расстояние на которое машинка должна подъехать к пути, даже в случае опережения своих предшественников
	REGISTER_CONST(maxStayingDistance)
	//Интервал отнсительно текущего путевого параметра на котором ищется проекция машинки (нужен для оптимизации поиска проекции)
	REGISTER_CONST(searchTrailDist)
	//Максимально допустимая ошибка при котором направление считается верным
	REGISTER_CONST(maxDirErrorCos)
	//Максимально допустимая ошибка при котором направление считается верным при разбегании
	REGISTER_CONST(maxWanderDirErrorCos)
	//Время остановки колонны для того чтобы пропустить другую группу
	REGISTER_CONST(pauseToGiveWay)
	//Расстояние допустимый отступ от идеальной позиции на пути, когда скорость берётся по скорости лидера
	REGISTER_CONST(pathNearIdealPosDist)
	//Допустимый отступ от лидера вперёд
	REGISTER_CONST(fwdPathOffset)
	//Допустимый отступ от лидера вбок
	REGISTER_CONST(sidePathOffset)

	//Obstacle avoiding
	//Минимальная длина переднего сенсора для обнаружения препятсвий
	REGISTER_CONST(frontDistMin)
	//Длина бокового сенсора
	REGISTER_CONST(sideDist)
	//Оступ бокового сенсора от центра машинки
	REGISTER_CONST(sideOffDist)
	//Угол расположения бокового сенсора
	REGISTER_CONST(angle)
	//При движении по пути - отступ от текущей позиции для определения среднего направления движения
	REGISTER_CONST(offsetToAverangePathDir)

	//Neighbours avoiding
	//Угол между направлением движения машинок, при котором движение считается параллельным
	REGISTER_CONST(parAngle)
	//Ограничение минимального расстояния, при котором препятсвия-машинки проходят фильтр
	REGISTER_CONST(minNeighbSearchDist)
	//Определяет минимальное расстояние, при котором препятсвия-машинки проходят фильтр
	REGISTER_CONST(neighbSearchTime)
	//врямя даваемое на поворот к желаемой скорости (баланс между осциляциями при объезде и точностью объезда)
	REGISTER_CONST(timeForAvoidTurn)
	//коэфициент обрезки конуса препятсвия по скорости (чем больше - тем больше обрезается конус (нужен приезде колонной))
	REGISTER_CONST(truncatedConeC)

	//Arrive behavior
	//Дистанция от точки назначения при которой начинаем учитывать направление (можем пойти на разворот)
	REGISTER_CONST(arrivedDist)
	//Отступ назад от точки назначения для выполнения разворота
	REGISTER_CONST(arriveBehindDist)
	//Максимальный допустимый угол между носом машинки и нужным направлением при котором направление считается достигнутым
	REGISTER_CONST(criticalDevDir)
	//Угол между носом машинки и нужным направлением при котором происходит выравнивание
	REGISTER_CONST(criticalArrivedDir)
	//расстояние до конечной точки с которого меняем состояние машинки на ArriveVector
	// сейчас согласовано с расстоянием от первой машинки до виртуального лидера в FormationWedge
	REGISTER_CONST(distToArrive) 
	//Время при котором принудительно состояние машинки меняется на ArriveVector
	REGISTER_CONST(timeLimit)
	//Время при котором машинка принудительно останавливается (шаблоны, остановка в конце пути)
	REGISTER_CONST(timeToStopLimit)
	//Коэфициент при вычислении желаемой скорости достижения точки targetSpeedForarrive (0-1)
	REGISTER_CONST(arriveSpeedCoeff)

	//Disperse behavior
	//Минимальное и максимальное время до остановки машинки от начала разбегания
	REGISTER_CONST(minTimeDisperse)
	REGISTER_CONST(maxTimeDisperse)
	REGISTER_CONST(wanderChangeDir)

	//Land group
	//При следовании по пути, отставание от целевой точки с которого начинаем проверять пересечение с препятствиями рельефа
	REGISTER_CONST(warningDist)
	//Интервал между посками пересечений с препятсвиями рельефа (при превышении warningDist)
	REGISTER_CONST(checkTime)
	//Интервал между проверками на возможность разбежаться
	REGISTER_CONST(disperseCheckDist)
	//Машинки не разбегаются если впереди на этой дистанции есть мост или тоннель
	REGISTER_CONST(bridgeVisibleDist)

	//Максимальная скорость на мосту (ограничение при движении по следу)
	REGISTER_CONST(maxSpeedOnBridge)
	//Интервал при котором врутальный лидер обновляет список препятсвий
	REGISTER_CONST(obsCheckingDist)
	//дополнительный отступ лидера при смене формаций
	REGISTER_CONST(goAheadDist)
	//Дистанция от моста на которой новый путь при объезде ищется по дороге
	REGISTER_CONST(bridgeIsNearDist)
	//Дистанция от моста на которой новый путь при объезде ищется все нороги (брод)
	REGISTER_CONST(bridgeIsVeryNearDist)
	//Предельно допустимая разница между длиной пути и длиной объезда, при которой совершается объезд
	REGISTER_CONST(maxBypassLength)
	//Дина пути по дороге который считается длинным (для объезда мостов)
	REGISTER_CONST(routeByRouteIsLong)

	//Настройки поиска пути
	//Стоимость рек при поиске пути в объезд моста
	REGISTER_CONST(bypassRiversWeight) 
	//Стоимость проезда по земле при поиске пути по-умолчанию
	REGISTER_CONST(defaultGroundWeight)
	//Стоимость рек при поиске пути по-умолчанию
	REGISTER_CONST(defaultRiversWeight)

	//Moving vehicle
	//Коэффисиент трения качения
	REGISTER_CONST(cFricGround)
	REGISTER_CONST(powSpeedC)
	REGISTER_CONST(speedDiffForceCoeff)
	//Влияет на плавность изменения ускорения шаг изменения = elapsedTime*smoothAccC. Трясучка влево-вправо.
	REGISTER_CONST(smoothAccC)
	//Влияет на плавность изменения кривизны шаг изменения = elapsedTime*smoothACurvC
	REGISTER_CONST(smoothCurvC)
	//Дистанция до конца следа, когда можно перестраиваться
	REGISTER_CONST(distTolerance)
	//Расстояние, проезжаемое между обновлением списка соседей
	REGISTER_CONST(checkNeighbDist)
	//Расстояние, проезжаемое между обновлением списка препятсвий
	REGISTER_CONST(checkObsDist)
	//Расстояние, проезжаемое между обновлением путевого параметра машинки по пути группы
	REGISTER_CONST(checkPathParamDist)
	//Максимальный радиус поиска препятсвий
	REGISTER_CONST(maxObsRadius)
	//Максимальный радиус поиска соседей
	REGISTER_CONST(maxNeighbRadius)
	//Скорось с езды вдоль препятсвия
	REGISTER_CONST(obsParallelSpeed)
	//Угол между носом машинки и границей препятсвия, начиная с которого считаем что едем || препятствию
	REGISTER_CONST(obsParallelAngle)
	//Максимальная скорость при которой действующая сила выравнивается по направлению
	REGISTER_CONST(maxAdjustedSpeed)
	// скорость то которой можно тормозить без обрезания силы == maxAdjustedSpeed + 0.1
	REGISTER_CONST(uTurnSpeed)
	//скорость, с которой начинается обрезка направления силы ниже максимального угла колёс
	REGISTER_CONST(speedWithMaxDiv)
	//коэфициент трения по идее cueffCentripetal == sqrt(k трения по асфальту * g * cos(up^N)) из k*N = mV^2/r
	//но лучше подобрать опытным путём
	REGISTER_CONST(coeffCentripetal)

	//Trail
	//Максимальная разница в скорости, допустимая при упрощениии пути
	REGISTER_CONST(maxSpeedDiff)
	//Косинус минимального допустимого угла при упрощении пути
	REGISTER_CONST(minSegmentsCosine) 
	//Зона действия ограничения по скорости в каждой контрольной точки пути
	REGISTER_CONST(weightDist)

	//woMovingVehicle
	//Интервал проверки земли (не попали ли в реку или на крутой склон)
	REGISTER_CONST(checkLandscapeDist)
	//Глубина погружения в реку
	REGISTER_CONST(diveDepth)
	//ограничение по скорости при проходе рек вброд (должно быть
	REGISTER_CONST(maxSpeedOnShallowWater)

	//woCar
	//скорость вращения гусениц
	REGISTER_CONST(CatTrackCoeff)
	//ограничение по изменению аргумента за кадр (чтобы в разных кадрах были разные положения гусениц)
	REGISTER_CONST(CatTrackMax)

	//Formations
	//Отступы в формациях
	REGISTER_CONST(fColumnFwd)
	REGISTER_CONST(fRowSide)
	REGISTER_CONST(fWedgeStartSide)
	REGISTER_CONST(fWedgeSide)
	REGISTER_CONST(fWedgeFwd)
	REGISTER_CONST(fVeeStartSide)
	REGISTER_CONST(fVeeSide)
	REGISTER_CONST(fVeeFwd)
	REGISTER_CONST(fEchelonFwd)
	REGISTER_CONST(fEchelonSide)
	REGISTER_CONST(fDiamondFwd)
	REGISTER_CONST(fDiamondSide)
	REGISTER_CONST(fHumanFwd)
	REGISTER_CONST(fHumanSide)

	REGISTER_CONST(sndSmoothAccC)
	REGISTER_CONST(sndBreakingAcc)

	//Время в течении которого продолжают рассылаться одинаковые сообщения об апдейте машинок и колонны
	REGISTER_CONST(timeForFinishSendUpdateMessages)

	TRANSPORT_API void load(const ed::string &file);
}
