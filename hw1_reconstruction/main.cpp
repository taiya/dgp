#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderCloud.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderVertexNormals.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include <OpenGP/SurfaceMesh/bounding_box.h>
//
#include "internal/MarchingCubes.h"
using namespace OpenGP;

///--- Three different ways
#include "ImplicitRBF.h"
#include "ImplicitHoppe.h"
#include "ImplicitSphere.h"

void reconstruct(SurfaceMesh& cloud, SurfaceMesh &output, uint res){    
    // Compute bounding cube for Marching Cubes grid.
    mDebug() << "Computing bounding box\n";
    Box3 bbox = OpenGP::bounding_box(cloud);
    bbox = OpenGP::bbox_cubified(bbox);
    bbox = OpenGP::bbox_scaled(bbox, 1.1);        

    // Setup marching cubes grid.
    std::cout << "Setup regular grid for the signed distance field\n" << std::flush;
    Grid grid(bbox, res, res, res);
    
    //--- Uncomment to choose
    ImplicitSphere implicit;
    //ImplicitHoppe implicit(cloud);
    //ImplicitRBF implicit(cloud);
    
    // Get grid's signed distance values by evaluating the implicit function
    mDebug() << "Evaluating function on the grid";
    for(uint x=0; x < res; ++x)
        for(uint y=0; y < res; ++y)
            for(uint z=0; z < res; ++z)
                grid(x, y, z) = implicit.eval( grid.point(x, y, z) );
    
    // Extracts isosurface by marching cubes
    mDebug() << "Extracting isosurface(" << res << ")";
    MarchingCubes::exec(grid, output);
    mDebug("Done! [#V:%d #F:%d]", output.n_vertices(), output.n_faces());
}

int main(int argc, char** argv) {
    std::string file = (argc==2) ? argv[1] : "sphere_cloud.obj";
    
    SurfaceMesh point_cloud;
    bool success = point_cloud.read(file);
    if (!success) mFatal() << "File not found: " << file;

    TrackballWindow window(__FILE__, 640, 480);

    // TASK
    // 1) Compute normals
    //    - fetch kNN (k~6-10) for each point
    //    - compute the covariance matrix
    //    - extract (unoriented) normal form the eigendecomposition
    //
    // 2) Re-orient normals coherently
    //    - build Reimannian graph and compute weights
    //    - construct minimal spanning tree
    //    - flip the normals by traversing the tree

    ///--- Display the input oriented point cloud
    SurfaceMeshRenderCloud points_gl(point_cloud);
    SurfaceMeshRenderVertexNormals normals_gl(point_cloud);
    window.scene.add(points_gl);
    window.scene.add(normals_gl);

    ///--- Compute reconstruction
    SurfaceMesh output;
    reconstruct(point_cloud, output, 30 /*grid resolution*/);
    output.update_face_normals();
    output.update_vertex_normals();
    output.write("output.obj");
    
    ///--- Renders reconstruction
    SurfaceMeshRenderFlat mesh_gl(output);
    // SurfaceMeshRenderShaded mesh_gl(output);
    window.scene.add(mesh_gl);
    return window.run();
}
