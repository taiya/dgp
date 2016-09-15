#include <OpenGP/GL/TrackballWindow.h>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
using namespace OpenGP;
#define  STRIP_CODE
int main(){
    SurfaceMesh mesh;
    
    //--- Let's create a simple triangle
    SurfaceMesh::Vertex v1 = mesh.add_vertex(Vec3(-1, -1, 0));
    SurfaceMesh::Vertex v2 = mesh.add_vertex(Vec3( 1, -1, 0));
    SurfaceMesh::Vertex v3 = mesh.add_vertex(Vec3( 1,  1, 0));
    mesh.add_triangle(v1, v2, v3);

#ifdef STRIP_CODE
    // TASK: create a quad by assembling 2x triangles
#else
    SurfaceMesh::Vertex v4 = mesh.add_vertex(Vec3(-1,  1, 0));
    mesh.add_triangle(v3, v4, v1);
#endif

#ifndef STRIP_CODE
#if 0
    // Now let's scale a single vertex.
    Point pos = mesh.position(v2);
    mesh.position(v2) = pos * 2;
    mesh.position(v2) = pos / 2;
#endif
    
#if 0
    // Now let us scale the whole mesh (around origin)
    SurfaceMesh::Vertex_iterator vIt, vBegin, vEnd;
    vBegin = mesh.vertices_begin();
    vEnd = mesh.vertices_end();
    for (vIt = vBegin; vIt != vEnd; ++vIt){
        SurfaceMesh::Vertex v = *vIt;
        // And now we can scale it as before.
        Point pos = mesh.position(v);
        mesh.position(v) = pos * 2;
    }
#endif

    // A simpler way to achieve the same goal (hurray c++11)
    for(const auto& vertex : mesh.vertices())
        mesh.position(vertex) *= .5;
#endif
    
    //--- To shade our model we need normals!
    mesh.update_face_normals();
    
    //--- Create window and show your data
    TrackballWindow window(__FILE__, 400, 400);
    SurfaceMeshRenderFlat render(mesh);
    window.scene.add(render);
    window.run();
}
