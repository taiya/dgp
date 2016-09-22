#pragma once
#include <OpenGP/MLogger.h>
#include <OpenGP/types.h>
#include <OpenGP/SurfaceMesh/Eigen.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
using namespace OpenGP;

class ImplicitRBF{
private:
    Scalar OFFSET_EPSILON_R = 0.01; //< normal offset distance relative to BBOX
    SurfaceMesh& cloud;
    SurfaceMesh::Vertex_property<Vec3> vpoints;
    SurfaceMesh::Vertex_property<Vec3> vnormals;
    Mat3xN  centers_;
    VecN    weights_;  
    
public:
    ImplicitRBF(SurfaceMesh& cloud) : cloud(cloud){
        vpoints = cloud.get_vertex_property<Vec3>("v:point");
        vnormals = cloud.get_vertex_property<Vec3>("v:normal");
        optimize_rbf_weights();
    }

    /// Evaluates RBF at position p
    Scalar eval(const Vec3& p) const{
            // TASK: evaluate the RBF implicit function at point "p"
    }

private:
    /// Fit RBF to given constraints
    void optimize_rbf_weights(){
        mDebug() << "Start Implicit RBF fitting";
        int n = cloud.n_vertices();
        int N = 2 * n;
    
        // Matrices for the RBF centers and their weights
        centers_.setZero(3, N);
        weights_.setZero(N);
    
        // Linear system matrix and right-hand-side vector for computing the weights
        MatMxN M(N, N);
        VecN d(N);
    
        // TASK:
        // 1) Determine the RBF centers, and store them into the columns of matrix centers_;
        //    The offset distance should be determined according to the bounding box diaginal length.
        // 2) Collect the on- and off-surface constraints, to set up the linear system matrix M
        //    and the right-hand-side d.
        // 3) Use the memeber function solve_linear_system(...) to solve the linear system
        //    to obtain RBF weights, and store them in the data member weights_.
    }

    /// Evaluates RBF kernel at point _x, with center at _center
    static Scalar kernel(const Vec3& center, const Vec3& x){
        double r = (x-center).norm();
        return r*r*r;
    }
    
    /// Solves the linear system Ax=b
    void solve_linear_system(const MatMxN& A, const VecN& b, VecN& x){
        // Solve the linear system using Eigen's Householder QR factorization
        Eigen::HouseholderQR<MatMxN> qr(A);
        x = qr.solve(b);
    }
};
