#pragma once
#include <map>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

#include "Grid.h"

//=============================================================================
namespace OpenGP {
//=============================================================================

/// TODO remove me
void marchingCubes(Grid const& grid, OpenGP::SurfaceMesh& mesh, OpenGP::Scalar isoval = 0);

class MarchingCubes{
public:
    typedef Eigen::Matrix<unsigned int, 3, 1> Vector3u;
private:
    Grid const& mGrid;
    OpenGP::SurfaceMesh& mMesh;
    OpenGP::Scalar mIsoVal;
    std::map<unsigned long int, OpenGP::SurfaceMesh::Vertex> mEdge2Vertex;
    static int edgeTable[256];
    static int triTable[256][17];
    
public:
    static void exec(const Grid& grid, OpenGP::SurfaceMesh& mesh, OpenGP::Scalar isoval = 0);
    MarchingCubes(const Grid& grid, OpenGP::SurfaceMesh& mesh, OpenGP::Scalar isoval = 0);

private:
    void processCube(unsigned int x, unsigned int y, unsigned int z);
    OpenGP::SurfaceMesh::Vertex addVertex(Vector3u const& p0, Vector3u const& p1);
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
