#include "Smoother.h"

#include <OpenGP/SurfaceMesh/Eigen.h>

using namespace Eigen;
using namespace OpenGP;

Smoother::Smoother(OpenGP::SurfaceMesh& mesh) :
    mesh(mesh)
{ }

Smoother::~Smoother()
{ }

void Smoother::init()
{
    int n = mesh.n_vertices();
    L = SparseMatrix<Scalar>(n, n);
}

void Smoother::use_cotan_laplacian()
{
    unsigned int n = mesh.n_vertices();
    typedef Eigen::Triplet<Scalar> Triplet;

    /* todo:
    Build the cotangent weighted laplacian matrix into L.
    See equation 7 here: http://www.cs.jhu.edu/~misha/Fall07/Papers/Nealen06.pdf#2
    tip: Translate that summation into a matrix. Also, don't include the area weight.
    */
    std::vector<Triplet> tripletList;
    tripletList.reserve(n * n);

    for (auto const& v_i : mesh.vertices())
    {
        for (auto const& edge : mesh.halfedges(v_i))
        {
            auto v_j = mesh.to_vertex(edge);
            tripletList.push_back(Triplet(v_i.idx(), v_j.idx(), 0));
        }

        tripletList.push_back(Triplet(v_i.idx(), v_i.idx(), 1));
    }

    L.setFromTriplets(tripletList.begin(), tripletList.end());
}

void Smoother::use_graph_laplacian()
{
    // Grab the number of vertices in the mesh.
    unsigned int n = mesh.n_vertices();

    typedef Eigen::Triplet<Scalar> Triplet;

    /* todo:
    Build the uniform weighted laplacian matrix into L.
    See "Laplacian matrix" here: https://en.wikipedia.org/wiki/Laplacian_matrix#Example
    */
    std::vector<Triplet> tripletList;
    tripletList.reserve(n * n);

    for (auto const& v_i : mesh.vertices())
    {
        for (auto const& edge : mesh.halfedges(v_i))
        {
            auto v_j = mesh.to_vertex(edge);
            tripletList.push_back(Triplet(v_i.idx(), v_j.idx(), 0));
        }

        tripletList.push_back(Triplet(v_i.idx(), v_i.idx(), 1));
    }

    L.setFromTriplets(tripletList.begin(), tripletList.end());
}

void Smoother::smooth_explicit(OpenGP::Scalar lambda)
{
    // Set up our identity matrix.
    int n = mesh.n_vertices();
    SparseMatrix<Scalar> I(n, n);
    I.setIdentity();

    auto P_t = MatrixXf(vertices_matrix(mesh));
    P_t.transposeInPlace();

    /* todo:
    Perform one iteration of the explicit solver and store the result in vertices_matrix(mesh)
    See: http://www.faculty.jacobs-university.de/llinsen/teaching/320491/Lecture13.pdf#22
    tip: make sure your matrix dimensions match when assigning to vertices_matrix(mesh)
    */
    vertices_matrix(mesh) = P_t;
}

void Smoother::smooth_implicit(OpenGP::Scalar lambda)
{
    int n = mesh.n_vertices();
    SparseMatrix<Scalar> I(n, n);
    I.setIdentity();

    auto P_t = MatrixXf(vertices_matrix(mesh));
    P_t.transposeInPlace();

    /* todo:
    Perform one iteration of the implicit solver and store the result in vertices_matrix(mesh)
    See: http://www.faculty.jacobs-university.de/llinsen/teaching/320491/Lecture13.pdf#23
    tip: use solve_linear_least_square (function below) to solve the linear system.
    */
    vertices_matrix(mesh) = P_t;
}

void Smoother::solve_linear_least_square(Eigen::SparseMatrix<OpenGP::Scalar>& A, Eigen::MatrixXf&B, Eigen::MatrixXf& X)
{
    typedef SimplicialLDLT<SparseMatrix<Scalar>> Solver;
    Solver solver;

    solver.compute(A);

    for (int i = 0; i < B.cols(); ++i)
    {
        X.col(i) = solver.solve(B.col(i));
    }
}
