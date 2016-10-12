#pragma once

#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

class Curvature
{
public:
    Curvature(OpenGP::SurfaceMesh& mesh);
    ~Curvature();

    void visualize_gauss_curvature();
    void visualize_mean_curvature();
    void visualize_k1_curvature();
    void visualize_k2_curvature();

private:
    void create_colours(OpenGP::SurfaceMesh::Vertex_property<OpenGP::Scalar> prop);
    bool gaussComputed;
    bool meanComputed;

    OpenGP::SurfaceMesh& mesh;
    OpenGP::SurfaceMesh::Vertex_property<OpenGP::Point> vpoint;
    OpenGP::SurfaceMesh::Vertex_property<float> vquality;

    OpenGP::SurfaceMesh::Vertex_property<OpenGP::Scalar> varea;
    OpenGP::SurfaceMesh::Edge_property<OpenGP::Scalar> ecotan;

    OpenGP::SurfaceMesh::Vertex_property<OpenGP::Scalar> vcurvature_K;
    OpenGP::SurfaceMesh::Vertex_property<OpenGP::Scalar> vcurvature_H;
    OpenGP::SurfaceMesh::Vertex_property<OpenGP::Scalar> vcurvature_k1;
    OpenGP::SurfaceMesh::Vertex_property<OpenGP::Scalar> vcurvature_k2;
};
