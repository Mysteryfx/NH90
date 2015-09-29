#ifndef OPENSTEER_SIMPLEVEHICLE_H
#define OPENSTEER_SIMPLEVEHICLE_H

#include "LocalSpace.h"
#include "Tools.h"


// SimpleVehicle adds concrete vehicle methods to SimpleVehicle_3
class SimpleVehicle : public LocalSpace2d
{
public:

    // constructor
    SimpleVehicle ();

    // destructor
    ~SimpleVehicle ();

    // reset vehicle state
    void reset (void)
    {
        // reset LocalSpace state
        resetLocalSpace ();

        setMass (6000.);          // mass (defaults to 1 so acceleration=force)
        setSpeed (0);         // speed along Forward direction.

        setRadius (5.);     // size of bounding sphere
		setTurnR(10.5);

        setMaxPower(5520.);   // steering force is clipped to this magnitude
		setMaxBreakingForce(16500.);
        setMaxSpeed (20.83);   // velocity is clipped to this magnitude
        setTopSpeedHandicap(1.0);

		setWidth(2.5);
		setLength(7.6);

        resetSmoothedCurvature ();
        resetSmoothedAcceleration ();
    }

    // get/set mass
    double mass (void) const {return _mass;}
    double setMass (double m) {return _mass = m;}

	double width() const {return _width;}
	double setWidth(double w);

	double length() const {return _length;}
	double setLength(double l);

	double height() const {return _height;};
	void setHeight(double h) {_height = h;};

    // get velocity of vehicle
    osg::Vec2d velocity (void) const {return forward() * _speed;}

    // get/set speed of vehicle  (may be faster than taking mag of velocity)
    double speed (void) const {return _speed;}
    double setSpeed (double s) {return _speed = s;}

    // size of bounding sphere, for obstacle avoidance, etc.
    double radius (void) const {return _radius;}
    double setRadius (double m) {return _radius = m;}

    double maxForce() const;
	double maxPower() const {return _maxPower;};
	double setMaxPower(double mpow) {return _maxPower = mpow;}
    
	double smoothBreakingForce() const;
	double maxBreakingForce(void) const {return _maxBreakingForce;}
	double setMaxBreakingForce(double mbf) {return _maxBreakingForce = mbf;}

    // get/set maxSpeed
    inline double maxSpeed (void) const {return _maxSpeed;}
    double setMaxSpeed(double ms) { return _maxSpeed = ms > _handicappedSpeed ? _handicappedSpeed : ms; }
    double setTopSpeed (double ms) {return _maxSpeed = _topSpeed = ms;}
    double setTopSpeedHandicap (float k);

	// Get/set gear count. Need for AI sounds
	double getGearCount() const { return _gearCount; }
	double setGearCount(int gearCount) { return _gearCount = gearCount; }
	// Get/set gear count. Need for AI sounds

	double turnR() const {return _turnR;}
	void setTurnR(double r) {_turnR = r;}


	double maxPossibleSpeed(double turnR) const;

	double targetSpeedForArrive(const osg::Vec2d& pos, double tolerance) const;
	double targetSpeedForArrive(double dist) const;

	double criticalBreakingDist() const;
	double maxBreakingDist() const;

	virtual double desiredSpeed() const {return speed();}

    // the default version: keep FORWARD parallel to velocity, change
    // UP as little as possible.
    virtual void regenerateLocalSpace (const osg::Vec2d& newVelocity,
                                        const double elapsedTime);

    // adjust the steering force passed to applySteeringForce.
    // allows a specific vehicle class to redefine this adjustment.
    // default is to disallow backward-facing steering at low speed.
    // xxx experimental 8-20-02
    virtual osg::Vec2d adjustRawSteeringForce (const osg::Vec2d& force,
                                            const double deltaTime);

	virtual osg::Vec2d adjustSteeringForMinimumTurningRadius(const osg::Vec2d& steering);

    // predict position of this vehicle at some time in the future
    // (assumes velocity remains constant)
    virtual osg::Vec2d predictFuturePosition (const double predictionTime) const;

    // get instantaneous curvature (since last update)
    double curvature (void) const {return _curvature;}

    // get/reset smoothedCurvature, smoothedAcceleration
    double smoothedCurvature (void) const {return _smoothedCurvature;}
    double resetSmoothedCurvature (double value = 0)
    {
        _lastForward = osg::zeroVD;
        _lastPosition = osg::zeroVD;
        return _smoothedCurvature = _curvature = value;
    }
    osg::Vec2d smoothedAcceleration (void) const {return _smoothedAcceleration;}
    osg::Vec2d resetSmoothedAcceleration (const osg::Vec2d& value = osg::zeroVD)
    {
        return _smoothedAcceleration = value;
    }

	osg::Vec2d leftTop() const;
	osg::Vec2d rightTop() const;
	osg::Vec2d leftBottom() const;
	osg::Vec2d rightBottom() const;

    // give each vehicle a unique number
    int serialNumber;
    static int serialNumberCounter;

	double nonZeroCurvature() const;
	double signedRadius() const; 

protected:
		// measure path curvature (1/turning-radius), maintain smoothed version
	void measurePathCurvature (const double elapsedTime);

private:

    double _mass;       // mass (defaults to unity so acceleration=force)

    double _radius;     // size of bounding sphere, for obstacle avoidance, etc.

	double _width;     
	double _length;     

	double _height;		//height above ground (y coord) 

    double _speed;      // speed along Forward direction.  Because local space
                        // is velocity-aligned, velocity = Forward * Speed

    double _maxPower;   // the maximum steering force = _maxPower/speed

	double _maxBreakingForce;

    double _maxSpeed;   // the maximum speed this vehicle is allowed to move 
                        // (velocity is clipped to this magnitude)
                        
    double _topSpeed; // speed limit by design
    double _handicappedSpeed; // damages slow vehicle down

	int _gearCount;		// Gear count for AI sound;

    double _curvature;
    osg::Vec2d _lastForward;
    osg::Vec2d _lastPosition;
    double _smoothedCurvature;
    osg::Vec2d _smoothedAcceleration;

	double _turnR;

	osg::Vec2d _leftTop, _rightTop, _leftBottom, _rightBottom;
	inline void updateHWD();
};

#include <ed/vector.h>
typedef ed::vector<SimpleVehicle*> AVGroup;

#endif // OPENSTEER_SIMPLEVEHICLE_H
