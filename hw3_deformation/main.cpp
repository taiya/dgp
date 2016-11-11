#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderFlat.h>
#include <OpenGP/SurfaceMesh/GL/SurfaceMeshRenderCloud.h>
#include "Deform.h"
using namespace OpenGP;

struct SelectionWindow : public GlfwWindow
{
    SurfaceMesh mesh;
    Deform deformator = Deform(mesh, this->scene);
    SurfaceMeshRenderFlat renderer = SurfaceMeshRenderFlat(mesh);
    bool mouse_clicked = false;

    SelectionWindow() :
        GlfwWindow("deformation", 400, 400)
    {
#if QUAD_MESH
        mesh.read("meshed_quad.obj");
#else
        mesh.read("woody.obj");
#endif
        CHECK(mesh.is_triangle_mesh());

#if QUAD_MESH
        for (auto const& vertex : mesh.vertices())
        {
            mesh.position(vertex) *= 0.5f;
        }
#endif

        mesh.update_face_normals();
        mesh.update_vertex_normals();

        deformator.initialize();

        this->scene.add(renderer);
    }

    bool mouse_press_callback(int button, int action, int /*mods*/) override
    {
        using namespace OpenGP;

        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                double xPos, yPos;
                glfwGetCursorPos(_window, &xPos, &yPos);
                deformator.mouse_down(unproject_mouse(xPos, yPos));
                mouse_clicked = true;
                return true;
            }
            else
            {
                mouse_clicked = false;
                deformator.mouse_up();
                return true;
            }
        }

        renderer.init_data();
        return false;
    }

    void mouse_move_callback(double xPos, double yPos) override
    {
        if (mouse_clicked)
        {
            deformator.mouse_down(unproject_mouse(xPos, yPos));
            renderer.init_data();
        }
    }

    OpenGP::Vec3 unproject_mouse(OpenGP::Scalar xPos, OpenGP::Scalar yPos){
        using namespace OpenGP;
        typedef Eigen::Matrix<Scalar, 4, 1> Vec4;

        float x = (2.0f * xPos) / _width - 1.0f;
        float y = 1.0f - (2.0f * yPos) / _height;

        Mat4x4 U = (scene.trackball_camera.projection_matrix() * scene.trackball_camera.view_matrix()).inverse();
        Vec4 near(x, y, 0, 1);

        Vec4 point = 2.0f * (U * near);

        return Vec3(point[0], point[1], point[2]);
    }

    bool key_callback(int key, int scancode, int action, int mods) override{
        GlfwWindow::key_callback(key, scancode, action, mods);
        
        if (action == GLFW_PRESS)
        {
            switch (key)
            {
            case GLFW_KEY_1:
                deformator.select_handle(1);
                break;

            case GLFW_KEY_2:
                deformator.select_handle(2);
                break;

            case GLFW_KEY_3:
                deformator.select_handle(3);
                break;

#if WOODY
            case GLFW_KEY_4:
                deformator.select_handle(4);
                break;

            case GLFW_KEY_5:
                deformator.select_handle(5);
                break;
#endif

            default:
                break;
            }
        }

        renderer.init_data();
        return true;
    }
};

int main(){
    SelectionWindow window;
    return window.run();
}
