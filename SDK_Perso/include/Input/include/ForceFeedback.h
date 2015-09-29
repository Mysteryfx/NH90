#pragma once

#include "InputExports.h"

class ConditionEffect;
class ConstantForceEffect;
class PeriodicEffect;
class INPUT_API ForceFeedback
{
public:
	ForceFeedback();
	~ForceFeedback();

	// ConditionEffect routines
	void initConditionEffect(ConditionEffect* fx);
	bool hasConditionEffect() const;
	void setConditionEffectGain(double gain);
	double getConditionEffectGain() const;
	void setConditionEffect(double offsetX, double offsetY, double magnitudeX, double magnitudeY);
	void playConditionEffect();
	void stopConditionEffect();

	// ConstantForceEffect routines
	void initConstantForceEffect(ConstantForceEffect* fx);
	bool hasConstantForceEffect() const;
	void setConstantForceEffectGain(double gain);
	double getConstantForceEffectGain() const;
	void setConstantForceEffect(double directionX, double directionY);
	void playConstantForceEffect();
	void stopConstantForceEffect();

	// PeriodicEffect routines
	void initPeriodicEffect(PeriodicEffect* fx);
	bool hasPeriodicEffect() const;
	void setPeriodicEffectGain(double gain);
	double getPeriodicEffectGain() const;
	void setPeriodicEffect(double directionX, double directionY, double frequency);
	void playPeriodicEffect();
	void stopPeriodicEffect();

	void setSwapAxes(bool swapAxes);
	bool getSwapAxes() const { return swapAxes_; };

	void setInvertX(bool invert);
	bool getInvertX() const { return invertX_; };
	
	void setInvertY(bool invert);
	bool getInvertY() const { return invertY_; };

	void restore();

private:
	ConditionEffect* conditionEffect_;
	ConstantForceEffect* constantForceEffect_;
	PeriodicEffect* periodicEffect_;
	bool swapAxes_;
	bool invertX_;
	bool invertY_;

    friend class DIJoystick;

    bool immediate_;

    void applyUpdates();
    void setImmediate(bool imm)  { immediate_ = imm; }
    bool isImmediate() const { return immediate_; }

};
