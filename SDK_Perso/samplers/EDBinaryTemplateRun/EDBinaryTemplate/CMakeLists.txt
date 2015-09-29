file(GLOB sources *.cpp)
file(GLOB headers *.h)

add_definitions(-DED_LOG_MODULE=\"EDBINARYTEMPLATE\" -DED_MODULE_NAME=\"EDBINARYTEMPLATE\" -DEDBINARYTEMPLATE_EXPORTS)

add_library(EDBinaryTemplate SHARED
	${sources}
	${headers}
)

TARGET_LINK_LIBRARIES(EDBinaryTemplate edCore edMath edObjects edTerrain Terrain World WorldGeneral Inter Space Flight FMBase GraphicsXP DXRenderer wRadio UserInterface Visualizer Input Transport Weapons WeaponsBase lua Weather CockpitBase)

