#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "utils/math.h"
#include "utils/gl_helper.h"

#include "HairSimulation.h"
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
vec3 force_generated(0.0f, 0.0f, 0.0f);
bool gravityOn = true;
bool reset_external_forces = false;
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
    force_generated += force;
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
            reset_external_forces = true;
            gravityOn = false;
            std::cout << "gravity turned OFF" << std::endl;
        } else {
            reset_external_forces = true;
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

Mouse prev_mouse_pos(0, 0);

static void MouseMotionCallback(GLFWwindow *window, double x, double y) {
    if (!dragging) {
        prev_mouse_pos = Mouse(x,y);
        return;
    }
    Mouse curr_mouse((float) x, (float) y);
    vec2 delta_mouse(curr_mouse.x - prev_mouse_pos.x, curr_mouse.y - prev_mouse_pos.y);
    prev_mouse_pos = curr_mouse;
    vec3 force((float) delta_mouse.x/1.0f, (float) delta_mouse.y/1.0f, 0.0f);
    force_generated += force;
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


    int image_nr = 0;
    double time_elapsed;

    Shader* basicShader = new Shader();
    basicShader->use();

    Camera camera(vec3(0, 0, 8), // Camera position (wEye)
                  vec3(0, 0, 0), // wLookat
                  vec3(0, 1, 0), // wVup
                  WIDTH, HEIGHT);

    vec3 head_center(0.0f, 0.55f, 1.0f);
    size_t nr_sims = 200;
    size_t nr_segments = 30;
    float l_seg = 0.025f;
    auto hair_simulation = new HairSimulation(head_center, nr_sims, nr_segments, l_seg);

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

    HairSimulationObject hairSim(basicShader, hair_simulation);

    // MAIN LOOP
    while (!glfwWindowShouldClose(window)) {
        // ticking every 24 FPS
        bool tick = false;

        double delta_time = glfwGetTime() - time_elapsed;
        time_elapsed = glfwGetTime();

        if (time_elapsed >= 1.0f / 24.0f) {
            glfwSetTime(0.0f);
            time_elapsed = 0.0f;
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
        if(reset_external_forces || !dragging) {
            force_generated += -hair_simulation->get_external_forces();
            if(gravityOn)
                force_generated += vec3(0.0f, -GRAVITY_ABS_VALUE, 0.0f);
            reset_external_forces = false;
        }

        //if there was force generated...
        // ... add it to the simulation
        if (force_generated.x != 0.0f ||
            force_generated.y != 0.0f ||
            force_generated.z != 0.0f)
        {
            hair_simulation->add_force_to_all_sims(force_generated);
            // ... and reset the force generated
            force_generated = vec3(0,0,0);
        }

        hair_simulation->update((float)delta_time);
        hair_simulation->draw();

        if (capturing) {
            glPointSize(20.0f);
            glBegin(GL_POINTS);
            glColor3f(200.0f, 0.0f, 0.0f);
            glVertex3f(-0.8f, -0.8f, 1.0f);
            glEnd();
        }

        if (tick && capturing) {
            char path[100];
            sprintf(path, "../renders/render%04d.bmp", image_nr++);
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
