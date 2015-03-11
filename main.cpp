#include "BufferObject.h"
#include <GL/glew.h>
#include <sys/time.h>
#include <math.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include "Complex Objects/Raindrop.h"
#include "Simple Objects/Cylinder.h"
#include "Simple Objects/Sphere.h"
#include "Complex Objects/MerryGoRound.h"

using namespace std;
void displayCallback(GLFWwindow*);

/* define global variables here */
Cylinder* spot;
Sphere sphere, sphere1;
Raindrop drop;
MerryGoRound mgr;
const int STORMSIZE = 100;
const float GRAVITY = 0.25;   /* m/sec^2 */
double rainSlant = 0.0;

glm::mat4 rain_drop_cf;
glm::mat4 camera_cf, light1_cf, light0_cf;
glm::mat4 *active;

struct RAIN{
    double xCoord;
    double yCoord;
    double zCoord;
    glm::mat4 rainCoord;
};

RAIN rainArray[STORMSIZE];

double oldX, oldY, oldZ;
bool is_anim_running = true;
double windX, windY, windZ;

/* light source setting */
GLfloat light0_color[] = {1.0, 1.0, 1.0, 1.0};   /* color */
GLfloat light1_color[] = {0.99, 0.98, 0.84, 1.0};  /* color */
GLfloat black_color[] = {0.0, 0.0, 0.0, 1.0};   /* color */

/*--------------------------------*
 * GLUT Reshape callback function *
 *--------------------------------*/


void reshapeCallback (GLFWwindow *win, int w, int h)
{
    glViewport (0, 0, w, h);

    /* switch to Projection matrix mode */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (60, (float) w / (float) h, 5.0, 100.0);

    /* switch back to Model View matrix mode */
    glMatrixMode (GL_MODELVIEW);
    camera_cf = glm::lookAt(glm::vec3(25,20,20), glm::vec3(0,0,10), glm::vec3(0,0,1));
}

/*================================================================*
 * Idle Callback function. This is the main engine for simulation *
 *================================================================*/
void updateCoordFrames()
{
    static double last_timestamp = 0;
    static int deg = 0;
    float delta, current;


    current = glfwGetTime();
    if (is_anim_running) {
        delta = (current - last_timestamp);

        oldZ -=.25;
        if(oldZ == -1){
            oldZ = 18;
        }
        rain_drop_cf = glm::translate(glm::vec3{oldX, oldY, oldZ});

        for(int i = 0; i < STORMSIZE; i++){
            rainArray[i].xCoord += windX/2;
            rainArray[i].yCoord += windY/2;
            rainArray[i].zCoord -= GRAVITY;
            if(rainArray[i].zCoord== -1){
                rainArray[i].xCoord = (rand() %  40) - 20;
                rainArray[i].yCoord = (rand() % 40) -30;
                rainArray[i].zCoord = 18+i+1;
            }
            rainArray[i].rainCoord = glm::translate(glm::vec3{rainArray[i].xCoord,
                    rainArray[i].yCoord, rainArray[i].zCoord});
        }
        rainSlant = windX + windY;
    }
    last_timestamp = current;
}

void myGLInit ()
{
    glClearColor (0, 0, 0, 1); /* black background */

    /* fill front-facing polygon */
    glPolygonMode (GL_FRONT, GL_FILL);
    /* draw outline of back-facing polygon */
    glPolygonMode (GL_BACK, GL_LINE);

    /* Enable depth test for hidden surface removal */
    glEnable (GL_DEPTH_TEST);

    /* enable back-face culling */
    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);

    /* Enable shading */
    glEnable (GL_LIGHTING);
    glEnable (GL_NORMALIZE); /* Tell OpenGL to renormalize normal vector
                              after transformation */
    /* initialize two light sources */
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT0, GL_AMBIENT, light0_color);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_color);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light0_color);
    glEnable (GL_LIGHT1);
    glLightfv (GL_LIGHT1, GL_AMBIENT, light1_color);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light1_color);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light1_color);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 40);

    glEnableClientState(GL_VERTEX_ARRAY);
}

/*--------------------------------*
 * GLUT display callback function *
 *--------------------------------*/
void displayCallback (GLFWwindow *win)
{
    /* clear the window */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadMatrixf(glm::value_ptr(camera_cf));

    /* Specify the reflectance property of the ground using glColor
       (instead of glMaterial....)
     */
    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3ub (114, 103, 103);

    glBegin (GL_QUADS);
    const int GROUND_SIZE = 40;
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (GROUND_SIZE, GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (-GROUND_SIZE, GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (-GROUND_SIZE, -GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (GROUND_SIZE, -GROUND_SIZE);
    glEnd();
    glDisable (GL_COLOR_MATERIAL);

    /* place the light source in the scene. */
    glLightfv (GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::column(light0_cf, 3)));

    /* recall that the last column of a CF is the origin of the CF */
    glLightfv (GL_LIGHT1, GL_POSITION, glm::value_ptr(glm::column(light1_cf, 3)));

    /* we use the Z-axis of the light CF as the spotlight direction */
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, glm::value_ptr(glm::column(light1_cf, 2)));

    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glPushMatrix();
    {
        glMultMatrixf(glm::value_ptr(light0_cf));

        /* Render light-0 as an emmisive object */
        if (glIsEnabled(GL_LIGHT0))
            glMaterialfv(GL_FRONT, GL_EMISSION, light0_color);
        sphere1.render();
        glMaterialfv(GL_FRONT, GL_EMISSION, black_color);
    }
    glPopMatrix();
    glDisable (GL_COLOR_MATERIAL);


    /*
    ** Render the Raindrops
    */
    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    for(int i = 0; i < STORMSIZE; i++){
        glPushMatrix();
        {
            glMultMatrixf(glm::value_ptr(rainArray[i].rainCoord));
            if(windX != 0)
                glRotated(-rainSlant*45, 1, 0, 0);
            if(windY != 0)
                glRotated(rainSlant*45, 1, 0, 0);
            drop.render();
        }
        glPopMatrix();
    }
    glDisable (GL_COLOR_MATERIAL);

    /*
    ** Render the Merry Go Round
    */
    glPushMatrix();
    mgr.render();
    glPopMatrix();

    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3ub (255, 255, 255);
    /* render the spot light using its coordinate frame */
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(light1_cf));
    spot->render();
    glPopMatrix();
    glDisable (GL_COLOR_MATERIAL);

    /* to make smooth transition between frame */
    glfwSwapBuffers(win);
}

