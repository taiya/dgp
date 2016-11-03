#include "Remesher.h"
#include <OpenGP/MLogger.h>

Remesher::Remesher(SurfaceMesh& m) : mesh(m) {
    points = mesh.vertex_property<Point>("v:point");
    vnormals = mesh.vertex_property<Normal>("v:normal");
}

void Remesher::remesh(Scalar edge_length) {
    target_edge_length = edge_length;

    // Perform one iteration of remeshing.
    split_long_edges();
    collapse_short_edges();
    flip_edges();
    tangential_smoothing();
}

void Remesher::split_long_edges() {
    /**
     *  \todo Split edges that are too long.
     *  \li Loop several times over all edges.
     *  \li Determine whether the edge is too long using is_to_long().
     *  \li If an edge is too long, split it (see SurfaceMesh::split(edge, vertex),
     *  set the position of the new vertex to the edge midpoint, and set its normal
     *  to the average of the endpoint normals.
     */
}

void Remesher::collapse_short_edges() {
    /**
     *  \todo Collapse edges that are too short.
     *  \li Loop several times over all edges.
     *  \li Determine if the edge is too short using is_too_short().
     *  \li If the edge is to short, collapse it (see SurfaceMesh::collapse(halfedge).
     *  \li Get rid of any extraenous edges (see SurfaceMesh::garbage_collection()).
     */
}

void Remesher::flip_edges() {
    /**
     *  \todo Flip edges to improve vertex valences.
     *  \li Loop several times over all edges (only consider non-boundary edges)
     *  \li Collect the four vertices of the two incident triangles.
     *  \li Determine the optimal valences for these vertices (boundary or non-boundary vertex?)
     *  \li If an edge flip improves the valences, then we flip it.
     *  \li Compare the sum of squared deviations from the optimal values.
     */
}

void Remesher::tangential_smoothing() {
    /**
     *  \todo Tangential smoothing to improve vertex distribution.
     *  \li Loop 10 times over all vertices, only consider non-boundary vertices.
     *  \li Compute uniform laplacian: vector from centre vertex to barycenter of its one-ring neighbours.
     *  \li Project this vector onto the tangent plane by subtracting its component in normal direction.
     */
}
