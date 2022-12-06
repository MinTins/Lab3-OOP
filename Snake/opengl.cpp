#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "opengl.h"
void initShaders(unsigned int &shaderID);
void checkCompileErrors(unsigned int object, std::string type);
void setProjection(unsigned int shaderID, int width, int height);
void initRenderData(unsigned int &data);
RGBA convertColor(const unsigned char* color);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const char *VSHADER_SOURCE = "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * model * vec4(aPos, 1.0);\n"
    "}\n";
const char *FSHADER_SOURCE = "#version 330 core\n"
    "out vec4 color;\n"
    "uniform vec3 spriteColor;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(spriteColor, 1.0);\n"
    "}\n";
OpenGL::OpenGL(int width, int height, const char *title, bool borders, int margin)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    this->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initShaders(this->shaderProgram);

    setProjection(this->shaderProgram, width, height);

    initRenderData(this->renderData);
}
GL_KEY OpenGL::GetKey()
{
    GL_KEY key = KEY_NONE;

    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        key = KEY_ESC;
    else if (glfwGetKey(this->window, GLFW_KEY_X) == GLFW_PRESS)
        key = KEY_X;
    else if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        key = KEY_W;
    else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        key = KEY_A;
    else if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        key = KEY_S;
    else if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        key = KEY_D;
    else if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
        key = KEY_UP_ARROW;
    else if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        key = KEY_LEFT_ARROW;
    else if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        key = KEY_DOWN_ARROW;
    else if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        key = KEY_RIGHT_ARROW;

    return key;
}


float OpenGL::getTime()
{
    return glfwGetTime();
}

void OpenGL::pollEvents()
{
    glfwPollEvents();
}
void OpenGL::clearWindow(const unsigned char *background)
{
    RGBA rgb = convertColor(background);

    glClearColor(rgb.red, rgb.green, rgb.blue, rgb.alpah);
    glClear(GL_COLOR_BUFFER_BIT);
}
void OpenGL::paintWindow()
{
    glfwSwapBuffers(this->window);
}
void OpenGL::DrawSprite(const float* position, const float* size, const unsigned char* color, float rotate, bool rotateCenter)
{
    glUseProgram(this->shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(*position, *(position+1), 0.0f));

    if (rotateCenter)
        model = glm::translate(model, glm::vec3(0.5f * (*size), 0.5f * (*(size+1)), 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    if (rotateCenter)
        model = glm::translate(model, glm::vec3(-0.5f * (*size), -0.5f * (*(size+1)), 0.0f));

    model = glm::scale(model, glm::vec3(*size, *(size+1), 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, "model"), 1, false, glm::value_ptr(model));

    RGBA rgb = convertColor(color);
    glUniform3f(glGetUniformLocation(this->shaderProgram, "spriteColor"), rgb.red, rgb.green, rgb.blue);
    glBindVertexArray(this->renderData);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void OpenGL::DrawSprite(float position_x, float position_y, float size_x, float size_y,
    int color_r, int color_g, int color_b)
{
    DrawSprite(position_x, position_y, size_x, size_y, 0.0f,
        color_r, color_g, color_b);
}

void OpenGL::DrawSprite(float position_x, float position_y, float size_x, float size_y, float rotate,
    float color_r, float color_g, float color_b)
{
    float position[2]{ position_x, position_y };
    float size[2]{ size_x, size_y };
    unsigned char color[4]{ 0, color_b, color_g, color_r };
    DrawSprite(position, size, color, rotate );
}
void initShaders(unsigned int &shaderID)
{
    unsigned int sVertex, sFragment;
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &VSHADER_SOURCE, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &FSHADER_SOURCE, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    shaderID = glCreateProgram();
    glAttachShader(shaderID, sVertex);
    glAttachShader(shaderID, sFragment);
    glLinkProgram(shaderID);
    checkCompileErrors(shaderID, "PROGRAM");
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}
void checkCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
void setProjection(unsigned int shaderID, int width, int height)
{
    glUseProgram(shaderID);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, false, glm::value_ptr(projection));
}
void initRenderData(unsigned int &data) {
    float vertices[] = {
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3
    };
    unsigned int VBO = 0, EBO = 0;
    glGenVertexArrays(1, &data);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(data);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
RGBA convertColor(const unsigned char* color) {
    RGBA out{ 1.0f, 1.0f, 1.0f, 1.0f };

    if (color) {
        if (*(color + 3) > 1.0f)
            out.red = *(color + 3) / 255.0f;
        else
            out.red = *(color + 3);

        if (*(color + 2) > 1.0f)
            out.green = *(color + 2) / 255.0f;
        else
            out.green = *(color + 2);

        if (*(color + 1) > 1.0f)
            out.blue = *(color + 1) / 255.0f;
        else
            out.blue = *(color + 1);
    }
    return out;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
