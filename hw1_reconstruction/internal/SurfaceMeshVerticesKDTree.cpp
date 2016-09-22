#include "SurfaceMeshVerticesKDTree.h"
#include <vector>
#include <OpenGP/types.h>
#include <OpenGP/SurfaceMesh/Eigen.h>
#include <OpenGP/external/nanoflann/nanoflann.hpp>

//=============================================================================
namespace OpenGP {
//=============================================================================

/// KD-tree adaptor for working with data directly stored in an Eigen Matrix, without duplicating the data storage.
/// This code is adapted from the KDTreeEigenMatrixAdaptor class of nanoflann.hpp
template <class MatrixType, int DIM = -1, class Distance = nanoflann::metric_L2, typename IndexType = int>
class KDTreeNanoflann {
private:
    typedef KDTreeNanoflann<MatrixType,DIM,Distance> self_t;
    typedef typename MatrixType::Scalar              num_t;
    typedef typename Distance::template traits<num_t,self_t>::distance_t metric_t;
    typedef nanoflann::KDTreeSingleIndexAdaptor< metric_t,self_t,DIM,IndexType>  index_t;
    index_t* index;
    const MatrixType &m_data_matrix;
public:
    KDTreeNanoflann(const MatrixType &mat, const int leaf_max_size = 10) : m_data_matrix(mat) {
        const size_t dims = mat.rows();
        index = new index_t( dims, *this, nanoflann::KDTreeSingleIndexAdaptorParams(leaf_max_size, dims ) );
        index->buildIndex();
    }
    ~KDTreeNanoflann() {delete index;}
    

    /// Query for the num_closest closest points to a given point (entered as query_point[0:dim-1]).
    inline void query(const num_t *query_point, const size_t num_closest, IndexType *out_indices, num_t *out_distances_sq) const {
        nanoflann::KNNResultSet<typename MatrixType::Scalar,IndexType> resultSet(num_closest);
        resultSet.init(out_indices, out_distances_sq);
        index->findNeighbors(resultSet, query_point, nanoflann::SearchParams());
    }
    /// Query for the closest points to a given point (entered as query_point[0:dim-1]).
    inline IndexType closest(const num_t *query_point) const {
        IndexType out_indices;
        num_t out_distances_sq;
        query(query_point, 1, &out_indices, &out_distances_sq);
        return out_indices;
    }
    const self_t & derived() const {return *this;}
    self_t & derived() {return *this;}
    inline size_t kdtree_get_point_count() const {return m_data_matrix.cols();}
    /// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
    inline num_t kdtree_distance(const num_t *p1, const size_t idx_p2,size_t size) const {
        num_t s=0;
        for (size_t i=0; i<size; i++) {
            const num_t d= p1[i]-m_data_matrix.coeff(i,idx_p2);
            s+=d*d;
        }
        return s;
    }
    /// Returns the dim'th component of the idx'th point in the class:
    inline num_t kdtree_get_pt(const size_t idx, int dim) const {
        return m_data_matrix.coeff(dim,idx);
    }
    /// Optional bounding-box computation: return false to default to a standard bbox computation loop.
    template <class BBOX> bool kdtree_get_bbox(BBOX&) const {return false;}
};


/// Slight specialization of adapter above to Mat3xN & L2 norms
class NanoflannAdapter : public KDTreeNanoflann<Mat3xN, 3, nanoflann::metric_L2_Simple>{
public:
    typedef KDTreeNanoflann<Mat3xN, 3, nanoflann::metric_L2_Simple> Super;
    NanoflannAdapter(const Mat3xN&mat, const int leaf_max_size = 10) : 
        Super(mat, leaf_max_size){}
};

SurfaceMeshVerticesKDTree::SurfaceMeshVerticesKDTree(SurfaceMesh &mesh){ 
    assert(_adapter == nullptr);
    // TODO: can we avoid creating a memory duplicate?
    _data = vertices_matrix(mesh);
    _adapter = new NanoflannAdapter(_data); 
}

SurfaceMeshVerticesKDTree::~SurfaceMeshVerticesKDTree(){ 
    if(_adapter!=nullptr) delete _adapter; 
}

SurfaceMesh::Vertex SurfaceMeshVerticesKDTree::closest_vertex(const Vec3 &p) const{
    int midx = _adapter->closest( p.data() );
    return SurfaceMesh::Vertex(midx);
}

std::vector<SurfaceMesh::Vertex> SurfaceMeshVerticesKDTree::kNN(const Vec3 &p,int N){
    std::vector<int> indxs(N,0);
    std::vector<Scalar> dists(N,0);
    _adapter->query( p.data(), N, indxs.data(), dists.data() );
    std::vector<SurfaceMesh::Vertex> verts;
    for(int index: indxs)
        verts.push_back( SurfaceMesh::Vertex(index) );    
    return verts;
}

//=============================================================================
} // namespace OpenGP
//=============================================================================
