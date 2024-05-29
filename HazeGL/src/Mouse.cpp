#include "HazeGL/Mouse.hpp"
#include "HazeGL/Window.hpp"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

HazeGL::Mouse::Mouse(const HazeGL::Window* window) {
    mWindow = window;
    mLastX = 0;
    mLastY = 0;
}

void HazeGL::Mouse::getPosition(float* x, float* y) {
    glfwGetCursorPos(mWindow->mWindow, &mLastX, &mLastY);

    *x = MAX(0, MIN(mLastX, (float)mWindow->mWidth));
    *y = MAX(0, MIN(mLastY, (float)mWindow->mHeight));
}

void HazeGL::Mouse::getPosition(double* x, double* y) {
    glfwGetCursorPos(mWindow->mWindow, &mLastX, &mLastY);

    *x = MAX(0, MIN(mLastX, (double)mWindow->mWidth));
    *y = MAX(0, MIN(mLastY, (double)mWindow->mHeight));
}

void HazeGL::Mouse::getPosition(uint32_t* x, uint32_t* y) {
    glfwGetCursorPos(mWindow->mWindow, &mLastX, &mLastY);

    *x = (uint32_t)MAX(0, MIN(mLastX, mWindow->mWidth));
    *y = (uint32_t)MAX(0, MIN(mLastY, mWindow->mHeight));
}
