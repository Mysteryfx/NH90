// В.Б. 15.02.02 ////////////////////////////////////////////////////////////////////////
// В связи с тем, что коды команд записываются в треки, эти коды не должны изменяться.
// Поэтому нельзя изменять порядок существующих команд, удалять ненужные команды или
// добавлять новые между существующими. Новые команды разрешается вставлять только в 
// конец данного списка. Вследствие этого, нельзя также комментировать/раскомментировать
// команды в данном списке - нужно находить другие способы внесения изменений, не
// приводящие к неработоспособности старых треков. Ненужные команды просто отмечайте
// комментарием /* OBSOLETE */, см. ниже. Новые команды вставляйте непосредственно
// перед iCommandMaximum.
// GSF. Также большая просьба синхронизировать команды с фильтрами в ActionProcessor.
// isVideoAction() - список видео-команд. isImportantAction() - вспомогательные.
/////////////////////////////////////////////////////////////////////////////////////////

#ifdef INPUT_LUA_BINDING
#undef _ICOMMAND_H_
#endif

#ifndef _ICOMMAND_H_
#define _ICOMMAND_H_

#ifndef INPUT_LUA_BINDING
#define REGISTER_COMMAND(cmd) cmd,
#define REGISTER_COMMAND_EX(cmd, value) cmd = value,
#endif

#ifndef INPUT_LUA_BINDING
enum iCommand
{
#endif

	REGISTER_COMMAND_EX(iCommandNull, 0)						// Нужно для перебора всех команд

