#pragma once
#include <OpenGP/types.h>
using namespace OpenGP;

/// This class stores a quadric as a symmetrix 4x4 matrix used by the error quadric mesh decimation algorithms.
class Quadric {
    typedef Vec3 Normal;
    typedef Vec3 Point;
public:
    /// Zero constructor
    Quadric() {}
    /// constructs the quadric from the point and normal specifying a plane
    Quadric(const Normal& n, const Point& p) {
        /// TASK: initialize the quadric given a normal and a point on a supporting plane
    }


    /// set all matric entries to zero
    void clear() {
        /// TASK: set the quadric to zero.
    }


    /// add two quadrics
    Quadric operator+( const Quadric& _q ) {
        /// TASK: implement quadric add
    }

    /// add given quadric to this quadric
    Quadric& operator+=( const Quadric& _q ) {
        /// TASK: implement quadric (self) add
    }

    // evaluate quadric Q at position p by computing (p^T * Q * p)
    double evaluate(const Point& p) const {
        /// TASK: evaluate the quadratic form at point p
    }

private:
    /// TASK: how to store the 4x4 symmetrix matrix?
};
