#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define M_PI 3.14159265358979323846

// Função para carregar os shaders de arquivo
std::string loadShaderSource(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo do shader: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf(); // Lê o arquivo inteiro para o buffer
    return buffer.str();    // Retorna o conteúdo como string
}

// Função para criar e compilar o shader
GLuint createShaderProgram(const std::string &vertexShaderCode, const std::string &fragmentShaderCode) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexSource = vertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, log);
        std::cerr << "Erro na compilação do Vertex Shader:\n" << log << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSource = fragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
        std::cerr << "Erro na compilação do Fragment Shader:\n" << log << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, log);
        std::cerr << "Erro ao ligar o programa de shaders:\n" << log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Função de callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 600, height = 600;
    GLFWwindow *window = glfwCreateWindow(width, height, "ShaderToy em OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Carregar shaders
    std::string vertexShaderSource = loadShaderSource("shaders/vertex_shader_art.glsl");
    std::string fragmentShaderSource = loadShaderSource("shaders/fragment_shader_art.glsl");

    if (vertexShaderSource.empty() || fragmentShaderSource.empty()) {
        std::cerr << "Falha ao carregar os shaders" << std::endl;
        return -1;
    }

    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    float vertices[] = {
        // Posição          // Coordenadas de textura
        -1.0f,  1.0f,      0.0f,  1.0f,
        -1.0f, -1.0f,      0.0f,  0.0f,
         1.0f, -1.0f,      1.0f,  0.0f,
         1.0f,  1.0f,      1.0f,  1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, // Primeiro triângulo
        0, 2, 3  // Segundo triângulo
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configurar atributo de posição
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configurar atributo de coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        int iResolutionLocation = glGetUniformLocation(shaderProgram, "iResolution");
        glUniform2f(iResolutionLocation, (float)width, (float)height);
        int iTimeLocation = glGetUniformLocation(shaderProgram, "iTime");
        glUniform1f(iTimeLocation, (float)glfwGetTime());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
