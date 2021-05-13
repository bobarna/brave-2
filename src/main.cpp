#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#include "utils/math.h"

#include "utils/other/stb_image_write.h"
#include "utils/save_image.h"
#include "rendering/Scene.h"
#include "Constants.h"
#include "utils/InputHandler.h"

GLFWwindow *window;

bool dragging = false;
int keyArr[350];

InputHandler *InputHandler = InputHandler::GetInstance();

vec3 forceGenerated(0.0f, 0.0f, 0.0f);
bool resetExternalForces = false;
bool capturing = false;


Scene Scene(WIDTH, HEIGHT);


static void Initialize() {
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glLoadIdentity();
//    glClearColor(196.0f / 255.0f, 233.0f / 255.0f, 241.0f / 255.0f, 1.0f);
    glClearColor(1,1,1, 1.0f);
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
    vec3 force(0.0f, 0.0f, 0.0f);
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
        default:
            break;
    }
    forceGenerated += force;
}

static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE)
        InputHandler->KeyRelease(key);
    else
        InputHandler->KeyPress(key);

    InputHandler->SetModifiers(mods);

    if (action == GLFW_RELEASE && key == GLFW_KEY_C) {
        if (capturing) capturing = false;
        else capturing = true;
    }

}

static void MouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            dragging = (action == GLFW_PRESS);
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
    Scene.Build();

    int imageNr = 0;
    double timeElapsed;

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

        // reset all external forces if gravity was toggled
        if (resetExternalForces || !dragging) {
            Scene.ResetExternalForces();
            resetExternalForces = false;
        }

        //if there was force generated...
        // ... add it to the simulation
        if (forceGenerated.x != 0.0f ||
            forceGenerated.y != 0.0f ||
            forceGenerated.z != 0.0f) {
            Scene.addForce(forceGenerated);
            // ... and reset the force generated
            forceGenerated = vec3(0, 0, 0);
        }


        //TODO guarantee dt to be infinitesimal
        Scene.Update(deltaTime);
        Scene.Render();

        // big red dot in lower left corner
//        if (capturing) {
//            glPointSize(20.0f);
//            glBegin(GL_POINTS);
//            glColor3f(200.0f, 0.0f, 0.0f);
//            glVertex3f(-0.8f, -0.8f, 1.0f);
//            glEnd();
//        }

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
