#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
using namespace OpenGP;

int main(){
    SurfaceMesh mesh;
    
    //--- Let's create a simple triangle
    SurfaceMesh::Vertex v1 = mesh.add_vertex(Vec3(-1, -1, 0));
    SurfaceMesh::Vertex v2 = mesh.add_vertex(Vec3( 1, -1, 0));
    SurfaceMesh::Vertex v3 = mesh.add_vertex(Vec3( 1,  1, 0));
    mesh.add_triangle(v1, v2, v3);

    // TASK: create a quad by adding a 4th vertex and using it to add a second triangle.
    // hint: mesh.add_vertex(), mesh.add_triangle()
    
    //--- To shade our model we need normals!
    mesh.update_face_normals();
    
    //--- Create window and show your data
    TrackballWindow window(__FILE__, 400, 400);
    SurfaceMeshRenderFlat render(mesh);
    window.scene.add(render);
    window.run();
}
