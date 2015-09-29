// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
//
// LocalSpace: a local coordinate system for 3d space
//
// Provide functionality such as transforming from local space to global
// space and vice versa.  Also regenerates a valid space from a perturbed
// "forward vector" which is the basis of abnstract vehicle turning.
//
// These are comparable to a 4x4 homogeneous transformation matrix where the
// 3x3 (R) portion is constrained to be a pure rotation (no shear or scale).
// The rows of the 3x3 R matrix are the basis vectors of the space.  They are
// all constrained to be mutually perpendicular and of unit length.  The top
// ("x") row is called "side", the middle ("y") row is called "up" and the
// bottom ("z") row is called forward.  The translation vector is called
// "position".  Finally the "homogeneous column" is always [0 0 0 1].
//
//     [ R R R  0 ]      [ Sx Sy Sz  0 ]
//     [ R R R  0 ]      [ Ux Uy Uz  0 ]
//     [ R R R  0 ]  ->  [ Fx Fy Fz  0 ]
//     [          ]      [             ]
//     [ T T T  1 ]      [ Tx Ty Tz  1 ]
//
// This file defines three classes:
//   LocalSpace:  pure virtual interface
//   LocalSpaceMixin:     mixin to layer LocalSpace functionality on any base
//   LocalSpace:          a concrete object (can be instantiated)
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 06-05-02 cwr: created 
//
//
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_LOCALSPACE_H
#define OPENSTEER_LOCALSPACE_H


#include <osg\Vec2d>
#include "Tools.h"

// ----------------------------------------------------------------------------

class LocalSpace2d
{
    // transformation as three orthonormal unit basis vectors and the
    // origin of the local space.  These correspond to the "rows" of
    // a 3x4 transformation matrix with [0 0 0 1] as the final column

private:

    osg::Vec2d _side;     //    side-pointing unit basis vector
    osg::Vec2d _forward;  // forward-pointing unit basis vector
    osg::Vec2d _position; // origin of local space

public:

    // accessors (get and set) for side, up, forward and position
    osg::Vec2d side     (void) const {return _side;};
    osg::Vec2d forward  (void) const {return _forward;};
    osg::Vec2d position (void) const {return _position;};
    osg::Vec2d setSide     (const osg::Vec2d& s) {return _side = s;};
    osg::Vec2d setForward  (const osg::Vec2d& f) {return _forward = f;};
    osg::Vec2d setPosition (const osg::Vec2d& p) {return _position = p;};
    
	osg::Vec2d setSide     (double x, double z)
	{
		_side.set(x,z);
		return _side;
	}

	osg::Vec2d setForward(double x, double z)
	{
		_forward.set(x,z);
		return _forward;
	}

	osg::Vec2d setPosition(double x, double z)
	{
		_position.set(x,z);
		return _position;
	}

    // ------------------------------------------------------------------------
    // Global compile-time switch to control handedness/chirality: should
    // LocalSpace use a left- or right-handed coordinate system?  This can be
    // overloaded in derived types (e.g. vehicles) to change handedness.

    bool rightHanded (void) const {return true;}


    // ------------------------------------------------------------------------
    // constructors


    LocalSpace2d (void)
    {
        resetLocalSpace ();
    };

    LocalSpace2d (const osg::Vec2d& Side,
                        const osg::Vec2d& Forward,
                        const osg::Vec2d& Position)
    {
        _side = Side;
        _forward = Forward;
        _position = Position;
    };

    void resetLocalSpace (void)
    {
        _forward.set(0, 1);
        _side.set(-1, 0);
        _position.set(0, 0);
    };


    // ------------------------------------------------------------------------
    // transform a direction in global space to its equivalent in local space


	template<typename T>
    T localizeDirection (const T& globalDirection) const
    {
        // osg::dot offset with local basis vectors to obtain local coordiantes
        return T(globalDirection*_side, globalDirection*_forward);
    };


    // ------------------------------------------------------------------------
    // transform a point in global space to its equivalent in local space


    osg::Vec2d localizePosition (const osg::Vec2d& globalPosition) const
    {
        // global offset from local origin
        osg::Vec2d globalOffset = globalPosition - _position;

        // osg::dot offset with local basis vectors to obtain local coordiantes
        return localizeDirection (globalOffset);
    };


    // ------------------------------------------------------------------------
    // transform a point in local space to its equivalent in global space


    osg::Vec2d globalizePosition (const osg::Vec2d& localPosition) const
    {
        return _position + globalizeDirection (localPosition);
    };


    // ------------------------------------------------------------------------
    // transform a direction in local space to its equivalent in global space


	template<typename T>
    T globalizeDirection (const T& localDirection) const
    {
        return T((_side    * localDirection.x()) + (_forward * localDirection.y()));
    };


    // ------------------------------------------------------------------------
    // set "side" basis vector to normalized cross product of forward and up


    void setUnitSideFromForward(void)
    {
        // derive new unit side basis vector from forward and up
        if (rightHanded())
			_side.set(-_forward.y(), _forward.x());
        else
            _side.set(_forward.y(), _forward.x());
    }


    // ------------------------------------------------------------------------
    // regenerate the orthonormal basis vectors given a new forward
    // (which is expected to have unit length)


    void regenerateOrthonormalBasisF (const osg::Vec2d& newUnitForward)
    {
        _forward = newUnitForward;
		setUnitSideFromForward ();
    }


    // for when the new forward is NOT know to have unit length

    void regenerateOrthonormalBasis (const osg::Vec2d& newForward)
    {
        regenerateOrthonormalBasisF(osg::normalize(newForward));
    }
};

// ----------------------------------------------------------------------------
#endif // OPENSTEER_LOCALSPACE_H
