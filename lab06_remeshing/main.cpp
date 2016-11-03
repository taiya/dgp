#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include "Remesher.h"
using namespace OpenGP;

struct MainWindow : public TrackballWindow {
    Scalar target_edge_length = nan();
    SurfaceMesh mesh;
    Remesher remesher = Remesher(mesh);
    SurfaceMeshRenderFlat renderer = SurfaceMeshRenderFlat(mesh);

    MainWindow(int argc, char** argv) : TrackballWindow(__FILE__, 640, 480) {
        std::string file = (argc==2) ? argv[1] : "sphere_hoppe.obj";
        if(!mesh.read(file)) mFatal() << "File not found: " << file;
        CHECK(mesh.is_triangle_mesh());
        mesh.update_face_normals();
        mesh.update_vertex_normals();
        this->scene.add(renderer);
        
        //-- Compute average edge length
        Scalar e_avg=0;
        Scalar inv_n_edges = 1.0/mesh.n_edges();
        for(SurfaceMesh::Edge e: mesh.edges()){
            Vec3 v0 = mesh.position( mesh.vertex(e,0) );
            Vec3 v1 = mesh.position( mesh.vertex(e,1) );
            e_avg += inv_n_edges * (v0-v1).norm();
        }
        mDebug() << "average edge length:" << e_avg;
    }

    bool key_callback(int key, int scancode, int action, int mods) override {
        TrackballWindow::key_callback(key, scancode, action, mods);
        if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
            remesher.remesh(target_edge_length); 
            mesh.update_face_normals();
            renderer.init_data();
            return true;
        }
        return false;
    }
};

int main(int argc, char** argv) {
    MainWindow window(argc, argv);
    return window.run();
}
