#pragma once 
#include <OpenGP/SurfaceMesh/Surfacemesh.h>
#include "Quadric.h"
#include "PriorityQueue.h"

/// @brief Halfedge mesh decimation with quadric error metrics
class Decimator{
    typedef SurfaceMesh::Halfedge Halfedge;
    typedef SurfaceMesh::Vertex   Vertex;
    typedef SurfaceMesh::Face     Face;

/// @{ data members
private:
    SurfaceMesh& mesh;                                 
    SurfaceMesh::Vertex_property<Vec3> vpoints;     
    SurfaceMesh::Vertex_property<Quadric> vquadrics;
    SurfaceMesh::Face_property<Vec3> fnormals;
    PriorityQueue queue; /// sort the halfedge collapses
    const float min_cos = std::cos(0.25*M_PI); ///< min angle (avoid face foldover)
/// @} 

/// @{ constructors
public:
    Decimator(SurfaceMesh& mesh) : mesh(mesh), queue(mesh){
        vpoints = mesh.vertex_property<Point>("v:point");
        fnormals = mesh.face_property<Normal>("f:normal");
        vquadrics = mesh.add_vertex_property<Quadric>("v:quadric");
    }        
    ~Decimator(){
        mesh.remove_vertex_property(vquadrics);        
        // now, delete the items that have been marked to be deleted
        // (SurfaceMesh::collapse marks several elements as such)
        mesh.garbage_collection();
    }
/// @}

/// @{ algorithm implementation
public:
    void init();
    void exec(unsigned int target_n_vertices);
private:
    bool  is_collapse_legal(Halfedge h);
    Scalar halfedge_collapse_cost(Halfedge h);
    void  enqueue_vertex(Vertex v);
/// @}
};
