#ifndef CONE_DEMO_H
#define CONE_DEMO_H

#include "../BufferObject.h"
#include <vector>

using namespace std;

class TCone{
private:
    const int N_POINTS = 13;
    const float TOP_RAD = 1.0f;
    const float BOT_RAD = 0.0f;
    const float CONE_HEIGHT = 1.0f;
    GLuint v_buf, i_buf;
    vector<float> vertices;
    vector<GLushort> index;

public:
    ~TCone();
    void build();
    void render() const;
};
#endif