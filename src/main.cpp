#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "utils/math.h"
#include "utils/gl_helper.h"

#include "PBDSimulation.h"
#include "utils/other/stb_image_write.h"
#include "utils/save_image.h"
#include "Shader.h"
#include "Camera.h"
#include "HairSimulationObject.h"

static int WIDTH = 600;
static int HEIGHT = 400;
GLFWwindow *window;

bool dragging = false;
int keyArr[350];

static float GRAVITY_ABS_VALUE = 0.98f;
vec3 forceGenerated(0.0f, 0.0f, 0.0f);
bool gravityOn = true;
bool resetExternalForces = false;
bool capturing = false;

static void Initialize() {
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(196.0f / 255.0f, 233.0f / 255.0f, 241.0f / 255.0f, 1.0f);
}

static void Update(GLFWwindow *window, float delta) {
    if (keyArr[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, 1);
}

static void RenderScene(GLFWwindow *window, float delta) {

}

static void Resize(GLFWwindow *window, int w, int h) {
    if (h < 1)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float) w / (float) h, 0.1f, 1000.0f);
    gluLookAt(0.0f, 0.0f, 30, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
}

static void getForce(char dir, float power = 0.05f) {
    vec3 force;
    switch (dir) {
        case 'x':
            force = power * vec3(-1.f, 0.f, .0f);
            break;
        case 'X':
            force = power * vec3(1.f, 0.f, .0f);
            break;
        case 'y':
            force = power * vec3(.0f, -1.f, .0f);
            break;
        case 'Y':
            force = power * vec3(.0f, 1.f, .0f);
            break;
        case 'z':
            force = power * vec3(.0f, .0f, -1.f);
            break;
        case 'Z':
            force = power * vec3(.0f, .0f, 1.f);
            break;
    }
    forceGenerated += force;
}

static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE) return;
    if (key == GLFW_KEY_X)
        if (mods && GLFW_MOD_SHIFT)
            getForce('X');
        else getForce('x');
    if (key == GLFW_KEY_Y)
        if (mods && GLFW_MOD_SHIFT)
            getForce('Y');
        else getForce('y');
    if (key == GLFW_KEY_Z)
        if (mods && GLFW_MOD_SHIFT)
            getForce('Z');
        else getForce('Z');
    if (key == GLFW_KEY_G) {
        // gravity
        if (gravityOn) {
            // reset all external forces
            resetExternalForces = true;
            gravityOn = false;
            std::cout << "gravity turned OFF" << std::endl;
        } else {
            resetExternalForces = true;
            gravityOn = true;
            std::cout << "gravity turned ON" << std::endl;
        }
    }
    if (key == GLFW_KEY_C) {
        if (capturing) {
            capturing = false;
            std::cout << "Capturing stopped." << std::endl;
        } else {
            capturing = true;
            std::cout << "Capturing started." << std::endl;
        }
    }

}

static void MouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            dragging = (action == GLFW_PRESS);
            std::cout << dragging<< std::endl;
            break;
    }
}

struct Mouse {
    float x;
    float y;

    Mouse(float x, float y) {
        this->x = x / WIDTH;
        this->y = 1 - (y / HEIGHT);
    }
};

Mouse prevMousePos(0, 0);

static void MouseMotionCallback(GLFWwindow *window, double x, double y) {
    if (!dragging) {
        prevMousePos = Mouse(x, y);
        return;
    }
    Mouse currMouse((float) x, (float) y);
    vec2 deltaMouse(currMouse.x - prevMousePos.x, currMouse.y - prevMousePos.y);
    prevMousePos = currMouse;
    vec3 force((float) deltaMouse.x / 1.0f, (float) deltaMouse.y / 1.0f, 0.0f);
    forceGenerated += force;
}


int main(int argc, char **argv) {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return 1;
    }

    glfwInit();
    window = glfwCreateWindow(WIDTH, HEIGHT, "Brave-2", NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION); // version as a string
    std::cerr << "Renderer: " << renderer << std::endl;
    std::cerr << "OpenGL version supported: " << version << std::endl;

    glfwSetWindowSizeCallback(window, Resize);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseClickCallback);
    glfwSetCursorPosCallback(window, MouseMotionCallback);

    srand(time(NULL));

    // Initialize
    Initialize();


    int imageNr = 0;
    double timeElapsed;

    Shader* basicShader = new Shader();
    basicShader->use();

    Camera camera(vec3(0, -0.2f, 1), // Camera position (wEye)
                  vec3(0, -.2f, 0), // wLookat
                  vec3(0, 1, 0), // wVup
                  WIDTH, HEIGHT);

    vec3 headCenter(0.0f, 0.0f, 0.0f);
    size_t nrSims = 200;
    size_t nrSegments = 30;
    float lSeg = 0.025f;
    auto hairSimulation = new PBDSimulation(headCenter, nrSims, nrSegments, lSeg);

    mat4 M(1.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f);

    mat4 V = camera.getState().V;
    mat4 P = camera.getState().P;
    mat4 MVP = M*V*P;
    std::cout << "M: " << std::endl << M << std::endl;
    std::cout << "V: " << std::endl << V << std::endl;
    std::cout << "P: " << std::endl << P << std::endl;
    std::cout << "MVP: " << std::endl << MVP << std::endl;
    glUniformMatrix4fv(glGetUniformLocation(basicShader->ID, "MVP"), 1, GL_TRUE, MVP);

    mat4 value;
    glGetUniformfv(basicShader->ID, glGetUniformLocation(basicShader->ID, "MVP"), (float*)&value);

    std::cout << "Returned from GPU:" << std::endl << value << std::endl;

//    HairSimulationObject hairSim(basicShader, hairSimulation);

    // MAIN LOOP
    while (!glfwWindowShouldClose(window)) {
        // ticking every 24 FPS
        bool tick = false;

        double deltaTime = glfwGetTime() - timeElapsed;
        timeElapsed = glfwGetTime();

        if (timeElapsed >= 1.0f / 24.0f) {
            glfwSetTime(0.0f);
            timeElapsed = 0.0f;
            tick = true;
        }

        //clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity(); //load identity matrix

        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT);

//        glUseProgram(shader_programme);
//        draw();

        // reset all external forces if gravity was toggled
        if(resetExternalForces || !dragging) {
            forceGenerated += -hairSimulation->getExternalForces();
            if(gravityOn)
                forceGenerated += vec3(0.0f, -GRAVITY_ABS_VALUE, 0.0f);
            resetExternalForces = false;
        }

        //if there was force generated...
        // ... add it to the simulation
        if (forceGenerated.x != 0.0f ||
            forceGenerated.y != 0.0f ||
            forceGenerated.z != 0.0f)
        {
            hairSimulation->addForce(forceGenerated);
            // ... and reset the force generated
            forceGenerated = vec3(0, 0, 0);
        }

        hairSimulation->update((float) deltaTime);
        hairSimulation->draw();

        if (capturing) {
            glPointSize(20.0f);
            glBegin(GL_POINTS);
            glColor3f(200.0f, 0.0f, 0.0f);
            glVertex3f(-0.8f, -0.8f, 1.0f);
            glEnd();
        }

        if (tick && capturing) {
            char path[100];
            sprintf(path, "../renders/render%04d.bmp", imageNr++);
            saveImage(path, window);
            std::cout << path << ".bmp printed" << std::endl;
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
