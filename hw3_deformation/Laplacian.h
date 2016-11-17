#pragma once
#include <Eigen/Sparse>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

struct Laplacian {
    typedef Eigen::SparseMatrix<Scalar> MySparseMatrix;
    static MySparseMatrix graph_laplacian(SurfaceMesh const& mesh) {
        unsigned int n = mesh.n_vertices();
        typedef Eigen::Triplet<Scalar> Triplet;

        std::vector<Triplet> laplacianList;
        laplacianList.reserve(n * n);

        int degree;
        for (auto const& v_i : mesh.vertices()) {
            degree = 0;
            for (auto const& edge : mesh.halfedges(v_i)) {
                auto v_j = mesh.to_vertex(edge);

                laplacianList.push_back(Triplet(v_i.idx(), v_j.idx(), -1));
                degree++;
            }

            laplacianList.push_back(Triplet(v_i.idx(), v_i.idx(), degree));
        }

        Eigen::SparseMatrix<Scalar> L(n, n);
        L.setFromTriplets(laplacianList.begin(), laplacianList.end());
        return L;
    }

    static MySparseMatrix laplace_beltrami(SurfaceMesh const& mesh) {
        unsigned int n = mesh.n_vertices();
        typedef Eigen::Triplet<Scalar> Triplet;

        SurfaceMesh::Halfedge betaEdge, alphaEdge;
        SurfaceMesh::Vertex vBeta, vAlpha, v_j;
        Point p_i, p_j, p_b, p_a, d_ib, d_ia, d_aj, d_bj, d_ij;
        Scalar alpha, beta, area, cotanAlpha, cotanBeta, cotanSum;

        Eigen::SparseMatrix<Scalar> L_omega(n, n);
        Eigen::SparseMatrix<Scalar> Area(n, n);

        std::vector<Triplet> omegaList;
        std::vector<Triplet> areaList;

        omegaList.reserve(n * n);
        areaList.reserve(n * n);

        int degree;
        for (auto const& v_i : mesh.vertices()) {
            cotanSum = 0.0f;
            area = 0.0f;
            degree = 0;

            for (auto const& edge : mesh.halfedges(v_i)) {
                v_j = mesh.to_vertex(edge);

                betaEdge = mesh.next_halfedge(edge);
                alphaEdge = mesh.next_halfedge(
                                mesh.opposite_halfedge(edge));

                vBeta = mesh.to_vertex(betaEdge);
                vAlpha = mesh.to_vertex(alphaEdge);

                p_i = mesh.position(v_i);
                p_j = mesh.position(v_j);
                p_b = mesh.position(vBeta);
                p_a = mesh.position(vAlpha);

                d_ib = (p_b - p_i).normalized();
                d_ia = (p_a - p_i).normalized();
                d_aj = (p_j - p_a).normalized();
                d_bj = (p_j - p_b).normalized();
                d_ij = (p_j - p_i).normalized();

                beta = std::acos(d_ib.dot(d_bj));
                alpha = std::acos(d_ia.dot(d_aj));

                cotanAlpha = 1.0f / std::tan(alpha);
                cotanBeta = 1.0f / std::tan(beta);
                Scalar alphabeta = cotanAlpha + cotanBeta;
                if (alphabeta < 0.0f)
                {
                    alphabeta = 0.0f;
                }

                omegaList.push_back(Triplet(v_i.idx(), v_j.idx(), alphabeta));
                cotanSum += alphabeta;

                area += (1 / 6.0f) * (d_ij.cross(d_ia)).norm();
                degree++;
            }

            omegaList.push_back(Triplet(v_i.idx(), v_i.idx(), -cotanSum));
            areaList.push_back(Triplet(v_i.idx(), v_i.idx(), 1.0f / (2.0f * area)));
        }

        L_omega.setFromTriplets(omegaList.begin(), omegaList.end());
        Area.setFromTriplets(areaList.begin(), areaList.end());

        return Area * L_omega;
    }
};

//=============================================================================
} // namespace OpenGP
//=============================================================================
