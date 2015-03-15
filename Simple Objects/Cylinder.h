#ifndef MY_CYLINDER_H
#define MY_CYLINDER_H

#include "../BufferObject.h"
#include <vector>
#include <GL/glew.h>

using namespace std;

class Cylinder {
private:
    const int N_POINTS = 20;
    GLuint v_buf, i_buf, n_buf;
    vector<float> vertices, normals;
    vector<GLushort> index;

public:
    ~Cylinder();
    void build(float topRad = 0.5f, float botRad = 0.5f, float height = 1.0);
    void render() const;
};
#endif
