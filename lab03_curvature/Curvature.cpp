#include "Curvature.h"

Curvature::Curvature(OpenGP::SurfaceMesh& mesh) : mesh(mesh) {
    vpoint = mesh.vertex_property<OpenGP::Point>("v:point");
    vquality = mesh.vertex_property<float>("v:quality");
    varea = mesh.add_vertex_property<OpenGP::Scalar>("v:area");
    ecotan = mesh.add_edge_property<OpenGP::Scalar>("e:cotan");
    vcurvature_K = mesh.add_vertex_property<OpenGP::Scalar>("v:curvature_K");
    vcurvature_H = mesh.add_vertex_property<OpenGP::Scalar>("v:curvature_H");
    vcurvature_k1 = mesh.add_vertex_property<OpenGP::Scalar>("v:curvature_k1");
    vcurvature_k2 = mesh.add_vertex_property<OpenGP::Scalar>("v:curvature_k2");
}

Curvature::~Curvature() {
    mesh.remove_vertex_property(varea);
    mesh.remove_edge_property(ecotan);
    mesh.remove_vertex_property(vcurvature_K);
    mesh.remove_vertex_property(vcurvature_H);
    mesh.remove_vertex_property(vcurvature_k1);
    mesh.remove_vertex_property(vcurvature_k2);
}

void Curvature::visualize_gauss_curvature() {
    using namespace OpenGP;
    /**
     *  \todo Compute cotangent weights per edge.
     *  \li for each edge, find the incident triangle(s) (it can be a boundary edge!)
     *  \li for each incident triangle, collect the three points, compute the angle
     *  opposite of the edge, and add its cotan ot he weight of this edge.
     */
    for (auto const& edge : mesh.edges())
        ecotan[edge] = 1.0;

    create_colours(vcurvature_K);
    gaussComputed = true;
}

void Curvature::visualize_mean_curvature() {
    using namespace OpenGP;

    if (!gaussComputed)
        return;

    /**
     *  \todo Compute the Voronoi area for each vertex.
     *  \li Loop over each vertex and set its area to zero.
     *  \li Loop over each triangle and add one third of its area to each of its vertices.
     */
    for (auto const& vertex : mesh.vertices())
        varea[vertex] = 1.0;

    /**
     *  \todo Compute the mean meanCature for each vertex.
     *  \li For each vertex, loop over its one-ring neighbours with a Halfedge_around_vertex_circulator.
     *  \li Compute the Laplace by summing the vector from the centre vertex to its neighbour
     *  weighted by the cotan edge weight.
     *  \li Compute the mean meanCature from the Laplace vector and store it in the vertex property meanC.
     */
    SurfaceMesh::Halfedge nextEdge;
    Point p, p0, p1, d0, d1;
    Scalar alpha, beta;
    Point laplace;

    for (const auto& vertex : mesh.vertices()) {
        vcurvature_H[vertex] = 0.0;
    }

    create_colours(vcurvature_H);

    meanComputed = true;
}

void Curvature::visualize_k1_curvature() {
    if (!gaussComputed && !meanComputed)
        return;

    /**
     *  \todo Compute the k1 Principal curvature using the already computed
     *  Gaussian and Mean curvatures.
     */
    for (auto const& vertex : mesh.vertices())
        vcurvature_k1[vertex] = 1.0;

    create_colours(vcurvature_k1);
}

void Curvature::visualize_k2_curvature() {
    if (!gaussComputed && !meanComputed)
        return;

    /**
     *  \todo Compute the k2 Principal curvature using the already computed
     *  Gaussian and Mean curvatures.
     */
    for (auto const& vertex : mesh.vertices())
        vcurvature_k2[vertex] = vcurvature_H[vertex] - sqrt((vcurvature_H[vertex] * vcurvature_H[vertex]) - vcurvature_K[vertex]);

    create_colours(vcurvature_k2);
}

void Curvature::create_colours(OpenGP::SurfaceMesh::Vertex_property<OpenGP::Scalar> prop) {
    using namespace OpenGP;

    // Determine min and max meanCature (remove outliers)
    // Use relative property value are 1D texture coordinate in range [0, 1].
    std::vector<Scalar> values;
    for (const auto& vertex : mesh.vertices()) {
        values.push_back(prop[vertex]);
    }

    // Sort array.
    std::sort(values.begin(), values.end());

    // Discard lower and upper 2%.
    unsigned int n = values.size() - 1;
    unsigned int i = n / 50;
    Scalar minProp = values[i];
    Scalar maxProp = values[n-1-i];

    for (const auto& vertex : mesh.vertices()) {
        vquality[vertex] = (prop[vertex] - minProp) / (maxProp - minProp);
    }
}
