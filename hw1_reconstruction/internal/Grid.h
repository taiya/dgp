#pragma once

#include <vector>
#include <OpenGP/types.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class Grid{
public:
    typedef Eigen::Matrix<unsigned int, 3, 1> Vector3u;
private:
    Vec3 mOrigin;
    Vec3 mXAxis, mYAxis, mZAxis;
    Vec3 mDx, mDy, mDz;
    unsigned int mXRes, mYRes, mZRes;
    std::vector<Scalar> mValues;
    
public:
    Grid(const Box3& box, uint xres, uint yres, uint zres): 
        Grid(box.min(), 
             Vec3(box.max().x() - box.min().x(), 0 ,0),
             Vec3(0, box.max().y() - box.min().y(), 0),
             Vec3(0, 0, box.max().z() - box.min().z()),
             xres, yres, zres){}   
    
    Grid(const Vec3& origin = Vec3(0, 0, 0),
         const Vec3& xAxis = Vec3(1, 0, 0),
         const Vec3& yAxis = Vec3(0, 1, 0),
         const Vec3& zAxis = Vec3(0, 0, 1),
         unsigned int xRes = 10,
         unsigned int yRes = 10,
         unsigned int zRes = 10)
    {        
        this->mOrigin = origin;
        
        /// Discretization axis
        this->mXAxis = xAxis;
        this->mYAxis = yAxis;
        this->mZAxis = zAxis;
        
        /// Resolution
        this->mXRes = xRes;
        this->mYRes = yRes;
        this->mZRes = zRes;
        
        /// block differentials
        mDx = xAxis / (float)(xRes - 1);
        mDy = yAxis / (float)(yRes - 1);
        mDz = zAxis / (float)(zRes - 1);
        
        
        mValues.clear();
        mValues.resize(xRes * yRes * zRes, 0.0f);
    }

    Vec3 const& origin() const { return mOrigin; }
    Vec3 const& xAxis() const { return mXAxis; }
    Vec3 const& yAxis() const { return mYAxis; }
    Vec3 const& zAxis() const { return mZAxis; }

    unsigned int xResolution() const { return mXRes; }
    unsigned int yResolution() const { return mYRes; }
    unsigned int zResolution() const { return mZRes; }

    Vec3 point(unsigned int x, unsigned int y, unsigned int z) const{
        return mOrigin + mDx * x + mDy * y + mDz * z;
    }

    /// grid(x,y,z) = value;
    Scalar& operator()(unsigned int x, unsigned int y, unsigned int z){
        return mValues[z + y * mZRes + x * mZRes * mYRes];
    }

    /// float value = grid(x,y,z);
    Scalar operator()(unsigned int x, unsigned int y, unsigned int z) const{
        return mValues[z + y * mZRes + x * mZRes * mYRes];
    }

    Vec3 point(Vector3u const& xyz) const{
        return mOrigin + mDx * xyz[0] + mDy * xyz[1] + mDz * xyz[2];
    }
    
    Scalar& operator()(Vector3u const& xyz){
        return mValues[xyz[2] + xyz[1] * mZRes + xyz[0] * mZRes * mYRes];
    }

    Scalar operator()(Vector3u const& xyz) const{ 
        return mValues[xyz[2] + xyz[1] * mZRes + xyz[0] * mZRes * mYRes];
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

