#include "HazeGL/Window.hpp"

#include <stdexcept>

void APIENTRY HZ_GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam) {
    (void) length;
    (void) userParam;
    char* _source;
    char* _type;
    char* _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = (char*)"API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = (char*)"WINDOW SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = (char*)"SHADER COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = (char*)"THIRD PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        _source = (char*)"APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        _source = (char*)"UNKNOWN";
        break;
    default:
        _source = (char*)"UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = (char*)"ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = (char*)"DEPRECATED BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = (char*)"UDEFINED BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        _type = (char*)"PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = (char*)"PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        _type = (char*)"OTHER";
        break;
    case GL_DEBUG_TYPE_MARKER:
        _type = (char*)"MARKER";
        break;
    default:
        _type = (char*)"UNKNOWN";
        break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
        _severity = (char*)"HIGH";
        break;

        case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = (char*)"MEDIUM";
        break;

        case GL_DEBUG_SEVERITY_LOW:
        _severity = (char*)"LOW";
        break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = (char*)"NOTIFICATION";
        break;

        default:
        _severity = (char*)"UNKNOWN";
        break;
    }

    printf("%d: %s of %s severity, raised from %s: %s\n",
            id, _type, _severity, _source, message);
}

HazeGL::Window::Window(uint32_t width, uint32_t height, std::string title) {
    mWidth = width;
    mHeight = height;
    mTitle = title;
    mDebug = false;
}

void HazeGL::Window::setDebug(bool debug) {
    mDebug = debug;
}

uint32_t HazeGL::Window::getWidth() const {
    return mWidth;
}

uint32_t HazeGL::Window::getHeight() const {
    return mHeight;
}

std::string HazeGL::Window::getTitle() const {
    return mTitle;
}

GLFWwindow* HazeGL::Window::getWindow() const {
    return mWindow;
}

void HazeGL::Window::enableBlending () {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool HazeGL::Window::shouldClose() const {
    return glfwWindowShouldClose(mWindow);
}

void HazeGL::Window::swapBuffers() const {
    glfwSwapBuffers(mWindow);
}

void HazeGL::Window::initialize(uint32_t version/*=450*/, uint32_t profile/*=GLFW_OPENGL_CORE_PROFILE*/) {
    initializeGLFW(version, profile);
    initializeWindow();
    createGLContext();
    initializeGLAD();

    if (mDebug) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(HZ_GLDebugMessageCallback, 0);
    }
}

void HazeGL::Window::initializeGLFW(uint32_t version/*=450*/, uint32_t profile/*=GLFW_OPENGL_CORE_PROFILE*/) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (version - (version % 100)) / 100);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (version % 100) / 10);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
}

void HazeGL::Window::initializeWindow() {
    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
    if (!mWindow) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
}

void HazeGL::Window::createGLContext() {
    glfwMakeContextCurrent(mWindow);
}

void HazeGL::Window::initializeGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}


void HazeGL::Window::getMousePosition(float* x, float* y) {
    double xpos, ypos;
    glfwGetCursorPos(mWindow, &xpos, &ypos);
    *x = (float)xpos;
    *y = (float)ypos;
}

void HazeGL::Window::getMousePosition(double* x, double* y) {
    glfwGetCursorPos(mWindow, x, y);
}

void HazeGL::Window::getMousePosition(uint32_t* x, uint32_t* y) {
    double xpos, ypos;
    glfwGetCursorPos(mWindow, &xpos, &ypos);
    *x = (uint32_t)xpos;
    *y = (uint32_t)ypos;
}
