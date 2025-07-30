// Include the GLAD header *before* GLFW — it provides modern OpenGL function pointers.
#include <glad/gl.h>

// GLFW provides window creation and context handling across platforms.
#include <GLFW/glfw3.h>

#include <iostream>

// ------------------------------
// Vertex Shader GLSL source code
// This shader runs on the GPU and transforms each vertex's position.
// ------------------------------
const char* vertexShaderSource = R"glsl(
    #version 410 core
    layout (location = 0) in vec3 aPos; // input: position of the vertex
    void main() {
        gl_Position = vec4(aPos, 1.0); // output: clip-space position
    }
)glsl";

// ------------------------------
// Fragment Shader GLSL source code
// This shader determines the color of each pixel in the triangle.
// ------------------------------
const char* fragmentShaderSource = R"glsl(
    #version 410 core
    out vec4 FragColor; // output: final color of this pixel
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0); // orange color (RGBA)
    }
)glsl";

int main() {
    // ------------------------------
    // Initialize GLFW library
    // ------------------------------
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Request OpenGL version 3.3 Core profile
    // Core profile means we want to use modern OpenGL without deprecated features.
    // ------------------------------
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Rectangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // ------------------------------
    // Load OpenGL function pointers using GLAD
    // ------------------------------
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // ------------------------------
    // Set up Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    // VAO stores the vertex attribute configuration
    // VBO holds the actual vertex data
    // ------------------------------
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);  // generate a VAO
    glGenBuffers(1, &VBO);       // generate a VBO

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ------------------------------
    // Tell OpenGL how to interpret the vertex data
    // ------------------------------
    glVertexAttribPointer(
        0,                  // attribute location (matches "layout (location = 0)")
        3,                  // number of components per vertex (x, y, z)
        GL_FLOAT,           // data type
        GL_FALSE,           // don't normalize
        3 * sizeof(float),  // stride (bytes between each vertex)
        (void*)0            // offset of the first attribute
    );
    glEnableVertexAttribArray(0); // enable attribute 0

    // ------------------------------
    // Compile Vertex Shader
    // ------------------------------
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);       // create shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr); // set shader source
    glCompileShader(vertexShader);                                 // compile the shader

    // ------------------------------
    // Compile Fragment Shader
    // ------------------------------
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // ------------------------------
    // Create a Shader Program and link both shaders
    // ------------------------------
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);   // attach vertex shader
    glAttachShader(shaderProgram, fragmentShader); // attach fragment shader
    glLinkProgram(shaderProgram);                  // link the program

    // Delete shader objects after linking — we no longer need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ------------------------------
    // Main render loop
    // ------------------------------
    while (!glfwWindowShouldClose(window)) {
        // Set the background color (RGBA)
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // clear the screen

        glUseProgram(shaderProgram);  // activate the shader program
        glBindVertexArray(VAO);       // bind the VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for input and window events
        glfwPollEvents();
    }

    // ------------------------------
    // Cleanup all OpenGL resources
    // ------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Destroy the window and clean up GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
