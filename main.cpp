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
#include <glm/gtx/vector_angle.hpp>
#include "Complex Objects/Raindrop.h"
#include "Simple Objects/Cylinder.h"
#include "Simple Objects/Sphere.h"
#include "Complex Objects/MerryGoRound.h"
#include "Complex Objects/Lamp.h"

using namespace std;
void displayCallback(GLFWwindow*);

/* define global variables here */
Cylinder* spot;
Sphere sphere1;
Raindrop drop;
MerryGoRound mgr;
Lamp lp;

float arc_ball_rad_square;
int screen_ctr_x, screen_ctr_y;

glm::mat4 rain_drop_cf;
glm::mat4 camera_cf, light1_cf, light0_cf, mgr_cf, lp_cf;
glm::mat4 *active;

struct RAIN{
    double xCoord;
    double yCoord;
    double zCoord;
    glm::mat4 rainCoord;
};

const int STORMSIZE = 200;
RAIN rainArray[STORMSIZE];

float GRAVITY = 0.25;   /* m/sec^2 */
double rainSlant = 0.0;
double oldX, oldY, oldZ;
float spinDegree;
bool is_anim_running = true;
double windX, windY;
bool setTimer = false;
int animTimer = 0;
double last_timestamp;
double last_lightning = 0;

/*Merry Go Round Settings (Brass)*/
GLfloat mgr_ambient[] = {0.329412, 0.223529, 0.027451, 1.000000};
//GLfloat mgr_diffuse[] = {0.780392, 0.568627, 0.113725, 1.000000};
GLfloat mgr_diffuse[] = {1.0, 0.0, 0.0, 1.000000};
GLfloat mgr_specular[] = {0.992157, 0.941176, 0.807843, 1.000000};
GLfloat mgr_shininess[] = {27.897400};

/*Light Post Settings (Copper)*/
GLfloat lp_ambient[] = {0.191250, 0.073500, 0.022500, 1.000000};
//GLfloat lp_diffuse[] = {0.703800, 0.270480, 0.082800, 1.000000};
GLfloat lp_diffuse[] = {0.5, 0.45, 0.43, 1.000000};
GLfloat lp_specular[] = {0.256777, 0.137622, 0.086014, 1.000000};
GLfloat lp_shininess[] = {12.800000};

/*Raindrop Settings (Silver)*/
GLfloat drop_ambient[] = {0.192250, 0.192250, 0.192250, 1.000000};
//GLfloat drop_diffuse[] = {0.507540, 0.507540, 0.507540, 1.000000};
GLfloat drop_diffuse[] = {0.0, 0.0, 1, 1.000000};
GLfloat drop_specular[] = {0.508273, 0.508273, 0.508273, 1.000000};
GLfloat drop_shininess[] = {51.200001};

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
    float current;

    current = glfwGetTime();
    if (is_anim_running) {

        //Reset After Time is Up
        if(last_timestamp > animTimer && animTimer != 0)
        {
            setTimer = false;
            animTimer = 0;
            windX = 0;
            windY = 0;
        }

        //Set During Time
        if(setTimer)
        {
            windX += 0.001;
            windY += 0.001;
            spinDegree = 0.1;
        }

        /*
        * Rain Animation
        */
        oldZ -=.25;
        if(oldZ == -1){
            oldZ = 18;
        }
        rain_drop_cf = glm::translate(glm::vec3{oldX, oldY, oldZ});

        for(int i = 0; i < STORMSIZE; i++){
            rainArray[i].xCoord += windX/2;
            rainArray[i].yCoord += windY/2;
            if(GRAVITY > 0)
                rainArray[i].zCoord -= GRAVITY;
            if(rainArray[i].zCoord < 0){
                rainArray[i].xCoord = (rand() %  40) - 20;
                rainArray[i].yCoord = (rand() % 40) -30;
                rainArray[i].zCoord = 18+i+1;
            }
            rainArray[i].rainCoord = glm::translate(glm::vec3{rainArray[i].xCoord,
                    rainArray[i].yCoord, rainArray[i].zCoord});
        }
        rainSlant = windX + windY;

        /*
        * Merry Go Round Animation
        */
        if(spinDegree > 0) {
            spinDegree -= 0.001;
            mgr_cf = mgr_cf * glm::rotate(spinDegree, glm::vec3{0, 0, 1});
        }
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

    glEnable (GL_LIGHT2);
    glLightfv (GL_LIGHT2, GL_AMBIENT, light0_color);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, light0_color);
    glLightfv (GL_LIGHT2, GL_SPECULAR, light0_color);
    glDisable(GL_LIGHT2);

    glEnableClientState(GL_VERTEX_ARRAY);
}

/*--------------------------------*
 * GLUT display callback function *
 *--------------------------------*/
