#include "MerryGoRound.h"
using namespace std;

Cylinder base;
Cylinder handleBase;
Cylinder disc;
Torus handle;

MerryGoRound::~MerryGoRound()
{
    disc.~Cylinder();
    base.~Cylinder();
    handleBase.~Cylinder();
    handle.~Torus();
}

void MerryGoRound::build()
{
    disc.build(10, 1, 4, {1, 0, 0});
    base.build(1.0f, 1.0f, 4.0f, {0.45f, 0.40f, 0.40f});
    handleBase.build(0.25f, 0.25f, 1.5f, {0.45f, 0.40f, 0.40f});
    handle.build(3.5f, 0.25f, 10, 10, 180, {0.45f, 0.40f, 0.40f});
}

void MerryGoRound::render() const
{
    glPushMatrix();
    {

        //glColor3ub(114, 103, 103);
        glPushMatrix();
        {
            glTranslated(0, 0, 2);
            base.render();
        }
        glPopMatrix();

        //glColor3ub(255, 0, 0);
        glPushMatrix();
        {
            glTranslated(0, 0, 4);
            disc.render();
        }
        glPopMatrix();

        int rotation = 0;

        glPushMatrix();
        {
            glTranslated(0, 0, 6);

            glPushMatrix();
            {
                for (int i = 0; i < 4; i++) {
                    glRotated(rotation, 0, 0, 1);
                    //glColor3ub(114, 103, 103);

                    glPushMatrix();
                    {
                        glTranslated(0, 1, 0);
                        handleBase.render();
                    }
                    glPopMatrix();

                    glPushMatrix();
                    {
                        glTranslated(0, 8, 0);
                        handleBase.render();
                    }
                    glPopMatrix();

                    //glColor3ub(114, 103, 103);
                    glPushMatrix();
                    {
                        glTranslated(0, 4.5, 0.75f);
                        glRotated(90, 0, 0, 1);
                        glRotated(90, 1, 0, 0);
                        handle.render();
                    }
                    glPopMatrix();
                    rotation += 90;
                }
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}