	/*OBSOLETE*/REGISTER_COMMAND(iCommandRotatePlus)						// Временная команда
	/*OBSOLETE*/REGISTER_COMMAND(iCommandRotateMinus)					// Временная команда
	/*OBSOLETE*/REGISTER_COMMAND(iCommandUpPilon)						// Временная команда
	/*OBSOLETE*/REGISTER_COMMAND(iCommandDownPilon)						// Временная команда
	/*OBSOLETE*/REGISTER_COMMAND(iCommandMissileFire)					// Временная команда
	/*OBSOLETE*/REGISTER_COMMAND(iCommandGunFire)						// Временная команда
	REGISTER_COMMAND(iCommandViewCockpit)					// Камера - кабина (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewAir)						// Камера - воздушные объекты (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewTower)						// Камера - башня (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandViewGround, 10)				// Камера - наземные объекты (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewMirage)						// Камера - миражи (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewChase)						// Следящая камера (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewNavy)						// Камера - морские объекты (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewFight)						// Камера - воздушный бой (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewAWACS)						// Камера - командный пункт (вид АВАКС) (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewFree)						// Свободная камера (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewTempCockpitOn)				// Включение камеры временной кабины (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewTempCockpitOff)				// Выключение камеры временной кабины (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewLock)						// Слежение за ближайшим объектом (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandViewUnlock, 20)				// Сброс слежения (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewMe)							// Показать свой самолет (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewPlus)						// Переключатель режима "+" для текущей камеры (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewFriends)					// Режим "Только свои" (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewEnemies)					// Режим "Только враги" (Non-simulation Action)
	/* OBSOLETE */REGISTER_COMMAND(iCommandViewNeutrals)					// Режим "Нейтральные" (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewAll)						// Режим "Все" (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewExplosion)					// Временная команда для демонстрации взрыва (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewLeft)						// Поворот взгляда налево (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewRight)						// Поворот взгляда направо (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandViewUp, 30)					// Поворот взгляда вверх (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewDown)						// Поворот взгляда вниз (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewLeftSlow)					// Поворот взгляда налево медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewRightSlow)					// Поворот взгляда направо медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewUpSlow)						// Поворот взгляда вверх медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewDownSlow)					// Поворот взгляда вниз медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCenter)						// Центрирование взгляда (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewForward)					// Приближение камеры (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewBack)						// Удаление камеры (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewForwardSlow)				// Приближение камеры медленное (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandViewBackSlow, 40)				// Удаление камеры медленное (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraLeft)					// Поворот камеры налево (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraRight)				// Поворот камеры направо (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraUp)					// Поворот камеры вверх (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraDown)					// Поворот камеры вниз (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraLeftSlow)				// Поворот камеры налево медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraRightSlow)			// Поворот камеры направо медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraUpSlow)				// Поворот камеры вверх медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraDownSlow)				// Поворот камеры вниз медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraCenter)				// Центрирование камеры (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandFriendlyChat, 50)				// Сетевой Чат только своим (Administrative Action)
	REGISTER_COMMAND(iCommandQuit)							// Выход (Administrative Action)
	REGISTER_COMMAND(iCommandBrakeGo)						// Пауза/пуск времени (клавиша S) (Administrative Action)
	REGISTER_COMMAND(iCommandAccelerate)						// Ускорение времени (Administrative Action) 
	REGISTER_COMMAND(iCommandOneFrame)						// Покадровое движение при остановке времени (Administrative Action)
	REGISTER_COMMAND(iCommandTrkEsc)							// Прерывание проигрывания трека (Administrative Action)
	/*OBSOLETE*/REGISTER_COMMAND(iCommandPlayerCntrl)		// Вывод панели профилирования - temporary
	REGISTER_COMMAND(iCommandChat)							// Сетевой Чат в режиме всем
	REGISTER_COMMAND(iCommandPlaneSAUAutomatic)				// Стабилизация углов
	REGISTER_COMMAND(iCommandPlaneSAUHBarometric)			// Стабилизация абсолютной высоты
	REGISTER_COMMAND_EX(iCommandPlaneSAUHRadio, 60)			// Полет с огибанием рельефа
	REGISTER_COMMAND(iCommandPlaneSAUHorizon)				// Приведение к горизонту
	REGISTER_COMMAND(iCommandPlaneAutopilot)					// Автопилот
	REGISTER_COMMAND(iCommandPlaneAUTOnOff)					// Переключение режима АУТ
	REGISTER_COMMAND(iCommandPlaneAUTIncrease)				// Увеличить заданную скорость
	REGISTER_COMMAND(iCommandPlaneAUTDecrease)				// Уменьшить заданную скорость
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTIncreaseRegime)			// Перейти на более высокий режим тяги
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTDecreaseRegime)			// Перейти на более низкий режим тяги
	REGISTER_COMMAND(iCommandPlaneGear)						// Шасси
	REGISTER_COMMAND(iCommandPlaneHook)						// Гак
	REGISTER_COMMAND_EX(iCommandPlanePackWing, 70)				// Складывание крыльев
	REGISTER_COMMAND(iCommandPlaneFonar)						// Фонарь
	REGISTER_COMMAND(iCommandPlaneFlaps)						// Закрылки
	REGISTER_COMMAND(iCommandPlaneAirBrake)					// Воздушный тормоз
	REGISTER_COMMAND(iCommandPlaneWheelBrakeOn)				// Включение тормоза колес
	REGISTER_COMMAND(iCommandPlaneWheelBrakeOff)				// Выключение тормоза колес
	REGISTER_COMMAND(iCommandPlaneParachute)					// Парашют
	REGISTER_COMMAND(iCommandPlaneDropSnar)					// Сброс ловушек
	REGISTER_COMMAND(iCommandPlaneWingtipSmokeOnOff)			// Вкл/выкл дымовых шашек на концах крыльев
	REGISTER_COMMAND(iCommandPlaneFuelOn)					// Включение слива топлива или заправки
	REGISTER_COMMAND_EX(iCommandPlaneFuelOff, 80)				// Выключение слива топлива или заправки
	REGISTER_COMMAND(iCommandPlaneSalvoOnOff)				// Залповый режим вкл/выкл
	REGISTER_COMMAND(iCommandPlaneJettisonWeapons)			// Сброс оружия парами
	REGISTER_COMMAND(iCommandPlaneEject)						// Катапультирование
	REGISTER_COMMAND(iCommandPlaneFire)						// Гашетка
	REGISTER_COMMAND(iCommandPlaneFireOff)					// Гашетку отпустили
	REGISTER_COMMAND(iCommandPlaneRadarOnOff)				// Включение / выключение радара
	REGISTER_COMMAND(iCommandPlaneEOSOnOff)					// Включение / выключение ОЛС
	REGISTER_COMMAND(iCommandPlaneRadarLeft)					// Поворот антенны РЛС влево
	REGISTER_COMMAND(iCommandPlaneRadarRight)				// Поворот антенны РЛС вправо
	REGISTER_COMMAND_EX(iCommandPlaneRadarUp, 90)				// Поворот антенны РЛС вверх
	REGISTER_COMMAND(iCommandPlaneRadarDown)					// Поворот антенны РЛС вниз
	REGISTER_COMMAND(iCommandPlaneRadarCenter)				// Возврат антенны РЛС в центр
	REGISTER_COMMAND(iCommandPlaneTrimLeft)					// Триммер влево
	REGISTER_COMMAND(iCommandPlaneTrimRight)					// Триммер вправо
	REGISTER_COMMAND(iCommandPlaneTrimUp)					// Триммер вверх
	REGISTER_COMMAND(iCommandPlaneTrimDown)					// Триммер вниз
	REGISTER_COMMAND(iCommandPlaneTrimCancel)				// Сброс триммера
	REGISTER_COMMAND(iCommandPlaneTrimLeftRudder)			// Триммер левой педали
	REGISTER_COMMAND(iCommandPlaneTrimRightRudder)			// Триммер правой педали
	REGISTER_COMMAND_EX(iCommandPlaneChangeLock, 100)			// Переключение подрежимов / захват
	REGISTER_COMMAND(iCommandPlaneChangeWeapon)				// Переключение подрежимов / перебор оружия
	REGISTER_COMMAND(iCommandPlaneChangeTarget)				// Перебор целей
	REGISTER_COMMAND(iCommandPlaneZoomIn)					// Изменение масштаба МФД +
	REGISTER_COMMAND(iCommandPlaneZoomOut)					// Изменение масштаба МФД -
	REGISTER_COMMAND(iCommandPlaneModeNAV)					// Переход в режим навигации
	REGISTER_COMMAND(iCommandPlaneModeBVR)					// Переход в режим дальнего воздушного боя
	REGISTER_COMMAND(iCommandPlaneModeVS)					// Переход в режим ближнего воздушного боя (полоса)
	REGISTER_COMMAND(iCommandPlaneModeBore)					// Переход в режим ближнего воздушного боя (кольцо)
	REGISTER_COMMAND(iCommandPlaneModeHelmet)				// Переход в режим ближнего воздушного боя (шлем)
	REGISTER_COMMAND_EX(iCommandPlaneModeFI0, 110)				// Переход в режим FI0
	REGISTER_COMMAND(iCommandPlaneModeGround)				// Переход в режим атаки наземных целей
	REGISTER_COMMAND(iCommandPlaneModeGrid)					// Переход в режим "сетка"
	REGISTER_COMMAND(iCommandPlaneModeCannon)				// Включение/выключение пушки
	REGISTER_COMMAND(iCommandPlaneDoAndHome)					// Выполни задание и вернись на базу
	REGISTER_COMMAND(iCommandPlaneDoAndBack)					// Выполни задание и вернись ко мне
	REGISTER_COMMAND(iCommandPlaneFormation)					// Идти рассеянным/плотным строем
	REGISTER_COMMAND(iCommandPlaneJoinUp)					// Собраться
	REGISTER_COMMAND(iCommandPlaneAttackMyTarget)			// Атакуй мою цель
	REGISTER_COMMAND(iCommandPlaneCoverMySix)				// Прикрой меня сзади
	REGISTER_COMMAND_EX(iCommandPlaneShipTakeOff, 120)			// Хочу взлететь с авианосца
	REGISTER_COMMAND(iCommandPlaneCobra)						// Маневр Кобра
	REGISTER_COMMAND(iCommandSoundOnOff)                     // Выключить/включить звук (Administrative Action)
	REGISTER_COMMAND(iCommandSoundMicOn)						// Включить прием звука с микрофона (Administrative Action)
	REGISTER_COMMAND(iCommandSoundMicOff)					// Отключить прием звука с микрофона (Administrative Action)
	REGISTER_COMMAND(iCommandViewRightMirrorOn)				// Взгляд на зеркала (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewRightMirrorOff)				// (Non-simulation Action)	
	REGISTER_COMMAND(iCommandViewLeftMirrorOn)				// (Non-simulation Action)			
	REGISTER_COMMAND(iCommandViewLeftMirrorOff)	// (Non-simulation Action)

	REGISTER_COMMAND(iCommandNaturalViewCockpitIn)			// Камера - кабина (естественный взгляд) (Non-simulation Action)
	/* OBSOLETE */	REGISTER_COMMAND_EX(iCommandNaturalViewCockpitOut, 130)	// Камера за кабиной (естественный взгляд)
	REGISTER_COMMAND(iCommandViewLndgOfficer)				// руководитель полетами на авианосце
	/* OBSOLETE */	REGISTER_COMMAND(iCommandViewCaptainBridge)				// капитанский мостик на авианосце
	/* OBSOLETE */	REGISTER_COMMAND(iCommandViewGroundAndPlane)				// наземный объект + ваш самолет
	/* OBSOLETE */	REGISTER_COMMAND(iCommandViewPlaneAndGround) 			// ваш самолет + наземный объект
	REGISTER_COMMAND(iCommandViewWeaponAndTarget)			// ракетa и цель (Non-simulation Action)
	REGISTER_COMMAND(iCommandActiveJamming)
	REGISTER_COMMAND(iCommandLandDetailsIncrease)			// Отладочное увеличение детализации (Non-simulation Action)
	REGISTER_COMMAND(iCommandLandDetailsDecrease)			// Отладочное уменьшение детализации (Non-simulation Action)
	REGISTER_COMMAND(iCommandSelecterLeft)				    // движение маркера захвата целей на HUD влево
	REGISTER_COMMAND_EX(iCommandSelecterRight, 140)			// вправо
	REGISTER_COMMAND(iCommandSelecterUp)					    // вверх
	REGISTER_COMMAND(iCommandSelecterDown)					// вниз
	REGISTER_COMMAND(iCommandRefusalTWS)						// отказ от СНП
	REGISTER_COMMAND(iCommandPlaneResetMasterWarning)
	REGISTER_COMMAND(iCommandPlaneFlapsOn)
	REGISTER_COMMAND(iCommandPlaneFlapsOff)
	REGISTER_COMMAND(iCommandPlaneAirBrakeOn)
	REGISTER_COMMAND(iCommandPlaneAirBrakeOff)
	REGISTER_COMMAND(iCommandViewWeapons)				// Камера вооружений (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandViewStatic, 150)			// Камера статических объектов (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewTargets)				// Камера целей
	REGISTER_COMMAND(iCommandInfoOnOff)					// Переключатель информационных строки (Non-simulation Action)
	/* OBSOLETE */REGISTER_COMMAND(iCommandInfoTextOnOff)				// Включить/выключить верхнюю информационную строку (Non-simulation Action)
	/* OBSOLETE */REGISTER_COMMAND(iCommandInfoDataOnOff)				// Включить/выключить нижнюю информационную строки (Non-simulation Action)
	REGISTER_COMMAND(iCommandPlaneAirRefuel)				// Переключатель положения штанги заправки
	REGISTER_COMMAND(iCommandBrightnessILS)				// Яркость ILS
	/* OBSOLETE */REGISTER_COMMAND(iCommandGraphicsFrameRate)			// Переключатель вывода частоты кадров (Administrative Action)
	REGISTER_COMMAND(iCommandViewFreeJump)				// Переключение на свободную камеру в текущей позиции (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewSpeedUp)				// Разогнать свободную камеру (положительное ускорение) (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandViewSlowDown, 160)			// Замедлить свободную камеру (отрицательное ускорение) (Non-simulation Action)
	REGISTER_COMMAND(iCommandPlaneAUTIncreaseLeft)
	REGISTER_COMMAND(iCommandPlaneAUTDecreaseLeft)
	REGISTER_COMMAND(iCommandPlaneAUTIncreaseRight)
	REGISTER_COMMAND(iCommandPlaneAUTDecreaseRight)
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTIncreaseRegimeLeft)
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTDecreaseRegimeLeft)
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTIncreaseRegimeRight)
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTDecreaseRegimeRight)
	REGISTER_COMMAND(iCommandPlaneImmortal)				// Переключатель бессмертия своего самолета
	REGISTER_COMMAND_EX(iCommandPlaneJettisonFuelTanksUp, 170)	
	REGISTER_COMMAND(iCommandPlaneJettisonWeaponsUp)
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTUnlockLeft)
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneAUTUnlockRight)
	/* OBSOLETE */REGISTER_COMMAND(iCommandPlaneTrimViewOnOff)			// Вкл/выкл индикации триммера на ИЛСе				
	REGISTER_COMMAND(iCommandPlaneLightsOnOff)			// Вкл/выкл огни на своем самолете
	REGISTER_COMMAND(iCommandPlaneDropSnarOnce)			// Однократный сброс ловушек
	REGISTER_COMMAND(iCommandViewAngleDefault)			// Установить умалчиваемый угол обзора камеры (Non-simulation Action)
	REGISTER_COMMAND(iCommandPlaneJettisonFuelTanks)		// Сброс топливных баков
	REGISTER_COMMAND(iCommandToggleCommandMenu)		// Вызов панели команд ведомым
	REGISTER_COMMAND_EX(iCommandViewSwitchReverse, 180)	// Обратное направление переключения объектов (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewSwitchForward)			// Прямое направление переключения объектов (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewObjectIgnore)			// Игнорировать объект при переключениях на него (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewObjectsAll)				// Очистить список игнорируемых объектов (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewTerrainLock)			// Захват точки на поверхности земли (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewFromTo)					// Переключатель направления камеры (от объекта/на объект) (Non-simulation Action)
	REGISTER_COMMAND(iCommandPlaneDown)					// Тонкое управление автопилотом по высоте
	REGISTER_COMMAND(iCommandPlaneUp)
	REGISTER_COMMAND(iCommandPlaneLeft)
	REGISTER_COMMAND(iCommandPlaneRight)
	REGISTER_COMMAND_EX(iCommandViewLocal, 190)			// Локальные повороты камеры
	REGISTER_COMMAND(iCommandDecelerate)					// Замедление модельного времени (Administrative Action)
	REGISTER_COMMAND(iCommandPlaneJump)      			// Перескакивание в текущий самолет
	REGISTER_COMMAND(iCommandPlaneUpStart)
	REGISTER_COMMAND(iCommandPlaneUpStop)
	REGISTER_COMMAND(iCommandPlaneDownStart)
	REGISTER_COMMAND(iCommandPlaneDownStop)
	REGISTER_COMMAND(iCommandPlaneLeftStart)
	REGISTER_COMMAND(iCommandPlaneLeftStop)
	REGISTER_COMMAND(iCommandPlaneRightStart)
	REGISTER_COMMAND_EX(iCommandPlaneRightStop, 200)
	REGISTER_COMMAND(iCommandPlaneLeftRudderStart)
	REGISTER_COMMAND(iCommandPlaneLeftRudderStop)
	REGISTER_COMMAND(iCommandPlaneRightRudderStart)
	REGISTER_COMMAND(iCommandPlaneRightRudderStop)
	REGISTER_COMMAND(iCommandViewUpRight)						// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewDownRight)						// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewDownLeft)						// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewUpLeft)							// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewStop)							// Прекратить движение камеры (Non-simulation Action) 
	REGISTER_COMMAND_EX(iCommandViewUpRightSlow, 210)				// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewDownRightSlow)					// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewDownLeftSlow)					// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewUpLeftSlow)						// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewStopSlow)						// (Non-simulation Action) 
	REGISTER_COMMAND(iCommandPlaneTrimStop)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewUp)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewUpRight)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewRight)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewDownRight)
	/* OBSOLETE */    REGISTER_COMMAND_EX(iCommandPointOfViewDown, 220)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewDownLeft)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewLeft)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewUpLeft)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewCenter)
	/* OBSOLETE */   REGISTER_COMMAND(iCommandPointOfViewStop)
	REGISTER_COMMAND(iCommandSelecterUpRight)
	REGISTER_COMMAND(iCommandSelecterDownRight)
	REGISTER_COMMAND(iCommandSelecterDownLeft)
	REGISTER_COMMAND(iCommandSelecterUpLeft)
	REGISTER_COMMAND_EX(iCommandSelecterStop, 230)
	REGISTER_COMMAND(iCommandPlaneRadarUpRight)
	REGISTER_COMMAND(iCommandPlaneRadarDownRight)
	REGISTER_COMMAND(iCommandPlaneRadarDownLeft)
	REGISTER_COMMAND(iCommandPlaneRadarUpLeft)
	REGISTER_COMMAND(iCommandPlaneRadarStop)
	REGISTER_COMMAND(iCommandViewSaveAngles)				// ??? (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewTempCockpitToggle)		// (Non-simulation Action)
	/* OBSOLETE */REGISTER_COMMAND(iCommandViewSaveAngles3)
	/* OBSOLETE */REGISTER_COMMAND(iCommandViewSaveAngles4)
	/* OBSOLETE */	REGISTER_COMMAND_EX(iCommandViewSaveAngles5, 240)
	/* OBSOLETE */REGISTER_COMMAND(iCommandViewSaveAngles6)
	/* OBSOLETE */REGISTER_COMMAND(iCommandViewSaveAngles7)
	/* OBSOLETE */REGISTER_COMMAND(iCommandViewSaveAngles8)
	/* OBSOLETE */REGISTER_COMMAND(iCommandViewSaveAngles9)
	REGISTER_COMMAND(iCommandViewCoordinatesInLinearUnits)	// (Non-simulation Action)	
	REGISTER_COMMAND(iCommandNoAcceleration)					// Ускорение модельного времени = 1 (Administrative Action)
	REGISTER_COMMAND(iCommandPlaneHUDFilterOnOff)			// Поднять/опустить светофильтр ИЛС (Ka-50)
	REGISTER_COMMAND(iCommandCockpitTakeControl)				// Переключатель управления для сетевого инспектора
	/* NOT IMPL */REGISTER_COMMAND(iCommandRollbackSave)		// Создание точки отката в треке (Administrative Action) 
	/* NOT IMPL */	REGISTER_COMMAND_EX(iCommandRollBack, 250)		// Откат на предыдущую точку (Administrative Action)
	/* NOT IMPL */REGISTER_COMMAND(iCommandRollForward)        // Переход на последующую точку (Administrative Action)
	REGISTER_COMMAND(iCommandPlaneMasterCaution)				// Аналог "Экран вызов" (МиГ-29) кнопка-лампа ЦСО (Ка-50)
	REGISTER_COMMAND(iCommandPlaneScalesReject)				// Scales reject / огранич. индикац. на ИЛС (Ка-50)
	REGISTER_COMMAND(iCommandPlaneBettyRepeat)				// Repeat Betty messages (Rf-50)
	/* NOT IMPL */	   REGISTER_COMMAND(iCommandSaveGame)		// (Administrative Action)
	/*OBSOLETE*/   REGISTER_COMMAND(iCommandLoadGame)
	/*OBSOLETE*/   REGISTER_COMMAND(iCommandScreenShot)						// (Administrative Action)
	REGISTER_COMMAND(iCommandThreatMissilePadlock)
	REGISTER_COMMAND(iCommandAllMissilePadlock)
	/*OBSOLETE*/	REGISTER_COMMAND_EX(iCommandFinishTrack, 260)
	REGISTER_COMMAND(iCommandMarkerState)					// (Non-simulation Action)
	REGISTER_COMMAND(iCommandDecreaseRadarScanArea)
	REGISTER_COMMAND(iCommandIncreaseRadarScanArea)
	REGISTER_COMMAND(iCommandMarkerStatePlane)				// (Non-simulation Action)
	REGISTER_COMMAND(iCommandMarkerStateRocket)				// (Non-simulation Action)
	REGISTER_COMMAND(iCommandMarkerStateShip)				// (Non-simulation Action)
	REGISTER_COMMAND(iCommandAWACSHomeBearing)
	REGISTER_COMMAND(iCommandAWACSTankerBearing)
	REGISTER_COMMAND(iCommandAWACSBanditBearing)
	REGISTER_COMMAND_EX(iCommandAWACSDeclare, 270)
	REGISTER_COMMAND(iCommandEasyRadarOnOff)
	REGISTER_COMMAND(iCommandAutoLockOnNearestAircraft)
	REGISTER_COMMAND(iCommandAutoLockOnCenterAircraft)
	REGISTER_COMMAND(iCommandAutoLockOnNextAircraft)
	REGISTER_COMMAND(iCommandAutoLockOnPreviousAircraft)
	REGISTER_COMMAND(iCommandAutoLockOnNearestSurfaceTarget)
	REGISTER_COMMAND(iCommandAutoLockOnCenterSurfaceTarget)
	REGISTER_COMMAND(iCommandAutoLockOnNextSurfaceTarget)
	REGISTER_COMMAND(iCommandAutoLockOnPreviousSurfaceTarget)
	REGISTER_COMMAND_EX(iCommandChangeGunRateOfFire, 280)
	REGISTER_COMMAND(iCommandChangeRippleQuantity)
	REGISTER_COMMAND(iCommandChangeRippleInterval)
	REGISTER_COMMAND(iCommandSwitchMasterArm)
	REGISTER_COMMAND(iCommandChangeReleaseMode)
	REGISTER_COMMAND(iCommandPlaneRadarChangeMode)
	REGISTER_COMMAND(iCommandChangeRWRMode)
	REGISTER_COMMAND(iCommandPromotePlane)					// Активизировать скрытого клиента (Inside Action)
	REGISTER_COMMAND(iCommandFlightClockReset)
	REGISTER_COMMAND(iCommandViewForwardSlowStop)			// (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandViewBackSlowStop, 290)			// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewForwardStop)				// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewBackStop)					// (Non-simulation Action)
	REGISTER_COMMAND(iCommandFreezeActionSystem)				// (Inside Action)
	REGISTER_COMMAND(iCommandUnfreezeActionSystem)			// (Inside Action)
	REGISTER_COMMAND_EX(iCommandViewHorStop, 295)					// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewVertStop)					// (Non-simulation Action)
	REGISTER_COMMAND(iCommandAutopilotEmergOFF_up)
	REGISTER_COMMAND(iCommandViewTowerJump)					// (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraJiggle)				// (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandPlaneCockpitIllumination, 300)

	REGISTER_COMMAND(iCommandHelicopter_PPR_button_K_up)
	REGISTER_COMMAND(iCommandHelicopter_PPR_button_H_up)
	REGISTER_COMMAND(iCommandHelicopter_PPR_button_T_up)
	REGISTER_COMMAND(iCommandHelicopter_PPR_button_B_up)
	REGISTER_COMMAND_EX(iCommandHelicopter_PPR_button_DIR_up, 305)

	REGISTER_COMMAND(iCommandTransferCursor)					// Перемещение курсора мыши (для треков) (Administrative Action)
	REGISTER_COMMAND(iCommandRecordCursorOnOff)				// Включение/выключение записи курсора мыши (для треков) (Inside Action)
	REGISTER_COMMAND(iCommandChangeRippleIntervalDown)		// Изменение интервала сброса бомб в залпе для А10 в обратном направлении

	REGISTER_COMMAND(iCommandEnginesStart)				// Запуск двигателей
	REGISTER_COMMAND_EX(iCommandEnginesStop, 310)			// Выключение двигателей

	REGISTER_COMMAND(iCommandLeftEngineStart)			// Запуск левого двигателя
	REGISTER_COMMAND(iCommandRightEngineStart)			// Запуск правого двигателя

	REGISTER_COMMAND(iCommandLeftEngineStop)				// Выключение левого двигателя
	REGISTER_COMMAND(iCommandRightEngineStop)			// Выключение правого двигателя

	REGISTER_COMMAND(iCommandPowerOnOff)					// Activer / désactiver le pouvoir (Puissance électrique) - 315

	REGISTER_COMMAND(iCommandAltimeterPressureIncrease)	// Увеличение давления на ВД
	REGISTER_COMMAND(iCommandAltimeterPressureDecrease)	// Уменьшение давления на ВД
	REGISTER_COMMAND(iCommandAltimeterPressureStop)

	REGISTER_COMMAND(iCommandTrackEditReplace)			// Режим редактирования трека с отменой старого (по умолчанию) (Administrative Action)
	REGISTER_COMMAND_EX(iCommandTrackEditInsert, 320)		// Режим редактирования трека с добавлением к старому (Administrative Action)

	REGISTER_COMMAND(iCommandViewFastMouse)				// Режим ускоренных аналоговых перемещений камеры
	REGISTER_COMMAND(iCommandViewSlowMouse)				// Режим замедленных аналоговых перемещений камеры
	REGISTER_COMMAND(iCommandViewNormalMouse)			// Режим нормальных аналоговых перемещений камеры

	REGISTER_COMMAND(iCommandRecordSubtitleOn)			// Включение показа субтитров (для треков) (Inside Action)
	REGISTER_COMMAND(iCommandRecordSubtitleOff)			// Выключение показа субтитров (для треков) (Inside Action)

	REGISTER_COMMAND(iCommandViewHUDOnlyOnOff)			// Переключатель режима кабины "Только ИЛС"

	REGISTER_COMMAND(iCommandRecoverHuman)				// Восстановить хьюмана
	REGISTER_COMMAND(iCommandPlaneHeadLightOnOff)		// Вкл/выкл фары на передней стойке шасси

	REGISTER_COMMAND(iCommandPlaneLockPadlock)				// Следить за ближайшим объектом
	REGISTER_COMMAND_EX(iCommandPlaneUnlockPadlock, 330)		// Бросить следить за объектом

	REGISTER_COMMAND(iCommandViewFastKeyboard)			// Режим ускоренных клавишных перемещений камеры
	REGISTER_COMMAND(iCommandViewSlowKeyboard)			// Режим замедленных клавишных перемещений камеры
	REGISTER_COMMAND(iCommandViewNormalKeyboard)			// Режим нормальных клавишных перемещений камеры

	REGISTER_COMMAND(iCommandViewExternalZoomIn)			// Zoom для внешних видов	
	REGISTER_COMMAND(iCommandViewExternalZoomInStop)
	REGISTER_COMMAND(iCommandViewExternalZoomOut)
	REGISTER_COMMAND(iCommandViewExternalZoomOutStop)
	REGISTER_COMMAND(iCommandViewExternalZoomDefault)
	REGISTER_COMMAND(iCommandMouseViewOnOff)			// Значения внешней и кокпитной установок мыши для треков (Inside Action)

	REGISTER_COMMAND_EX(iCommandViewCameraMoveMode, 340)	// Переключатель режима перемещения камеры (служебная - вместо клавиши S в треках)
	REGISTER_COMMAND(iCommandViewFightGround)			// Камера самолет-наземный юнит/корабль (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraUpLeft)			// Поворот камеры вверх-налево (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraUpRight)			// Поворот камеры вверх-направо (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraDownLeft)			// Поворот камеры вниз-налево (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraDownRight)		// Поворот камеры вниз-направо (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewPanToggle)				// Переключатель pan-режима (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraReturn)			// Возврат камеры (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewLocomotivesToggle)		// Фильтр только_локомотивы/все для Ctrl-F12 (Non-simulation Action)
	REGISTER_COMMAND(iCommandPlaneLaunchPermissionOverride)	// Снятие блокировки с оружия
	REGISTER_COMMAND_EX(iCommandPlanePickleOn, 350)		// Пуск ракет/сброс бомб для западных самолетов
	REGISTER_COMMAND(iCommandPlanePickleOff)
	REGISTER_COMMAND(iCommandViewCameraBaseReturn)		// Возврат базиса камеры (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraUpLeftSlow)		// Поворот камеры вверх-налево медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraUpRightSlow)		// Поворот камеры вверх-направо медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraDownLeftSlow)		// Поворот камеры вниз-налево медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandViewCameraDownRightSlow)	// Поворот камеры вниз-направо медленный (Non-simulation Action)
	REGISTER_COMMAND(iCommandPlaneDropFlareOnce)			// Однократный сброс ИК-ловушек
	REGISTER_COMMAND(iCommandPlaneDropChaffOnce)			// Однократный сброс дипольных отражателей
	REGISTER_COMMAND(iCommandViewRear)					// Вид назад (Non-simulation Action)
	REGISTER_COMMAND_EX(iCommandScoresWindowToggle, 360)			// показать\скрыть окно сетевой статистики в режиме симуляции
	REGISTER_COMMAND(iCommandCockpitClickOn)		// Начать управление указанным контролом в кабине
	REGISTER_COMMAND(iCommandCockpitClickOff)	// Завершить управление текущим управляемым контролом в кабине
	REGISTER_COMMAND(iCommandCockpitClickModeOnOff)	// Режим Clickable cockpit включить/выключить

	// CMD
	REGISTER_COMMAND(iCommandPlaneCMDDispence)				// Пассивные средства РЭБ выброс ловушек
	REGISTER_COMMAND(iCommandPlaneCMDDispenceOff)
	REGISTER_COMMAND(iCommandPlaneCMDDispenceStop)			// Пассивные средства РЭБ остановка выброса ловушек
	REGISTER_COMMAND(iCommandPlaneCMDDispenceStopOff)
	REGISTER_COMMAND(iCommandPlaneCMDChangeRippleQuantity)	// Пассивные средства РЭБ изменить количество в серии
	REGISTER_COMMAND(iCommandPlaneCMDChangeRippleQuantityOff)
	REGISTER_COMMAND_EX(iCommandPlaneCMDChangeRippleInterval, 370)	// Пассивные средства РЭБ изменить интервал в серии
	REGISTER_COMMAND(iCommandPlaneCMDChangeRippleIntervalOff)
	REGISTER_COMMAND(iCommandPlaneCMDChangeBurstAmount)		// Пассивные средства РЭБ изменить количество серий
	REGISTER_COMMAND(iCommandPlaneCMDChangeBurstAmountOff)
	REGISTER_COMMAND(iCommandPlaneCMDCancelCurrentProgram)	// Пассивные средства РЭБ сброс программы
	REGISTER_COMMAND(iCommandPlaneCMDCancelCurrentProgramOff)
	REGISTER_COMMAND(iCommandPlaneCMDChangeBoard)			// Пассивные средства РЭБ выбор борта
	REGISTER_COMMAND(iCommandPlaneCMDShowAmountOrProgram)	// Пассивные средства РЭБ смена индикации - наличие ловушек или текущая программа

	REGISTER_COMMAND(iCommandPlaneCancelWeaponsDelivery)		// сброс режима работы авионики (Ка - 50)
	REGISTER_COMMAND(iCommandPlaneMasterCautionOff)
	// если МФД несколько распределение происходит по идентификатору устройства  и событию iCommandCockpitClickOn внутри Cockpit см. cockpit::Clickable
	// определение перенесено в Actions.h 	в следствие параметризации вызова по идентификатору устройства
	REGISTER_COMMAND_EX(iCommandPlane_SpotLight_hor_stop, 380)											
	REGISTER_COMMAND(iCommandHelicopter_SelectWeapon_hor_up)
	REGISTER_COMMAND(iCommandHelicopter_SelectWeapon_vert_up)							
	REGISTER_COMMAND(iCommandViewChaseArcade) // chase камера для аркадного вида ) плавающая									
	REGISTER_COMMAND(iCommandAutoLockOnClear)		// снять цель с EasyRadar
	REGISTER_COMMAND_EX(iCommandHelicopterHover_up, 385)		
	REGISTER_COMMAND(iCommandPlaneStabTangBank)			// Стабилизация тангажа и крена
	REGISTER_COMMAND(iCommandPlaneStabHbarBank)			// Стабилизация Hbar и крена
	REGISTER_COMMAND(iCommandPlaneStabHorizon)			// Стабилизация тангажа и крена
	REGISTER_COMMAND(iCommandPlaneStabHbar)				// Стабилизация Hbar
	REGISTER_COMMAND_EX(iCommandPlaneStabHrad, 390)				// Стабилизация Hrad
	REGISTER_COMMAND(iCommandActiveIRJamming)			// IR OnOff
	REGISTER_COMMAND(iCommandPlaneLaserRangerOnOff)		// Вкл/Выкл лазерный дальномер
	REGISTER_COMMAND(iCommandPlaneNightTVOnOff)         // Вкл/Выкл ночной канал телевизионной системы (IR or LLTV) 
	REGISTER_COMMAND(iCommandPlaneChangeRadarPRF)       // Изменить частоту повторения импульсов радара
	REGISTER_COMMAND(iCommandViewKeepTerrain)			// Удерживать свободную камеру около земли
	REGISTER_COMMAND(iCommandViewSnapView0)
	REGISTER_COMMAND(iCommandViewSnapView1)
	REGISTER_COMMAND(iCommandViewSnapView2)
	REGISTER_COMMAND(iCommandViewSnapView3)
	REGISTER_COMMAND_EX(iCommandViewSnapView4, 400)
	REGISTER_COMMAND(iCommandViewSnapView5)
	REGISTER_COMMAND(iCommandViewSnapView6)
	REGISTER_COMMAND(iCommandViewSnapView7)
	REGISTER_COMMAND(iCommandViewSnapView8)
	REGISTER_COMMAND(iCommandViewSnapView9)
	REGISTER_COMMAND(iCommandViewSnapViewStop)
	REGISTER_COMMAND(iCommandViewBinocular)
	REGISTER_COMMAND(iCommandPlaneStabCancel)			// Сброс всех режимов стабилизации и автопилота
	REGISTER_COMMAND(iCommandPlaneThreatWarnSoundVolumeDown)	// Уменьшение громкости звуковых сигналов СПО
	REGISTER_COMMAND_EX(iCommandPlaneThreatWarnSoundVolumeUp, 410)	// Увеличение громкости звуковых сигналов СПО
	REGISTER_COMMAND(iCommandViewLaserOnOff)
	REGISTER_COMMAND(iCommandPlaneIncreaseBase_Distance)     // Увеличение базы цели
	REGISTER_COMMAND(iCommandPlaneDecreaseBase_Distance)     // Уменьшение базы цели
	REGISTER_COMMAND(iCommandPlaneStopBase_Distance)
    REGISTER_COMMAND(iCommandViewIRPointerOnOff)
	REGISTER_COMMAND(iCommandViewNVGOnOff)//!!!!!!Not an avionics action, only view!!!!!!
	/* OBSOLETE */	Obsolete_iCommandViewAWACSZoomOutOn,
	/* OBSOLETE */	Obsolete_iCommandViewAWACSZoomOutOff,
	/* OBSOLETE */	Obsolete_iCommandViewAWACSZoomInOn,
	/* OBSOLETE */	Obsolete_iCommandViewAWACSDetectionAreas,
	/* OBSOLETE */	Obsolete_iCommandViewAWACSDangerousAreas,
	/* OBSOLETE */	Obsolete_iCommandViewAWACSShowLabels,
	REGISTER_COMMAND(iCommandViewAWACSUnitInfoOn)
	REGISTER_COMMAND(iCommandViewAWACSUnitInfoOff)
	REGISTER_COMMAND(iCommandViewIR)
	REGISTER_COMMAND(iCommandViewTargetType)
	REGISTER_COMMAND(iCommandPlaneAutopilotOverrideOn)
	REGISTER_COMMAND(iCommandPlaneAutopilotOverrideOff)
	REGISTER_COMMAND(iCommandPlaneRouteAutopilot)
	REGISTER_COMMAND_EX(iCommandPlaneGearUp, 430)
	REGISTER_COMMAND(iCommandPlaneGearDown)
	REGISTER_COMMAND(iCommandViewAWACSJump)
	REGISTER_COMMAND(iCommandCockpitSecondClickOn)	// есть некоторые контролы которые одновременно ось и кнопка 
	REGISTER_COMMAND(iCommandCockpitSecondClickOff)	// Завершить управление текущим управляемым контролом в кабине
	REGISTER_COMMAND(iCommandCompassRose)
	REGISTER_COMMAND(iCommandViewAWACSClearDistance)
	REGISTER_COMMAND(iCommandViewAWACSOutOfFocus)
	REGISTER_COMMAND(iCommandViewNightVisionGogglesOn)
	REGISTER_COMMAND(iCommandPlaneDesignate_CageOn)
	REGISTER_COMMAND_EX(iCommandPlaneDesignate_CageOff, 440)
	REGISTER_COMMAND(iCommandPlaneDesignate_CageOn_vertical)
	REGISTER_COMMAND(iCommandPlaneDesignate_CageOn_horizontal)

	//Data Link Panel PRTs (Пульт Режимов Целеуказания ПРЦ)
	REGISTER_COMMAND(iCommandPlaneDLK_Target1)
	REGISTER_COMMAND(iCommandPlaneDLK_Target2)
	REGISTER_COMMAND(iCommandPlaneDLK_Target3)
	REGISTER_COMMAND(iCommandPlaneDLK_RefPoint)
	REGISTER_COMMAND(iCommandPlaneDLK_Wingman1)
	REGISTER_COMMAND(iCommandPlaneDLK_Wingman2)
	REGISTER_COMMAND(iCommandPlaneDLK_Wingman3)
	REGISTER_COMMAND_EX(iCommandPlaneDLK_Wingman4, 450)
	REGISTER_COMMAND(iCommandPlaneDLK_All)
	REGISTER_COMMAND(iCommandPlaneDLK_Erase)
	REGISTER_COMMAND(iCommandPlaneDLK_Ingress)
	REGISTER_COMMAND(iCommandPlaneDLK_SendMemory)

	// NAV panel controls (Ка-50 - щиток ПВИ)
	REGISTER_COMMAND(iCommandPlaneNavChangePanelModeRight)
	REGISTER_COMMAND(iCommandPlaneNavChangePanelModeLeft)
	REGISTER_COMMAND(iCommandPlaneNavSetFixtakingMode)
	REGISTER_COMMAND(iCommandPlaneNav_DLK_OnOff)
	REGISTER_COMMAND(iCommandPlaneNav_PB0)
	REGISTER_COMMAND_EX(iCommandPlaneNav_PB1, 460)
	REGISTER_COMMAND(iCommandPlaneNav_PB2)
	REGISTER_COMMAND(iCommandPlaneNav_PB3)
	REGISTER_COMMAND(iCommandPlaneNav_PB4)
	REGISTER_COMMAND(iCommandPlaneNav_PB5)
	REGISTER_COMMAND(iCommandPlaneNav_PB6)
	REGISTER_COMMAND(iCommandPlaneNav_PB7)
	REGISTER_COMMAND(iCommandPlaneNav_PB8)
	REGISTER_COMMAND(iCommandPlaneNav_PB9)
	REGISTER_COMMAND(iCommandPlaneNav_Steerpoints)
	REGISTER_COMMAND_EX(iCommandPlaneNav_INU_realign, 470)
	REGISTER_COMMAND(iCommandPlaneNav_POS_corrMode)
	REGISTER_COMMAND(iCommandPlaneNav_INU_precise_align)
	REGISTER_COMMAND(iCommandPlaneNav_Airfields)
	REGISTER_COMMAND(iCommandPlaneNav_INU_normal_align)
	REGISTER_COMMAND(iCommandPlaneNav_Targets)
	REGISTER_COMMAND(iCommandPlaneNav_Enter)
	REGISTER_COMMAND(iCommandPlaneNav_Cancel)
	REGISTER_COMMAND(iCommandPlaneNav_POS_init)
	REGISTER_COMMAND(iCommandPlaneNav_SelfCoord)
	REGISTER_COMMAND_EX(iCommandPlaneNav_CourseTimeRange, 480)
	REGISTER_COMMAND(iCommandPlaneNav_Wind)
	REGISTER_COMMAND(iCommandPlaneNav_THeadingTimeRangeF)
	REGISTER_COMMAND(iCommandPlaneNav_BearingRangeT)

	REGISTER_COMMAND(iCommandViewPitCameraMoveUp)
	REGISTER_COMMAND_EX(iCommandViewPitCameraMoveDown, 485)
	REGISTER_COMMAND(iCommandViewPitCameraMoveLeft)
	REGISTER_COMMAND(iCommandViewPitCameraMoveRight)
	REGISTER_COMMAND(iCommandViewPitCameraMoveForward)
	REGISTER_COMMAND(iCommandViewPitCameraMoveBack)	
	REGISTER_COMMAND_EX(iCommandViewPitCameraMoveStop, 490)
	REGISTER_COMMAND(iCommandViewPitCameraMoveCenter)
	REGISTER_COMMAND(iCommandViewPitHeadOnOff)
	REGISTER_COMMAND(iCommandPlaneCockpitIlluminationPanels) //obsolete
	REGISTER_COMMAND(iCommandPlaneCockpitIlluminationGauges) //obsolete

	//ABRIS
	REGISTER_COMMAND_EX(iCommandPlane_ABRIS_Power, 495)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_1)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_2)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_3)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_4)
	REGISTER_COMMAND_EX(iCommandPlane_ABRIS_BT_5, 500)
	REGISTER_COMMAND(iCommandPlane_ABRIS_Axis_Increase)
	REGISTER_COMMAND(iCommandPlane_ABRIS_Axis_Decrease)
	REGISTER_COMMAND(iCommandPlane_ABRIS_Axis_Push)
	REGISTER_COMMAND(iCommandPlane_ABRIS_Brightness_Increase)
	REGISTER_COMMAND(iCommandPlane_ABRIS_Brightness_Decrease)

	REGISTER_COMMAND(iCommandPlane_RouteMode)
	REGISTER_COMMAND(iCommandPlane_DescentMode)
	REGISTER_COMMAND_EX(iCommandPlane_DescentModeOff, 508)

	REGISTER_COMMAND(iCommandPlane_LockOn_start)          //Ка-50 АЗ нажать 
	REGISTER_COMMAND_EX(iCommandPlane_LockOn_finish  , 510) //Ка-50 АЗ отжать

	REGISTER_COMMAND(iCommandPlane_SpotLight_left)
	REGISTER_COMMAND(iCommandPlane_SpotLight_right)
	REGISTER_COMMAND(iCommandPlane_SpotLight_up)
	REGISTER_COMMAND(iCommandPlane_SpotLight_down)
	REGISTER_COMMAND_EX(iCommandPlane_SpotLight_vert_stop, 515)

	REGISTER_COMMAND(iCommandPlaneRotorTipLights)
	REGISTER_COMMAND(iCommandPlane_SpotSelect_switch)
	REGISTER_COMMAND(iCommandPlaneAntiCollisionLights)
	REGISTER_COMMAND(iCommandPlaneNavLights_CodeModeOn)
	REGISTER_COMMAND_EX(iCommandPlaneNavLights_CodeModeOff, 520)
	REGISTER_COMMAND(iCommandPlaneFormationLights)

	REGISTER_COMMAND(iCommandPlane_EngageAirDefenses)
	REGISTER_COMMAND(iCommandPlane_EngageGroundTargets)

	REGISTER_COMMAND(iCommandPlane_AutomaticTracking_Gunsight_switch)
	REGISTER_COMMAND_EX(iCommandPlane_K041_Power, 525)
	REGISTER_COMMAND(iCommandPlane_AutomaticTurn)
	REGISTER_COMMAND(iCommandPlane_GroundMovingTarget)
	REGISTER_COMMAND(iCommandPlane_AirborneTarget)
	REGISTER_COMMAND(iCommandPlane_HeadOnAspect)
	REGISTER_COMMAND_EX(iCommandPlane_ChangeDeliveryMode_right, 530)
	REGISTER_COMMAND(iCommandPlane_ChangeDeliveryMode_left)

	REGISTER_COMMAND(iCommandPlane_WeaponMode_Manual_Auto)
	REGISTER_COMMAND(iCommandPlane_WeaponMode_switch)
	REGISTER_COMMAND(iCommandPlane_AmmoTypeSelect)
	REGISTER_COMMAND(iCommandPlane_FireRate)

	REGISTER_COMMAND_EX(iCommandPlaneDropSnarOnceOff, 536)

	REGISTER_COMMAND(iCommandHelicopterHover)
	REGISTER_COMMAND(iCommandAutopilotEmergOFF)

	// HOTAS
	// В качестве Slew Control Hat Switch (TDC)
	// и Weapon Release Switch будут использоваться
	// уже существующие команды (iCommandSelecter... и
	// iCommandPlanePickleOn).
	// Соответственно они должны находиться в категории HOTAS

	REGISTER_COMMAND(iCommandPlane_HOTAS_CoolieUp)
	REGISTER_COMMAND_EX(iCommandPlane_HOTAS_CoolieDown, 540)
	REGISTER_COMMAND(iCommandPlane_HOTAS_CoolieLeft)
	REGISTER_COMMAND(iCommandPlane_HOTAS_CoolieRight)
	REGISTER_COMMAND(iCommandPlane_HOTAS_CoolieOff)

	REGISTER_COMMAND(iCommandPlane_HOTAS_TargetManagementSwitchUp)
	REGISTER_COMMAND_EX(iCommandPlane_HOTAS_TargetManagementSwitchDown, 545)
	REGISTER_COMMAND(iCommandPlane_HOTAS_TargetManagementSwitchLeft)
	REGISTER_COMMAND(iCommandPlane_HOTAS_TargetManagementSwitchRight)
	REGISTER_COMMAND(iCommandPlane_HOTAS_TargetManagementSwitchOff)

	REGISTER_COMMAND(iCommandPlane_HOTAS_DataManagementSwitchUp)
	REGISTER_COMMAND_EX(iCommandPlane_HOTAS_DataManagementSwitchDown, 550)
	REGISTER_COMMAND(iCommandPlane_HOTAS_DataManagementSwitchLeft)
	REGISTER_COMMAND(iCommandPlane_HOTAS_DataManagementSwitchRight)
	REGISTER_COMMAND(iCommandPlane_HOTAS_DataManagementSwitchOff)

	REGISTER_COMMAND(iCommandPlane_HOTAS_TriggerSecondStage)
	REGISTER_COMMAND(iCommandPlane_HOTAS_TriggerFirstStage)

	REGISTER_COMMAND(iCommandPlane_HOTAS_CMS_Up)
	REGISTER_COMMAND(iCommandPlane_HOTAS_CMS_Down)
	REGISTER_COMMAND(iCommandPlane_HOTAS_CMS_Left)
	REGISTER_COMMAND(iCommandPlane_HOTAS_CMS_Right)
	REGISTER_COMMAND_EX(iCommandPlane_HOTAS_CMS_Off, 560)

	REGISTER_COMMAND(iCommandPlane_HOTAS_MasterModeControlButton)
	REGISTER_COMMAND(iCommandPlane_HOTAS_NoseWheelSteeringButton)

	REGISTER_COMMAND(iCommandPlane_HOTAS_BoatSwitchForward)
	REGISTER_COMMAND(iCommandPlane_HOTAS_BoatSwitchAft)
	REGISTER_COMMAND(iCommandPlane_HOTAS_BoatSwitchCenter)

	REGISTER_COMMAND(iCommandPlane_HOTAS_ChinaHatForward)
	REGISTER_COMMAND(iCommandPlane_HOTAS_ChinaHatAft)

	REGISTER_COMMAND(iCommandPlane_HOTAS_PinkySwitchForward)
	REGISTER_COMMAND(iCommandPlane_HOTAS_PinkySwitchAft)
	REGISTER_COMMAND_EX(iCommandPlane_HOTAS_PinkySwitchCenter, 570)

	REGISTER_COMMAND(iCommandPlane_HOTAS_LeftThrottleButton)

	REGISTER_COMMAND(iCommandPlane_HOTAS_MIC_SwitchUp)
	REGISTER_COMMAND(iCommandPlane_HOTAS_MIC_SwitchDown)
	REGISTER_COMMAND(iCommandPlane_HOTAS_MIC_SwitchLeft)
	REGISTER_COMMAND(iCommandPlane_HOTAS_MIC_SwitchRight)
	REGISTER_COMMAND(iCommandPlane_HOTAS_MIC_SwitchOff)

	REGISTER_COMMAND(iCommandPlane_HOTAS_SpeedBrakeSwitchForward)
	REGISTER_COMMAND(iCommandPlane_HOTAS_SpeedBrakeSwitchAft)
	REGISTER_COMMAND(iCommandPlane_HOTAS_SpeedBrakeSwitchCenter)

	REGISTER_COMMAND(iCommandPlane_ADF_Homer_change)	
	REGISTER_COMMAND(iCommandPlane_ADF_Channel_next)
	REGISTER_COMMAND(iCommandPlane_ADF_Channel_prevous)
	REGISTER_COMMAND(iCommandPlane_ADF_Mode_change)
	REGISTER_COMMAND_EX(iCommandPlane_ADF_Receiver_Mode_change, 585)
	REGISTER_COMMAND(iCommandPlane_ADF_Volume_up)
	REGISTER_COMMAND(iCommandPlane_ADF_Volume_down)
	REGISTER_COMMAND(iCommandPlane_ADF_Test)

	REGISTER_COMMAND(iCommandPlane_HOTAS_ChinaHatOff)

	REGISTER_COMMAND_EX(iCommandHelicopter_PPR_button_K, 590)
	REGISTER_COMMAND(iCommandHelicopter_PPR_button_H)
	REGISTER_COMMAND(iCommandHelicopter_PPR_button_T)
	REGISTER_COMMAND(iCommandHelicopter_PPR_button_B)
	REGISTER_COMMAND(iCommandHelicopter_PPR_button_DIR)
	REGISTER_COMMAND_EX(iCommandHelicopter_PPR_BAR_RV, 595)

	REGISTER_COMMAND(iCommandHelicopter_Selected_Engine_start)
	REGISTER_COMMAND(iCommandHelicopter_Selected_Engine_interrupt_start)
	REGISTER_COMMAND(iCommandHelicopter_APU_stop)	
	REGISTER_COMMAND(iCommandHelicopter_Engine_Select)

	REGISTER_COMMAND(iCommandHelicopter_Left_Engine_Lock)
	REGISTER_COMMAND(iCommandHelicopter_Right_Engine_Lock)
	REGISTER_COMMAND_EX(iCommandHelicopter_Rotor_Lock, 605)

	REGISTER_COMMAND(iCommandPlane_HOTAS_NoseWheelSteeringButtonOff)

	//REGISTER_COMMAND(iCommandViewNightVisionGogglesOff)

	REGISTER_COMMAND(iCommandPlane_R_800_1st_wheel_up)
	REGISTER_COMMAND(iCommandPlane_R_800_1st_wheel_down)
	REGISTER_COMMAND(iCommandPlane_R_800_2nd_wheel_up)
	REGISTER_COMMAND_EX(iCommandPlane_R_800_2nd_wheel_down, 610)
	REGISTER_COMMAND(iCommandPlane_R_800_3rd_wheel_up)
	REGISTER_COMMAND(iCommandPlane_R_800_3rd_wheel_down)
	REGISTER_COMMAND(iCommandPlane_R_800_4th_wheel_up)
	REGISTER_COMMAND(iCommandPlane_R_800_4th_wheel_down)
	REGISTER_COMMAND_EX(iCommandPlane_R_800_Noise_Reductor_on_off, 615)
	REGISTER_COMMAND(iCommandPlane_R_800_Modulation_change)
	REGISTER_COMMAND(iCommandPlane_R_800_50_100_switch)
	REGISTER_COMMAND(iCommandPlane_R_800_ADF_Sound_switch)
	REGISTER_COMMAND(iCommandPlane_R_800_Emergency_Radio)
	REGISTER_COMMAND_EX(iCommandPlane_R_800_Test, 620)
	REGISTER_COMMAND(iCommandPlane_R_800_Power_on_off)//тумблер УКВ-2

	REGISTER_COMMAND(iCommandPlane_R_828_Channel_next)	
	REGISTER_COMMAND(iCommandPlane_R_828_Channel_prevous)
	REGISTER_COMMAND(iCommandPlane_R_828_Noise_Reductor_on_off)
	REGISTER_COMMAND_EX(iCommandPlane_R_828_ASU, 625)
	REGISTER_COMMAND(iCommandPlane_R_828_volume_up)
	REGISTER_COMMAND(iCommandPlane_R_828_volume_down)
	REGISTER_COMMAND(iCommandPlane_R_828_Power_on_off)//тумблер УКВ-1

	REGISTER_COMMAND(iCommandPlane_SPU9_SPU_9_on_off) //тумблер АВСК
	REGISTER_COMMAND_EX(iCommandPlane_SPU9_radio_change, 630)

	REGISTER_COMMAND(iCommandPlaneCollectiveIncrease)  //дискретное управление ручкой ШАГ-ГАЗ
	REGISTER_COMMAND(iCommandPlaneCollectiveDecrease)

	REGISTER_COMMAND(iCommandPlane_HOTAS_MasterModeControlButtonUP)

	REGISTER_COMMAND(iCommandPlane_HOTAS_TDC_depress_on)
	REGISTER_COMMAND_EX(iCommandPlane_HOTAS_TDC_depress_off, 635)

	REGISTER_COMMAND(iCommandPlaneStabHbarHeading)
	REGISTER_COMMAND(iCommandPlaneStabPathHold) 

	REGISTER_COMMAND(iCommandPlane_HOTAS_TriggerSecondStage_Off)
	REGISTER_COMMAND(iCommandPlane_HOTAS_TriggerFirstStage_Off)

	REGISTER_COMMAND_EX(iCommandPlaneDLK_Self_ID_change, 640)
	REGISTER_COMMAND(iCommandPlaneDLK_mode_change)

	REGISTER_COMMAND_EX(iCommandPlaneLeftMFD_OSB1,642)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB2)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB3)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB4)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB5)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB6)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB7)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB8)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB9)//650
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB10)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB11)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB12)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB13)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB14)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB15)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB16)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB17)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB18)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB19)//660
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB20)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_ADJ_Increase)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_ADJ_Decrease)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_DSP_Increase)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_DSP_Decrease)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_BRT_Increase)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_BRT_Decrease)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_CON_Increase)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_CON_Decrease)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_SYM_Up)//670
	REGISTER_COMMAND(iCommandPlaneLeftMFD_SYM_Dn)

	REGISTER_COMMAND_EX(iCommandPlaneRightMFD_OSB1,672)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB2)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB3)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB4)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB5)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB6)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB7)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB8)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB9)//680
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB10)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB11)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB12)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB13)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB14)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB15)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB16)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB17)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB18)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB19)//690
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB20)

	REGISTER_COMMAND(iCommandPlaneRightMFD_ADJ_Increase)
	REGISTER_COMMAND(iCommandPlaneRightMFD_ADJ_Decrease)
	REGISTER_COMMAND(iCommandPlaneRightMFD_DSP_Increase)
	REGISTER_COMMAND(iCommandPlaneRightMFD_DSP_Decrease)
	REGISTER_COMMAND(iCommandPlaneRightMFD_BRT_Increase)
	REGISTER_COMMAND(iCommandPlaneRightMFD_BRT_Decrease)
	REGISTER_COMMAND(iCommandPlaneRightMFD_CON_Increase)
	REGISTER_COMMAND(iCommandPlaneRightMFD_CON_Decrease)
	REGISTER_COMMAND(iCommandPlaneRightMFD_SYM_Up)//700
	REGISTER_COMMAND(iCommandPlaneRightMFD_SYM_Dn)

