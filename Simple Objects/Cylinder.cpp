#include "Cylinder.h"
#include <glm/gtc/quaternion.hpp>
#include <lcms.h>

Cylinder::~Cylinder() {
    glDeleteBuffers(1, &v_buf);
    glDeleteBuffers(1, &i_buf);
    glDeleteBuffers(1, &n_buf);
    glDeleteBuffers(1, &c_buf);
}

void Cylinder::build(float topRad, float botRad, float height, glm::vec3 color) {
    glGenBuffers (1, &v_buf);
    glGenBuffers (1, &i_buf);
    glGenBuffers (1, &n_buf);
    glGenBuffers (1, &c_buf);
    float delta = 2 * M_PI / N_POINTS;


    /* points in top ring */

    float angle = 0.0f;
    for (int k = 0; k  < N_POINTS; k++)
    {
        float x = topRad * cos(angle);
        float y = topRad * sin(angle);
        vertices.push_back (x);
        vertices.push_back (y);
        vertices.push_back (height/2);
        /*Push Back Colors*/
        colors.push_back(color[0]);
        colors.push_back(color[1]);
        colors.push_back(color[2]);
        angle += delta;

    }

    /* vertices at the bottom ring */
    angle = 0.0f;
    for (int k = 0; k  < N_POINTS; k++)
    {
        float x = botRad * cos(angle);
        float y = botRad * sin(angle);
        vertices.push_back (x);
        vertices.push_back (y);
        vertices.push_back (-height/2);
        /*Push Back Colors*/
        colors.push_back(color[0]);
        colors.push_back(color[1]);
        colors.push_back(color[2]);
        angle += delta;
    }

    /* push the TOP CENTER point */
    vertices.push_back (0);  /* x */
    vertices.push_back (0); /* y */
    vertices.push_back (height/2); /* z */
    /*Push Back Colors*/
    colors.push_back(color[0]);
    colors.push_back(color[1]);
    colors.push_back(color[2]);

    for (int n = 0; n < 2; n++) {
        angle = 0;
        for (int k = 0; k < N_POINTS; k++) {
            float xTop = topRad * cos(angle);
            float yTop = topRad * sin(angle);
            float xBot = botRad * cos(angle);
            float yBot = botRad * sin(angle);
            glm::vec3 vertTangent{xTop - xBot, yTop - yBot, height};
            glm::vec3 horTangent{sin(angle), -cos(angle), 0};
            glm::vec3 normal = glm::normalize(glm::cross(vertTangent, horTangent));
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
            angle += delta;
        }
    }
    normals.push_back(0); /* normal vector for the top center vertice */
    normals.push_back(0);
    normals.push_back(1.0f);

    /* fill in the vertices */

    for (int k = 0; k < N_POINTS; k++)
    {
        index.push_back (k);
        index.push_back (k + N_POINTS);
    }
    /* close the quad_strip_index for wall */
    index.push_back (0);
    index.push_back (N_POINTS);

    /* first index of the tri-fan */
    index.push_back (N_POINTS * 2);
    /* index of rest of the tri-fan */
    for (int k = 0; k < N_POINTS; k++)
        index.push_back (k);
    /* close the last arc of the tri-fan */
    index.push_back (0);


    /* copy the two CPU arrays to GPU */

    /* select the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, v_buf);
    /* allocate in GPU and copy from CPU */
    glBufferData (GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
            vertices.data(), GL_STATIC_DRAW);
    /* deselect the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    /*Color Buffer*/
    glBindBuffer (GL_ARRAY_BUFFER, c_buf);
    glBufferData (GL_ARRAY_BUFFER,
            colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    /* select the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, n_buf);
    /* allocate in GPU and copy from CPU */
    glBufferData (GL_ARRAY_BUFFER, normals.size() * sizeof(float),
            normals.data(), GL_STATIC_DRAW);
    /* deselect the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, i_buf);
    /* allocate in GPU and copy from CPU */
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLushort),
            index.data(), GL_STATIC_DRAW);
    /* deselect the buffer */
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cylinder::render() const {
    /* select the buffs */
    glPushAttrib(GL_ENABLE_BIT);
    //glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer (GL_ARRAY_BUFFER, v_buf);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, c_buf);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, n_buf);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, i_buf);

    int N = 2*N_POINTS + 2;
    glDrawElements (GL_QUAD_STRIP, 2*N_POINTS + 2, GL_UNSIGNED_SHORT, 0);
    glDrawElements (GL_TRIANGLE_FAN, N_POINTS + 2, GL_UNSIGNED_SHORT,
            (void *) (sizeof(GLushort) * N));

    /* deselect the buffs */
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();
}
