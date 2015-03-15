#include "Lamp.h"
using namespace std;

Cylinder stand;
Torus top;
GLUquadric *e = gluNewQuadric();


Lamp::~Lamp()
{
    stand.~Cylinder();
    top.~Torus();
}

void Lamp::build()
{
    stand.build(0.4f, 0.4f, 16.0f, {0.72f, 0.45f, 0.2f});
    top.build(1.5f, 0.4f, 10, 10, 135, {0.72f, 0.45f, 0.2f});
    gluQuadricOrientation(e, GLU_OUTSIDE);
    gluQuadricNormals(e, GLU_SMOOTH);

}

void Lamp::render() const
{
    glPushMatrix();
    glRotated(180 , 0, 0, 1);
    glTranslated(0, 3, 1);

    glColor3ub (50, 192, 50);
    glPushMatrix();
    glTranslated(0, 11, 0);
    gluDisk(e, 0, 2, 100, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 11, 8);
    stand.render();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 9.5f, 16);
    //glTranslated(0, 0, 16);
    glRotated(90, 0, 1, 0);
    glRotated(90, 0, 0, 1);
    top.render();
    glPopMatrix();

    glPopMatrix();

}