#ifndef __ANGLE_LIMITS__
#define __ANGLE_LIMITS__

struct angles_node{
    double left,right,top,bottom;
};

class AngleLimits {
public:
    AngleLimits();
    ~AngleLimits();

    void                load(Lua::Config& c, bool new_cinematic, bool navy);
    bool                is_in_limits(double angleZ_)const;
    bool                is_in_limits(double angleY_,double angleZ_, int* l_r = NULL, int* u_d = NULL) const;
    void                putIntoLimits(const double angleYPrev, const double angleZPrev, double& angleYNext, double& angleZNext, bool forSmoothTransition = true);
    void                putIntoLimits_arg(const double angleYPrev, const double angleZPrev, double& angleYNext, double& angleZNext);
    double              get_bestY(double angleY_, double angleZ_)const;

    const angles_node&  getLimitsSector(unsigned int id) const;
    const angles_node&  getLimitsSector_arg(unsigned int id) const;
    void                getMaxLeftMaxRight(double& maxL, double& maxR) const;
    void                getMaxBottomMaxTop(double& maxB, double& maxT) const;
    double              angleY_to_arg_old(double angle_) const;
    double              arg_to_angleY_old(double arg_) const;
    double              angleZ_to_arg_old(double angle_, bool navy) const;
    double              arg_to_angleZ_old(double arg_, bool navy) const;
    
    const size_t inline sectorsNum() const {return angles.size();};
    const bool inline   isFullCircle() const {return fullCircle_;};

private:
    ed::vector<angles_node> angles;
    ed::vector<angles_node> angles_arg;

    double              maxLeft_, maxRight_, maxBottom_, maxTop_;
    bool                fullCircle_;
    double              maxLeftArg_, maxRightArg_, maxBottomArg_, maxTopArg_;
    bool                new_cinematic_;

    int                 backupSectorIndex;
};

#endif
