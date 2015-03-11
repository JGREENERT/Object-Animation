#include "Torus.h"
#include <iostream>
#include <glm/trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <lcms.h>

void Torus::build (float RAD, float rad, int majorRings, int minorRings, int span) {
    this->majorRings = majorRings;
    this->minorRings = minorRings;
    fullTorus = span == 360;
    /* build a UNIT sphere */

    float dMinor = 2 * M_PI / (minorRings - 1);
    float dMajor = glm::radians((float)span) / (majorRings - 1);
    float minAngle = 0;
    for (int k = 0; k < minorRings; k++) {
        float z = rad * sin(minAngle);
        float majAngle = 0;
        for (int m = 0; m < majorRings; m++) {
            float R = RAD + rad * cos(minAngle);
            vertices.push_back(R * cos(majAngle));
            vertices.push_back(R * sin(majAngle));
            vertices.push_back(z);
            /* calculate normal */
            glm::vec3 majTangent{-sin(majAngle), cos(majAngle), 0};
            glm::vec3 minTangent{
                    -cos(majAngle) * sin(minAngle),
                    -sin(majAngle) * sin(minAngle), cos(minAngle)};
            glm::vec3 n = glm::normalize(glm::cross(majTangent, minTangent));
            normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);
            majAngle += dMajor;
        };
        /* index to go back to the first vertex on this ring */
        minAngle += dMinor;
    }

    /* generate the indices of the quad strips */
    int ringStart = 0;
    const int N = majorRings * minorRings;
    for (int k = 0; k < minorRings; k++) {
        for (int m = 0; m < majorRings; m++) {
            indices.push_back((ringStart + m + majorRings) % N);
            indices.push_back(ringStart + m);
        }
        ringStart += majorRings;
    }

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
            vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat),
            normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),
            indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Torus::render() const {
    glPushAttrib(GL_ENABLE_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    if (glIsBuffer(normal_buffer)) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
        glNormalPointer(GL_FLOAT, 0, 0);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    GLushort *ptr = 0;
    const int N = 2 * majorRings; // + (fullTorus ? 2 : 0);
    for (int k = 0; k < minorRings; k++) {
        /* each quad strip must be rendered using a separate call */
        glDrawElements(GL_QUAD_STRIP, N,
                GL_UNSIGNED_SHORT, ptr);
        ptr += N;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();
}
