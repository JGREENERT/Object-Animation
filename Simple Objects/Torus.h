#ifndef MY_TORUS_H
#define MY_TORUS_H

#include "../BufferObject.h"
#include <vector>
#include <GL/glew.h>
using namespace std;

class Torus {
private:
    GLuint vertex_buffer, index_buffer, normal_buffer;
    vector<GLushort> indices;
    vector<GLfloat> vertices, normals;
    int majorRings, minorRings;
    bool fullTorus;
public:
    void build(float majorRad, float minorRad, int majorRing, int minorRing, int span = 360);
    void render() const;
};
#endif