#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include "Smoother.h"
using namespace OpenGP;

struct MainWindow : public TrackballWindow {
    SurfaceMesh mesh;
    Smoother smoother = Smoother(mesh);
    SurfaceMeshRenderFlat renderer = SurfaceMeshRenderFlat(mesh);

    MainWindow(int argc, char** argv) : TrackballWindow(__FILE__, 640, 480) {
        std::string file = (argc==2) ? argv[1] : "indorelax.obj";
        if(!mesh.read(file)) mFatal() << "File not found: " << file;
        // CHECK(mesh.is_triangle_mesh());
        mesh.triangulate();
        mesh.update_face_normals();
        mesh.update_vertex_normals();

        smoother.init();

        this->scene.add(renderer);
        //smoother.use_graph_laplacian();
        smoother.use_cotan_laplacian();
    }

    bool key_callback(int key, int scancode, int action, int mods) override {
        TrackballWindow::key_callback(key, scancode, action, mods);
        if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
            mLogger() << "Smoothing..." << MLogger::nonewline;
                // smoother.smooth_explicit(0.01f);
                smoother.smooth_implicit(.1f);
            mLogger() << "Done!";
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
