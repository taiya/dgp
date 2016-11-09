#include "Deform.h"
#include <OpenGP/MLogger.h>
#include <OpenGP/SurfaceMesh/eigen.h>
#include "Laplacian.h"
using namespace OpenGP;

// configuration of handles for the pre-supplied meshes
#if QUAD_MESH
const float handleRadius = 0.0f;

const std::vector<int> handleEntries = { 0, 1, 2 };
const std::vector<int> handleIds = { 1, 2, 3};
#else
const float handleRadius = 0.2f;

const std::vector<int> handleEntries = { 8, 22, 73, 90, 183 };
const std::vector<int> handleIds = { 1, 2, 3, 4, 5 };
#endif

Deform::Deform(OpenGP::SurfaceMesh& mesh, OpenGP::SceneGraph& scene)
    : mesh(mesh), scene(scene)
{
    vpoint = mesh.vertex_property<Point>("v:point");
    vhandle = mesh.add_vertex_property<int>("v:handle");

    for (auto const& vertex : mesh.vertices())
    {
        vhandle[vertex] = 0;
    }

    selected_handle = 0;
}

Deform::~Deform()
{
    mesh.remove_vertex_property(vhandle);
}

void Deform::initialize()
{
    construct_handles();
    factor_matrices();
}

void Deform::construct_handles()
{
    int i = 0;
    k = 0;
    for (const auto& vertex : mesh.vertices())
    {
        // If we have selected all of the handles, then exit the loop.
        if (i == (int) handleEntries.size())
        {
            break;
        }
        
        // Check if the vertex is a handle.
        if (vertex.idx() == handleEntries[i])
        {
            // Set the handle first.
            vhandle[vertex] = handleIds[i];
            ++k;

            // Grab the current position of the vertex.
            auto start = mesh.position(vertex);

            // Now loop over all the vertices again and find those
            // that are within handleRadius of start.
            for (auto const& neighbour : mesh.vertices())
            {
                if (neighbour == vertex)
                {
                    continue;
                }

                auto member = mesh.position(neighbour);

                float distance = (member - start).norm();

                if (distance < handleRadius)
                {
                    vhandle[neighbour] = handleIds[i];
                    ++k;
                }

            }
            ++i;
        }
    }

    // At this stage we know how many handles there are, so we can compute
    // u and k so we can access them later.
    u = mesh.n_vertices() - k;
}

void Deform::construct_permutation_matrix()
{
    /**
     * \todo Construct the permutation matrix that splits the point matrix V 
     * and the Laplacian matrix L into the required blocks.
     * \li First, store the indices of all the vertex handles (remember, a
     * vertex that isn't a handle has vhandle set to 0).
     * \li Next, identify those indices that will remain fixed by the
     * permutation.
     * \li Permute the indices. Remember: in the case of the V matrix, the 
     * final result is the following:
     *
     *           u             k
     * V = (p p p .... p | h h ... h)
     */
}

void Deform::factor_matrices()
{
    /**
     * \todo Perform the matrix factorization that allows us to deform in real
     * time.
     * \li Get the squared laplacian.
     * \li Construct the permutation matrix (using construct_permutation_matrix())
     * \li Permute V (the points matrix, see vertices_matrix), and the Laplacian.
     * \li Assign L_uu, L_uk, v_u, and v_k to the appropriate blocks of the 
     * corresponding matrices (see block).
     * \li Set the handle renderer to visualize your handles.
     * \li Compute the Cholesky decomposition of L_uu.
     * Note: the matrix to factorize varies between deformation algorithms.
     */
}

void Deform::select_handle(int index)
{
    // We don't accept handle 0 or anything beyond the size.
    if (index < 1 || index > (int) handleEntries.size())
    {
        return;
    }

    // Store the selected handle.
    selected_handle = index;
}

// this function is called every frame while the mouse is being dragged.
void Deform::mouse_down(OpenGP::Point const& pos)
{
    if (selected_handle == 0)
    {
        return;
    }

    /**
     * \todo Move the selected handle to the current mouse position based on
     * the barycentre (geometric mean) of the handle.
     * \li First, obtain all the points that belong to the selected handle.
     * \li Next, compute the barycentre (geometric mean) of the set.
     * \li Compute the displacement between the barycentre and the cursor position.
     * \li Translate all the points in set using the displacement.
     * \li Finally, update the mesh according to the current deformation method.
     *     For linearized deformation energy:
     *         simply solve the system.
     *     For multi-resolution deformation:
     *         solve the system to deform the smooth mesh,
     *         then reapply the displacements of details afterward.
     *     For as-rigid-as-possible deformation:
     *          Optimize the rotation and the position in alternation. Repeat this alternation 3 times.
     *          Optimizing rotation consists of finding a rigid rotation matrix for each vertex
     *          Optimizing position consists of solving a linear system.
     */
}

void Deform::mouse_up()
{
    if (selected_handle == 0)
    {
        return;
    }
}
