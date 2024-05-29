#include "HazeGL/Window.hpp"
#include "HazeGL/Mouse.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>


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
    mMouse = nullptr;
    mPointers = {};
}

HazeGL::Window::~Window() {
    if (mMouse!= nullptr) {
        delete mMouse;
    }
    glfwTerminate();
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

void HazeGL::Window::setTitle(const std::string& title) {
    mTitle = title;
}

void HazeGL::Window::displayMsPerFrame(bool updateOnlyVariables) {
    static double lastTime;
    static uint32_t nbFrames = 0;

    if (updateOnlyVariables) {
        nbFrames++;
        return;
    }

    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    if (delta >= 0.5) { // If last cout was more than 0.5 sec ago
        std::stringstream result;

        result << mTitle << " [" << 1000.0/double(nbFrames) << " ms]";
        
        glfwSetWindowTitle(mWindow, result.str().c_str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}

void HazeGL::Window::updateTitle(bool showMsPerFrame/*=true*/, uint32_t msPerUpdate/*=1000*/) {
    static double lastTime;
    
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;

    if (delta >= float(msPerUpdate) / 1000.0) {
        if (showMsPerFrame) {
            displayMsPerFrame(false);
        } else {
            glfwSetWindowTitle(mWindow, mTitle.c_str());
        }

        lastTime = currentTime;
    } else {
        displayMsPerFrame(true);
    }
}

void HazeGL::Window::enableBlending() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool HazeGL::Window::isOpen() const {
    return !glfwWindowShouldClose(mWindow);
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
    glfwSetWindowUserPointer(mWindow, (void*)this);
}

void HazeGL::Window::createGLContext() {
    glfwMakeContextCurrent(mWindow);
}

void HazeGL::Window::initializeGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

HazeGL::Mouse* HazeGL::Window::getMouse() {
    if (mMouse == nullptr) {
        mMouse = new Mouse(this);
    }
    return mMouse;
}


void HazeGL::Window::setWindowPosCallback(GLFWwindowposfun proc, void* pointer/*=nullptr*/) {
    glfwSetWindowPosCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.windowPosCallbackPointer = pointer;
    }
}

void HazeGL::Window::setWindowResizeCallback(GLFWwindowsizefun proc, void* pointer/*=nullptr*/) {
    glfwSetWindowSizeCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.windowResizeCallbackPointer = pointer;
    }
}

void HazeGL::Window::setWindowCloseCallback(GLFWwindowclosefun proc, void* pointer/*=nullptr*/) {
    glfwSetWindowCloseCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.windowCloseCallbackPointer = pointer;
    }
}

void HazeGL::Window::setWindowFocusCallback(GLFWwindowfocusfun proc, void* pointer/*=nullptr*/) {
    glfwSetWindowFocusCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.windowFocusCallbackPointer = pointer;
    }
}

void HazeGL::Window::setWindowMaximizeCallback(GLFWwindowmaximizefun proc, void* pointer/*=nullptr*/) {
    glfwSetWindowMaximizeCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.windowMaximizeCallbackPointer = pointer;
    }
}

void HazeGL::Window::setFrameBufferResizeCallback(GLFWframebuffersizefun proc, void* pointer/*=nullptr*/) {
    glfwSetFramebufferSizeCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.frameBufferResizeCallbackPointer = pointer;
    }
}

void HazeGL::Window::setKeyCallback(GLFWkeyfun proc, void* pointer/*=nullptr*/) {
    glfwSetKeyCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.keyCallbackPointer = pointer;
    }
}

void HazeGL::Window::setMouseButtonCallback(GLFWmousebuttonfun proc, void* pointer/*=nullptr*/) {
    glfwSetMouseButtonCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.mouseButtonCallbackPointer = pointer;
    }
}

void HazeGL::Window::setCursorPosCallback(GLFWcursorposfun proc, void* pointer/*=nullptr*/) {
    glfwSetCursorPosCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.cursorPosCallbackPointer = pointer;
    }
}

void HazeGL::Window::setCursorEnterCallback(GLFWcursorenterfun proc, void* pointer/*=nullptr*/) {
    glfwSetCursorEnterCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.cursorEnterCallbackPointer = pointer;
    }
}

void HazeGL::Window::setScrollCallback(GLFWscrollfun proc, void* pointer/*=nullptr*/) {
    glfwSetScrollCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.scrollCallbackPointer = pointer;
    }
}

void HazeGL::Window::setDropCallback(GLFWdropfun proc, void* pointer/*=nullptr*/) {
    glfwSetDropCallback(mWindow, proc);
    if (pointer != nullptr) {
        mPointers.dropCallbackPointer = pointer;
    }
}

void* HazeGL::Window::getWindowPosCallbackPointer() const { return mPointers.windowPosCallbackPointer; }
void* HazeGL::Window::getWindowResizeCallbackPointer() const { return mPointers.windowResizeCallbackPointer; }
void* HazeGL::Window::getWindowCloseCallbackPointer() const { return mPointers.windowCloseCallbackPointer; }
void* HazeGL::Window::getWindowFocusCallbackPointer() const { return mPointers.windowFocusCallbackPointer; }
void* HazeGL::Window::getWindowMaximizeCallbackPointer() const { return mPointers.windowMaximizeCallbackPointer; }
void* HazeGL::Window::getFrameBufferResizeCallbackPointer() const { return mPointers.frameBufferResizeCallbackPointer; }
void* HazeGL::Window::getKeyCallbackPointer() const { return mPointers.keyCallbackPointer; }
void* HazeGL::Window::getMouseButtonCallbackPointer() const { return mPointers.mouseButtonCallbackPointer; }
void* HazeGL::Window::getCursorPosCallbackPointer() const { return mPointers.cursorPosCallbackPointer; }
void* HazeGL::Window::getCursorEnterCallbackPointer() const { return mPointers.cursorEnterCallbackPointer; }
void* HazeGL::Window::getScrollCallbackPointer() const { return mPointers.scrollCallbackPointer; }
void* HazeGL::Window::getDropCallbackPointer() const { return mPointers.dropCallbackPointer; }