#include "MerryGoRound.h"
using namespace std;

Cylinder base;
Cylinder handleBase;
Torus handle;
GLUquadric *d = gluNewQuadric();

MerryGoRound::~MerryGoRound()
{
    base.~Cylinder();
    handleBase.~Cylinder();
    handle.~Torus();
}

void MerryGoRound::build()
{
    base.build(1.0f, 1.0f, 8.0f);
    handleBase.build(0.25f, 0.25f, 1.5f);
    handle.build(1.5f, 0.25f, 10, 10, 180);
    gluQuadricOrientation(d, GLU_OUTSIDE);
    gluQuadricNormals(d, GLU_SMOOTH);
}

void MerryGoRound::render() const
{
    glPushMatrix();

    glColor3ub (114, 103, 103);
    glPushMatrix();
    base.render();
    glPopMatrix();

    glColor3ub (255, 0, 0);
    glPushMatrix();
    glTranslated(0, 0, 8);
    gluDisk(d, 0, 10, 100, 5);
    glPopMatrix();

    int rotation = 0;

    glPushMatrix();
    glTranslated(5, 3, 0);

    glPushMatrix();
    for(int i = 0; i < 4; i++) {
        glRotated(rotation, 0, 0, 1);
        glColor3ub(114, 103, 103);

        glPushMatrix();
        glTranslated(0, 2, 14);
        handleBase.render();
        glPopMatrix();

        glPushMatrix();
        glTranslated(0, 5, 14);
        handleBase.render();
        glPopMatrix();

        glColor3ub (114, 103, 103);
        glPushMatrix();
        glTranslated(0, 3.5, 14.75);
        glRotated(90, 0, 0, 1);
        glRotated(90, 1, 0, 0);
        handle.render();
        glPopMatrix();

        rotation += 90;
    }
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}