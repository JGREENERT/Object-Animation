#include "MerryGoRound.h"
using namespace std;

Cylinder base;
GLUquadric *d = gluNewQuadric();

MerryGoRound::~MerryGoRound()
{
    base.~Cylinder();
}

void MerryGoRound::build()
{
    base.build(1.0f, 1.0f, 8.0f);
    gluQuadricOrientation(d, GLU_OUTSIDE);
    gluQuadricNormals(d, GLU_SMOOTH);
}

void MerryGoRound::render() const
{
    glPushMatrix();
    base.render();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, 8);
    gluDisk(d, 0, 10, 100, 1);
    glPopMatrix();
}