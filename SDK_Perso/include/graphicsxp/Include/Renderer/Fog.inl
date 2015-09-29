void edcg::Fog::calcCoefs()
{
#ifdef EDGE
	static const double lg = -4.0;
#else
	// I got bug about fog distance, so in lockon I use old constant value.
	static const double lg = -3.0;
#endif
	static const double MAX_HEIGHT = 120000.0;
	static const double CAMERA_HEIGHT_FOR_MAX_DISTANCE = 10000.0;

	_coefs.x() = -lg / MAX_HEIGHT;
	_coefs.y() = (-lg / _distance) * exp(_coefs.x() * CAMERA_HEIGHT_FOR_MAX_DISTANCE);

	if(_layerHeight > 1.0e-6 && _layerDist > 1.0e-6){
		_coefs.z() = -lg / _layerHeight;
		_coefs.w() = -lg / _layerDist;
	}else{
		_coefs.z() = _coefs.w() = 0.0;
	}
}

double edcg::Fog::calcAtt(double a, double b, double camHeight, double dist, double cosEta)const
{
	double c = a * camHeight;
	double d = a * cosEta;
	double exp_c = exp(std::min(-c, 40.0));

	return std::abs(d) > 1.0e-6 ? exp(-b * (exp_c - exp(std::min(-d * dist - c, 40.0))) / d) : exp(-b * exp_c * dist);
}

edcg::Fog::Fog():_distance(5000.0),_layerHeight(0.0),_layerDist(0.0)
{
	calcCoefs();
}

edcg::Fog::~Fog()
{
}

void edcg::Fog::setDistance(double dist)
{
	_distance = dist;
	calcCoefs();
}

void edcg::Fog::setLayer(double height, double dist)
{
	_layerHeight = height;
	_layerDist = dist;
	calcCoefs();
}

void edcg::Fog::setLayerHeight(double height)
{
	_layerHeight = height;
	calcCoefs();
}

void edcg::Fog::setLayerDistance(double dist)
{
	_layerDist = dist;
	calcCoefs();
}

double edcg::Fog::getDistance()const{
	return _distance;
}

void edcg::Fog::getLayer(double& height, double& dist)const
{
	height = _layerHeight;
	dist = _layerDist;
}

void edcg::Fog::getLayer(float& height, float& dist)const
{
	height = (float)_layerHeight;
	dist = (float)_layerDist;
}

double edcg::Fog::getLayerHeight()const
{
	return _layerHeight;
}

const osg::Vec4f& edcg::Fog::getCoefficients()const
{
	return _coefs;
}

inline double edcg::Fog::calculateAttenuation(const osg::Vec3d &cameraPos, const osg::Vec3d &objPos)const
{
	osg::Vec3d dir(objPos - cameraPos);
	double dist = dir.length();
	dir.normalize();
	double cosEta = dir.y();

	double attD = calcAtt(_coefs.x(), _coefs.y(), cameraPos.y(), dist, cosEta);
	double attL = calcAtt(_coefs.z(), _coefs.w(), cameraPos.y(), dist, cosEta);

	return attD * attL;
}
