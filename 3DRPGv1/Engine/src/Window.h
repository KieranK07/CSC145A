#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool shouldClose() { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }
    void pollEvents() { glfwPollEvents(); }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    GLFWwindow* getNativeWindow() const { return window; }

private:
    GLFWwindow* window;
    int width, height;
};