void displayCallback (GLFWwindow *win)
{
    /* clear the window */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    /* place the camera using the camera coordinate frame */
    glMultMatrixf (glm::value_ptr(camera_cf));

    /* Specify the reflectance property of the ground using glColor
       (instead of glMaterial....)
     */
    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3ub (0, 92, 9);
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

    /*
    ** Render the Moon
    */
    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glPushMatrix();
    {
        glMultMatrixf(glm::value_ptr(light0_cf));
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
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, drop_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, drop_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, drop_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, drop_shininess);
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


    /*
    ** Render the Merry Go Round
    */
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mgr_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mgr_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mgr_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mgr_shininess);
    glPushMatrix();
    {
        glMultMatrixf(glm::value_ptr(mgr_cf));
        mgr.render();
    }
    glPopMatrix();

    /*
    ** Render the Spotlight
    */
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, lp_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lp_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lp_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, lp_shininess);
    glPopMatrix();
    {
        glMultMatrixf(glm::value_ptr(lp_cf));
        lp.render();
        glPushMatrix();
        {
            glEnable (GL_COLOR_MATERIAL);
            glMultMatrixf(glm::value_ptr(light1_cf));
            spot->render();
            glDisable(GL_COLOR_MATERIAL);
        }
        glPopMatrix();
    }
    glPopMatrix();

    if((rand() %  100) < 2){
        glEnable(GL_LIGHT2);
        last_lightning = glfwGetTime();
    } else{
        if(glfwGetTime() - last_lightning < 10000){
            glDisable(GL_LIGHT2);
        }
    }

    /* to make smooth transition between frame */
    glfwSwapBuffers(win);
}

void myModelInit ()
{
    windX = 0;
    windY = 0;

    sphere1.build(5, 15);
    spot = new Cylinder();
    spot -> build(1 + tan(glm::radians(50.0f)), 1.5, 2.5, {1, 1, 1});

    drop.build();
    mgr.build();
    lp.build();

    active = &camera_cf;

    light0_cf = glm::translate(glm::vec3{-25, 8, 26});
    int counter = 5;
    for(int i = 0; i < STORMSIZE; i++){
        rainArray[i].xCoord = (rand() %  80) - 40;
        rainArray[i].yCoord = (rand() % 80) - 40;
        rainArray[i].zCoord = 18;
        rainArray[i].rainCoord = glm::translate(glm::vec3{rainArray[i].xCoord,
                rainArray[i].yCoord, rainArray[i].zCoord});
    }

    rain_drop_cf = glm::translate(glm::vec3{-12, 4, 18});
    mgr_cf = glm::translate(glm::vec3{0, 0, 0});
    lp_cf = glm::translate(glm::vec3{0, 0, 0});

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
        glDisable(GL_LIGHT2);
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
                spinDegree += 0.01;
                if(windX < 1){
                    windX +=.01;
                }
                break;
            case GLFW_KEY_S:
                spinDegree += 0.01;
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
            case GLFW_KEY_M:
                active = &mgr_cf;
                break;
            case GLFW_KEY_SPACE: /* pause the animation */
                is_anim_running ^= true;
                break;
            case GLFW_KEY_C:
                active = &camera_cf; //Set Camera Active
                break;
            case GLFW_KEY_F:
                GRAVITY -= 0.1f; //Reduce Rain Fall Speed
                break;
            case GLFW_KEY_G:
                GRAVITY += 0.1f; //Increase Rain Fall Speed
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
                spinDegree += 0.01;
                if(windX > -1){
                    windX -=.05;
                }
                break;
            case GLFW_KEY_S:
                spinDegree += 0.01;
                if(windY > -1){
                    windY -=.05;
                }
                break;
            case GLFW_KEY_T:
                animTimer = last_timestamp + 10;
                setTimer = true;
                break;
            case GLFW_KEY_L:
                active = &lp_cf;
                break;
        }
    }
}

/*
    The virtual trackball technique implemented here is based on:
    https://www.opengl.org/wiki/Object_Mouse_Trackball
*/
void cursor_handler (GLFWwindow *win, double xpos, double ypos) {
    int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
    static glm::vec3 first_click;
    static glm::mat4 current_cam;
    static bool is_tracking = false;

    glm::vec3 this_vec;
    if (state == GLFW_PRESS) {
        /* TODO: use glUnproject? */
        float x = (xpos - screen_ctr_x);
        float y = -(ypos - screen_ctr_y);
        float hypot_square = x * x + y * y;
        float z;

        /* determine whether the mouse is on the sphere or on the hyperbolic sheet */
        if (2 * hypot_square < arc_ball_rad_square)
            z = sqrt(arc_ball_rad_square - hypot_square);
        else
            z = arc_ball_rad_square / 2.0 / sqrt(hypot_square);
        if (!is_tracking) {
            /* store the mouse position when the button was pressed for the first time */
            first_click = glm::normalize(glm::vec3{x, y, z});
            current_cam = camera_cf;
            is_tracking = true;
        }
        else {
            /* compute the rotation w.r.t the initial click */
            this_vec = glm::normalize(glm::vec3{x, y, z});
            /* determine axis of rotation */
            glm::vec3 N = glm::cross(first_click, this_vec);

            /* determine the angle of rotation */
            float theta = glm::angle(first_click, this_vec);

            /* create a quaternion of the rotation */
            glm::quat q{cos(theta / 2), sin(theta / 2) * N};
            /* apply the rotation w.r.t to the current camera CF */
            camera_cf = current_cam * glm::toMat4(glm::normalize(q));
        }
        displayCallback(win);
    }
    else {
        is_tracking = false;
    }
}

void scroll_handler (GLFWwindow *win, double xscroll, double yscroll) {
    /* translate along the camera Z-axis */
    glm::mat4 z_translate = glm::translate((float)yscroll * glm::vec3{0, 0, 1});
    camera_cf =  z_translate * camera_cf;
    displayCallback(win);

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
    glfwSetCursorPosCallback(win, cursor_handler);
    glfwSetScrollCallback(win, scroll_handler);
    glfwMakeContextCurrent(win);

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
