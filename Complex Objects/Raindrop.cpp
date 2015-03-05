#include "Raindrop.h"
using namespace std;

TCone tc;
GLUquadric *s = gluNewQuadric();

Raindrop::~Raindrop()
{
    tc.~TCone();
}

void Raindrop::build()
{
    tc.build();
    gluQuadricOrientation(s, GLU_OUTSIDE);
    gluQuadricNormals(s, GLU_SMOOTH);
}

void Raindrop::render() const
{
    glColor3f(0, 0, 1);

    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glScaled(0.25f, 0.25f, 0.25f);
    glTranslated(0, 0, 0);
    tc.render();
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glScaled(0.25f, 0.25f, 0.25f);
    glTranslated(0, 0, 1);
    gluSphere(s, 1, 10, 10);
    glPopMatrix();
}