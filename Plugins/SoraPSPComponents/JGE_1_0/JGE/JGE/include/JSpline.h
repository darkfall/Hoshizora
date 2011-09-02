//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _JSPLINE_H
#define _JSPLINE_H

#include "JRenderer.h"
#include <vector>

using namespace std;

#define MID_POINT_THRESHOLD			1.0f


//////////////////////////////////////////////////////////////////////////
/// Position of a single dot on screen.
/// 
//////////////////////////////////////////////////////////////////////////
class Point
{
public:

	//////////////////////////////////////////////////////////////////////////
	/// Constructor.
	/// 
	/// @param _x - X position.
	/// @param _y - Y position.
	/// 
	//////////////////////////////////////////////////////////////////////////
	Point(float _x, float _y) { x = _x; y = _y; }

	//////////////////////////////////////////////////////////////////////////
	/// Constructor, set position to default (0.0f, 0.0f)
	/// 
	//////////////////////////////////////////////////////////////////////////
	Point() { x = 0.0f; y = 0.0f; }

	float x;	///< X position.
	float y;	///< Y position.
};


//////////////////////////////////////////////////////////////////////////
/// Catmull Rom spline.
/// 
//////////////////////////////////////////////////////////////////////////
class JSpline
{

public:

	//////////////////////////////////////////////////////////////////////////
	/// Constructor.
	///
	///
	//////////////////////////////////////////////////////////////////////////
	JSpline();
	~JSpline();

	//////////////////////////////////////////////////////////////////////////
	/// Load spline from a file.
	/// 
	/// Here is a sample spline definition file:
	///
	/// @code
	/// <?xml version="1.0" standalone="no" ?>
	/// <path>
    /// <contro_point x="89" y="270" />
    /// <contro_point x="113" y="154" />
    /// <contro_point x="227" y="94" />
    /// <contro_point x="347" y="154" />
    /// <contro_point x="367" y="278" />
	/// </path>
	/// @endcode
	///
	/// @param filename - Name of spline definition file.
	/// @param xscale - Scaling factor for X of all control points.
	/// @param yscale - Scaling factor for Y of all control points.
	/// 
	/// @return True if loaded.
	/// 
	//////////////////////////////////////////////////////////////////////////
	bool Load(const char *filename, float xscale=1.0f, float yscale=1.0f);

	//////////////////////////////////////////////////////////////////////////
	/// Add a control point to the spline.
	/// 
	/// @param pt - Control point.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void AddControlPoint(const Point &pt);

	//////////////////////////////////////////////////////////////////////////
	/// Get a control point of the spline.
	/// 
	/// @param index - Control point index.
	/// 
	/// @return Control point.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void GetControlPoint(Point &point, int index);

	//////////////////////////////////////////////////////////////////////////
	/// Work out all pixels of the spline.
	/// 
	/// @note Have to call this function before calling GetPixel, GetPixelCount 
	///			and Render.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void GeneratePixels();

	//////////////////////////////////////////////////////////////////////////
	/// Get a point between 2nd and 3rd control point.
	/// 
	/// @param t - Fraction of the curve between 2nd and 3rd control point. (0.0f ~ 1.0f)
	/// @param p0 - 1st control point.
	/// @param p1 - 2nd control point.
	/// @param p2 - 3rd control point.
	/// @param p3 - 4th control point.
	/// 
	/// @return Position of the desire point.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void PointOnCurve(Point &out, float t, const Point &p0, const Point &p1, const Point &p2, const Point &p3);

	//////////////////////////////////////////////////////////////////////////
	/// Get a number of pixels for this spline.
	/// 
	/// @return Number of pixels for this spline.
	/// 
	//////////////////////////////////////////////////////////////////////////
	int GetPixelCount();

	//////////////////////////////////////////////////////////////////////////
	/// Get a pixel on the spline.
	/// 
	/// @param index - Pixel index.
	/// 
	/// @return Position of the desire point.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void GetPixel(Point &point, int index);

	//////////////////////////////////////////////////////////////////////////
	/// Render the spline to screen.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void Render(float x, float y, PIXEL_TYPE color=ARGB(255,255,255,255), PIXEL_TYPE controlColor=ARGB(192,0,192,0));

private:

	vector<Point> mMidPoints;
	vector<Point> mPixels;
	int mCount;

};


#endif

