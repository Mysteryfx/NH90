#ifndef Clouds_h__
#define Clouds_h__

#include <ed/vector.h>

//#include "FMMathematics/math.h"
#include "Randomizer/RandomObject.h"
#include "ed/map.h"
#include "osg/Vec2i"
#include "osg/Vec3d"

/*
 Облака вместе с механизмом генерации.

 Облака рассортированы по квадратам со стороной в 20 км. Такой квадрат соответствует "земельному" первого уровня.
Облака хранятся в квадрате в ed::map<short, Cloud>. Каждое облако имеет уникальный идентификатор, состоящий из
индексов квадрата и номера облака внутри квадрата. Этот индентификатор используется для стыковки с графической частью.

Механизм создания при старте:
 При создании квадрата в нем сразу генерятся облака, поэтому он должен знать свое положене и бальность облачности.
Поэтому конструктор по умолчанию отсутствует. Квадрат рубится на ячейки по 5 км, в центре каждой из которых может
быть облако. Итого не более 16 облаков. В зависимости от бальности, изменяется вероятность появления облака.
 Если генератор случайных чисел показывает, что облаку в данной позиции быть, оно создается, при этом, во избежание
квадратно-гнездового вида позиция немножко рандомизируется. Среднеквадратическое отклонение для каждого из уровней
бальности задано в Cloud::densityToPositionStdDev_.
 Также случайно, но с учетом бальности подбирается размер облака и интесивность осадков из него. Максимальный радиус
облака чуть более трех км, при десятибальной облачности.

Механизм динамического зарождения и ликвидации:
 На очередном шаге обновления квадат проверяет количество облаков. Если их больше, чем надо, выбиратеся случайное облако
и квадрат пытается его убить. Облако за 5 минут уменьшается до радиуса = 0. Если облако уже умирающее - откладываем
до следующего шага обновления. Если облаков меньше, чем надо, рождается облако с нулевым радиусом, которое за 5 минут
вырастает до нормального размера.

Механизм обновления:
 Атмосфера обновляет по одному квадрату каждые 0,02 секунды. Это дает обновление каждого квадрата примерно раз 
в две-три минуты, в зависимости от количества. Поскольку за это время нветер не изменяется, то если кому-то
(графике, прозрачности, турбулентности) понадобится актуальная позиция облака, она будет вычилсяться каждый раз.
Размазанное обновление сделано для уменьшения нагрузки, незапоминание новой позиции облака после внешнего запроса - 
для сетевой совместимости.



*/ 

class viCloudObject;

namespace Weather
{

//! Clouds are stored in squares 20*20km, in ed::map<short, Cloud>
//! Cloud has a unique identifier which is used for interaction with graphic.
struct CloudId 
{
    short sqX;
    short sqZ;
    unsigned short num;

    CloudId(short x=0, short z=0, unsigned short n=0) : sqX(x),
                sqZ(z),
                num(n) {}
};


enum CLOUD_PROCESS
{
    CLOUD_PROCESS_NONE,
    CLOUD_PROCESS_GROWING,
    CLOUD_PROCESS_DYING,
};


#define MAX_CLOUDS_DENSITY 10.0
#define PRECIPITATION_DENSITY 5.0
const double processTime = 300; // Процесс длится 5 минут, 300 секунд
const double processSpeed = 1.0 / processTime;


//! Class reprasenting "physical" cloud.
class Cloud
{
public:
    Cloud();
    Cloud(const osg::Vec3d& position, int cloudsDensity, CloudId id, bool justBorn);

    //! \name General data fuctions
    //! \{

    //! Position, xyz, [m]
    const osg::Vec3d& getPosition() const;

    //! Radius, [m]. Actual for horizontal plane, thickhess is less
    float getRadius() const;

    //! Precipitation power, 0 to 1
    float getPrecipitationPower() const;

    //! Shape number. Float, 0 to 1. Shoud be multiplied by shapes count.
    float getShapeNumber() const;
    //! \}

    //! \name Identifier
    //! \{
    const CloudId& getId() const;
    void changeId(const CloudId& newId);
    //! \}

    //! \name Interaction with graphics
    //! \{

    //! Generate new visual cloud
    void visualize();

    //! Delete visual cloud
    void unvisualize();

    //! Callback for generated visual cloud
    void setVisualCloud(viCloudObject* cloud);

    //! Callback for generated visual cloud
    void unsetVisualCloud();

    const unsigned long getVisualCloudId() const;

    bool visualized(){return visualCloud != NULL;}

    void updateVisual(double dTime, const osg::Vec3d& wind, const osg::Vec3d& squarePos);
    //! \}

    //! \name Update
    //! \{
    void update(double dTime, const osg::Vec3d& wind, const osg::Vec3d& squarePos, bool& removeMe);

    void die();
    bool isDying()
    {
        return CLOUD_PROCESS_DYING == process;
    }
    bool isGrowing()
    {
        return CLOUD_PROCESS_GROWING == process;
    }
    //! \}

private:
    //! Radius coefficient. 0 to 1 when cloud is growing, 1 to 0 when dying.
    //! Radius itself doesn't change but just multiplies by coefficient.
    double calcRadiusCoefficient(const osg::Vec3d& cloudPos, const osg::Vec3d& squarePos);

private:
    //! \name General data
    //! \{
    CloudId id;
    osg::Vec3d position;
    float radius;
    float radiusCoefficient;
    float precipitationPower;
    float shape; 
    //! }
    
    //! \name Update
    //! \{
    CLOUD_PROCESS process;
    double processBeginTime;
    //! \}

    static const double densityToPositionStdDev[ int(MAX_CLOUDS_DENSITY+1) ];

    viCloudObject* visualCloud;

    static RandomObject		rndUniform;
    static RandomObject		rndNormal;
};


typedef ed::map<unsigned short, Cloud> CloudMap;
typedef ed::map<unsigned short, Cloud>::iterator CloudIterator;


//! \name class representing 20*20 km square
class CloudSquare
{
public:
    CloudSquare(const osg::Vec2i& index);
    ~CloudSquare();

    CloudSquare& operator = (const CloudSquare& cs);
    
    int getCloudsDensity()
    {
        return cloudsDensity;
    }

    double getCeiling()
    {
        return ceiling;
    }

    double getUpperEdge()
    {
        return upperEdge;
    }

    ed::map<unsigned short, Cloud>& getClouds()
    {
        return clouds;
    }

    Cloud* getCloudByVisualId(unsigned long id);

    void update();

    //! \name Interaction with graphic
    //! \{
    void updateVisual();
    void visualize();
    void unvisualize();
    //! \}

private:
    unsigned short findFreeCloudNumber();
    osg::Vec3d findFreeSpace();

    void addCloud(Cloud cloud);
    CloudIterator removeCloud(const CloudIterator& it);

    CloudIterator checkCloudToSquareMapping(CloudIterator& it);
    void controlCloudsQuantity(osg::Vec3d& sqPos);
    
private:
    osg::Vec2i index;
    int cloudsDensity;
    CloudMap clouds;
    double ceiling;
    double upperEdge;

    int nDyingClouds;

    double lastUpdateTime; // для движущихся облаков надо, потому как обновляют они размазанно по времени

    bool visualized;

    static RandomObject rndUniform;
};

}
#endif // Clouds_h__