#pragma once
#include <Eigen/Sparse>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/Gl/SceneGraph.h>
#include <OpenGP/GL/PointsRenderer.h>

#define QUAD_MESH 0
#define WOODY !QUAD_MESH

class Deform
{
public:
    Deform(OpenGP::SurfaceMesh& mesh, OpenGP::SceneGraph& scene);
    ~Deform();
    
    void initialize();
    
    void select_handle(int index);
    void mouse_down(OpenGP::Point const& pos);
    void mouse_up();

private:
    void construct_handles();
    void factor_matrices();
    void construct_permutation_matrix();

    OpenGP::SurfaceMesh& mesh;
    OpenGP::SceneGraph& scene;
    OpenGP::SurfaceMesh::Vertex_property<OpenGP::Point> vpoint;
    OpenGP::SurfaceMesh::Vertex_property<int> vhandle;
    
    // the handle for the current deformation operation
    // 0 if no handle is selected
    int selected_handle;
    
    // number of unconstrained vertices
    int u
    // number of constrained vertices
    int k;
    
    // solver you can configure and use for a sparse symmetric positive definite linear system
    Eigen::SimplicialCholesky<Eigen::SparseMatrix<OpenGP::Scalar>,Eigen::RowMajor> solver;
    
    // the permutation matrix used to improve the factorization
    Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic> permute;
    // the unconstrained vertices
    Eigen::MatrixXf v_u;
    // the constrained vertices
    Eigen::MatrixXf v_k;
    // the upper part of the matrix with the equations to solve for the unconstrained vertices
    Eigen::SparseMatrix<OpenGP::Scalar> L_uu;
    // the lower part of the matrix, with the equations for the constrained vertices (identity)
    Eigen::SparseMatrix<OpenGP::Scalar> L_uk;
};
