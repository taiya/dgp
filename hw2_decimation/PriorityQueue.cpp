#include "PriorityQueue.h"

PriorityQueue::PriorityQueue(SurfaceMesh &mesh) : mesh(mesh), _queue(vcost){
    vcost = mesh.add_vertex_property<Scalar>("v:cost");
    vtarget = mesh.add_vertex_property<Halfedge>("v:target");
}

PriorityQueue::~PriorityQueue(){
    mesh.remove_vertex_property(vtarget);
    mesh.remove_vertex_property(vcost);
}

void PriorityQueue::insert_or_update(PriorityQueue::Halfedge h, Scalar h_cost){
    if(!h.is_valid()) return;
    Vertex v = mesh.from_vertex(h);
    
    // if (!=-1) then it's already in queue, so needs updating
    // but std::set doesn't support update, thus we remove+insert
    if(vcost[v] != -1.0){
        _queue.erase(v);
        vcost[v] = -1.0;
    }
    
    // Couldn't find a feasible candidate?
    // ... then the vertex is left alone
    if(!h.is_valid()) return;
    
    // Good candidate exists, let's insert it in queue        
    vtarget[v] = h;
    vcost[v] = h_cost;
    _queue.insert(v);
}

PriorityQueue::Halfedge PriorityQueue::pop(){
    Vertex v = *_queue.begin(); //< get 1st element
    _queue.erase(_queue.begin());  //< then erase it
    return vtarget[v];
}

void PriorityQueue::clear(){ 
    // initialize vertex costs
    for(auto v: mesh.vertices())
        vcost[v] = -1.0;
    
    // initialize priority queue
    _queue.clear(); 
}
