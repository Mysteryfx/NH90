
inline const StraightLine& StraightLine::operator ()(const Vector3& p1,const Vector3& p2)
{
	// По двум точкам.
	
	float x21 = p2.y - p1.y;
	float y21 = p2.z - p1.z;

	float check_value = 1.0f + x21;
	if (check_value == 1.0f)
		x21 = 0.0f;

	check_value = 1.0f + y21;
	if (check_value == 1.0f)
		y21 = 0.0f;

	A = -y21;
	B = x21;
	C = y21 * p1.y - x21 * p1.z;

	return (*this);
}

inline const StraightLine& StraightLine::operator ()(const Vector3& p1,const Vector3& p2,const Vector3& side)
{
	// По двум точкам.

	float x21 = p2.y - p1.y;
	float y21 = p2.z - p1.z;

	float check_value = 1.0f + x21;
	if (check_value == 1.0f)
		x21 = 0.0f;

	check_value = 1.0f + y21;
	if (check_value == 1.0f)
		y21 = 0.0f;

	A = -y21;
	B = x21;
	C = y21 * p1.y - x21 * p1.z;
	
	if (point_side(side) > 0.0f)
	{
		A = -A;
		B = -B;
		C = -C;
	}

	return (*this);
}

inline const StraightLine& StraightLine::operator ()(float A1, float B1, float C1)
{
	A = A1;
	B = B1;
	C = C1;

	return (*this);
}

inline StraightLine StraightLine::operator - () const
{
	return StraightLine(-(this->A), -(this->B), -(this->C));
}

inline double StraightLine::point_side(const Vector3& side) const
{
	double res = A * side.y + B * side.z + C;

	double check_value = 1.0 + res;
	if (check_value == 1.0)
		return 0.0;

	return res;
}

inline Vector3 StraightLine::intersection(const StraightLine& line) const
{
	const float& A2 = line.A;
	const float& B2 = line.B;
	const float& C2 = line.C;
	
	float x, y;
	
	if (A == 0.0f)
	{
		//assert(A2);
		//assert(B);
		y = -(C / B);
		x = (-C2 - B2 * y) / A2;
	}
	else if (B == 0.0f)
	{
		//assert(A);
		//assert(B2);
		x = -(C / A);
		y = (-C2 - A2 * x) / B2;
	}
	else if (A2 == 0.0f)
	{
		//assert(B2);
		//assert(A);
		y = -(C2 / B2);
		x = (-C - B * y) / A;
	}
	else if (B2 == 0.0f)
	{
		//assert(A2);
		//assert(B);

		x = -(C2 / A2);
		y = (-C - A * x) / B;
	}
	else
	{
		//assert(B2);
		//assert(B);
		x = ((B2 * C) / B - C2) / (A2 - (A * B2) / B);
		y = (-C2 - A2 * x) / B2;
	}

	return Vector3(0.0f, x, y);

	/*
	Vector point(double t) { return org + (dest - org) * (float)t; }
 	int intersect(Edge &e, double &t)
	{
		{ return dest - org; }
		Vector v = e.vector();
		Vector n = Vector(-v.y, v.x);
		double denom = n * vector();
		if (denom != 0.0)
		{
			t = - (n * (org - e.org)) / denom;	
		}
	}
	*/
}


inline double StraightLine::distance(const Vector3& side) const
{
	double norm = sqrt(A*A + B*B);
	if(norm == 0.0)
		return 0.0;

	double res = (A * side.y + B * side.z + C) / norm;

	double check_value = 1.0 + res;
	if (check_value == 1.0)
		return 0.0;

	return res;
}
