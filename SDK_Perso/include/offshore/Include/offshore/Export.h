#ifndef EXPORT_FUNCTIONS_H
#define EXPORT_FUNCTIONS_H

#include "../offshore_config.h"
#include "landscape3/lMaterialAlias.h"
#include "landscape4/lGeometry.h"

#include "offshore/scene.h"

namespace export4
{

/**
 * Назначить метериал и семантику если не назначены
 * 
 * @param scene входная сцена
 * @param nodeIndex индекс ноды для которой необходимо выполнить проверку
 * @param defmaterial материал по умолчанию
 * @param defsemantic семантика по умолчанию
 */
OFFSHORE_API void applyDefaults(offshore::Scene& scene, int rootnode, const char* defmaterial, const char* defsemantic="");

/**
 * Переименовать uvset "1" -> "UV0"
 * 
 * @param scene входная сцена
 * @param nodeIndex индекс ноды для которой необходимо выполнить проверку
 */
OFFSHORE_API void applyOffshoreNamingConventions(offshore::Scene& scene, int rootnode);

/**
 * Проверить что на объект назначен материал, и вернуть material alias если он существует для этого материала
 * Если в материале не указан edmaterial заменит его на defmaterial
 * 
 * @param scene входная сцена
 * @param nodeIndex индекс ноды для которой необходимо выполнить проверку
 * @param matFile файл с material alias-ами
 * @param errors массив для передачи ошибок
 * @return поинтер на material alias для этого шейпа, NULL если что-то зафейлилось
 */
OFFSHORE_API material2::MaterialAlias* getMaterialAlias(const offshore::Scene& scene, int nodeIndex, material2::MaterialAliasFile& matFile, ed::vector<offshore::Notification>& notifications);
OFFSHORE_API material2::MaterialAlias* getMaterialAlias(const offshore::Scene& scene, int nodeIndex, material2::MaterialAliasFile& matFile, ed::vector<offshore::Error>& errors);
OFFSHORE_API material2::MaterialAlias* getMaterialAlias(const offshore::Material* material, material2::MaterialAliasFile& matFile, ed::vector<offshore::Notification>& notifications, const ed::string& nodename, const osg::Vec3d& errorPos=osg::Vec3d(0, 0, 0));

/**
 * Иерархию нод развернуть в список. В dstScene копируются только дети парента. Разворачивает инстансы.
 *
 * @param srcScene входная сцена
 * @param srcNodeIndex индекс парент-ноды входной сцены, для детей которой выполняем объединение
 * @param dstScene выходная сцена
 * @param dstNodeIndex индекс парент-ноды выходной сцены
 * @param rootTransform матрица на которую преумножаем чайлдов srcScene. Необходимо для разворачивания инстансов
 */
OFFSHORE_API void hierarchyToList(offshore::Scene& srcScene, int srcNodeIndex, offshore::Scene& dstScene, int dstNodeIndex, const osg::Matrixd& rootTransform = osg::Matrixd());

/**
 * Объединить одинаковые материалы; меши с одинаковыми параметрами объединить в один
 * В процессе назначит правильный материал для texturearray и положит соответствующий индекс в uvset "FRAGMENT"
 *
 * @param scene входная сцена
 * @param rootNodeIndex индекс ноды входной сцены, для детей которой выполняем объединение
 * @param dstScene выходная сцена
 * @param dstRootNode индекс ноды выходной сцены, куда добавляем результат, полученный в результате объединения
 * @param errors ошибки, полученные в процессе объединения
 */
OFFSHORE_API void mergeNodes(offshore::Scene& scene, int rootNodeIndex, offshore::Scene& dstScene, int dstRootNode, material2::MaterialAliasFile& matFile, ed::vector<offshore::Notification>& notifications);
OFFSHORE_API void mergeNodes(offshore::Scene& scene, int rootNodeIndex, offshore::Scene& dstScene, int dstRootNode, material2::MaterialAliasFile& matFile, ed::vector<offshore::Error>& errors);

/**
 * Подготовить шейп к экспорту. Прогнать preexport процедуры. Убрать лишние параметры и uv-сеты.
 *
 * @param scene входная сцена
 * @param nodeIndex индекс ноды шейпа в сцене
 * @param matAlias material alias для конверта
 * @param errors массив для передачи ошибок
 * @return true если все прошло успешно
 */
OFFSHORE_API bool prepareToExport(offshore::Scene& scene, int nodeIndex, material2::MaterialAlias* matAlias, ed::vector<offshore::Notification>& notifications);
OFFSHORE_API bool prepareToExport(offshore::Scene& scene, int nodeIndex, material2::MaterialAlias* matAlias, ed::vector<offshore::Error>& errors);

/**
 * Конвертация материала
 *
 * @param offshoreMaterial оффшорный материал
 * @param nodeParams параметры ноды шейпа чей материал конвертится
 * @param matAlias material alias для конверта
 * @param material сконверченный материал
 * @param errors массив для передачи ошибок
 * @return true если все прошло успешно
 */
OFFSHORE_API bool convertMaterial(offshore::Material& offshoreMaterial, ed::map<ed::string, ed::string>& nodeParams, material2::MaterialAlias* matAlias, material3::Material& material, ed::vector<offshore::Notification>& notifications);
OFFSHORE_API bool convertMaterial(offshore::Material& offshoreMaterial, ed::map<ed::string, ed::string>& nodeParams, material2::MaterialAlias* matAlias, material3::Material& material, ed::vector<offshore::Error>& errors);

/**
 * Разбить все полигоны меша на треугольники. Необходимо перед конвертацией в geometry4
 *
 * @param scene исходная оффшор сцена
 * @param nodeIndex индекс ноды меша для конвертации
 * @param errors массив для передачи ошибок
 * @return true если все прошло успешно
 */
OFFSHORE_API bool assembleUniqueVertices(offshore::Scene& scene, int nodeIndex, ed::vector<offshore::Notification>& notifications);
OFFSHORE_API bool assembleUniqueVertices(offshore::Scene& scene, int nodeIndex, ed::vector<offshore::Error>& errors);

/**
 * Сконвертить оффшорный шейп в геосорс4
 *
 * @param scene исходная оффшор сцена
 * @param nodeIndex индекс ноды меша для конвертации
 * @param matAlias material alias для конверта
 * @param geoSource сконверченный геосорс
 * @param errors массив для передачи ошибок
 * @return true если все прошло успешно
 */
OFFSHORE_API bool convertToGeometry4(offshore::Scene& scene, int nodeIndex, material2::MaterialAlias* matAlias, geometry4::GeometrySource& geoSource, ed::vector<offshore::Notification>& notifications);
OFFSHORE_API bool convertToGeometry4(offshore::Scene& scene, int nodeIndex, material2::MaterialAlias* matAlias, geometry4::GeometrySource& geoSource, ed::vector<offshore::Error>& errors);

/**
 * Для отладки. Сконвертить геосорс4 обратно в оффшорный меш
 * 
 * @param geoSource входной геосорс для дампа
 * @param mesh выходной оффшорный меш
 * @return true если все прошло успешно
 */
OFFSHORE_API bool dumpGeometry4ToOffshore(geometry4::GeometrySource& geoSource, offshore::Mesh& mesh);

}
#endif
