#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "utils/math.h"
#include "utils/gl_helper.h"

#include "FTL_Simulation.h"
#include "HairSimulation.h"
#include "utils/other/stb_image_write.h"
#include "utils/save_image.h"

static int WIDTH = 600;
static int HEIGHT = 400;
//double rotatex = 0, rotatey = 0, mousex = 0, mousey = 0;
bool dragging = false;
int keyArr[350];

vec3 force_generated(0.0f, 0.0f, 0.0f);
bool gravityOn = false;
bool capturing = false;

static void Initialize() {
    glViewport(0,0,WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(196.0f / 255.0f, 233.0f / 255.0f, 241.0f / 255.0f, 1.0f);
}

static void Update(GLFWwindow *window, float delta) {
    if (keyArr[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, 1);
//    rotatex += keyArr[GLFW_KEY_LEFT] - keyArr[GLFW_KEY_RIGHT];
//    rotatey += keyArr[GLFW_KEY_UP] - keyArr[GLFW_KEY_DOWN];
}

static void RenderScene(GLFWwindow *window, float delta) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);

    glBegin(GL_LINE_LOOP);
    glVertex2f(0.25, 0.25);
    glVertex2f(0.75, 0.25);
    glVertex2f(0.75, 0.75);
    glVertex2f(0.25, 0.75);
    glEnd();
    glFlush();
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
//    std::cout << force.x << " " << force.y << " " << force.z << std::endl;
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
        gravityOn ? gravityOn=false : gravityOn=true;
        std::cout << "gravity switched" << std::endl;
    }
    if (key == GLFW_KEY_C) {
        if(capturing) {
            capturing = false;
            std::cout << "Capturing stopped." << std::endl;
        } else
        {
            capturing = true;
            std::cout << "Capturing started." << std::endl;
        }
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

Mouse prev_mouse_pos(0, 0);

static void MouseMotionCallback(GLFWwindow *window, double x, double y) {
    if (!dragging) return;
    Mouse curr_mouse((float) x, (float) y);
    vec2 delta_mouse(curr_mouse.x - prev_mouse_pos.x, curr_mouse.y - prev_mouse_pos.y);
    prev_mouse_pos = curr_mouse;
//    std::cout << delta_mouse.x << " " << delta_mouse.y << std::endl;
    vec3 force((float) delta_mouse.x/3.0f, (float) delta_mouse.y/3.0f, 0.0f);
    force_generated += force;
}

const char *vertex_shader = R"(
    #version 330 core
        layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
        layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

        out vec3 ourColor; // output a color to the fragment shader

        void main()
        {
            gl_Position = vec4(aPos, 0, 1);
            ourColor = aColor; // set ourColor to the input color we got from the vertex data
        }
    )";

const char *fragment_shader = R"(
        #version 330 core
        in vec3 ourColor;
        out vec4 fragmentColor;

        void main()
        {
            fragmentColor = vec4(ourColor, 1);
        }
    )";

void initVbo(GLuint &vao) {
    float points[] = {
            0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
            -0.2f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.2f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(float), points, GL_STATIC_DRAW);

    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) (2 * sizeof(float)));
}

void initShader(GLuint &shader_programme) {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

GLuint vao;

void draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char **argv) {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return 1;
    }
    GLFWwindow *window;

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

//
//    glfwSetWindowSizeCallback(window, Resize);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseClickCallback);
    glfwSetCursorPosCallback(window, MouseMotionCallback);


//    initVbo(vao);
//    GLuint shader_programme;
//    initShader(shader_programme);


    srand(time(NULL));

    // Initialize
    Initialize();
    vec3 head_center(0.0f, 0.55f, 1.0f);
//    HairSimulation hair_simulation(head_center, 400, 30, 0.025f);
    HairSimulation hair_simulation(head_center, 200, 15, 0.05f);

//    vec3 center(0, 0, 1.0f);
//    HairSimulation hair_simulation(center, 1, 50, 0.005f);

    int image_nr = 0;
    double time_elapsed = 0.0f;
    while (!glfwWindowShouldClose(window)) {

        // ticking every 24 FPS
        bool tick = false;
        double time_elapased = glfwGetTime();
        if(time_elapased >= 1.0f/24.0f) {
            glfwSetTime(0.0f);
            tick = true;
        }

        //clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();//load identity matrix

        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT);

//        glUseProgram(shader_programme);
//        draw();
//        force_generated = vec3(0,-0.5f,0);
        if( force_generated.x != 0.0f ||
            force_generated.y != 0.0f ||
            force_generated.z != 0.0f )
            hair_simulation.add_force_to_all_sims(force_generated);

        // gravity
        if(gravityOn) {
            hair_simulation.add_force_to_all_sims(vec3(0,-0.5, 0));
            gravityOn = false;
        }

        // resetting generated force
        force_generated = vec3(0, 0, 0);
        hair_simulation.update(0.08f);

        hair_simulation.draw();

        if(capturing) {
            glPointSize(20.0f);
            glBegin(GL_POINTS);
            glColor3f(200.0f, 0.0f, 0.0f);
            glVertex3f(-0.8f, -0.8f, 1.0f);
            glEnd();
        }

        if(tick && capturing) {
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
