#include "Decimator.h"
#include <OpenGP/MLogger.h>

/// is the collapse of halfedge h allowed? (check for manifold, foldovers, etc...)
bool Decimator::is_collapse_legal(Halfedge h){
    // tests for collapse candidate: v0 ---> v1
    Vertex v0 = mesh.from_vertex(h);
    Vertex v1 = mesh.to_vertex(h);

    /// TASK: check if h would cause a non-manifold change (bad!!!)
    /// hint: search the SurfaceMesh documentation
    
    /// TASK: what do you do if you have boundaries?
    /// note: this part is optional (use watertight input!)
    if( false /*.....*/ )
        return false;

    /// TEST: check for face foldovers
    bool causes_foldover = false;
    Point old_p0 = vpoints[v0]; ///< used to undo collapse simulation
    vpoints[v0] = vpoints[v1]; ///< simulates the collapse
    {
        /// TASK: Check that the (post-collapse) faces have cos(dihedral)<min_cos
        /// note: decimation would still run without this!
        /// hint: see SurfaceMesh::compute_face_normal()
    }
    vpoints[v0] = old_p0; ///< undo simulation
    if(causes_foldover) 
        return false;

    return true; ///< all tests passed!
}

/// what is the priority of collapsing the halfedge h?
Scalar Decimator::halfedge_collapse_cost(Halfedge h){
    /// TASK: Compute the priority (quadric error) for collapsing halfedge "h"  
    /// hint: See what the class Quadric provides
    /// hint: Get/set the quadric of a vertex v by calling quadrics[v]
    return nan();
}

/// Find smallest half-edge collapse for vertex and (potentially) add it to the queue
void Decimator::enqueue_vertex(Vertex v){
    Halfedge best_halfedge; ///< invalid!
    Scalar best_halfedge_cost = inf();
    
    /// TASK
    /// 1) find smallest error out-going halfedge collapse
    /// 2) add the best halfedge to the priority queue
}

/// Initialization
void Decimator::init(){
    // why? because face normals are needed to compute the initial quadrics
    mesh.update_face_normals();

    /// TASK: initialize (per-vertex) quadrics
    /// 1) quadric at vertex is the sum of the quadrics of its incident triangles
    /// hint: see Quadric.h and store the quadric in the vertex property vquadrics[]
    
    /// TASK: traverse all vertices and initialize the priority queue
    /// hint: Decimator::enqueue_vertex is to be used here
}

void Decimator::exec(unsigned int target_n_vertices){
    mLogger() << "Decimator::exec" << mesh.n_vertices() << target_n_vertices << "in progress...";

    while (mesh.n_vertices()>target_n_vertices && !queue.is_empty()){
        // performing collapse v0 ---> v1
        Halfedge h = queue.pop();        
        Vertex v0 = mesh.from_vertex(h);
        Vertex v1 = mesh.to_vertex(h);

        /// TASK: main execution logic
        /// 1) check if this collapse is legal
        /// 2) perform the halfedge collapse (see docs)
        /// 3) update the quadric of v1 
        /// 4) re-compute the collapse costs in neighborhood of v1
        
        // just some debug output (for long processes)
        if (mesh.n_vertices() % 1000 == 0)
            mLogger() << "#v:" << mesh.n_vertices();
    }
}
