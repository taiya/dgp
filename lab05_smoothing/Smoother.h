#pragma once
#include <Eigen/Sparse>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

class Smoother {
private:
    OpenGP::SurfaceMesh& mesh;
    Eigen::SparseMatrix<OpenGP::Scalar> L;    
public:
    Smoother(OpenGP::SurfaceMesh& mesh);
    ~Smoother();

public:
    void init();
    void use_cotan_laplacian();
    void use_graph_laplacian();
    void smooth_explicit(OpenGP::Scalar lambda);
    void smooth_implicit(OpenGP::Scalar lambda);

private:
    void solve_linear_least_square(Eigen::SparseMatrix<OpenGP::Scalar>& A, Eigen::MatrixXf& B, Eigen::MatrixXf& X);
};
