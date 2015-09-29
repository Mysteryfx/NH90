#ifndef OSG_SUBDIV_H
#define OSG_SUBDIV_H

#include "OSGMath.h"

namespace ed
{
template <class T> void subdivPolyline(const ed::vector<T>& in, ed::vector<T>& out, ed::vector<T>& outtangents, int subdivSteps, int startIndex=0, int segmentsCount=-1)
{
	if(&out!=&in)
		out = in;

	startIndex = std::max(0, startIndex);
	if (segmentsCount == -1)
	{
		segmentsCount = in.size() - 1;
	}
	if (startIndex > in.size()-1 || segmentsCount<1)
	{
		out.clear();
		return;
	}
	if (startIndex + segmentsCount >= in.size())
	{
		segmentsCount = in.size() - 1 - startIndex;
	}
	int endIndex = startIndex + segmentsCount;

	if(subdivSteps<1 )
	{
		for (int i = 0; i < segmentsCount+1; ++i)
			out[i] = out[i + startIndex];

		{
			outtangents.resize(segmentsCount+1);
			for (int i = 0; i < segmentsCount+1; ++i)
			{
				int v = i + startIndex;
				if(v==0)
				{
					outtangents[i] = in[v+1]-in[v];
					outtangents[i].normalize();
				}
				else if(v==in.size()-1)
				{
					outtangents[i] = in[v]-in[v-1];
					outtangents[i].normalize();
				}
				else
				{
					osg::Vec3d dir1 = in[v+1]-in[v];
					osg::Vec3d dir2 = in[v]-in[v-1];
					dir1.normalize();
					dir2.normalize();
					outtangents[i] = dir1+dir2;
					outtangents[i].normalize();
					outtangents[i] *= 1/(dir1*outtangents[i]);
				}
			}
		}

		out.resize(segmentsCount+1);
		return;
	}

	// Применение subdivision с маской 1 / 4 (1, 2, 1) заданное при помощи STEPS раз
	for (int j = 0; j < subdivSteps; j++)
	{
		ed::vector<T> newVerts;
		newVerts.reserve(2 * out.size() + 1);
		newVerts.push_back(out[0]);
		T middleLeft = (out[0] + out[1]) / 2;
		newVerts.push_back(middleLeft);
		int endVert = out.size() - 1;
		for (int i = 1; i < endVert; i++)
		{
			T middleRight = (out[i] + out[i + 1]) / 2;
			T newVert = (middleLeft + out[i] * 2 + middleRight) / 4;
			newVerts.push_back(newVert);
			newVerts.push_back(middleRight);
			middleLeft = middleRight;
		}
		newVerts.push_back(out[endVert]);

		out = newVerts;
	}

	startIndex *= (1 << subdivSteps);
	endIndex   *= (1 << subdivSteps);
	int splinePartSize = endIndex - startIndex + 1;

	{
		outtangents.resize(splinePartSize);
		for (int i = 0; i < splinePartSize; ++i)
		{
			int v = i + startIndex;
			if(v==0)
			{
				outtangents[i] = out[v+1]-out[v];
				outtangents[i].normalize();
			}
			else if(v==out.size()-1)
			{
				outtangents[i] = out[v]-out[v-1];
				outtangents[i].normalize();
			}
			else
			{
				osg::Vec3d dir1 = out[v+1]-out[v];
				osg::Vec3d dir2 = out[v]-out[v-1];
				dir1.normalize();
				dir2.normalize();
				outtangents[i] = dir1+dir2;
				outtangents[i].normalize();
				outtangents[i] *= 1/(dir1*outtangents[i]);
			}
		}
	}

	if (splinePartSize < out.size())
	{
		for (int i = 0; i < splinePartSize; ++i)
		{
			out[i] = out[i + startIndex];
		}
		out.resize(splinePartSize);
	}
}
// [&](int vertexIndex)->T
template <class T, typename Function> void subdivPolylineSegment(ed::vector<T>& out, ed::vector<T>& outtangents, int segment, int subdivSteps, int cvCount, const Function& f)
{
	int startIndex = std::max(0, segment);
	ed::vector<T> in(4);
	if( segment>0)
		in[0] = f(segment-1);
	else
		in[0] = f(segment)*2-f(segment+1);
	in[1] = f(segment);
	in[2] = f(segment+1);
	if( segment+2<cvCount)
		in[3] = f(segment+2);
	else
		in[3] = f(segment+1)*2-f(segment);

	ed::subdivPolyline(in, out, outtangents, subdivSteps, 1, 1);
}

template <class T> void subdivPolyline(const ed::vector<T>& in, ed::vector<T>& out, int subdivSteps, int startIndex=0, int segmentsCount=-1)
{
	if(&out!=&in)
		out = in;

	startIndex = std::max(0, startIndex);
	if (segmentsCount == -1)
	{
		segmentsCount = in.size() - 1;
	}
	if (startIndex > in.size()-1 || segmentsCount<1)
	{
		out.clear();
		return;
	}
	if (startIndex + segmentsCount >= in.size())
	{
		segmentsCount = in.size() - 1 - startIndex;
	}
	int endIndex = startIndex + segmentsCount;

	if(subdivSteps<1 )
	{
		for (int i = 0; i < segmentsCount+1; ++i)
		{
			out[i] = out[i + startIndex];
		}
		out.resize(segmentsCount+1);
		return;
	}

	// Применение subdivision с маской 1 / 4 (1, 2, 1) заданное при помощи STEPS раз
	for (int j = 0; j < subdivSteps; j++)
	{
		ed::vector<T> newVerts;
		newVerts.reserve(2 * out.size() + 1);
		newVerts.push_back(out[0]);
		T middleLeft = (out[0] + out[1]) / 2;
		newVerts.push_back(middleLeft);
		int endVert = out.size() - 1;
		for (int i = 1; i < endVert; i++)
		{
			T middleRight = (out[i] + out[i + 1]) / 2;
			T newVert = (middleLeft + out[i] * 2 + middleRight) / 4;
			newVerts.push_back(newVert);
			newVerts.push_back(middleRight);
			middleLeft = middleRight;
		}
		newVerts.push_back(out[endVert]);

		out = newVerts;
	}

	startIndex *= (1 << subdivSteps);
	endIndex   *= (1 << subdivSteps);
	int splinePartSize = endIndex - startIndex + 1;

	if (splinePartSize < out.size())
	{
		for (int i = 0; i < splinePartSize; ++i)
		{
			out[i] = out[i + startIndex];
		}
		out.resize(splinePartSize);
	}
}

// [&](int vertexIndex)->T
template <class T, typename Function> void subdivPolylineSegment(ed::vector<T>& out, int segment, int subdivSteps, int cvCount, const Function& f)
{
	int startIndex = std::max(0, segment);
	ed::vector<T> in(4);
	if( segment>0)
		in[0] = f(segment-1);
	else
		in[0] = f(segment)*2-f(segment+1);
	in[1] = f(segment);
	in[2] = f(segment+1);
	if( segment+2<cvCount)
		in[3] = f(segment+2);
	else
		in[3] = f(segment+1)*2-f(segment);

	ed::subdivPolyline(in, out, subdivSteps, 1, 1);
}

// [&](int vertexIndex)->osg::Vec3d
template <typename Function> void subdivPolylineSegment2(
	ed::vector<osg::Vec3d>& respoints, ed::vector<osg::Vec3d>& restangents, 
	int segment, int subdivSteps, int cvCount, const Function& f
	)
{
	int startIndex = std::max(0, segment);
	ed::vector<osg::Vec3d> in(4);
	if( segment>0)
		in[0] = f(segment-1);
	else
		in[0] = f(segment)*2-f(segment+1);
	in[1] = f(segment);
	in[2] = f(segment+1);
	if( segment+2<cvCount)
		in[3] = f(segment+2);
	else
		in[3] = f(segment+1)*2-f(segment);

	const osg::Matrixd splineMatrix = osg::Matrixd( 1.0f / 6.0f, -3.0f / 6.0f,  3.0f / 6.0f, -1.0f / 6.0f,
												4.0f / 6.0f,         0.0f, -6.0f / 6.0f,  3.0f / 6.0f,
												1.0f / 6.0f,  3.0f / 6.0f,  3.0f / 6.0f, -3.0f / 6.0f,
														0.0f,         0.0f,         0.0f,  1.0f / 6.0f );

	osg::Matrixd segmentMatrixes;
	osg::Matrixd pointsCoords;
	for (int i = 0; i < 4; i++)
	{
		pointsCoords(0, i) = in[i].x(); 
		pointsCoords(1, i) = in[i].y();
		pointsCoords(2, i) = in[i].z();
		pointsCoords(3, i) = 0.0f;
	}
	segmentMatrixes = pointsCoords * splineMatrix;

	int rescount = (1<<subdivSteps) + 1;
	for(int v=0; v<rescount; v++)
	{
		double param = v/(rescount-1.0);
		// Вычисляем координаты точки
		{
			double param2 = param * param;
			osg::Vec4d coords(1.0f, param, param2, param2 * param);
			coords = segmentMatrixes * coords;
			osg::Vec3d point(coords.x(), coords.y(), coords.z());
			respoints.push_back(point);
		}
		// Вычисляем тангент
		{
			osg::Vec4d ddcoords(0.0f, 1.0f, 2.0f*param, 3.0f * param * param);
			ddcoords = segmentMatrixes * ddcoords;

			osg::Vec3d tangent = ddcoords.xyz();
			tangent.normalize();
			restangents.push_back(tangent);
		}
	}
}

}

#endif /*UTILS_H_*/
