#ifndef DULIMARTA_BUFFER_OBJ
#define DULIMARTA_BUFFER_OBJ
#ifdef WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <GL/glew.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include <vector>
#include <iostream>
#include <stdexcept>
#include <glm/glm.hpp>
using namespace std;

class BufferObject {
protected:
    GLuint vertex_buffer, color_buffer, index_buffer;
    std::vector <glm::vec3> all_points, all_colors;
    vector<GLushort> all_index;
private:
    bool build_complete;
public:
    BufferObject() {
        vertex_buffer = color_buffer = index_buffer = 0;
        build_complete = false;
    }

    ~BufferObject() {
#if 0
        if (glIsBuffer(vertex_buffer))
            glDeleteBuffers(1, &vertex_buffer);
        if (glIsBuffer(color_buffer))
            glDeleteBuffers(1, &color_buffer);
        if (glIsBuffer(index_buffer))
            glDeleteBuffers(1, &index_buffer);
#endif
    }

    void post_build() {
        build_complete = true;
    }

    virtual void build(void *data) = 0;

    virtual void render(bool draw_outline) const {
        if (!build_complete) {
            return;
        }
        /* bind vertex buffer */
        if (glIsBuffer(vertex_buffer)) {
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glVertexPointer(3, GL_FLOAT, 0, 0);
        }
        /* bind color buffer */
        if (glIsBuffer(color_buffer)) {
            glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
            glColorPointer(3, GL_FLOAT, 0, 0);
        }

        /* bind the index buffer */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

        /* render the polygon */
        glPolygonMode(GL_FRONT, GL_FILL);
        glDrawElements(GL_TRIANGLES, all_index.size(), GL_UNSIGNED_SHORT, 0);

        if (draw_outline) {
            /* remove from BEGIN to END to disable rendering the polygon outline */
            glPolygonMode(GL_FRONT, GL_LINE);

            glEnable(GL_POLYGON_OFFSET_LINE);
            /* With polygon offset enabeld,  lines are rendered away from the polygons.
           Negative offset: lines are now rendered CLOSER to the camera */
            glPolygonOffset(-1.0, 1);

            glDisableClientState(GL_COLOR_ARRAY);
            glColor3ub(255, 255, 255);
            glDrawElements(GL_TRIANGLES, all_index.size(), GL_UNSIGNED_SHORT, 0);
            glEnableClientState(GL_COLOR_ARRAY);
            glDisable(GL_POLYGON_OFFSET_LINE);
        }

        /* unbind the buffers */
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};
#endif
