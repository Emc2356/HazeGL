#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <string>

#include <stdint.h>

namespace HazeGL {
    class Mouse;

    class Window {
    public:
        Window(uint32_t width, uint32_t height, std::string title);
        ~Window();

        void initialize(uint32_t version=450, uint32_t profile=GLFW_OPENGL_CORE_PROFILE);
        void initializeGLFW(uint32_t version=450, uint32_t profile=GLFW_OPENGL_CORE_PROFILE);
        void initializeWindow();
        void createGLContext();
        void initializeGLAD();

        void setDebug(bool);

        void enableBlending();

        bool isOpen() const;
        void swapBuffers() const;

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        std::string getTitle() const;
        GLFWwindow* getWindow() const;

        void setTitle(const std::string&);
        void updateTitle(bool showMsPerFrame=false, uint32_t msPerUpdate=1000);

        HazeGL::Mouse* getMouse();

        void* getWindowPosCallbackPointer() const;
        void* getWindowResizeCallbackPointer() const;
        void* getWindowCloseCallbackPointer() const;
        void* getWindowFocusCallbackPointer() const;
        void* getWindowMaximizeCallbackPointer() const;
        void* getFrameBufferResizeCallbackPointer() const;
        void* getKeyCallbackPointer() const;
        void* getMouseButtonCallbackPointer() const;
        void* getCursorPosCallbackPointer() const;
        void* getCursorEnterCallbackPointer() const;
        void* getScrollCallbackPointer() const;
        void* getDropCallbackPointer() const;

        void setWindowPosCallback(GLFWwindowposfun proc, void* pointer=nullptr);
        void setWindowResizeCallback(GLFWwindowsizefun proc, void* pointer=nullptr);
        void setWindowCloseCallback(GLFWwindowclosefun proc, void* pointer=nullptr);
        void setWindowFocusCallback(GLFWwindowfocusfun proc, void* pointer=nullptr);
        void setWindowMaximizeCallback(GLFWwindowmaximizefun proc, void* pointer=nullptr);
        void setFrameBufferResizeCallback(GLFWframebuffersizefun proc, void* pointer=nullptr);
        void setKeyCallback(GLFWkeyfun proc, void* pointer=nullptr);
        void setMouseButtonCallback(GLFWmousebuttonfun proc, void* pointer=nullptr);
        void setCursorPosCallback(GLFWcursorposfun proc, void* pointer=nullptr);
        void setCursorEnterCallback(GLFWcursorenterfun proc, void* pointer=nullptr);
        void setScrollCallback(GLFWscrollfun proc, void* pointer=nullptr);
        void setDropCallback(GLFWdropfun proc, void* pointer=nullptr);
    private:
        struct PointerData {
            void* windowPosCallbackPointer=nullptr;
            void* windowResizeCallbackPointer=nullptr;
            void* windowCloseCallbackPointer=nullptr;
            void* windowFocusCallbackPointer=nullptr;
            void* windowMaximizeCallbackPointer=nullptr;
            void* frameBufferResizeCallbackPointer=nullptr;
            void* keyCallbackPointer=nullptr;
            void* mouseButtonCallbackPointer=nullptr;
            void* cursorPosCallbackPointer=nullptr;
            void* cursorEnterCallbackPointer=nullptr;
            void* scrollCallbackPointer=nullptr;
            void* dropCallbackPointer=nullptr;
        };

        void displayMsPerFrame(bool);

        GLFWwindow* mWindow;
        uint32_t mWidth;
        uint32_t mHeight;
        std::string mTitle;
        HazeGL::Mouse* mMouse;
        bool mDebug;
        PointerData mPointers;
    friend class HazeGL::Mouse;
    };
}