void myModelInit ()
{
    windX = 0;
    windY = 0;
    windZ = 0;

    sphere.build(15, 20);
    spot = new Cylinder();
    spot -> build(1 + tan(glm::radians(40.0f)), 1, 2);

    sphere1.build(5, 15);
    spot = new Cylinder();
    spot -> build(1 + tan(glm::radians(50.0f)), 1.5, 2.5);

    drop.build();
    mgr.build();

    active = &camera_cf;

    light0_cf = glm::translate(glm::vec3{-25, 8, 26});
    int counter = 5;
    for(int i = 0; i < STORMSIZE; i++){
        rainArray[i].xCoord = (rand() %  40) - 20;
        rainArray[i].yCoord = (rand() % 40) -30;
        rainArray[i].zCoord = 18;
        rainArray[i].rainCoord = glm::translate(glm::vec3{rainArray[i].xCoord,
                rainArray[i].yCoord, rainArray[i].zCoord});
    }

    rain_drop_cf = glm::translate(glm::vec3{-12, 4, 18});

    oldX = -12;
    oldY = 4;
    oldZ = 18;

    light1_cf = glm::translate(glm::vec3{0, -10, 18});
    light1_cf = light1_cf * glm::rotate (glm::radians(-120.0f), glm::vec3{1,0,0});
}

void keyCallback (GLFWwindow *win, int key, int scan_code, int action, int mods) {
    if (action == GLFW_RELEASE)
    {
        windX = 0;
        windY = 0;
        return; /* ignore key release action */
    }

    if (mods == GLFW_MOD_SHIFT) {
        switch (key) {
            case GLFW_KEY_UP: /* tilt */
                *active *= glm::rotate(glm::radians(-3.0f), glm::vec3{1.0f, 0.0f, 0.0f});
                break;
            case GLFW_KEY_DOWN: /* tilt */
                *active *= glm::rotate(glm::radians(+3.0f), glm::vec3{1.0f, 0.0f, 0.0f});
                break;
            case GLFW_KEY_LEFT: /* pan left */
                *active *= glm::rotate(glm::radians(-3.0f), glm::vec3{0.0f, 1.0f, 0.0f});
                break;
            case GLFW_KEY_RIGHT: /* pan right */
                *active *= glm::rotate(glm::radians(+3.0f), glm::vec3{0.0f, 1.0f, 0.0f});
                break;
            case GLFW_KEY_X:
                *active *= glm::translate(glm::vec3{1, 0, 0});
                break;
            case GLFW_KEY_Y:
                *active *= glm::translate(glm::vec3{0, 1, 0});
                break;
            case GLFW_KEY_Z:
                *active *= glm::translate(glm::vec3{0, 0, 1});
                break;
            case GLFW_KEY_D:
                if(windX < 1){
                    windX +=.05;
                }
                break;
            case GLFW_KEY_S:
                if(windY < 1){
                    windY +=.05;
                }
                break;
            default:
                break;
        };

    }
    else {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                exit(0);
            case GLFW_KEY_0:
                active = &light0_cf;
                if (glIsEnabled(GL_LIGHT0))
                    glDisable(GL_LIGHT0);
                else
                    glEnable(GL_LIGHT0);
                break;
            case GLFW_KEY_1:
                active = &light1_cf;
                if (glIsEnabled(GL_LIGHT1))
                    glDisable(GL_LIGHT1);
                else
                    glEnable(GL_LIGHT1);
                break;

            case GLFW_KEY_SPACE: /* pause the animation */
                is_anim_running ^= true;
                break;
            case GLFW_KEY_C:
                active = &camera_cf;
                break;
            case GLFW_KEY_F:
                break;
            case GLFW_KEY_X:
                *active *= glm::translate(glm::vec3{-1, 0, 0});
                break;
            case GLFW_KEY_Y:
                *active *= glm::translate(glm::vec3{0, -1, 0});
                break;
            case GLFW_KEY_Z:
                *active *= glm::translate(glm::vec3{0, 0, -1});
                break;
            case GLFW_KEY_D:
                if(windX > -1){
                    windX -=.05;
                }
                break;
            case GLFW_KEY_S:
                if(windY > -1){
                    windY -=.05;
                }
                break;
        }
    }
}

int main (int argc, char **argv)
{

    glfwInit();
    GLFWwindow *win;
    win = glfwCreateWindow(100, 50, "Animation", NULL, NULL);

    glfwMakeContextCurrent(win);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf (stderr, "GLEW init error %s\n",
                glewGetErrorString(err));
        exit (EXIT_FAILURE);
    }

    srand (time(0));

    myGLInit ();
    myModelInit ();

    /* setup display callback function */
    glfwSetFramebufferSizeCallback(win, reshapeCallback);
    glfwSetWindowRefreshCallback(win, displayCallback);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetWindowSize(win, 800, 600);

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        updateCoordFrames();
        displayCallback(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();

}