/*OBSOLETE*/	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB_OFF)
/*OBSOLETE*/	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB_OFF)

	//ELECTRIC
	REGISTER_COMMAND(iCommandGroundPowerDC)
	REGISTER_COMMAND_EX(iCommandGroundPowerDC_Cover, 705)	
	REGISTER_COMMAND(iCommandPowerBattery1)
	REGISTER_COMMAND(iCommandPowerBattery1_Cover)
	REGISTER_COMMAND(iCommandPowerBattery2)	
	REGISTER_COMMAND(iCommandPowerBattery2_Cover)
	REGISTER_COMMAND_EX(iCommandGroundPowerAC, 710)
	REGISTER_COMMAND(iCommandPowerGeneratorLeft)
	REGISTER_COMMAND(iCommandPowerGeneratorRight)
	REGISTER_COMMAND(iCommandElectricalPowerInverter)
	//DATALINK
	REGISTER_COMMAND(iCommandDatalinkTLK)
	REGISTER_COMMAND_EX(iCommandDatalinkUHF_TLK, 715)
	//SCRAMBLER
	REGISTER_COMMAND(iCommandDatalinkUHF_SA)

	//FUEL
	REGISTER_COMMAND(iCommandFuelPumpsTankForward)
	REGISTER_COMMAND(iCommandFuelPumpsTankRear)
	REGISTER_COMMAND(iCommandFuelExternalPumpsTanksInner)
	REGISTER_COMMAND_EX(iCommandFuelExternalPumpsTanksOuter, 720)
	REGISTER_COMMAND(iCommandFuelmeterPower)
	REGISTER_COMMAND(iCommandLeftEngineFuelValveShutOff)
	REGISTER_COMMAND(iCommandLeftEngineFuelValveShutOffCover)
	REGISTER_COMMAND(iCommandRightEngineFuelValveShutOff)
	REGISTER_COMMAND_EX(iCommandRightEngineFuelValveShutOffCover, 725)
	REGISTER_COMMAND(iCommandAPUFuelValveShutOff)
	REGISTER_COMMAND(iCommandAPUFuelValveShutOffCover)
	REGISTER_COMMAND(iCommandFuelBypassValve)
	REGISTER_COMMAND(iCommandFuelBypassValveCover)

	//EJECTING SYSTEM
	REGISTER_COMMAND_EX(iCommandEjectingSystemPower1, 730)
	REGISTER_COMMAND(iCommandEjectingSystemPower1Cover)
	REGISTER_COMMAND(iCommandEjectingSystemPower2)
	REGISTER_COMMAND(iCommandEjectingSystemPower3)

	//OTHERS
	REGISTER_COMMAND(iCommandWeaponsControlSystem)
	REGISTER_COMMAND_EX(iCommandWeaponsControlSystemCover, 735)
	REGISTER_COMMAND(iCommandMainHydraulics)
	REGISTER_COMMAND(iCommandMainHydraulicsCover)
	REGISTER_COMMAND(iCommandIFF_Power)
	REGISTER_COMMAND(iCommandIFF_PowerCover)
	REGISTER_COMMAND_EX(iCommandTargetingNavigationSystemsPower, 740)

	REGISTER_COMMAND(iCommandElectronicControlSystemLeftEngine)
	REGISTER_COMMAND(iCommandElectronicControlSystemLeftEngineCover)
	REGISTER_COMMAND(iCommandElectronicControlSystemRightEngine)
	REGISTER_COMMAND(iCommandElectronicControlSystemRightEngineCover)
	REGISTER_COMMAND_EX(iCommandHeadingMethodSwitch, 745)

	//HUD
	REGISTER_COMMAND(iCommandHUDBrightnessUp)
	REGISTER_COMMAND(iCommandHUDBrightnessDown)
	REGISTER_COMMAND(iCommandHUDTest)
	REGISTER_COMMAND(iCommandHUDTest_up)

	//GEARS
	REGISTER_COMMAND_EX(iCommandEmergencyGear, 750)
	REGISTER_COMMAND(iCommandEmergencyGearCover)

	//CLOCK
	REGISTER_COMMAND(iCommandMechClock_LeftLever_Down)
	REGISTER_COMMAND(iCommandMechClock_LeftLever_Down_up)
	REGISTER_COMMAND(iCommandMechClock_LeftLever_Up)
	REGISTER_COMMAND_EX(iCommandMechClock_LeftLever_TurnLeft, 755)
	REGISTER_COMMAND(iCommandMechClock_LeftLever_TurnRight)
	REGISTER_COMMAND(iCommandMechClock_RightLever_Down)
	REGISTER_COMMAND(iCommandMechClock_RightLever_Down_up)
	REGISTER_COMMAND(iCommandMechClock_RightLever_Rotate_left)
	REGISTER_COMMAND_EX(iCommandMechClock_RightLever_Rotate_right, 760)

	//SYSTEMs
	REGISTER_COMMAND(iCommandBKOMode)
	REGISTER_COMMAND(iCommandLampsControl)
	REGISTER_COMMAND(iCommandRotorRPMHigh)

	//HSI
	REGISTER_COMMAND(iCommandHSI_CourseRotaryLeft)
	REGISTER_COMMAND_EX(iCommandHSI_CourseRotaryRight, 765)
	REGISTER_COMMAND(iCommandHSI_HeadingRotaryLeft)
	REGISTER_COMMAND(iCommandHSI_HeadingRotaryRight)
	REGISTER_COMMAND(iCommandHSI_Test)
	REGISTER_COMMAND(iCommandHSI_Test_up)
	REGISTER_COMMAND_EX(iCommandHSI_SteeringSelection, 770)

	//ADI
	REGISTER_COMMAND(iCommandADI_ZeroPitchTrimLeft)
	REGISTER_COMMAND(iCommandADI_ZeroPitchTrimRight)
	REGISTER_COMMAND(iCommandADI_Test)
	REGISTER_COMMAND(iCommandADI_Test_up)
	REGISTER_COMMAND_EX(iCommandADI_TestCover, 775)

	//RALT
	REGISTER_COMMAND(iCommandRALT_DangerousAltitudeRotaryLeft)
	REGISTER_COMMAND(iCommandRALT_DangerousAltitudeRotaryRight)
	REGISTER_COMMAND(iCommandRALT_Test)
	REGISTER_COMMAND(iCommandRALT_Test_up)

	//UPs
	//DATALINK
	REGISTER_COMMAND_EX(iCommandPlaneDLK_Target1_up, 780)
	REGISTER_COMMAND(iCommandPlaneDLK_Target2_up)
	REGISTER_COMMAND(iCommandPlaneDLK_Target3_up)
	REGISTER_COMMAND(iCommandPlaneDLK_RefPoint_up)
	REGISTER_COMMAND(iCommandPlaneDLK_Wingman1_up)
	REGISTER_COMMAND_EX(iCommandPlaneDLK_Wingman2_up, 785)
	REGISTER_COMMAND(iCommandPlaneDLK_Wingman3_up)
	REGISTER_COMMAND(iCommandPlaneDLK_Wingman4_up)
	REGISTER_COMMAND(iCommandPlaneDLK_All_up)
	REGISTER_COMMAND(iCommandPlaneDLK_Erase_up)
	REGISTER_COMMAND_EX(iCommandPlaneDLK_Ingress_up, 790)
	REGISTER_COMMAND(iCommandPlaneDLK_SendMemory_up)
	//RADIOs
	REGISTER_COMMAND(iCommandPlane_ADF_Test_up)
	REGISTER_COMMAND(iCommandPlane_R_800_Test_up)
	REGISTER_COMMAND(iCommandPlane_R_828_ASU_up)
	//ПВР
	REGISTER_COMMAND_EX(iCommandPlane_AutomaticTurn_up, 795)
	REGISTER_COMMAND(iCommandPlane_GroundMovingTarget_up)
	REGISTER_COMMAND(iCommandPlane_AirborneTarget_up)
	REGISTER_COMMAND(iCommandPlane_HeadOnAspect_up)
	REGISTER_COMMAND(iCommandPlaneCancelWeaponsDelivery_up)
	//SYSTEM
	REGISTER_COMMAND_EX(iCommandLampsControl_up, 800)
	REGISTER_COMMAND(iCommandRotorRPMHigh_up)
	//ENGINES
	REGISTER_COMMAND(iCommandHelicopter_Selected_Engine_start_up)
	REGISTER_COMMAND(iCommandHelicopter_Selected_Engine_interrupt_start_up)
	REGISTER_COMMAND(iCommandHelicopter_APU_stop_up)
	//ABRIS
	REGISTER_COMMAND_EX(iCommandPlane_ABRIS_BT_1_up, 805)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_2_up)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_3_up)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_4_up)
	REGISTER_COMMAND(iCommandPlane_ABRIS_BT_5_up)
	REGISTER_COMMAND_EX(iCommandPlane_ABRIS_Axis_Increase_up, 810)
	REGISTER_COMMAND(iCommandPlane_ABRIS_Axis_Decrease_up)
	REGISTER_COMMAND(iCommandPlane_ABRIS_Axis_Push_up)
	REGISTER_COMMAND(iCommandPlaneADIAHRIllumination)	
	REGISTER_COMMAND(iCommandControlPanelIllumination)
	REGISTER_COMMAND_EX(iCommandCockpitIllumination, 815) //obsolete
	REGISTER_COMMAND(iCommandActivePauseOnOff) // 'Active Pause' feature

	//ENGINES CONTROL additional
	REGISTER_COMMAND(iCommandHelicopter_Engine_Launch_method)
	REGISTER_COMMAND(iCommandHelicopter_Rotor_Anti_Icing_System)
	REGISTER_COMMAND(iCommandHelicopter_Engines_Anti_Icing_Dust_Protection_System)
	REGISTER_COMMAND_EX(iCommandHelicopter_TC_RPM, 820)
	REGISTER_COMMAND(iCommandHelicopter_TC_RPM_Cover)
	REGISTER_COMMAND(iCommandHelicopter_Engine_Temperature_Control)
	REGISTER_COMMAND(iCommandHelicopter_Engine_Temperature_Control_Cover)

	REGISTER_COMMAND(iCommandHelicopter_SelectWeapon_Internal)
	REGISTER_COMMAND_EX(iCommandHelicopter_SelectWeapon_External, 825)
	REGISTER_COMMAND(iCommandHelicopter_SelectWeapon_ALL)
	REGISTER_COMMAND(iCommandHelicopter_SelectWeapon_AA)

	REGISTER_COMMAND(iCommandHelicopter_SignalFlares_switch_OnOff)
	REGISTER_COMMAND(iCommandHelicopter_SignalFlares_GREEN)
	REGISTER_COMMAND_EX(iCommandHelicopter_SignalFlares_RED, 830)
	REGISTER_COMMAND(iCommandHelicopter_SignalFlares_WHITE)
	REGISTER_COMMAND(iCommandHelicopter_SignalFlares_YELLOW)

	REGISTER_COMMAND(iCommandHelicopter_PanelsIlluminationButtonDown)
	REGISTER_COMMAND(iCommandHelicopter_PanelsIlluminationButtonUp)

	REGISTER_COMMAND_EX(iCommandCockpitShowPilotOnOff, 835)

	REGISTER_COMMAND(iCommandEasyRadarScaleUp)
	REGISTER_COMMAND(iCommandEasyRadarScaleOut)

	REGISTER_COMMAND(iCommandEngineLaunchMethod)

	REGISTER_COMMAND(iCommandPlaneCollectiveStop)

	REGISTER_COMMAND_EX(iCommandPlane_Helmet_Brightess_Up, 840)
	REGISTER_COMMAND(iCommandPlane_Helmet_Brightess_Down)

	REGISTER_COMMAND(iCommandPlane_I251_Brightness_Up)
	REGISTER_COMMAND(iCommandPlane_I251_Brightness_Down)

	REGISTER_COMMAND(iCommandPlane_I251_Contrast_Up)
	REGISTER_COMMAND_EX(iCommandPlane_I251_Contrast_Down, 845)

	REGISTER_COMMAND(iCommandPlane_I251_Background_WhiteBlack)

	REGISTER_COMMAND(iCommandPlane_LaserCode_Next)
	REGISTER_COMMAND(iCommandPlane_LaserCode_Previous)

	REGISTER_COMMAND(iCommandReloadTables)			// Перезагрузить таблицы. Сугубо отладочная команда.

	REGISTER_COMMAND_EX(iCommandPlane_JettisonMode_explosion, 850)
	REGISTER_COMMAND(iCommandPlane_ShowControls)

	REGISTER_COMMAND(iCommandPlane_CallHelpWindow)

	// Перенастройка оборотов свободной турбины ("Номин. - Низк.")
	REGISTER_COMMAND(iCommandPlane_ReadjustFreeTurbineRPM_Up)
	REGISTER_COMMAND(iCommandPlane_ReadjustFreeTurbineRPM_Down)

	REGISTER_COMMAND_EX(iCommandPlaneWheelParkingBrake, 855)	// стояночный тормоз колес

	REGISTER_COMMAND(iCommandHelicopter_CollectiveStopper_Up)
	REGISTER_COMMAND(iCommandHelicopter_CollectiveStopper_Down)

	REGISTER_COMMAND(iCommandPlaneNav_PB0_off)
	REGISTER_COMMAND(iCommandPlaneNav_PB1_off)
	REGISTER_COMMAND_EX(iCommandPlaneNav_PB2_off, 860)
	REGISTER_COMMAND(iCommandPlaneNav_PB3_off)
	REGISTER_COMMAND(iCommandPlaneNav_PB4_off)
	REGISTER_COMMAND(iCommandPlaneNav_PB5_off)
	REGISTER_COMMAND(iCommandPlaneNav_PB6_off)
	REGISTER_COMMAND(iCommandPlaneNav_PB7_off)
	REGISTER_COMMAND(iCommandPlaneNav_PB8_off)
	REGISTER_COMMAND(iCommandPlaneNav_PB9_off)


	REGISTER_COMMAND(iCommandPlaneNav_Steerpoints_off)
	REGISTER_COMMAND(iCommandPlaneNav_INU_realign_off)
	REGISTER_COMMAND_EX(iCommandPlaneNav_POS_corrMode_off, 870)
	REGISTER_COMMAND(iCommandPlaneNav_INU_precise_align_off)
	REGISTER_COMMAND(iCommandPlaneNav_Airfields_off)
	REGISTER_COMMAND(iCommandPlaneNav_INU_normal_align_off)
	REGISTER_COMMAND(iCommandPlaneNav_Targets_off)
	REGISTER_COMMAND(iCommandPlaneNav_Enter_off)
	REGISTER_COMMAND(iCommandPlaneNav_Cancel_off)
	REGISTER_COMMAND(iCommandPlaneNav_POS_init_off)
	REGISTER_COMMAND(iCommandPlaneNav_SelfCoord_off)
	REGISTER_COMMAND(iCommandPlaneNav_CourseTimeRange_off)
	REGISTER_COMMAND_EX(iCommandPlaneNav_Wind_off, 880)
	REGISTER_COMMAND(iCommandPlaneNav_THeadingTimeRangeF_off)
	REGISTER_COMMAND(iCommandPlaneNav_BearingRangeT_off)

	//верхняя левая панель
	REGISTER_COMMAND(iCommand_CPT_MECH_WindscreenWiper_Speed)
	REGISTER_COMMAND(iCommandAccelReset)
	// LWS
	REGISTER_COMMAND(iCommand_LWS_Reset) 
	// LaseRanger
	REGISTER_COMMAND(iCommandLaserRangeModeSwitchCover)
	REGISTER_COMMAND(iCommandLaserRangeModeSwitch)
	REGISTER_COMMAND(iCommandLaserRangeReset)
	// AGR
	REGISTER_COMMAND(iCommand_AGR_Power)
	REGISTER_COMMAND_EX(iCommand_WEAP_INTERF_MasterSimulate, 890)
	REGISTER_COMMAND(iCommand_CPT_MECH_VMG_HYDRO_EKRAN_Cover)
	REGISTER_COMMAND(iCommand_CPT_MECH_VMG_HYDRO_EKRAN)
	REGISTER_COMMAND(iCommand_LWS_BIT_On)
	REGISTER_COMMAND(iCommand_IlluminationInterf_Control_Panel_Illumination)
	REGISTER_COMMAND(iCommand_LWS_Power)
	REGISTER_COMMAND(iCommand_UV26_PowerCover)
	REGISTER_COMMAND(iCommand_UV26_Power)
	REGISTER_COMMAND(iCommand_UV26_BITCover)
	REGISTER_COMMAND(iCommand_UV26_BIT)
	REGISTER_COMMAND_EX(iCommand_C061K_power, 900)
	REGISTER_COMMAND(iCommand_C061K_heat)
	REGISTER_COMMAND(iCommand_VMS_ALMAZ_UP_Check)
	REGISTER_COMMAND(iCommand_EjectInterface_Circuit_Test)
	REGISTER_COMMAND(iCommand_ENG_INTERF_Right_Engine_RT_12_6)
	REGISTER_COMMAND(iCommand_ENG_INTERF_Left_Engine_RT_12_6)
	REGISTER_COMMAND(iCommand_ENG_INTERF_EngVibrDetectorBIT)

	REGISTER_COMMAND(iCommand_ENG_INTERF_TempIndTestwRunningEng)
	REGISTER_COMMAND(iCommand_ENG_INTERF_TempIndTestwStoppedEng)

	REGISTER_COMMAND(iCommand_FuelSysInterf_FuelQuantityIndTestOn)

	REGISTER_COMMAND_EX(iCommandPitotAoAHeat, 910)
	REGISTER_COMMAND(iCommandPitotRam)
	REGISTER_COMMAND(iCommandHelicopter_Rotor_Anti_Icing_Dust_Protection_System)

	REGISTER_COMMAND(iCommand_VMS_ALMAZ_UP_EmergencyOn)
	REGISTER_COMMAND(iCommand_VMS_ALMAZ_UP_SpeechInfoOff)
	REGISTER_COMMAND(iCommand_VMS_ALMAZ_UP_Repeat)

	REGISTER_COMMAND(iCommand_SHKVAL_scan_vel_up)
	REGISTER_COMMAND(iCommand_SHKVAL_scan_vel_down)
	REGISTER_COMMAND(iCommand_SHKVAL_windscreen_wiper)
	REGISTER_COMMAND(iCommand_PVI_BRIGHTNESS_LEVER_up)
	REGISTER_COMMAND_EX(iCommand_PVI_BRIGHTNESS_LEVER_down, 920)
	REGISTER_COMMAND(iCommand_FireExting_LeftEngineExting)
	REGISTER_COMMAND(iCommand_FireExting_APUExting)
	REGISTER_COMMAND(iCommand_FireExting_RightEngineExting)
	REGISTER_COMMAND(iCommand_FireExting_VentilatorExting)
	REGISTER_COMMAND(iCommand_FireExting_ExtinguishersToDown)
	REGISTER_COMMAND(iCommand_FireExting_ExtinguishersCover)
	REGISTER_COMMAND(iCommand_FireExting_Signals)
	REGISTER_COMMAND(iCommand_FireExting_Reservoirs)
	REGISTER_COMMAND(iCommand_FireExting_ReservoirsCover)
	REGISTER_COMMAND_EX(iCommand_IlluminationInterf_Cockpit_Illumination, 930)
	REGISTER_COMMAND(iCommand_FireExting_TestGroups)

	REGISTER_COMMAND(iCommand_AGR_Test)
	REGISTER_COMMAND(iCommand_AGR_Oriet)
	REGISTER_COMMAND(iCommand_AGR_axis_left)
	REGISTER_COMMAND(iCommand_AGR_axis_right)
	REGISTER_COMMAND(iCommand_IlluminationInterf_Cockpit_Night_Illumination_brightness_up)
	REGISTER_COMMAND(iCommand_IlluminationInterf_Cockpit_Night_Illumination_brightness_down)
	REGISTER_COMMAND(iCommand_IlluminationInterf_Panels_Illumination_brightness_up)
	REGISTER_COMMAND(iCommand_IlluminationInterf_Panels_Illumination_brightness_down)
	REGISTER_COMMAND_EX(iCommand_IlluminationInterf_ADI_AHR_Illumination_brightness_up, 940)
	REGISTER_COMMAND(iCommand_IlluminationInterf_ADI_AHR_Illumination_brightness_down)
	REGISTER_COMMAND(iCommand_IlluminationInterf_Control_Panel_Illumination_brightness_up)
	REGISTER_COMMAND(iCommand_IlluminationInterf_Control_Panel_Illumination_brightness_down)
	REGISTER_COMMAND(iCommand_NAVINTERF_HeadingCourse)

	REGISTER_COMMAND(iCommandPlaneDLK_None)
	REGISTER_COMMAND(iCommandPlaneDLK_None_up)

	REGISTER_COMMAND(iCommandWeaponsBallisticSelectorRight)
	REGISTER_COMMAND(iCommandWeaponsBallisticSelectorLeft)

	REGISTER_COMMAND(iCommand_ZMS_3_MagVarRotaryRight)
	REGISTER_COMMAND_EX(iCommand_ZMS_3_MagVarRotaryLeft, 950)

	REGISTER_COMMAND(iCommand_EjectInterface_Circuit_SelectRight)
	REGISTER_COMMAND(iCommand_EjectInterface_Circuit_SelectLeft)

	REGISTER_COMMAND(iCommand_CPT_MECH_PitotSystemHeatTest)


	REGISTER_COMMAND(iCommand_FireDetectorsTest1stGroup)
	REGISTER_COMMAND(iCommand_FireDetectorsTest2ndGroup)
	REGISTER_COMMAND(iCommand_FireDetectorsTest3rdGroup)


	REGISTER_COMMAND(iCommandPlaneTrimOn)					// Нажать кнопку триммера (вертолет или перевод РУС в режим кнюпеля триммера)
	REGISTER_COMMAND(iCommandPlaneTrimOff)					// Отжать кнопку триммера

	REGISTER_COMMAND(iCommandPlaneLevitation)

	REGISTER_COMMAND_EX(iCommandPlaneEmergencyBrake,960)
	REGISTER_COMMAND(iCommandPlaneWheelBrakeLeftOn)			// Включение тормоза левого колеса
	REGISTER_COMMAND(iCommandPlaneWheelBrakeLeftOff)		// Выключение тормоза левого колеса
	REGISTER_COMMAND(iCommandPlaneWheelBrakeRightOn)		// Включение тормоза правого колеса
	REGISTER_COMMAND(iCommandPlaneWheelBrakeRightOff)		// Выключение тормоза правого колеса
	REGISTER_COMMAND(iCommandPlaneEmergencyDisconnectLever)		

	//Command menu items
	REGISTER_COMMAND(ICommandMenuItem1)
	REGISTER_COMMAND(ICommandMenuItem2)
	REGISTER_COMMAND(ICommandMenuItem3)
	REGISTER_COMMAND(ICommandMenuItem4)
	REGISTER_COMMAND(ICommandMenuItem5)//970
	REGISTER_COMMAND(ICommandMenuItem6)
	REGISTER_COMMAND(ICommandMenuItem7)
	REGISTER_COMMAND(ICommandMenuItem8)
	REGISTER_COMMAND(ICommandMenuItem9)
	REGISTER_COMMAND(ICommandMenuItem10)
	REGISTER_COMMAND(ICommandMenuItem11)
	REGISTER_COMMAND(ICommandMenuItem12)
	REGISTER_COMMAND(ICommandMenuExit)

	//Command dialogs panel commands
	REGISTER_COMMAND(ICommandSwitchDialog)
	REGISTER_COMMAND_EX(ICommandSwitchToCommonDialog,980)

	REGISTER_COMMAND(ICommandToggleConsole)

	
	REGISTER_COMMAND_EX(iCommandPlaneLeftMFD_OSB1_Off,982)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB2_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB3_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB4_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB5_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB6_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB7_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB8_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB9_Off)//990
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB10_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB11_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB12_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB13_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB14_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB15_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB16_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB17_Off)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB18_Off)
	REGISTER_COMMAND_EX(iCommandPlaneLeftMFD_OSB19_Off,1000)//1000 Get!!!!!!
	REGISTER_COMMAND(iCommandPlaneLeftMFD_OSB20_Off)

	REGISTER_COMMAND(iCommandPlaneLeftMFD_ADJ_Stop)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_DSP_Stop)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_BRT_Stop)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_CON_Stop)
	REGISTER_COMMAND(iCommandPlaneLeftMFD_SYM_Stop)

	REGISTER_COMMAND_EX(iCommandPlaneRightMFD_OSB1_Off,1007)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB2_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB3_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB4_Off)//1010
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB5_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB6_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB7_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB8_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB9_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB10_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB11_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB12_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB13_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB14_Off)//1020
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB15_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB16_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB17_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB18_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB19_Off)
	REGISTER_COMMAND(iCommandPlaneRightMFD_OSB20_Off)

	REGISTER_COMMAND(iCommandPlaneRightMFD_ADJ_Stop)
	REGISTER_COMMAND(iCommandPlaneRightMFD_DSP_Stop)
	REGISTER_COMMAND(iCommandPlaneRightMFD_BRT_Stop)
	REGISTER_COMMAND(iCommandPlaneRightMFD_CON_Stop)//1030
	REGISTER_COMMAND(iCommandPlaneRightMFD_SYM_Stop)

	REGISTER_COMMAND(iCommandThrottleIncrease) //клавиатурное "плавное" управление РУД
	REGISTER_COMMAND(iCommandThrottleDecrease)
	REGISTER_COMMAND(iCommandThrottleStop)
	REGISTER_COMMAND(iCommandThrottle1Increase)
	REGISTER_COMMAND(iCommandThrottle1Decrease)
	REGISTER_COMMAND(iCommandThrottle1Stop)
	REGISTER_COMMAND(iCommandThrottle2Increase)
	REGISTER_COMMAND(iCommandThrottle2Decrease)
	REGISTER_COMMAND_EX(iCommandThrottle2Stop,1040)

	REGISTER_COMMAND(iCommandPlane_HOTAS_CMS_Zaxis)
	REGISTER_COMMAND(iCommandPlane_HOTAS_CMS_Zaxis_Off)
	
	REGISTER_COMMAND(iCommandViewJFO) //камера привязанная к юниту
	REGISTER_COMMAND(iCommandViewSmoke1) //Дымы наводчика
	REGISTER_COMMAND(iCommandViewSmoke2)
	REGISTER_COMMAND(iCommandViewSmoke3)

    REGISTER_COMMAND(iCommandPlane_FLAPS_UP)
    REGISTER_COMMAND(iCommandPlane_FLAPS_MNR)
    REGISTER_COMMAND(iCommandPlane_FLAPS_DN)
    
    //LASTE control panel
    REGISTER_COMMAND_EX(iCommandPlane_EAC_ARM,1050)
    REGISTER_COMMAND(iCommandPlane_EAC_OFF)

    REGISTER_COMMAND(iCommandPlane_RDR_ALTM_NRM)
    REGISTER_COMMAND(iCommandPlane_RDR_ALTM_DIS)

    //ENGINE Control Panel
    REGISTER_COMMAND(iCommandPlane_LG_WRN_Silence)
    REGISTER_COMMAND(iCommandPlane_APU_Start)
    REGISTER_COMMAND(iCommandPlane_APU_Off)

    REGISTER_COMMAND   (iCommandPlane_L_ENG_OPER_Ign)
    REGISTER_COMMAND   (iCommandPlane_L_ENG_OPER_Norm)
    REGISTER_COMMAND   (iCommandPlane_L_ENG_OPER_Motor)

    REGISTER_COMMAND_EX(iCommandPlane_R_ENG_OPER_Ign,1060)
    REGISTER_COMMAND   (iCommandPlane_R_ENG_OPER_Norm)
    REGISTER_COMMAND   (iCommandPlane_R_ENG_OPER_Motor)

    REGISTER_COMMAND   (iCommandPlane_L_ENG_FUEL_FLOW_Norm)
    REGISTER_COMMAND   (iCommandPlane_L_ENG_FUEL_FLOW_Override)

    REGISTER_COMMAND_EX(iCommandPlane_R_ENG_FUEL_FLOW_Norm,1065)
    REGISTER_COMMAND   (iCommandPlane_R_ENG_FUEL_FLOW_Override)

    REGISTER_COMMAND   (iCommandPlaneQuickPadlockOn)
    REGISTER_COMMAND   (iCommandPlaneQuickPadlockOff)
	REGISTER_COMMAND   (iCommandPlaneThrottleFrictionDec)
	REGISTER_COMMAND   (iCommandPlaneThrottleFrictionInc)

	//A10 - Electrical power control panel
	REGISTER_COMMAND   (iCommandAPUGeneratorPower)
	REGISTER_COMMAND   (iCommandEmergencyFlood)
	REGISTER_COMMAND   (iCommandBatteryPower)
	REGISTER_COMMAND   (iCommandElectricalPowerInverterSTBY)
	REGISTER_COMMAND   (iCommandElectricalPowerInverterOFF)
	REGISTER_COMMAND   (iCommandElectricalPowerInverterTEST)
	//A10 - Electrical circuit breakers
	//A10 - Fuel system control panel
	REGISTER_COMMAND   (iCommandPlaneFSExtTanksWingValve)
	REGISTER_COMMAND   (iCommandPlaneFSExtTanksFuselageValve)
	REGISTER_COMMAND   (iCommandPlaneFSSigAmplifier)
	REGISTER_COMMAND   (iCommandPlaneFSLineCheck)
	REGISTER_COMMAND   (iCommandPlaneFSTankGate)
	REGISTER_COMMAND   (iCommandPlaneFSTankCrossFeed)
	REGISTER_COMMAND   (iCommandPlaneFSLeftWingBoostPump)
	REGISTER_COMMAND   (iCommandPlaneFSRightWingBoostPump)
	REGISTER_COMMAND   (iCommandPlaneFSLeftWingFillDisable)
	REGISTER_COMMAND   (iCommandPlaneFSRightWingFillDisable)
	REGISTER_COMMAND   (iCommandPlaneFSLeftMainBoostPump)
	REGISTER_COMMAND   (iCommandPlaneFSRightMainBoostPump)
	REGISTER_COMMAND   (iCommandPlaneFSLeftMainFillDisable)
	REGISTER_COMMAND   (iCommandPlaneFSRightMainFillDisable)
	REGISTER_COMMAND   (iCommandPlaneFSReceiverLever)
	REGISTER_COMMAND   (iCommandPlaneFSQuantityIndicatorSelectorINT)
	REGISTER_COMMAND   (iCommandPlaneFSQuantityIndicatorSelectorMAIN)
	REGISTER_COMMAND   (iCommandPlaneFSQuantityIndicatorSelectorWING)
	REGISTER_COMMAND   (iCommandPlaneFSQuantityIndicatorSelectorEXTWING)
	REGISTER_COMMAND   (iCommandPlaneFSQuantityIndicatorSelectorEXTCTR)
	REGISTER_COMMAND   (iCommandPlaneFSQuantityIndicatorTest)
	REGISTER_COMMAND   (iCommandPlaneFSRefuelLigtingDecrease)
	REGISTER_COMMAND   (iCommandPlaneFSRefuelLigtingIncrease)
	//A10 - Emergency flight control panel
	REGISTER_COMMAND   (iCommandPlaneEmerSpeedBrakeRetract)
	REGISTER_COMMAND   (iCommandPlaneEmerPitchRollTrim)
	REGISTER_COMMAND   (iCommandPlaneEmerTrimNoseDn)
	REGISTER_COMMAND   (iCommandPlaneEmerTrimLWingDn)
	REGISTER_COMMAND   (iCommandPlaneEmerTrimRWingDn)
	REGISTER_COMMAND   (iCommandPlaneEmerTrimNoseUP)
	REGISTER_COMMAND   (iCommandPlaneEmerTrimNeutral)
	REGISTER_COMMAND   (iCommandPlaneEmerAileronDisengageL)
	REGISTER_COMMAND   (iCommandPlaneEmerAileronDisengageOff)
	REGISTER_COMMAND   (iCommandPlaneEmerAileronDisengageR)
	REGISTER_COMMAND   (iCommandPlaneEmerElevatorDisengageL)
	REGISTER_COMMAND   (iCommandPlaneEmerElevatorDisengageOff)
	REGISTER_COMMAND   (iCommandPlaneEmerElevatorDisengageR)
	REGISTER_COMMAND   (iCommandPlaneEmerFlapsRetract)
	REGISTER_COMMAND   (iCommandPlaneEmerFlightControl)
	//A10 - Circuit breaker panel
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_1)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_2)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_3)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_4)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_5)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_6)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_7)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_8)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_9)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_10)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_11)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_12)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_13)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_14)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_15)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_16)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_17)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_18)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_19)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_20)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_21)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_22)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_23)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_24)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_25)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_26)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_27)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_28)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_29)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_30)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_31)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_32)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_33)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_34)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_35)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_36)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_37)
	REGISTER_COMMAND   (iCommandPlaneCircuitBreaker_38)
	//A-10 Stability Augmentation system panel
	REGISTER_COMMAND   (iCommandPlaneYawSASEngageL)
	REGISTER_COMMAND   (iCommandPlaneYawSASEngageR)
	REGISTER_COMMAND   (iCommandPlanePitchSASEngageL)
	REGISTER_COMMAND   (iCommandPlanePitchSASEngageR)
	REGISTER_COMMAND   (iCommandPlaneSASMonitorTestLeft)
	REGISTER_COMMAND   (iCommandPlaneSASMonitorTestOff)
	REGISTER_COMMAND   (iCommandPlaneSASMonitorTestRight)
	REGISTER_COMMAND   (iCommandPlaneSASTakeOffTrim)
	//A-10 Inercom control panel
	REGISTER_COMMAND   (iCommandPlaneIntercomINTDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomINTIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomINTPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomFMDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomFMIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomFMPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomVHFDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomVHFIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomVHFPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomUHFDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomUHFIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomUHFPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomAIMDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomAIMIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomAIMPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomIFFDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomIFFIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomIFFPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomILSDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomILSIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomILSPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomTCNDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomTCNIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomTCNPress)
	REGISTER_COMMAND   (iCommandPlaneIntercomVolumeDecrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomVolumeIncrease)
	REGISTER_COMMAND   (iCommandPlaneIntercomHM)
	REGISTER_COMMAND   (iCommandPlaneIntercomINT)
	REGISTER_COMMAND   (iCommandPlaneIntercomVHF)
	REGISTER_COMMAND   (iCommandPlaneIntercomFM)
	REGISTER_COMMAND   (iCommandPlaneIntercomHF)
	//A-10 Auxiliary lighting control panel
	REGISTER_COMMAND   (iCommandPlaneRefuelStatusIndLTSDecrease)
	REGISTER_COMMAND   (iCommandPlaneRefuelStatusIndLTSIncrease) 
	REGISTER_COMMAND   (iCommandPlaneNightVisionLTSTCP) 
	REGISTER_COMMAND   (iCommandPlaneNightVisionLTSALL) 
	REGISTER_COMMAND   (iCommandPlaneNightVisionLTSOFF) 
	//REGISTER_COMMAND   (iCommandPlaneWeaponStationLTSOff) //NOT IMPL IN SYSTEM
	//REGISTER_COMMAND   (iCommandPlaneWeaponStationLTSBrt) //NOT IMPL IN SYSTEM
	REGISTER_COMMAND   (iCommandPlaneHarsSasNormOverride) 
	REGISTER_COMMAND   (iCommandPlaneFireDetectBleedAirTest)
	//A-10 UHF Radio
	REGISTER_COMMAND   (iCommandPlaneUHFPresetChannelSelectorInc)
	REGISTER_COMMAND   (iCommandPlaneUHFPresetChannelSelectorDec)
	REGISTER_COMMAND   (iCommandPlaneUHF100MhzSelector1)
	REGISTER_COMMAND   (iCommandPlaneUHF100MhzSelector2)
	REGISTER_COMMAND   (iCommandPlaneUHF100MhzSelector3)
	REGISTER_COMMAND   (iCommandPlaneUHF100MhzSelectorA)
	REGISTER_COMMAND   (iCommandPlaneUHF10MhzSelectorInc)
	REGISTER_COMMAND   (iCommandPlaneUHF10MhzSelectorDec)
	REGISTER_COMMAND   (iCommandPlaneUHF1MhzSelectorInc)
	REGISTER_COMMAND   (iCommandPlaneUHF1MhzSelectorDec)
	REGISTER_COMMAND   (iCommandPlaneUHF01MhzSelectorInc)
	REGISTER_COMMAND   (iCommandPlaneUHF01MhzSelectorDec)
	REGISTER_COMMAND   (iCommandPlaneUHF0025MhzSelectorInc)
	REGISTER_COMMAND   (iCommandPlaneUHF0025MhzSelectorDec)
	REGISTER_COMMAND   (iCommandPlaneUHFTestDisplay)
	REGISTER_COMMAND   (iCommandPlaneUHFStatus)
	REGISTER_COMMAND   (iCommandPlaneUHFLoad)
	REGISTER_COMMAND   (iCommandPlaneUHFVolumeDec)
	REGISTER_COMMAND   (iCommandPlaneUHFVolumeInc)
	REGISTER_COMMAND   (iCommandPlaneUHFFunctionDialOFF)
	REGISTER_COMMAND   (iCommandPlaneUHFFunctionDialMAIN)
	REGISTER_COMMAND   (iCommandPlaneUHFFunctionDialBOTH)
	REGISTER_COMMAND   (iCommandPlaneUHFFunctionDialADF)
	REGISTER_COMMAND   (iCommandPlaneUHFFreqModeDialMNL)
	REGISTER_COMMAND   (iCommandPlaneUHFFreqModeDialPRESET)
	REGISTER_COMMAND   (iCommandPlaneUHFFreqModeDialGRD)
	REGISTER_COMMAND   (iCommandPlaneUHFTTone) //NOT IMPLEMENTED IN UHF
	REGISTER_COMMAND   (iCommandPlaneUHFSquelch)
	REGISTER_COMMAND   (iCommandPlaneUHFPanelCover)

	//A-10 CDU panel
	//!!! Do not change commands count and order. 
	//!!! It must be exactly like order in enum_avCDU_commands.
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_3L)
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_5L)
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_7L)
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_9L)
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_3R)
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_5R)
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_7R)
	REGISTER_COMMAND	(iCommandPlaneCDU_LSK_9R)

	REGISTER_COMMAND	(iCommandPlaneCDU_SYS)
	REGISTER_COMMAND	(iCommandPlaneCDU_NAV)
	REGISTER_COMMAND	(iCommandPlaneCDU_WP)
	REGISTER_COMMAND	(iCommandPlaneCDU_OSET)
	REGISTER_COMMAND	(iCommandPlaneCDU_FPM)
	REGISTER_COMMAND	(iCommandPlaneCDU_PREV)

	REGISTER_COMMAND	(iCommandPlaneCDU_1)
	REGISTER_COMMAND	(iCommandPlaneCDU_2)
	REGISTER_COMMAND	(iCommandPlaneCDU_3)
	REGISTER_COMMAND	(iCommandPlaneCDU_4)
	REGISTER_COMMAND	(iCommandPlaneCDU_5)
	REGISTER_COMMAND	(iCommandPlaneCDU_6)
	REGISTER_COMMAND	(iCommandPlaneCDU_7)
	REGISTER_COMMAND	(iCommandPlaneCDU_8)
	REGISTER_COMMAND	(iCommandPlaneCDU_9)
	REGISTER_COMMAND	(iCommandPlaneCDU_0)
	REGISTER_COMMAND	(iCommandPlaneCDU_PNT)
	REGISTER_COMMAND	(iCommandPlaneCDU_SLASH)

	REGISTER_COMMAND	(iCommandPlaneCDU_A)
	REGISTER_COMMAND	(iCommandPlaneCDU_B)
	REGISTER_COMMAND	(iCommandPlaneCDU_C)
	REGISTER_COMMAND	(iCommandPlaneCDU_D)
	REGISTER_COMMAND	(iCommandPlaneCDU_E)
	REGISTER_COMMAND	(iCommandPlaneCDU_F)
	REGISTER_COMMAND	(iCommandPlaneCDU_G)
	REGISTER_COMMAND	(iCommandPlaneCDU_H)
	REGISTER_COMMAND	(iCommandPlaneCDU_I)
	REGISTER_COMMAND	(iCommandPlaneCDU_J)
	REGISTER_COMMAND	(iCommandPlaneCDU_K)
	REGISTER_COMMAND	(iCommandPlaneCDU_L)
	REGISTER_COMMAND	(iCommandPlaneCDU_M)
	REGISTER_COMMAND	(iCommandPlaneCDU_N)
	REGISTER_COMMAND	(iCommandPlaneCDU_O)
	REGISTER_COMMAND	(iCommandPlaneCDU_P)
	REGISTER_COMMAND	(iCommandPlaneCDU_Q)
	REGISTER_COMMAND	(iCommandPlaneCDU_R)
	REGISTER_COMMAND	(iCommandPlaneCDU_S)
	REGISTER_COMMAND	(iCommandPlaneCDU_T)
	REGISTER_COMMAND	(iCommandPlaneCDU_U)
	REGISTER_COMMAND	(iCommandPlaneCDU_V)
	REGISTER_COMMAND	(iCommandPlaneCDU_W)
	REGISTER_COMMAND	(iCommandPlaneCDU_X)
	REGISTER_COMMAND	(iCommandPlaneCDU_Y)
	REGISTER_COMMAND	(iCommandPlaneCDU_Z)

	REGISTER_COMMAND	(iCommandPlaneVOID_1)
	REGISTER_COMMAND	(iCommandPlaneVOID_2)
	REGISTER_COMMAND	(iCommandPlaneCDU_MK)
	REGISTER_COMMAND	(iCommandPlaneCDU_BCK)
	REGISTER_COMMAND	(iCommandPlaneCDU_SPC)
	REGISTER_COMMAND	(iCommandPlaneCDU_CLR)
	REGISTER_COMMAND	(iCommandPlaneCDU_FA)


	REGISTER_COMMAND	(iCommandPlaneCDU_DIM)
	REGISTER_COMMAND	(iCommandPlaneCDU_BRT)
	REGISTER_COMMAND	(iCommandPlaneCDU_PGUP)
	REGISTER_COMMAND	(iCommandPlaneCDU_PGDN)
	REGISTER_COMMAND	(iCommandPlaneCDU_BLANC_L)
	REGISTER_COMMAND	(iCommandPlaneCDU_BLANC_R)
	REGISTER_COMMAND	(iCommandPlaneCDU_PLUS)
	REGISTER_COMMAND	(iCommandPlaneCDU_MINUS)

	//A-10 UFC panel
	//!!! Do not change commands count and order. 
	//!!! It must be exactly like order in enum_avCDU_commands.
	REGISTER_COMMAND	(iCommandPlaneUFC_1)
	REGISTER_COMMAND	(iCommandPlaneUFC_2)
	REGISTER_COMMAND	(iCommandPlaneUFC_3)
	REGISTER_COMMAND	(iCommandPlaneUFC_4)
	REGISTER_COMMAND	(iCommandPlaneUFC_5)
	REGISTER_COMMAND	(iCommandPlaneUFC_6)
	REGISTER_COMMAND	(iCommandPlaneUFC_7)
	REGISTER_COMMAND	(iCommandPlaneUFC_8)
	REGISTER_COMMAND	(iCommandPlaneUFC_9)
	REGISTER_COMMAND	(iCommandPlaneUFC_0)

	REGISTER_COMMAND	(iCommandPlaneUFC_SPC)
	REGISTER_COMMAND	(iCommandPlaneUFC_HACK)
	REGISTER_COMMAND	(iCommandPlaneUFC_FUNC)
	REGISTER_COMMAND	(iCommandPlaneUFC_LTR)
	REGISTER_COMMAND	(iCommandPlaneUFC_CLR)
	REGISTER_COMMAND	(iCommandPlaneUFC_ENT)
	REGISTER_COMMAND	(iCommandPlaneUFC_MK)
	REGISTER_COMMAND	(iCommandPlaneUFC_ALT)
	REGISTER_COMMAND	(iCommandPlaneUFC_MASTER_WARNING)
	REGISTER_COMMAND	(iCommandPlaneUFC_STEER_UP)
	REGISTER_COMMAND	(iCommandPlaneUFC_STEER_DOWN)
	REGISTER_COMMAND	(iCommandPlaneUFC_DATA_UP)
	REGISTER_COMMAND	(iCommandPlaneUFC_DATA_DOWN)
	REGISTER_COMMAND	(iCommandPlaneUFC_SEL_UP)
	REGISTER_COMMAND	(iCommandPlaneUFC_SEL_DOWN)
	REGISTER_COMMAND	(iCommandPlaneUFC_DEPR_UP)
	REGISTER_COMMAND	(iCommandPlaneUFC_DEPR_DOWN)
	REGISTER_COMMAND	(iCommandPlaneUFC_INTEN_INCREASE)
	REGISTER_COMMAND	(iCommandPlaneUFC_INTEN_DECREASE)

	//A-10 CMSP panel
	REGISTER_COMMAND	(iCommandPlaneCMSP_OSB_1)
	REGISTER_COMMAND	(iCommandPlaneCMSP_OSB_2)
	REGISTER_COMMAND	(iCommandPlaneCMSP_OSB_3)
	REGISTER_COMMAND	(iCommandPlaneCMSP_OSB_4)

	REGISTER_COMMAND	(iCommandPlaneCMSP_NEXT_UP)
	REGISTER_COMMAND	(iCommandPlaneCMSP_NEXT_DOWN)

	REGISTER_COMMAND	(iCommandPlaneCMSP_RTN)
	REGISTER_COMMAND	(iCommandPlaneCMSP_JTSN)
	REGISTER_COMMAND	(iCommandPlaneCMSP_BRT_DEC)
	REGISTER_COMMAND	(iCommandPlaneCMSP_BRT_INC)

	REGISTER_COMMAND	(iCommandPlaneCMSP_MWS_MENU)
	REGISTER_COMMAND	(iCommandPlaneCMSP_MWS_ON)

	REGISTER_COMMAND	(iCommandPlaneCMSP_JMR_MENU)
	REGISTER_COMMAND	(iCommandPlaneCMSP_JMR_ON)

	REGISTER_COMMAND	(iCommandPlaneCMSP_RWR_MENU)
	REGISTER_COMMAND	(iCommandPlaneCMSP_RWR_ON)

	REGISTER_COMMAND	(iCommandPlaneCMSP_DISP_MENU)
	REGISTER_COMMAND	(iCommandPlaneCMSP_DISP_ON)

	REGISTER_COMMAND	(iCommandPlaneCMSP_MODE_OFF)
	REGISTER_COMMAND	(iCommandPlaneCMSP_MODE_STBY)
	REGISTER_COMMAND	(iCommandPlaneCMSP_MODE_MAN)
	REGISTER_COMMAND	(iCommandPlaneCMSP_MODE_SEMI)
	REGISTER_COMMAND	(iCommandPlaneCMSP_MODE_AUTO)

	//A-10 CMSC panel
	REGISTER_COMMAND	(iCommandPlaneCMSC_JMR)
	REGISTER_COMMAND	(iCommandPlaneCMSC_MWS)
	REGISTER_COMMAND	(iCommandPlaneCMSC_BRT_DEC)
	REGISTER_COMMAND	(iCommandPlaneCMSC_BRT_INC)
	REGISTER_COMMAND	(iCommandPlaneCMSC_AUD_DEC)
	REGISTER_COMMAND	(iCommandPlaneCMSC_AUD_INC)
	REGISTER_COMMAND	(iCommandPlaneCMSC_PRI)
	REGISTER_COMMAND	(iCommandPlaneCMSC_SEP)
	REGISTER_COMMAND	(iCommandPlaneCMSC_UNK)

	//A-10 Lighting panel
	REGISTER_COMMAND	(iCommandPlanePositionLightsFlash)
	REGISTER_COMMAND	(iCommandPlanePositionLightsOff)
	REGISTER_COMMAND	(iCommandPlanePositionLightsSteady)
	REGISTER_COMMAND	(iCommandPlaneFormationLightsDecrease)
	REGISTER_COMMAND	(iCommandPlaneFormationLightsIncrease)
	REGISTER_COMMAND	(iCommandPlaneEngInstLightsDecrease)
	REGISTER_COMMAND	(iCommandPlaneEngInstLightsIncrease)
	REGISTER_COMMAND	(iCommandPlaneNoseIllum)
	REGISTER_COMMAND	(iCommandPlaneAuxInstLightsDecrease)
	REGISTER_COMMAND	(iCommandPlaneAuxInstLightsIncrease)
	REGISTER_COMMAND	(iCommandPlaneSignalLTS)
	REGISTER_COMMAND	(iCommandPlaneFltInstLightsDecrease)
	REGISTER_COMMAND	(iCommandPlaneFltInstLightsIncrease)
	REGISTER_COMMAND	(iCommandPlaneAccelCompLights)
	REGISTER_COMMAND	(iCommandPlaneFloodLightsDecrease)
	REGISTER_COMMAND	(iCommandPlaneFloodLightsIncrease)
	REGISTER_COMMAND	(iCommandPlaneConsoleLightsDecrease)
	REGISTER_COMMAND	(iCommandPlaneConsoleLightsIncrease)

	//A-10 Armament HUD Control Panel
	REGISTER_COMMAND	(iCommandPlaneAHCPMasterArm)
	REGISTER_COMMAND	(iCommandPlaneAHCPMasterSafe)
	REGISTER_COMMAND	(iCommandPlaneAHCPMasterTrain)
	REGISTER_COMMAND	(iCommandPlaneAHCPGUNPACArm)
	REGISTER_COMMAND	(iCommandPlaneAHCPGUNSafe)
	REGISTER_COMMAND	(iCommandPlaneAHCPGUNArm)
	REGISTER_COMMAND	(iCommandPlaneAHCPLaserArm)
	REGISTER_COMMAND	(iCommandPlaneAHCPLaserSafe)
	REGISTER_COMMAND	(iCommandPlaneAHCPLaserTrain)
	REGISTER_COMMAND	(iCommandPlaneAHCPTGPOnOff)
	REGISTER_COMMAND	(iCommandPlaneAHCPAltSCEBaro)
	REGISTER_COMMAND	(iCommandPlaneAHCPAltSCEDelta)
	REGISTER_COMMAND	(iCommandPlaneAHCPAltSCERadar)
	REGISTER_COMMAND	(iCommandPlaneAHCPHUDModeDayNight)
	REGISTER_COMMAND	(iCommandPlaneAHCPHUDModeNormSTBY)
	REGISTER_COMMAND	(iCommandPlaneAHCPCICUOnOff)
	REGISTER_COMMAND	(iCommandPlaneAHCPJTRSOnOff)
	REGISTER_COMMAND	(iCommandPlaneAHCPIFFCCOn)
	REGISTER_COMMAND	(iCommandPlaneAHCPIFFCCTest)
	REGISTER_COMMAND	(iCommandPlaneAHCPIFFCCOff)

	//A-10 Landing Gear panel
	REGISTER_COMMAND	(iCommandPlaneLGPAntiSkid)
	REGISTER_COMMAND	(iCommandPlaneLGPDownlockOverride)
	REGISTER_COMMAND	(iCommandPlaneLGPLandLight)
	REGISTER_COMMAND	(iCommandPlaneLGPLightsOff)
	REGISTER_COMMAND	(iCommandPlaneLGPTaxiLight)

	//A-10 NMSP
	REGISTER_COMMAND	(iCommandPlaneNMSP_HARS)
	REGISTER_COMMAND	(iCommandPlaneNMSP_EGI)
	REGISTER_COMMAND	(iCommandPlaneNMSP_TISL)
	REGISTER_COMMAND	(iCommandPlaneNMSP_STRPT)
	REGISTER_COMMAND	(iCommandPlaneNMSP_ANCHR)
	REGISTER_COMMAND	(iCommandPlaneNMSP_TCN)
	REGISTER_COMMAND	(iCommandPlaneNMSP_ILS)
	REGISTER_COMMAND	(iCommandPlaneNMSP_PTR_ABLE_STOW)

	//A-10 TISL Control Panel.
	// NOT IMPL IN IFFCC
	/*REGISTER_COMMAND	(iCommandPlaneTISL_MODE_OFF)
	REGISTER_COMMAND	(iCommandPlaneTISL_MODE_CAGE)
	REGISTER_COMMAND	(iCommandPlaneTISL_MODE_DIVE)
	REGISTER_COMMAND	(iCommandPlaneTISL_MODE_LVLBAR)
	REGISTER_COMMAND	(iCommandPlaneTISL_MODE_LVLWIDE)
	REGISTER_COMMAND	(iCommandPlaneTISL_OVER10)
	REGISTER_COMMAND	(iCommandPlaneTISL_5_10)
	REGISTER_COMMAND	(iCommandPlaneTISL_UNDER5)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE_TISL)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE_BOTH)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE_AUX)
	REGISTER_COMMAND	(iCommandPlaneTISL_ENTER)
	REGISTER_COMMAND	(iCommandPlaneTISL_TRACK)
	REGISTER_COMMAND	(iCommandPlaneTISL_OVERTEMP)
	REGISTER_COMMAND	(iCommandPlaneTISL_BITE)
	REGISTER_COMMAND	(iCommandPlaneTISL_ATAT1_UP)
	REGISTER_COMMAND	(iCommandPlaneTISL_ATAT1_DOWN)
	REGISTER_COMMAND	(iCommandPlaneTISL_ATAT2_UP)
	REGISTER_COMMAND	(iCommandPlaneTISL_ATAT2_DOWN)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE1_UP)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE1_DOWN)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE2_UP)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE2_DOWN)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE3_UP)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE3_DOWN)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE4_UP)
	REGISTER_COMMAND	(iCommandPlaneTISL_CODE4_DOWN)*/

	//A-10 FireSystem
	REGISTER_COMMAND	(iCommand_FireExting_Discharge_Left)
	REGISTER_COMMAND	(iCommand_FireExting_Discharge_Center)
	REGISTER_COMMAND	(iCommand_FireExting_Discharge_Right)

	//A-10 Left Front
	REGISTER_COMMAND	(iCommandPlane_HARS_Fast_Erect)
	REGISTER_COMMAND	(iCommandPlane_Altimeter_PNEU)
	REGISTER_COMMAND	(iCommandPlane_Altimeter_ELECT)
	REGISTER_COMMAND	(iCommandPlane_Altimeter_CENTER)

	//A-10 Auxiliary Avionics Panel
	REGISTER_COMMAND	(iCommandPlane_AAP_CDU_Power)
	REGISTER_COMMAND	(iCommandPlane_AAP_EGI_Power)
	REGISTER_COMMAND	(iCommandPlane_AAP_PageSel_OTHER)
	REGISTER_COMMAND	(iCommandPlane_AAP_PageSel_POSITION)
	REGISTER_COMMAND	(iCommandPlane_AAP_PageSel_STEER)
	REGISTER_COMMAND	(iCommandPlane_AAP_PageSel_WAYPT)

	REGISTER_COMMAND	(iCommandPlane_AAP_SteerPt_FLTPLAN)
	REGISTER_COMMAND	(iCommandPlane_AAP_SteerPt_MARK)
	REGISTER_COMMAND	(iCommandPlane_AAP_SteerPt_MISSION)

	REGISTER_COMMAND	(iCommandPlane_AAP_SteerSw_Up)
	REGISTER_COMMAND	(iCommandPlane_AAP_SteerSw_Down)
	//A-10 Tacan Control Panel
	REGISTER_COMMAND	(iCommandPlane_TACAN_Mode_Off)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Mode_Rec)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Mode_TR)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Mode_AAREC)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Mode_AATR)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Volume_Decrease)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Volume_Increase)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Chanel_Ones_Decrease)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Chanel_Ones_Increase)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Chanel_Tens_Decrease)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Chanel_Tens_Increase)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Chanel_XY_Mode)
	REGISTER_COMMAND	(iCommandPlane_TACAN_Test)

	//A-10 ILS Control Panel
	REGISTER_COMMAND	(iCommandPlane_ILS_Power)
	REGISTER_COMMAND	(iCommandPlane_ILS_MHz_Decrease)
	REGISTER_COMMAND	(iCommandPlane_ILS_MHz_Increase)
	REGISTER_COMMAND	(iCommandPlane_ILS_KHz_Decrease)
	REGISTER_COMMAND	(iCommandPlane_ILS_KHz_Increase)
	REGISTER_COMMAND	(iCommandPlane_ILS_Volume_Decrease)
	REGISTER_COMMAND	(iCommandPlane_ILS_Volume_Increase)

	//A-10 HARS Control Panel
	REGISTER_COMMAND	(iCommandPlane_HARS_Mode)
	REGISTER_COMMAND	(iCommandPlane_HARS_LAT_Correction_Decrease)
	REGISTER_COMMAND	(iCommandPlane_HARS_LAT_Correction_Increase)
	REGISTER_COMMAND	(iCommandPlane_HARS_HemisphereSel)
	REGISTER_COMMAND	(iCommandPlane_HARS_MagneticVar_Min15)
	REGISTER_COMMAND	(iCommandPlane_HARS_MagneticVar_Null)
	REGISTER_COMMAND	(iCommandPlane_HARS_MagneticVar_Plus15)
	REGISTER_COMMAND	(iCommandPlane_HARS_SyncButton)
	REGISTER_COMMAND	(iCommandPlane_HARS_HDG_Decrease)
	REGISTER_COMMAND	(iCommandPlane_HARS_HDG_Increase)

	//Stall warning control panel
	REGISTER_COMMAND	(iCommandPlane_STALL_Vol_Decrease)
	REGISTER_COMMAND	(iCommandPlane_STALL_Vol_Increase)
	REGISTER_COMMAND	(iCommandPlane_Peak_Vol_Decrease)
	REGISTER_COMMAND	(iCommandPlane_Peak_Vol_Increase)

	//A-10 VHF FM Radio
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Volume_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Volume_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Mode_EMERFM)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Mode_EMERAM)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Mode_MAN)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Mode_PRE)

	REGISTER_COMMAND	(iCommandPlane_VHF_FM_FreqMode_OFF)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_FreqMode_TK)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_FreqMode_DN)

	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Squelch)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Tone)

	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Load)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Preset_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_Preset_Inc)

	REGISTER_COMMAND	(iCommandPlane_VHF_FM_10MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_10MHz_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_1MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_1MHz_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_01MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_01MHz_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_025MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_FM_025MHz_Inc)

	//A-10 VHF AM Radio
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Volume_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Volume_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Mode_EMERFM)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Mode_EMERAM)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Mode_MAN)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Mode_PRE)

	REGISTER_COMMAND	(iCommandPlane_VHF_AM_FreqMode_OFF)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_FreqMode_TK)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_FreqMode_DN)

	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Squelch)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Tone)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Load)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Preset_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_Preset_Inc)

	REGISTER_COMMAND	(iCommandPlane_VHF_AM_10MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_10MHz_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_1MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_1MHz_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_01MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_01MHz_Inc)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_025MHz_Dec)
	REGISTER_COMMAND	(iCommandPlane_VHF_AM_025MHz_Inc)

	//A-10 Environment System Panel
	REGISTER_COMMAND	(iCommandPlane_Oxygen_Emergency)
	REGISTER_COMMAND	(iCommandPlane_Oxygen_Normal)
	REGISTER_COMMAND	(iCommandPlane_Oxygen_TestMark)
	REGISTER_COMMAND	(iCommandPlane_Oxygen_Supply)
	REGISTER_COMMAND	(iCommandPlane_Oxygen_100Oxygen)
	REGISTER_COMMAND	(iCommandPlane_Oxygen_Ind_Test)

	REGISTER_COMMAND	(iCommandPlane_Env_WindshieldDefogDeice)
	REGISTER_COMMAND	(iCommandPlane_Env_CanopyDefog_Dec)
	REGISTER_COMMAND	(iCommandPlane_Env_CanopyDefog_Inc)
	REGISTER_COMMAND	(iCommandPlane_Env_WindshieldRainRemove)
	REGISTER_COMMAND	(iCommandPlane_Env_WindshieldCenter)
	REGISTER_COMMAND	(iCommandPlane_Env_WindshieldWash)
	REGISTER_COMMAND	(iCommandPlane_Env_BleddAir)
	REGISTER_COMMAND	(iCommandPlane_Env_TempPress_NORM)
	REGISTER_COMMAND	(iCommandPlane_Env_TempPress_DUMP)
	REGISTER_COMMAND	(iCommandPlane_Env_TempPress_RAM)
	REGISTER_COMMAND	(iCommandPlane_Env_MainAirSupply)
	REGISTER_COMMAND	(iCommandPlane_Env_FlowLevel_Decrease)
	REGISTER_COMMAND	(iCommandPlane_Env_FlowLevel_Increase)
	REGISTER_COMMAND	(iCommandPlane_Env_AirConditioner_AUTO)
	REGISTER_COMMAND	(iCommandPlane_Env_AirConditioner_HOT)
	REGISTER_COMMAND	(iCommandPlane_Env_AirConditioner_COLD)
	REGISTER_COMMAND	(iCommandPlane_Env_TempLevel_Decrease)
	REGISTER_COMMAND	(iCommandPlane_Env_TempLevel_Increase)

	REGISTER_COMMAND	(iCommandPlaneLeftMFD_BRTMODE_OFF)
	REGISTER_COMMAND	(iCommandPlaneLeftMFD_BRTMODE_NIGHT)
	REGISTER_COMMAND	(iCommandPlaneLeftMFD_BRTMODE_DAY)

	REGISTER_COMMAND	(iCommandPlaneRightMFD_BRTMODE_OFF)
	REGISTER_COMMAND	(iCommandPlaneRightMFD_BRTMODE_NIGHT)
	REGISTER_COMMAND	(iCommandPlaneRightMFD_BRTMODE_DAY)

	REGISTER_COMMAND	(iCommandSeatArmHandle)
	REGISTER_COMMAND	(iCommandPlaneLockPadlockTerrain)

    REGISTER_COMMAND	(iCommandTrainingUserResponded)

    REGISTER_COMMAND	(iCommandViewBriefing)
	REGISTER_COMMAND	(iCommandDebriefing)

	REGISTER_COMMAND	(iCommandExtLadderButtonCover)
	REGISTER_COMMAND	(iCommandExtLadderButton)
	REGISTER_COMMAND	(iCommandExtLadderRelease)
	REGISTER_COMMAND	(iCommandArmGroundSafetyCover)
	REGISTER_COMMAND	(iCommandArmGroundSafetySwitch)

	REGISTER_COMMAND	(iCommandDVADRFunctionOff)
	REGISTER_COMMAND	(iCommandDVADRFunctionSTDBY)
	REGISTER_COMMAND	(iCommandDVADRFunctionREC)

	REGISTER_COMMAND	(iCommandAntiGSuitTest)
	REGISTER_COMMAND	(iCommandCanopyActuatorDisengage)
	REGISTER_COMMAND	(iCommandCanopyJettison)
	REGISTER_COMMAND	(iCommandCanopyJettisonUnlockButton)
	
	//A-10 Antenna Select Panel						
	REGISTER_COMMAND	(iCommandIFFAntennaUpper)
	REGISTER_COMMAND	(iCommandIFFAntennaBoth)
	REGISTER_COMMAND	(iCommandIFFAntennaLower)
	REGISTER_COMMAND	(iCommandUHFAntennaUpper)
	REGISTER_COMMAND	(iCommandUHFAntennaAuto)
	REGISTER_COMMAND	(iCommandUHFAntennaLower)
	REGISTER_COMMAND	(iCommandEGI_HQ_TOD)
	
	REGISTER_COMMAND	(iCommandPlane_HOTAS_LeftThrottleButton_Off)

	REGISTER_COMMAND	(iCommandRWRIntensityDecrease)
	REGISTER_COMMAND	(iCommandRWRIntensityIncrease)

	REGISTER_COMMAND	(iCommandMissionResourcesManagement)

	REGISTER_COMMAND	(iCommandViewLaserChangeCode)

	//separate commands for HOTAS warthog, to fix synchronization issues
	REGISTER_COMMAND	(iCommand_LASTE_from_PATH_to_ALTHDG)
	REGISTER_COMMAND	(iCommand_LASTE_from_ALT_to_ALTHDG)

	REGISTER_COMMAND   (iCommandPlane_L_ENG_OPER_Norm_from_Motor)
	REGISTER_COMMAND   (iCommandPlane_R_ENG_OPER_Norm_from_Motor)
	REGISTER_COMMAND   (iCommandPlane_L_ENG_OPER_Norm_from_Ign)
	REGISTER_COMMAND   (iCommandPlane_R_ENG_OPER_Norm_from_Ign)

	REGISTER_COMMAND   (iCommandPlane_FLAPS_MNR_from_UP)
	REGISTER_COMMAND   (iCommandPlane_FLAPS_MNR_from_DN)

    //manual vehicle control
    REGISTER_COMMAND   (iCommandGViewAction)    //Dmut: переключение на камеру-прицел для наземки
    REGISTER_COMMAND   (iCommandGChangeWS)      //Dmut: смена активного оружия (по циклу)
    REGISTER_COMMAND   (iCommandGChangeLN)      //Dmut: смена активного оружия (по циклу)
    REGISTER_COMMAND   (iCommandGReloadWeapon)  //Dmut: перезарядка активного оружия
    REGISTER_COMMAND   (iCommandGFire)          //Dmut: выстрел или начало очереди 
    REGISTER_COMMAND   (iCommandGFireStop)      //Dmut: конец очереди 
    REGISTER_COMMAND   (iCommandGZoomIn)        //Dmut: приблизить FOV
    REGISTER_COMMAND   (iCommandGZoomOut)    //Dmut: вернуть в норму FOV
    REGISTER_COMMAND   (iCommandGChangeSpeedH)  //SFINX альтернативное управление (по скорости) по горизонту
    REGISTER_COMMAND   (iCommandGChangeSpeedV)  //SFINX альтернативное управление (по скорости) по высоте
    REGISTER_COMMAND   (iCommandGLockon)        //Dmut: захватить цель
	REGISTER_COMMAND   (iCommandGLockoff)       //Dmut: снять захват цели
	REGISTER_COMMAND   (iCommandGGroupModeOn)  //Включить режим управления группой (3d вид)
	REGISTER_COMMAND   (iCommandGGroupModeOff) //Выключить режим управления группой
	REGISTER_COMMAND   (iCommandGHoldCursorModeOn)  //Включить привязку курсора к экранным координатам при стрельбе
	REGISTER_COMMAND   (iCommandGHoldCursorModeOff) //Выключить привязку курсора к экранным координатам при стрельбе

	REGISTER_COMMAND   (iCommandViewParticleEffect) //для отладки: создать текущий эффект, в месте куда смотрит камера

	REGISTER_COMMAND   (iCommandPlaneShowKneeboard) 

    REGISTER_COMMAND   (iCommandGIsometricView) //Dmut: вынесенная точка камеры ON\OFF
	REGISTER_COMMAND   (iCommandPlane_SPU9_PTT_on)
	REGISTER_COMMAND   (iCommandPlane_SPU9_PTT_off)
	
	REGISTER_COMMAND   (iCommandPlane_P_51_PTT_on)
	REGISTER_COMMAND   (iCommandPlane_P_51_PTT_off)	

	REGISTER_COMMAND   (iCommandViewTransposeModeOn)
	REGISTER_COMMAND   (iCommandViewTransposeModeOff)	

	REGISTER_COMMAND   (iCommandViewBtn1Down)
	REGISTER_COMMAND   (iCommandViewBtn1Up)
	REGISTER_COMMAND   (iCommandViewBtn2Down)
	REGISTER_COMMAND   (iCommandViewBtn2Up)
	REGISTER_COMMAND   (iCommandViewBtn3Down)
	REGISTER_COMMAND   (iCommandViewBtn3Up)

	REGISTER_COMMAND   (iCommandPlane_P_51_WarEmergencyPower)

	REGISTER_COMMAND	(iCommandViewCockpitChangeSeat)
	REGISTER_COMMAND   (iCommandViewCockpitSetPilotSeat)
	REGISTER_COMMAND   (iCommandViewCockpitSetOperatorSeat)
	REGISTER_COMMAND   (iCommandViewCockpitSetGunnerSeat1)
	REGISTER_COMMAND   (iCommandViewCockpitSetGunnerSeat2)

    REGISTER_COMMAND   (iCommandGViewWSCenter)  //Dmut:Return turret to center position
    REGISTER_COMMAND   (iCommandGStabilize)     //Dmut:Turret stabilizer On/Off

    // panov {
	REGISTER_COMMAND(iCommandViewPauseOnOff)
	// panov }

	REGISTER_COMMAND(iCommandSlaveToTanker)

	REGISTER_COMMAND(iCommandViewNVG_GainUp)
	REGISTER_COMMAND(iCommandViewNVG_GainDown)
	
	REGISTER_COMMAND(iCommandVehicle_LaunchRedFlare)
	REGISTER_COMMAND(iCommandVehicle_LaunchGreenFlare)
	REGISTER_COMMAND(iCommandVehicle_LaunchWhiteFlare)

	REGISTER_COMMAND(iCommand_ExplorationStart)
	REGISTER_COMMAND(iCommand_ExplorationTargetSelection)
	REGISTER_COMMAND(iCommand_ExplorationCansel)

	REGISTER_COMMAND(iCommandViewNVGOff) //!!!!!!Not an avionics action, only view!!!!!!
	/* OBSOLETE */REGISTER_COMMAND(iCommandGZoomUpdate)    //Panov: выставить FOV при смене оружия (GroundCockpit)
	REGISTER_COMMAND(iCommandTakeControlTrack) // для машинок

    REGISTER_COMMAND   (iCommandGSelectFeedSlot1)      //SFINX: выбор 1-го слота подачи боеприпасов
    REGISTER_COMMAND   (iCommandGSelectFeedSlot2)      //SFINX: выбор 2-го слота подачи боеприпасов
    REGISTER_COMMAND   (iCommandGEmissionToggle) // SFINX: включение и отключение излучения (сюда же лазерные дальномеры)
	
	REGISTER_COMMAND   (iCommandToggleMirrors) // on/off mirrors rendering in cockpit
	REGISTER_COMMAND   (iCommandToggleReceiveMode) // on/off mirrors rendering in cockpit
		
	REGISTER_COMMAND(iCommandPlaneChangeLockUp) // informs avionics that iCommandPlaneChangeLock key was released
	REGISTER_COMMAND(iCommandTrainingUserRespondedBack) // user do not understand at want to repeat 

	REGISTER_COMMAND(iCommandClockElapsedTimeReset) // Reset Clock elapsed time meter

    // лазерный дальномер в наземном кокпите
    REGISTER_COMMAND(iCommandGLaserRangerOn)
    REGISTER_COMMAND(iCommandGLaserRangerOff)
    REGISTER_COMMAND(iCommandGLaserRangerReset)
	//команды для груза
	REGISTER_COMMAND(iCommandExtCargoHook)
    REGISTER_COMMAND(iCommandExternalCargoAutounhook)
	// Example of usage: F-15 Return To Search, F-15 reset to NDTWS
	REGISTER_COMMAND(iCommandSensorReset)

    REGISTER_COMMAND(iCommandViewInfoControlPanelOnOff)

	// Команды управления поворотом башни машинки для клавиатуры

	REGISTER_COMMAND(iCommandRotateTurretLeftPressed) // Поворот башни влево.
	REGISTER_COMMAND(iCommandRotateTurretRightPressed) // Поворот башни вправо.
	REGISTER_COMMAND(iCommandRotateTurretUpPressed) // Подъем ствола башни вверх.
	REGISTER_COMMAND(iCommandRotateTurretDownPressed) // Опускание ствола башни вниз.
	
	//Track or mission restart on the way
	REGISTER_COMMAND(iCommandMissionRestart)
    
	REGISTER_COMMAND(iCommandVehicle_ReloadStart) // For manual reloading
    REGISTER_COMMAND(iCommandVehicle_AmmoMaintainInterrupt) // To abort reloading|rearming
	REGISTER_COMMAND(iCommandReloadShaders) // перекомпилировать измененные шейдера
	
    REGISTER_COMMAND(iCommandVehicleElevationCorrectionUp)
    REGISTER_COMMAND(iCommandVehicleElevationCorrectionDown)
    REGISTER_COMMAND(iCommandVehicleAzimuthCorrectionLeft)
    REGISTER_COMMAND(iCommandVehicleAzimuthCorrectionRight)

    REGISTER_COMMAND(iCommandVehicleSightReticleAlternate)

	REGISTER_COMMAND(iCommandExternalCargoIndicator)

    REGISTER_COMMAND(iCommandGReticleLightToggle)
    REGISTER_COMMAND(iCommandGZoomInPressed)
    REGISTER_COMMAND(iCommandGZoomOutPressed)

	REGISTER_COMMAND(iCommandRotateTurretLeftReleased) // Останов поворота башни влево.
	REGISTER_COMMAND(iCommandRotateTurretRightReleased) // Останов поворота башни вправо.
	REGISTER_COMMAND(iCommandRotateTurretUpReleased) // Останов подъема ствола башни вверх.
	REGISTER_COMMAND(iCommandRotateTurretDownReleased) // Останов опускания ствола башни вниз.

    // стрельба из спаренного пулемета и основного орудия по разным кнопкам
    REGISTER_COMMAND(iCommandVehicle_FireSecondary) 
    REGISTER_COMMAND(iCommandVehicle_FireSecondaryOff)

    REGISTER_COMMAND(iCommand_Ground_IFF_Down)
    REGISTER_COMMAND(iCommandDriverView)

	// Insérer nouvelles commandes juste avant ce commentaire!
	// ATTENTION! Avec 500 départs commande analogique.
	// Oykin: lancer des commandes analogiques reportées à 2000
	REGISTER_COMMAND(iCommandMaximum)							// Количество всех команд
	// Сюда ничего не вставлять!

#ifndef INPUT_LUA_BINDING
};
#endif

#endif

#ifdef INPUT_LUA_BINDING
#define _ICOMMAND_H_
#endif