#pragma once
#include <OpenGP/types.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

class NanoflannAdapter;
class SurfaceMeshVerticesKDTree{
    NanoflannAdapter* _adapter = nullptr; ///< internal
    Mat3xN _data;
public:
    SurfaceMeshVerticesKDTree(SurfaceMesh& mesh);
    ~SurfaceMeshVerticesKDTree();
    /// Find the closest neighbor to "p"
    SurfaceMesh::Vertex closest_vertex(const Vec3& p) const;
    /// Finds the k nearest neighbors to "p"
    std::vector<SurfaceMesh::Vertex> kNN(const Vec3 &p, int N);
};

//=============================================================================
} // namespace OpenGP
//=============================================================================

