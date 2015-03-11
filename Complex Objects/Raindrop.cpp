#include "Raindrop.h"
using namespace std;

Cylinder c;
GLUquadric *s = gluNewQuadric();

Raindrop::~Raindrop()
{
    c.~Cylinder();
}

void Raindrop::build()
{
    c.build(0.5f, 0.0f, 1.0f);
    gluQuadricOrientation(s, GLU_OUTSIDE);
    gluQuadricNormals(s, GLU_SMOOTH);
}

void Raindrop::render() const
{
    glColor3f(0, 0, 1);

    glPushMatrix();
    glRotated(180, 1, 0, 0);
    glScaled(0.25f, 0.25f, 0.25f);
    c.render();
    glPopMatrix();

    glPushMatrix();
    glScaled(0.12f, 0.12f, 0.12f);
    glTranslated(0, 0, -1);
    gluSphere(s, 1, 10, 10);
    glPopMatrix();
}