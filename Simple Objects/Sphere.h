#ifndef MY_SPHERE_H
#define MY_SPHERE_H

#include "../BufferObject.h"
#include <vector>
#include <GL/glew.h>
using namespace std;

class Sphere {
private:
    GLuint vertex_buffer, index_buffer, normal_buffer, color_buffer;
    vector<GLushort> indices;
    vector<GLfloat> vertices, normals, colors;
    int numLongitudeRings, numLatitudeRings;
    int start_of_northpole;

public:
    void build(int latRings, int lonRings, glm::vec3 color = {1, 1, 1});
    void render() const;
};
#endif