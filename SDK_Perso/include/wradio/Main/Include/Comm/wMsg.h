BEGIN_WMESSAGES
	REGISTER_WMESSAGE_EX(wMsgNull, 4000)	
		//Leader
		//Leader to wingmen/pair/flight/wing/group
		REGISTER_WMESSAGE(wMsgLeaderNull)

			REGISTER_WMESSAGE(wMsgLeaderToWingmenNull)
				//Engage
				REGISTER_WMESSAGE(wMsgLeaderEngageMyTarget)
				REGISTER_WMESSAGE(wMsgLeaderEngageBandits)
				REGISTER_WMESSAGE(wMsgLeaderEngageGroundTargets)
				REGISTER_WMESSAGE(wMsgLeaderEngageArmor)
				REGISTER_WMESSAGE(wMsgLeaderEngageArtillery)
				REGISTER_WMESSAGE(wMsgLeaderEngageAirDefenses)
				REGISTER_WMESSAGE(wMsgLeaderEngageUtilityVehicles)
				REGISTER_WMESSAGE(wMsgLeaderEngageInfantry)
				REGISTER_WMESSAGE(wMsgLeaderEngageNavalTargets)
				REGISTER_WMESSAGE(wMsgLeaderEngageDlinkTarget)
				REGISTER_WMESSAGE(wMsgLeaderEngageDlinkTargets)
				REGISTER_WMESSAGE(wMsgLeaderEngageDlinkTargetByType)
				REGISTER_WMESSAGE(wMsgLeaderEngageDlinkTargetsByType) 
				REGISTER_WMESSAGE(wMsgLeaderFulfilTheTaskAndJoinUp)
				REGISTER_WMESSAGE(wMsgLeaderFulfilTheTaskAndReturnToBase)
				REGISTER_WMESSAGE(wMsgLeaderRayTarget)
				REGISTER_WMESSAGE(wMsgLeaderMyEnemyAttack)
				REGISTER_WMESSAGE(wMsgLeaderCoverMe)
				//Maneuvers
				REGISTER_WMESSAGE(wMsgLeaderPincerRight)
				REGISTER_WMESSAGE(wMsgLeaderPincerLeft)
				REGISTER_WMESSAGE(wMsgLeaderPincerHigh)
				REGISTER_WMESSAGE(wMsgLeaderPincerLow)
				REGISTER_WMESSAGE(wMsgLeaderBreakRight)
				REGISTER_WMESSAGE(wMsgLeaderBreakLeft)
				REGISTER_WMESSAGE(wMsgLeaderBreakHigh)
				REGISTER_WMESSAGE(wMsgLeaderBreakLow)
				REGISTER_WMESSAGE(wMsgLeaderClearRight)
				REGISTER_WMESSAGE(wMsgLeaderClearLeft)
				REGISTER_WMESSAGE(wMsgLeaderPump)
				//Navigation
				REGISTER_WMESSAGE(wMsgLeaderAnchorHere)
				REGISTER_WMESSAGE(wMsgLeaderFlyAndOrbitAtSteerPoint)
				REGISTER_WMESSAGE(wMsgLeaderFlyAndOrbitAtSPI)
				REGISTER_WMESSAGE(wMsgLeaderFlyAndOrbitAtPoint)
				REGISTER_WMESSAGE(wMsgLeaderReturnToBase)
				REGISTER_WMESSAGE(wMsgLeaderGoRefueling)
				REGISTER_WMESSAGE(wMsgLeaderJoinUp)
				REGISTER_WMESSAGE(wMsgLeaderFlyRoute)
				REGISTER_WMESSAGE(wMsgLeaderMakeRecon)
				REGISTER_WMESSAGE(wMsgLeaderMakeReconAtPoint)
				//State
				REGISTER_WMESSAGE(wMsgLeaderRadarOn)
				REGISTER_WMESSAGE(wMsgLeaderRadarOff)
				REGISTER_WMESSAGE(wMsgLeaderDisturberOn)
				REGISTER_WMESSAGE(wMsgLeaderDisturberOff)
				REGISTER_WMESSAGE(wMsgLeaderSmokeOn)
				REGISTER_WMESSAGE(wMsgLeaderSmokeOff)
				REGISTER_WMESSAGE(wMsgLeaderJettisonWeapons)
				REGISTER_WMESSAGE(wMsgLeaderFenceIn)
				REGISTER_WMESSAGE(wMsgLeaderFenceOut)
				REGISTER_WMESSAGE(wMsgLeaderOut)
				//Formation
				//Airplane
				REGISTER_WMESSAGE(wMsgLeaderLineAbreast)
				REGISTER_WMESSAGE(wMsgLeaderGoTrail)
				REGISTER_WMESSAGE(wMsgLeaderWedge)
				REGISTER_WMESSAGE(wMsgLeaderEchelonRight)
				REGISTER_WMESSAGE(wMsgLeaderEchelonLeft)
				REGISTER_WMESSAGE(wMsgLeaderFingerFour)
				REGISTER_WMESSAGE(wMsgLeaderSpreadFour)
				REGISTER_WMESSAGE(wMsgLeaderCloseFormation)
				REGISTER_WMESSAGE(wMsgLeaderOpenFormation)	
				//Helicopter
				REGISTER_WMESSAGE(wMsgLeaderHelGoHeavy)
				REGISTER_WMESSAGE(wMsgLeaderHelGoEchelon)
				REGISTER_WMESSAGE(wMsgLeaderHelGoSpread)
				REGISTER_WMESSAGE(wMsgLeaderHelGoTrail)
				REGISTER_WMESSAGE(wMsgLeaderHelGoOverwatch)
				REGISTER_WMESSAGE(wMsgLeaderHelGoLeft)
				REGISTER_WMESSAGE(wMsgLeaderHelGoRight)
				REGISTER_WMESSAGE(wMsgLeaderHelGoTight)
				REGISTER_WMESSAGE(wMsgLeaderHelGoCruise)
				REGISTER_WMESSAGE(wMsgLeaderHelGoCombat)			
			REGISTER_WMESSAGE(wMsgLeaderToWingmenMaximum)

			//Leader to service
			REGISTER_WMESSAGE(wMsgLeaderToServiceNull)
				//Leader to ATC
				REGISTER_WMESSAGE(wMsgLeaderToATCNull)
					REGISTER_WMESSAGE(wMsgLeaderRequestEnginesLaunch)
					REGISTER_WMESSAGE(wMsgLeaderRequestControlHover)
					REGISTER_WMESSAGE(wMsgLeaderRequestTaxiToRunway)
					REGISTER_WMESSAGE(wMsgLeaderRequestTakeOff)
					REGISTER_WMESSAGE(wMsgLeaderAbortTakeoff)
					REGISTER_WMESSAGE(wMsgLeaderRequestAzimuth)
					REGISTER_WMESSAGE(wMsgLeaderInbound)
					REGISTER_WMESSAGE(wMsgLeaderAbortInbound)
					REGISTER_WMESSAGE(wMsgLeaderRequestLanding)	
				REGISTER_WMESSAGE(wMsgLeaderToATCMaximum)
				
				//Leader to AWACS
				REGISTER_WMESSAGE(wMsgLeaderToAWACSNull)
					REGISTER_WMESSAGE(wMsgLeaderVectorToBullseye)
					REGISTER_WMESSAGE(wMsgLeaderVectorToNearestBandit)
					REGISTER_WMESSAGE(wMsgLeaderVectorToHomeplate)
					REGISTER_WMESSAGE(wMsgLeaderVectorToTanker)
					REGISTER_WMESSAGE(wMsgLeaderDeclare)
					REGISTER_WMESSAGE(wMsgLeaderPicture)
				REGISTER_WMESSAGE(wMsgLeaderToAWACSMaximum)

				//Leader to Tanker
				REGISTER_WMESSAGE(wMsgLeaderToTankerNull)
					REGISTER_WMESSAGE(wMsgLeaderIntentToRefuel)	
					REGISTER_WMESSAGE(wMsgLeaderAbortRefueling)
					REGISTER_WMESSAGE(wMsgLeaderReadyPreContact)
					REGISTER_WMESSAGE(wMsgLeaderStopRefueling)
				REGISTER_WMESSAGE(wMsgLeaderToTankerMaximum)
							
				//Player to FAC
				REGISTER_WMESSAGE(wMsgLeaderToFACNull)
					REGISTER_WMESSAGE(wMsgLeaderCheckIn)
					REGISTER_WMESSAGE(wMsgLeaderCheckOut)
					REGISTER_WMESSAGE(wMsgLeaderReadyToCopy)
					REGISTER_WMESSAGE(wMsgLeaderReadyToCopyRemarks)
					REGISTER_WMESSAGE(wMsgLeader9LineReadback)
					REGISTER_WMESSAGE(wMsgLeaderRequestTasking)		
					REGISTER_WMESSAGE(wMsgLeaderRequestBDA)		
					REGISTER_WMESSAGE(wMsgLeaderRequestTargetDescription)
					REGISTER_WMESSAGE(wMsgLeaderUnableToComply)
					REGISTER_WMESSAGE(wMsgLeaderFACRepeat)
					REGISTER_WMESSAGE(wMsgLeader_IP_INBOUND)
					REGISTER_WMESSAGE(wMsgLeader_ONE_MINUTE)
					REGISTER_WMESSAGE(wMsgLeader_IN)
					REGISTER_WMESSAGE(wMsgLeader_OFF)
					REGISTER_WMESSAGE(wMsgLeader_ATTACK_COMPLETE)
					REGISTER_WMESSAGE(wMsgLeaderAdviseWhenReadyToCopyBDA)
					REGISTER_WMESSAGE(wMsgLeaderContact)
					REGISTER_WMESSAGE(wMsgLeader_NO_JOY)
					REGISTER_WMESSAGE(wMsgLeader_CONTACT_the_mark)
					REGISTER_WMESSAGE(wMsgLeader_SPARKLE)
					REGISTER_WMESSAGE(wMsgLeader_SNAKE)
					REGISTER_WMESSAGE(wMsgLeader_PULSE)
					REGISTER_WMESSAGE(wMsgLeader_STEADY)
					REGISTER_WMESSAGE(wMsgLeader_ROPE)
					REGISTER_WMESSAGE(wMsgLeader_CONTACT_SPARKLE)
					REGISTER_WMESSAGE(wMsgLeader_STOP)
					REGISTER_WMESSAGE(wMsgLeader_TEN_SECONDS)
					REGISTER_WMESSAGE(wMsgLeader_LASER_ON)
					REGISTER_WMESSAGE(wMsgLeader_SHIFT)
					REGISTER_WMESSAGE(wMsgLeader_SPOT)
					REGISTER_WMESSAGE(wMsgLeader_TERMINATE)
					REGISTER_WMESSAGE(wMsgLeaderGuns)
					REGISTER_WMESSAGE(wMsgLeaderBombsAway)
					REGISTER_WMESSAGE(wMsgLeaderRIFLE)
					REGISTER_WMESSAGE(wMsgLeaderRockets)
					REGISTER_WMESSAGE(wMsgLeaderBDA)
					REGISTER_WMESSAGE(wMsgLeaderINFLIGHTREP)
				REGISTER_WMESSAGE(wMsgLeaderToFACMaximum)
				
				//Leader to Ground Crew
				REGISTER_WMESSAGE(wMsgLeaderToGroundCrewNull)
					REGISTER_WMESSAGE(wMsgLeaderSpecialCommand)
					REGISTER_WMESSAGE(wMsgLeaderRequestRefueling)
					REGISTER_WMESSAGE(wMsgLeaderRequestCannonReloading)
					REGISTER_WMESSAGE(wMsgLeaderRequestRearming)
					REGISTER_WMESSAGE(wMsgLeaderGroundToggleElecPower)
					REGISTER_WMESSAGE(wMsgLeaderGroundToggleWheelChocks)
					REGISTER_WMESSAGE(wMsgLeaderGroundRepair)
				REGISTER_WMESSAGE(wMsgLeaderToGroundCrewMaximum)			
			REGISTER_WMESSAGE(wMsgLeaderToServiceMaximum)

		REGISTER_WMESSAGE(wMsgLeaderMaximum)	
	
		//Wingmen	
		REGISTER_WMESSAGE(wMsgWingmenNull)
			//Answers	
			REGISTER_WMESSAGE(wMsgWingmenCopy)
			REGISTER_WMESSAGE(wMsgWingmenNegative)
			//Reports
			REGISTER_WMESSAGE(wMsgWingmenHelReconBearing)
			REGISTER_WMESSAGE(wMsgWingmenHelReconEndFound)
			REGISTER_WMESSAGE(wMsgWingmenHelReconEndNotFound)
			REGISTER_WMESSAGE(wMsgWingmenHelLaunchAbortTask)
				//Contacts
				REGISTER_WMESSAGE(wMsgWingmenRadarContact)
				REGISTER_WMESSAGE(wMsgWingmenContact)
				REGISTER_WMESSAGE(wMsgWingmenTallyBandit)
				REGISTER_WMESSAGE(wMsgWingmenNails)
				REGISTER_WMESSAGE(wMsgWingmenSpike)
				REGISTER_WMESSAGE(wMsgWingmenMudSpike)
			//Events
			REGISTER_WMESSAGE(wMsgWingmenIamHit) 
			REGISTER_WMESSAGE(wMsgWingmenIveTakenDamage)
			REGISTER_WMESSAGE(wMsgWingmenEjecting) 
			REGISTER_WMESSAGE(wMsgWingmenBailOut)
			REGISTER_WMESSAGE(wMsgWingmenWheelsUp)
			REGISTER_WMESSAGE(wMsgWingmenHelOccupFormLeft)
			REGISTER_WMESSAGE(wMsgWingmenHelOccupFormRight)
			REGISTER_WMESSAGE(wMsgWingmenHelOccupFormBehind)
			REGISTER_WMESSAGE(wMsgWingmenOnPosition)
				//Weapon launch	
				REGISTER_WMESSAGE(wMsgWingmenGuns)
				REGISTER_WMESSAGE(wMsgWingmenFoxFrom)
				REGISTER_WMESSAGE(wMsgWingmenFox1)
				REGISTER_WMESSAGE(wMsgWingmenFox2)
				REGISTER_WMESSAGE(wMsgWingmenFox3)	
				REGISTER_WMESSAGE(wMsgWingmenBombsAway)
				REGISTER_WMESSAGE(wMsgWingmenGBUAway)
				REGISTER_WMESSAGE(wMsgWingmenMagnum)
				REGISTER_WMESSAGE(wMsgWingmenMissileAway)
				REGISTER_WMESSAGE(wMsgWingmenRifle)
				REGISTER_WMESSAGE(wMsgWingmenBruiser)
				REGISTER_WMESSAGE(wMsgWingmenRockets)
				REGISTER_WMESSAGE(wMsgWingmenSmoke)
			//Status messages
			REGISTER_WMESSAGE(wMsgWingmenRadarOff)
			REGISTER_WMESSAGE(wMsgWingmenRadarOn)
			REGISTER_WMESSAGE(wMsgWingmenMusicOff)
			REGISTER_WMESSAGE(wMsgWingmenMusicOn)
			REGISTER_WMESSAGE(wMsgWingmenBingo)
			REGISTER_WMESSAGE(wMsgWingmenKansas)
			REGISTER_WMESSAGE(wMsgWingmenWinchester)		
				//Task
				REGISTER_WMESSAGE(wMsgWingmenRolling)
				REGISTER_WMESSAGE(wMsgWingmenRTB)
				REGISTER_WMESSAGE(wMsgWingmenBugout)
				REGISTER_WMESSAGE(wMsgWingmenRejoin)
				REGISTER_WMESSAGE(wMsgWingmenFollowScanMode)
					//Engaging
					REGISTER_WMESSAGE(wMsgWingmenAttackingPrimary)
					REGISTER_WMESSAGE(wMsgWingmenEngagingBandit)
					REGISTER_WMESSAGE(wMsgWingmenEngagingHelicopter)
					REGISTER_WMESSAGE(wMsgWingmenEngagingSAM)
					REGISTER_WMESSAGE(wMsgWingmenEngagingAAA)
					REGISTER_WMESSAGE(wMsgWingmenEngagingArmor)
					REGISTER_WMESSAGE(wMsgWingmenEngagingArtillery)
					REGISTER_WMESSAGE(wMsgWingmenEngagingVehicle)
					REGISTER_WMESSAGE(wMsgWingmenEngagingShip)
					REGISTER_WMESSAGE(wMsgWingmenEngagingBunker)
					REGISTER_WMESSAGE(wMsgWingmenEngagingStructure)
					//Attack stage
					REGISTER_WMESSAGE(wMsgWingmenRunningIn)
					REGISTER_WMESSAGE(wMsgWingmenRunningOff)
					REGISTER_WMESSAGE(wMsgWingmenBanditDestroyed)
					REGISTER_WMESSAGE(wMsgWingmenTargetDestroyed)
					//Defensive
					REGISTER_WMESSAGE(wMsgWingmenEngagedDefensive)				
			//Requests
			REGISTER_WMESSAGE(wMsgWingmenRequestPermissionToAttack)
			REGISTER_WMESSAGE(wMsgWingmenSplashMyBandit)					
			//Attack warning    
			REGISTER_WMESSAGE(wMsgWingmenSAMLaunch)
			REGISTER_WMESSAGE(wMsgWingmenMissileLaunch)
			REGISTER_WMESSAGE(wMsgWingmenCheckYourSix)		
		REGISTER_WMESSAGE(wMsgWingmenMaximum)
		
		//Flights
		REGISTER_WMESSAGE(wMsgFlightNull)
			REGISTER_WMESSAGE(wMsgFlightAirbone)
			REGISTER_WMESSAGE(wMsgFlightPassingWaypoint)
			REGISTER_WMESSAGE(wMsgFlightOnStation)
			REGISTER_WMESSAGE(wMsgFlightDepartingStation)
			REGISTER_WMESSAGE(wMsgFlightRTB)
			REGISTER_WMESSAGE(wMsgFlightTallyBandit)
			REGISTER_WMESSAGE(wMsgFlightTally)
			REGISTER_WMESSAGE(wMsgFlightEngagingBandit)
			REGISTER_WMESSAGE(wMsgFlightEngaging)
			REGISTER_WMESSAGE(wMsgFlightSplashBandit)
			REGISTER_WMESSAGE(wMsgFlightTargetDestroyed)
			REGISTER_WMESSAGE(wMsgFlightDefensive)
			REGISTER_WMESSAGE(wMsgFlightMemberDown)
		REGISTER_WMESSAGE(wMsgFlightMaximum)
		
		//Services
		REGISTER_WMESSAGE(wMsgServiceNull)
			//ATC
			REGISTER_WMESSAGE(wMsgATCNull)
				REGISTER_WMESSAGE(wMsgATCClearedForEngineStartUp)
				REGISTER_WMESSAGE(wMsgATCEngineStartUpDenied)
				REGISTER_WMESSAGE(wMsgATCClearedToTaxiRunWay)
				REGISTER_WMESSAGE(wMsgATCTaxiDenied)
				REGISTER_WMESSAGE(wMsgATCHoldPosition)
				REGISTER_WMESSAGE(wMsgATCYouAreClearedForTO)
				REGISTER_WMESSAGE(wMsgATCTakeoffDenied)
				REGISTER_WMESSAGE(wMsgATCYouHadTakenOffWithNoPermission)
				REGISTER_WMESSAGE(wMsgATCTrafficBearing)
				REGISTER_WMESSAGE(wMsgATCYouAreClearedForLanding)
				REGISTER_WMESSAGE(wMsgATCYouAreAboveGlidePath)
				REGISTER_WMESSAGE(wMsgATCYouAreOnGlidePath)
				REGISTER_WMESSAGE(wMsgATCYouAreBelowGlidePath)
				REGISTER_WMESSAGE(wMsgATCTaxiToParkingArea)
				REGISTER_WMESSAGE(wMsgATCGoAround)
				REGISTER_WMESSAGE(wMsgATCContinueTaxi)
				REGISTER_WMESSAGE(wMsgATCOrbitForSpacing)
				REGISTER_WMESSAGE(wMsgATCClearedForVisual)
				REGISTER_WMESSAGE(wMsgATCFlyHeading)
				REGISTER_WMESSAGE(wMsgATCAzimuth)
				REGISTER_WMESSAGE(wMsgATCGoSecondary)
				REGISTER_WMESSAGE(wMsgATCClearedControlHover)
				REGISTER_WMESSAGE(wMsgATCControlHoverDenied)
				REGISTER_WMESSAGE(wMsgATCCheckLandingGear)
			REGISTER_WMESSAGE(wMsgATCMaximum)
			
			//AWACS	
			REGISTER_WMESSAGE(wMsgAWACSNull)
				REGISTER_WMESSAGE(wMsgAWACSVectorToBullseye)
				REGISTER_WMESSAGE(wMsgAWACSBanditBearingForMiles)
				REGISTER_WMESSAGE(wMsgAWACSVectorToNearestBandit)
				REGISTER_WMESSAGE(wMsgAWACSPopUpGroup)
				REGISTER_WMESSAGE(wMsgAWACSHomeBearing)
				REGISTER_WMESSAGE(wMsgAWACSTankerBearing)
				REGISTER_WMESSAGE(wMsgAWACSContactIsFriendly)
				REGISTER_WMESSAGE(wMsgAWACSContactIsHostile)
				REGISTER_WMESSAGE(wMsgAWACSClean)
				REGISTER_WMESSAGE(wMsgAWACSMerged)
				REGISTER_WMESSAGE(wMsgAWACSNoTankerAvailable)
				REGISTER_WMESSAGE(wMsgAWACSPicture)
			REGISTER_WMESSAGE(wMsgAWACSMaximum)
			
			//Tanker
			REGISTER_WMESSAGE(wMsgTankerNull)	
				REGISTER_WMESSAGE(wMsgTankerClearedPreContact)
				REGISTER_WMESSAGE(wMsgTankerClearedContact)
				REGISTER_WMESSAGE(wMsgTankerContact)
				REGISTER_WMESSAGE(wMsgTankerReturnPreContact)
				REGISTER_WMESSAGE(wMsgTankerChicksInTow)
				REGISTER_WMESSAGE(wMsgTankerFuelFlow)
				REGISTER_WMESSAGE(wMsgTankerRefuelingComplete)
				REGISTER_WMESSAGE(wMsgTankerDisconnectNow)
				REGISTER_WMESSAGE(wMsgTankerBreakawayBreakaway)
			REGISTER_WMESSAGE(wMsgTankerMaximum)    
			
			//FAC
			REGISTER_WMESSAGE(wMsgFACNull)
				REGISTER_WMESSAGE(wMsgFAC_CONTINUE)		
				REGISTER_WMESSAGE(wMsgFAC_ABORT)
				REGISTER_WMESSAGE(wMsgFAC_ABORT_ATTACK)
				REGISTER_WMESSAGE(wMsgFAC_CLERED_HOT)
				REGISTER_WMESSAGE(wMsgFAC_CLEARED_TO_ENGAGE)
				REGISTER_WMESSAGE(wMsgFAC_CLEARED_TO_DEPART)
				REGISTER_WMESSAGE(wMsgFACNoTaskingAvailableStandBy)
				REGISTER_WMESSAGE(wMsgFACNoTaskingAvailable)
				REGISTER_WMESSAGE(wMsgFACType1InEffectAdviseWhenReadyFor9Line)
				REGISTER_WMESSAGE(wMsgFACType2InEffectAdviseWhenReadyFor9Line)
				REGISTER_WMESSAGE(wMsgFACType3InEffectAdviseWhenReadyFor9Line)
				REGISTER_WMESSAGE(wMsgFACAdviseWhenReadyForRemarksAndFutherTalkOn)
				REGISTER_WMESSAGE(wMsgFACStandbyForData)
				REGISTER_WMESSAGE(wMsgFACReadBackCorrect)
				REGISTER_WMESSAGE(wMsgFACNoTaskingAvailableClearedToDepart)
				REGISTER_WMESSAGE(wMsgFACReport_IP_INBOUND)
				REGISTER_WMESSAGE(wMsgFACReportWhenAttackComplete)
				REGISTER_WMESSAGE(wMsgFACThatIsYourTarget)
				REGISTER_WMESSAGE(wMsgFACThatIsNotYourTarget)
				REGISTER_WMESSAGE(wMsgFACThatIsFriendly)
				REGISTER_WMESSAGE(wMsgFACYourTarget)		
				REGISTER_WMESSAGE(wMsgFAC9lineBrief)
				REGISTER_WMESSAGE(wMsgFAC9lineBriefWP)
				REGISTER_WMESSAGE(wMsgFAC9lineBriefIRPointer)
				REGISTER_WMESSAGE(wMsgFAC9lineBriefLaser)
				REGISTER_WMESSAGE(wMsgFAC9lineRemarks)
				REGISTER_WMESSAGE(wMsgFACTargetDescription)		
				REGISTER_WMESSAGE(wMsgFACTargetHit)
				REGISTER_WMESSAGE(wMsgFACTargetDestroyed)
				REGISTER_WMESSAGE(wMsgFACTargetPartiallyDestroyed)
				REGISTER_WMESSAGE(wMsgFACTargetNotDestroyed)		
				REGISTER_WMESSAGE(wMsgUseWeapon)
				REGISTER_WMESSAGE(wMsgFACMarkOnDeck)
				REGISTER_WMESSAGE(wMsgFACFromTheMark)
				REGISTER_WMESSAGE(wMsgFAC_SPARKLE)
				REGISTER_WMESSAGE(wMsgFAC_SNAKE)
				REGISTER_WMESSAGE(wMsgFAC_PULSE)
				REGISTER_WMESSAGE(wMsgFAC_STEADY)
				REGISTER_WMESSAGE(wMsgFAC_STOP)
				REGISTER_WMESSAGE(wMsgFAC_ROPE)
				REGISTER_WMESSAGE(wMsgFAC_LASER_ON)
				REGISTER_WMESSAGE(wMsgFAC_LASING)
				REGISTER_WMESSAGE(wMsgFAC_SHIFT)
				REGISTER_WMESSAGE(wMsgFAC_TERMINATE)
				REGISTER_WMESSAGE(wMsgFAC_NoMark)
				REGISTER_WMESSAGE(wMsgFAC_SAM_launch)
				REGISTER_WMESSAGE(wMsgFACAreYouReadyToCopy)
				REGISTER_WMESSAGE(wMsgFACWhereAreYouGoing)
				REGISTER_WMESSAGE(wMsgFACDoYouSeeTheMark)
			REGISTER_WMESSAGE(wMsgFACMaximum)
			
			//ССС
			REGISTER_WMESSAGE(wMsgCCCNull)
				REGISTER_WMESSAGE(wMsgCCCFollowTo)
				REGISTER_WMESSAGE(wMsgCCCTasking)
				REGISTER_WMESSAGE(wMsgCCCResume)
				REGISTER_WMESSAGE(wMsgCCCRTB)
			REGISTER_WMESSAGE(wMsgCCCMaximum)
			
			//Ground Crew
			REGISTER_WMESSAGE(wMsgGroundCrewNull)
				REGISTER_WMESSAGE(wMsgGroundCrewCopy)		   //Copy  that
				REGISTER_WMESSAGE(wMsgGroundCrewNegative)      //
				REGISTER_WMESSAGE(wMsgGroundCrewReloadDone)    //Reload done,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewRefuelDone)    //Refuel done,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewHMSDone)       //There is your HMS,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewNVGDone)       //There is your gogles,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewGroundPowerOn) //Ground power on,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewGroundPowerOff)//Ground power off,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewWheelChocksOn) //The wheels chocked,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewWheelChocksOff)//The wheels unchocked,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewTurboGearOn)   //Turbo gear on ,sir!
				REGISTER_WMESSAGE(wMsgGroundCrewTurboGearOff)  //Turbo gear off,sir!
				REGISTER_WMESSAGE(wMsgGroundDone)
				REGISTER_WMESSAGE(wMsgGroundCrewStop)          //What hell are you doing ,man !!!!
			REGISTER_WMESSAGE(wMsgGroundCrewMaximum)
		REGISTER_WMESSAGE(wMsgServiceMaximum)

		//Betty
		REGISTER_WMESSAGE(wMsgBettyNull)
			REGISTER_WMESSAGE(wMsgBettyLeftEngineFire )
			REGISTER_WMESSAGE(wMsgBettyRightEngineFire)
			REGISTER_WMESSAGE(wMsgBettyMaximumAOA)
			REGISTER_WMESSAGE(wMsgBettyAOAOverLimit)
			REGISTER_WMESSAGE(wMsgBettyMaximumG)
			REGISTER_WMESSAGE(wMsgBettyGearDown)
			REGISTER_WMESSAGE(wMsgBettyGearUp)			
			REGISTER_WMESSAGE(wMsgBettyMaximumSpeed)
			REGISTER_WMESSAGE(wMsgBettyMinimumSpeed)
			REGISTER_WMESSAGE(wMsgBettyMissile3Low)		
			REGISTER_WMESSAGE(wMsgBettyMissile3High)
			REGISTER_WMESSAGE(wMsgBettyMissile6Low)		
			REGISTER_WMESSAGE(wMsgBettyMissile6High)		
			REGISTER_WMESSAGE(wMsgBettyMissile9Low)		
			REGISTER_WMESSAGE(wMsgBettyMissile9High)		
			REGISTER_WMESSAGE(wMsgBettyMissile12Low)		
			REGISTER_WMESSAGE(wMsgBettyMissile12High)	
			REGISTER_WMESSAGE(wMsgBettyBingoFuel)
			REGISTER_WMESSAGE(wMsgBettyAttitudeIndicatorFailure)
			REGISTER_WMESSAGE(wMsgBettyRadarFailure)		
			REGISTER_WMESSAGE(wMsgBettyEOSFailure)
			REGISTER_WMESSAGE(wMsgBettySystemsFailure)
			REGISTER_WMESSAGE(wMsgBettyTWSFailure)
			REGISTER_WMESSAGE(wMsgBettyMLWSFailure)			
			REGISTER_WMESSAGE(wMsgBettyECMFailure)			
			REGISTER_WMESSAGE(wMsgBettyNCSFailure)			
			REGISTER_WMESSAGE(wMsgBettyACSFailure)			
			REGISTER_WMESSAGE(wMsgBettyThrottleBackLeftEngine)
			REGISTER_WMESSAGE(wMsgBettyThrottleBackRightEngine)
			REGISTER_WMESSAGE(wMsgBettyPower)
			REGISTER_WMESSAGE(wMsgBettyHydrolicsFailure)
			REGISTER_WMESSAGE(wMsgBettyEject)
			REGISTER_WMESSAGE(wMsgBettyGOverLimit)
			REGISTER_WMESSAGE(wMsgBettyFuel1500)
			REGISTER_WMESSAGE(wMsgBettyFuel800)
			REGISTER_WMESSAGE(wMsgBettyFuel500)
			REGISTER_WMESSAGE(wMsgBettyPullUp)			
			REGISTER_WMESSAGE(wMsgBettyLaunchAuthorised)
			REGISTER_WMESSAGE(wMsgBettyMissileMissile)
			REGISTER_WMESSAGE(wMsgBettyShootShoot)
			REGISTER_WMESSAGE(wMsgBettyFlightControls)
			REGISTER_WMESSAGE(wMsgBettyWarningWarning)
			REGISTER_WMESSAGE(wMsgBettyMessageBegin)
			REGISTER_WMESSAGE(wMsgBettyMessageEnd)
			REGISTER_WMESSAGE(wMsgBettyGearDownSingle)
			REGISTER_WMESSAGE(wMsgBettyCancel)
		REGISTER_WMESSAGE(wMsgBettyMaximum)

		//Ka-50 ALMAZ messages
		REGISTER_WMESSAGE(wMsgALMAZ_Null)
			REGISTER_WMESSAGE(wMsgALMAZ_IS_READY)					//Речевой информатор исправен
			REGISTER_WMESSAGE(wMsgALMAZ_WATCH_EKRAN)				//Смотри УСТ
			REGISTER_WMESSAGE(wMsgALMAZ_THREAT)						//Атака берегись
			REGISTER_WMESSAGE(wMsgALMAZ_CHECK_OIL_PRESS_LEFT_TRANSM)//Проверь давление масла левого редуктора
			REGISTER_WMESSAGE(wMsgALMAZ_CHECK_OIL_PRESS_RIGHT_TRANSM)//Проверь давление масла правого редуктора
			REGISTER_WMESSAGE(wMsgALMAZ_LEFT_ENG_FIRE)   			//Пожар левого двигателя
			REGISTER_WMESSAGE(wMsgALMAZ_RIGHT_ENG_FIRE)				//Пожар правого двигателя
			REGISTER_WMESSAGE(wMsgALMAZ_APU_FIRE)					//Пожар вспомогательной силовой установки
			REGISTER_WMESSAGE(wMsgALMAZ_HYDRO_FIRE)					//Пожар в отсеке гидравлики
			REGISTER_WMESSAGE(wMsgALMAZ_FAN_FIRE)					//Пожар в отсеке вентиляции
			REGISTER_WMESSAGE(wMsgALMAZ_LEFT_ENG_TORQUE)			//Раскрутка турбины левого двигателя
			REGISTER_WMESSAGE(wMsgALMAZ_RIGHT_ENG_TORQUE)			//Раскрутка турбины правого двигателя
			REGISTER_WMESSAGE(wMsgALMAZ_DANGER_ENG_VIBR)			//Опасно вибрация двигателя
			REGISTER_WMESSAGE(wMsgALMAZ_DATA)                       //Принять ЦУ
			REGISTER_WMESSAGE(wMsgALMAZ_MAIN_HYDRO)					//Отказ основной гидросистемы
			REGISTER_WMESSAGE(wMsgALMAZ_COMMON_HYDRO)    			//Отказ вспомогательной гидросистемы
			REGISTER_WMESSAGE(wMsgALMAZ_LOWER_GEAR)					//Выпусти шасси
			REGISTER_WMESSAGE(wMsgALMAZ_CHECK_MAIN_TRANSM)			//Проверь параметры масла главного редуктора
			REGISTER_WMESSAGE(wMsgALMAZ_TURN_ON_BACKUP_TRANSP)		//Включи запасной код ответчика
			REGISTER_WMESSAGE(wMsgALMAZ_ELEC_ON_ACCUM)				//Сеть на аккумуляторе
			REGISTER_WMESSAGE(wMsgALMAZ_USE_TV)						//Работай по телевизору
			REGISTER_WMESSAGE(wMsgALMAZ_USE_MANUAL_ATTACK_KI_TV)	//Включи резервное управление. Работай по коллиматору и телевизору
			REGISTER_WMESSAGE(wMsgALMAZ_FAILURE_WCS_ROCKET)			//Отказ СУО. Нет управления подвесками
			REGISTER_WMESSAGE(wMsgALMAZ_GUN_ACTUATOR_FAILURE) 		//Отказ ППУ
			REGISTER_WMESSAGE(wMsgALMAZ_MIN_FUEL)					//Минимальный остаток топлива
			REGISTER_WMESSAGE(wMsgALMAZ_TURN_ON_ROTOR_ANTIICE)		//Обледенение. Включи ПОС винтов.
			REGISTER_WMESSAGE(wMsgALMAZ_RADIO_ALT)					//Отказ радиовысотомера
			REGISTER_WMESSAGE(wMsgALMAZ_INS)						//Отказ курсовертикали
			REGISTER_WMESSAGE(wMsgALMAZ_TURN_ON_GRID_USE_FIXED_GUN)	//Включи сетку коллиматора. Работай с неподвижной ПУ.
			REGISTER_WMESSAGE(wMsgALMAZ_TURN_ON_DC_AC_CONVERT)		//Включи преобразователь
			REGISTER_WMESSAGE(wMsgALMAZ_CHECK_LEFT_TRANSM)			//Проверь параметры масла левого редуктора
			REGISTER_WMESSAGE(wMsgALMAZ_CHECK_RIGHT_TRANSM)			//Проверь параметры масла правого редуктора
			REGISTER_WMESSAGE(wMsgALMAZ_ACTUATOR_OIL_PRESSURE)		//Мало давление масла приводов
			REGISTER_WMESSAGE(wMsgALMAZ_FAILURE_LEFT_PROBE_HEATER)	//Отказ обогрева ПВД левого
			REGISTER_WMESSAGE(wMsgALMAZ_FAILURE_RIGHT_PROBE_HEATER)	//Отказ обогрева ПВД правого
			REGISTER_WMESSAGE(wMsgALMAZ_DNS)     					//Отказ доплеровского измерителя скорости
			REGISTER_WMESSAGE(wMsgALMAZ_FAILURE_NAV_POSITION)		//Нет счисления координат
			REGISTER_WMESSAGE(wMsgALMAZ_GENERATOR_FAILURE)			//Отказ генератора
			REGISTER_WMESSAGE(wMsgALMAZ_DC_RECTIF_FAILURE)			//Отказ выпрямителя
			REGISTER_WMESSAGE(wMsgALMAZ_ENG_DIGITAL_CONTROL_FAILURE)//Отказ электронного регулятора двигателя
			REGISTER_WMESSAGE(wMsgALMAZ_LOW_COCKPIT_PRESSURE)		//разгерметизация кабины. Выходи из зоны.
			REGISTER_WMESSAGE(wMsgALMAZ_NO_HYDRO_PRESSURE)			//Нет давления наддува гидросистемы
			REGISTER_WMESSAGE(wMsgALMAZ_FAILURE_AIRCOND)			//Отказ кондиционирования и вентиляции в кабине
			REGISTER_WMESSAGE(wMsgALMAZ_FAILURE_ROTOR_ANTIICE )		//Отказ ПОС винтов
			REGISTER_WMESSAGE(wMsgALMAZ_NO_MOV_GUN_STOP)			//Нет стопора ППУ
		REGISTER_WMESSAGE(wMsgALMAZ_Maximum)

		//Mi8 RI65 messages
		REGISTER_WMESSAGE(wMsgRI65_Null)
			REGISTER_WMESSAGE(wMsgRI65_IS_READY)						// Речевой информатор исправен
			REGISTER_WMESSAGE(wMsgRI65_LEFT_ENGINE_FIRE)				// Пожар в отсеке левого двигателя
			REGISTER_WMESSAGE(wMsgRI65_RIGHT_ENGINE_FIRE)				// Пожар в отсеке правого двигателя
			REGISTER_WMESSAGE(wMsgRI65_MAIN_TRANSMISSION_FIRE)			// Пожар в отсеке главного редуктора
			REGISTER_WMESSAGE(wMsgRI65_HEATER_FIRE)						// Пожар в отсеке обогревателя
			REGISTER_WMESSAGE(wMsgRI65_SWITCH_OFF_LEFT_ENGINE)			// Выключи левый двигатель
			REGISTER_WMESSAGE(wMsgRI65_SWITCH_OFF_RIGHT_ENGINE)			// Выключи правый двигатель
			REGISTER_WMESSAGE(wMsgRI65_LEFT_ENGINE_VIBRATION)			// Опасная вибрация левого двигателя
			REGISTER_WMESSAGE(wMsgRI65_RIGHT_ENGINE_VIBRATION)			// Опасная вибрация правого двигателя
			REGISTER_WMESSAGE(wMsgRI65_MAIN_HYDRO_FAILURE)				// Отказала основная гидросистема
			REGISTER_WMESSAGE(wMsgRI65_EMERGENCY_FUEL)					// Аварийный остаток топлива
			REGISTER_WMESSAGE(wMsgRI65_ICING)							// Обледенение
			REGISTER_WMESSAGE(wMsgRI65_TRANSMISSION_MALFUNCTION)		// Неисправность в редукторах
			REGISTER_WMESSAGE(wMsgRI65_GENERATOR1_FAILURE)				// Отказал первый генератор
			REGISTER_WMESSAGE(wMsgRI65_GENERATOR2_FAILURE)				// Отказал второй генератор
			REGISTER_WMESSAGE(wMsgRI65_PUMP_FEEDER_FUEL_TANK_FAILURE)	// Отказал насос расходного бака
			REGISTER_WMESSAGE(wMsgRI65_PUMPS_MAIN_FUEL_TANKS_FAILURE)	// Отказали насосы основных топливных баков
			REGISTER_WMESSAGE(wMsgRI65_BOARD)	// борт
			REGISTER_WMESSAGE(wMsgRI65_0_BEGIN)	// 0
			REGISTER_WMESSAGE(wMsgRI65_0_END)	// 0
			REGISTER_WMESSAGE(wMsgRI65_1_BEGIN)	// 1
			REGISTER_WMESSAGE(wMsgRI65_1_END)	// 1
			REGISTER_WMESSAGE(wMsgRI65_2_BEGIN)	// 2
			REGISTER_WMESSAGE(wMsgRI65_2_END)	// 2
			REGISTER_WMESSAGE(wMsgRI65_3_BEGIN)	// 3
			REGISTER_WMESSAGE(wMsgRI65_3_END)	// 3
			REGISTER_WMESSAGE(wMsgRI65_4_BEGIN)	// 4
			REGISTER_WMESSAGE(wMsgRI65_4_END)	// 4
			REGISTER_WMESSAGE(wMsgRI65_5_BEGIN)	// 5
			REGISTER_WMESSAGE(wMsgRI65_5_END)	// 5
			REGISTER_WMESSAGE(wMsgRI65_6_BEGIN)	// 6
			REGISTER_WMESSAGE(wMsgRI65_6_END)	// 6
			REGISTER_WMESSAGE(wMsgRI65_7_BEGIN)	// 7
			REGISTER_WMESSAGE(wMsgRI65_7_END)	// 7
			REGISTER_WMESSAGE(wMsgRI65_8_BEGIN)	// 8
			REGISTER_WMESSAGE(wMsgRI65_8_END)	// 8
			REGISTER_WMESSAGE(wMsgRI65_9_BEGIN)	// 9
			REGISTER_WMESSAGE(wMsgRI65_9_END)	// 9
		REGISTER_WMESSAGE(wMsgRI65_Maximum)

		//A-10 VMU messages
		REGISTER_WMESSAGE(wMsgA10_VMU_Null)
			REGISTER_WMESSAGE(wMsgA10_VMU_Alert)
			REGISTER_WMESSAGE(wMsgA10_VMU_Altitude)
			REGISTER_WMESSAGE(wMsgA10_VMU_WarningAutopilot)
			REGISTER_WMESSAGE(wMsgA10_VMU_Ceiling)
			REGISTER_WMESSAGE(wMsgA10_VMU_IFF)
			REGISTER_WMESSAGE(wMsgA10_VMU_Obstacle)
			REGISTER_WMESSAGE(wMsgA10_VMU_Pullup)
			REGISTER_WMESSAGE(wMsgA10_VMU_Speedbreak)
		REGISTER_WMESSAGE(wMsgA10_VMU_Maximum)
	
		REGISTER_WMESSAGE(wMsgBeacon)
		REGISTER_WMESSAGE(wMsgBroadcast)
		REGISTER_WMESSAGE(wMsgDatalinkDatagram)
		REGISTER_WMESSAGE(wMsgPlayerVoIP)

        // for controllable ground units
        REGISTER_WMESSAGE(wMsgGroundNull)
            REGISTER_WMESSAGE(wMsgGroundROE)
                REGISTER_WMESSAGE(wMsgGroundROEFire)
                REGISTER_WMESSAGE(wMsgGroundROEReturnFire)
                REGISTER_WMESSAGE(wMsgGroundROEHoldFire)
            REGISTER_WMESSAGE(wMsgGroundAlarmState)
                REGISTER_WMESSAGE(wMsgGroundAlarmStateAuto)
                REGISTER_WMESSAGE(wMsgGroundAlarmStateRed)
                REGISTER_WMESSAGE(wMsgGroundAlarmStateGreen)
        REGISTER_WMESSAGE(wMsgGroundMaximum)

		// for controllable navy units
		REGISTER_WMESSAGE(wMsgNavyNull)
			REGISTER_WMESSAGE(wMsgLeaderStop)
			REGISTER_WMESSAGE(wMsgLeaderMoving)
		REGISTER_WMESSAGE(wMsgNavyMaximum)

	REGISTER_WMESSAGE(wMsgMaximum)
        
        // for embrking


END_WMESSAGES