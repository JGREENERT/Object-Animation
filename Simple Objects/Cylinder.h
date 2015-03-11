#ifndef MY_CYLINDER_H
#define MY_CYLINDER_H

#include "../BufferObject.h"
#include <vector>
#include <GL/glew.h>

using namespace std;

class Cylinder {
private:
    const int N_POINTS = 20;
//    const float TOP_RAD = 1.0f;
//    const float BOT_RAD = 0.8f;
//    const float CONE_HEIGHT = 0.5f;
    GLuint v_buf, i_buf, n_buf;
    vector<float> vertices, normals;
    vector<GLushort> index;

public:
    ~Cylinder();
    void build(float topRad = 0.5f, float botRad = 0.5f, float height = 1.0);
    void render() const;
};
#endif
