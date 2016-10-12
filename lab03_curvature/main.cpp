#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
using namespace OpenGP;

#include "Curvature.h"

int main(int argc, char** argv) {
    std::string file = (argc==2) ? argv[1] : "indorelax.obj";
    TrackballWindow window(__FILE__, 640, 480);

    SurfaceMesh mesh;
    if(!mesh.read(file)) 
        mFatal() << "File not found: " << file;

    mesh.triangulate();
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    Curvature curvature(mesh);
    curvature.visualize_gauss_curvature();
    curvature.visualize_mean_curvature();
    curvature.visualize_k1_curvature();

    SurfaceMeshRenderShaded mesh_shaded(mesh);
    window.scene.add(mesh_shaded);
    mesh_shaded.colormap_enabled(true);
    mesh_shaded.colormap_set_range(0, 1);

    return window.run();
}
