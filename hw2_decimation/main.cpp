#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderShaded.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include <OpenGP/GL/TrackballWindow.h>
using namespace OpenGP;

#include "Decimator.h"

struct MainWindow : public TrackballWindow{
    SurfaceMesh mesh;
    Decimator decimator = Decimator(mesh);
    SurfaceMeshRenderFlat renderer = SurfaceMeshRenderFlat(mesh);
 
    MainWindow(int argc, char** argv) : TrackballWindow(__FILE__,640,480){
        std::string file = (argc==2) ? argv[1] : "indorelax.obj";
        if(!mesh.read(file)) mFatal() << "File not found: " << file;
        mesh.update_face_normals(); ///< shading
        this->scene.add(renderer);
        decimator.init();
    }
    
    bool key_callback(int key, int scancode, int action, int mods) override{
        TrackballWindow::key_callback(key, scancode, action, mods);
        if(key==GLFW_KEY_SPACE && action==GLFW_RELEASE){
            decimator.exec( .9*mesh.n_vertices() );
            mesh.update_face_normals();
            renderer.init_data();
            return true;
        }
        return false;
    }
};

int main(int argc, char** argv){
    MainWindow window(argc, argv);
    return window.run();
}
