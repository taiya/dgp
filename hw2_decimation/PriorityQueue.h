#pragma once
#include <set>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

using namespace OpenGP;

/// @brief compare functor for priority queue
class VertexCompare{
    typedef SurfaceMesh::Vertex Vertex;
    typedef SurfaceMesh::Vertex_property<float> PerVertexFloat;
private:
    const PerVertexFloat& vprio_;
public:
    VertexCompare(const PerVertexFloat& _vprio) : vprio_(_vprio) {}
    
    /// is error introduced by removing v0 less than that of v1?
    bool operator()(Vertex v0, Vertex v1) const{
        float p0 = vprio_[v0];
        float p1 = vprio_[v1];
        // note: std::set needs UNIQUE keys -> handle equal priorities
        return ( (p0 == p1) ? (v0.idx() < v1.idx()) : (p0 < p1) );
    }
};

/// priority queue to fetch the next vertex removal operation (halfedge collapse)
class PriorityQueue {
    typedef SurfaceMesh::Vertex Vertex;
    typedef SurfaceMesh::Halfedge Halfedge;
    typedef std::set<SurfaceMesh::Vertex, VertexCompare> VertexQueue;
private:
    SurfaceMesh& mesh;
    VertexQueue _queue;
    SurfaceMesh::Vertex_property<Scalar> vcost;
    SurfaceMesh::Vertex_property<Halfedge> vtarget;

public:
    PriorityQueue(SurfaceMesh& mesh);
    ~PriorityQueue();

public:
    /// If the queue is empty, then we have nothign else to decimate
    bool is_empty(){ return _queue.empty(); }
    
    /// Insert the halfedge h to the collapse candidates queue
    /// If the "from" vertex already has a candidate, then update it.
    void insert_or_update(Halfedge h, Scalar h_cost);
    
    /// Pop the best collapse candidate from the queue
    Halfedge pop();
    
    /// Reset everything (useful for iterative executions)
    void clear();
};
