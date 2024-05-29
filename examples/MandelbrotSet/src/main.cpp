#include <HazeGL/HazeGL>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

#include <stdio.h>
#include <stdint.h>


struct MandelbrotData {
    float reStart;
    float reEnd;
    float imStart;
    float imEnd;
    float infinity;
    uint32_t precision;
    glm::vec2 resolution;
};


std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + path);
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);
    return buffer;
}


void onMouseAction(GLFWwindow* windowPtr, int button, int action, int mods) {
    (void) mods;
    bool zoom;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        zoom = true;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        zoom = false;
    } else {
        return;
    }
    HazeGL::Window* window = (HazeGL::Window*)(glfwGetWindowUserPointer(windowPtr));
    HazeGL::Mouse* mouse = window->getMouse();
    MandelbrotData* data = (MandelbrotData*)window->getMouseButtonCallbackPointer();
    float mX, mY;
    mouse->getPosition(&mX, &mY);

    float x = (mX / data->resolution.x) * (data->reEnd - data->reStart) + data->reStart;
    float y = (mY / data->resolution.y) * (data->imEnd - data->imStart) + data->imStart;

    float newWidth;
    float newHeight;

    if (zoom) {
        newWidth  = (data->reEnd - data->reStart) / 1.5;
        newHeight = (data->imEnd - data->imStart) / 1.5;
    } else {
        newWidth  = (data->reEnd - data->reStart) * 1.5;
        newHeight = (data->imEnd - data->imStart) * 1.5;
    }


    data->reStart = x - newWidth / 2;
    data->reEnd = x + newWidth / 2;
    data->imStart = y - newHeight / 2;
    data->imEnd = y + newHeight / 2;
}


void onKeyPress(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
    (void) scancode;
    (void) mods;
    bool increase;
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        increase = true;
    } else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        increase = false;
    } else {
        return;
    }
    HazeGL::Window* window = (HazeGL::Window*)(glfwGetWindowUserPointer(windowPtr));
    MandelbrotData* data = (MandelbrotData*)window->getKeyCallbackPointer();

    if (increase) {
        data->precision += 100;
    } else {
        if (data->precision > 100) {
            data->precision -= 100;
        } else {
            data->precision = 1;
        }
    }
}


int main(void) {
    uint32_t WIDTH = 1280;
    uint32_t HEIGHT = 720;
    HazeGL::Window window(WIDTH, HEIGHT, "Mandelbrot Set");

    MandelbrotData mandelbrotData = {
        -2.0f, 1.0f,
        -1.0f, 1.0f,
        2.0f, 500,
        glm::vec2((float)WIDTH, (float)HEIGHT)
    };

    window.setDebug(true);
    window.initialize(450, GLFW_OPENGL_CORE_PROFILE);

    window.setWindowResizeCallback([](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); });
    window.setMouseButtonCallback(onMouseAction, &mandelbrotData);
    window.setKeyCallback(onKeyPress, &mandelbrotData);

    window.enableBlending();

    glm::mat4 projection = glm::ortho(
        0.0f, (float)window.getWidth(),
        (float)window.getHeight(), 0.0f,
        -1.0f, 1.0f
    );

    std::vector<float> verticies = {
        0.0f                    , (float)window.getHeight(), // bottom left
        0.0f                    , 0.0f                     , // top left
        (float)window.getWidth(), 0.0f                     , // top right
        (float)window.getWidth(), (float)window.getHeight(), // bottom right
    };

    std::vector<uint32_t> indicies = {
        0, 1, 2,
        2, 3, 0,
    };

    HazeGL::VertexArray vao;
    HazeGL::VertexBuffer vbo(verticies);
    HazeGL::IndexBuffer ibo(indicies);

    HazeGL::VertexBufferLayout layout;
    layout.pushFloat(2); // xy

    vao.addBuffer(vbo, layout);

    HazeGL::Shader shader(
        readFile("./examples/MandelbrotSet/src/glsl/mandelbrot.vert"),
        readFile("./examples/MandelbrotSet/src/glsl/mandelbrot.frag")
    );

    shader.setUniform("uMVP", projection);
    shader.setUniform("uResolution", glm::vec2(static_cast<float>(WIDTH), static_cast<float>(HEIGHT)));

    HazeGL::Renderer renderer;

    while (window.isOpen()) {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.setUniform("uPrecision", mandelbrotData.precision);
        shader.setUniform("uInfinity", mandelbrotData.infinity);
        shader.setUniform("uReStart", mandelbrotData.reStart);
        shader.setUniform("uReEnd", mandelbrotData.reEnd);
        shader.setUniform("uImStart", mandelbrotData.imStart);
        shader.setUniform("uImEnd", mandelbrotData.imEnd);

        renderer.draw(vao, ibo, shader);

        window.setTitle("Mandelbrot Set - Precision: " + std::to_string(mandelbrotData.precision));

        window.updateTitle(false, 500);

        window.swapBuffers();
        
        glfwPollEvents();
    }

    return 0;
}