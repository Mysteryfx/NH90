
inline bool CheckBoxLayout(StraightLine *line,Vector3 *array4x1)
{
	int ret = 0;
	for (int i = 0;i < 4 ;i++)
	{
		if (line->point_side(array4x1[i]) > 0)
		{
			ret++;
		}
	}
	return (ret != 4);
}

inline void checkEpsilon(float &f)
{
	if((float)(1.0f + f) == 1.0f)
	{
		f = 0.0f; 
	}
}

inline void checkEpsilon_d(double &f)
{
	if((float)(1.0f + f) == 1.0f)
	{
		f = 0.0; 
	}
}

inline bool CheckEmptyLine(Vector3 &v1,Vector3 &v2)
{
	return (v1.y == v2.y) && (v1.z == v2.z);
}