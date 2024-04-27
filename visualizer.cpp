#include "visualizer.h"
#include "exception.h"
#include "vis/shaderData.h"
#include "vis/embeddedShaderData.h"

const char *Visualizer::vertexShaderSource = R"0B3R0N(
    layout (location = IN_POSITION) in vec3 aPos;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.x, 1.0);
    }
)0B3R0N";

const char *Visualizer::fragmentShaderSource = R"0B3R0N(
    layout (location = OUT_COLOR) out vec4 fragColor;
    void main() {
        fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)0B3R0N";

const char *Visualizer::vertexShaderSource2 = R"0B3R0N(
    layout(std140, binding = UB_CAMERA) uniform Camera {
        CameraData data;
    } ub_Camera;
    
    uniform vec4 uColor;

    layout (location = IN_POSITION) in vec3 aPos;
    layout (location = IN_NORMAL) in vec3 aNorm;
    layout (location = IN_TEXCOORD) in vec2 aUV;

    out vec3 normal;
    out vec4 color;
    noperspective centroid out vec2 uv;
    
    void main() {
        gl_Position = uModelView * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        normal = uModelView * vec4(aNorm.x, aNorm.u, aNorm.z, 0.0);
        color = uColor;
        uv = vec2(aUV.x, aUV.y);
    }
)0B3R0N";

const char *Visualizer::fragmentShaderSource2 = R"0B3R0N(
    in vec3 normal;
    in vec4 color;
    noperspective centroid in vec2 uv;

    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)0B3R0N";

void gFramebufferSizeCallback(GLFWwindow * iWindow, int iWidth, int iHeight) {
    Visualizer *ptr = static_cast<Visualizer *>(glfwGetWindowUserPointer(iWindow));
    ptr->framebufferSizeCallback(iWidth, iHeight);
}

Visualizer::Visualizer(deque<OccupancyGrid *> & iOccupancyQueue, mutex & iOccupancyQueueMutex, const string &iWindowTitle, size_t iWidth, size_t iHeight)
: glAvailable(false), width(iWidth), height(iHeight), lastTimeStamp(), frequency(0.0), occupancyQueue(iOccupancyQueue), occupancyQueueMutex(iOccupancyQueueMutex) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    THROW_IF_NULL(window = glfwCreateWindow(width, height, iWindowTitle.c_str(), NULL, NULL), "Failed to create GLFW window");

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, gFramebufferSizeCallback);

    THROW_IF_NOT(gladLoadGL(glfwGetProcAddress), "Failed to initialize GLAD");

    int success;
    char infoLog[512];

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    string vertexSource(processGLSLSource(vertexShaderSource));
    const char * vtxSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vtxSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        THROW(string("Error: failed to compile vertex shader: ") + infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    string fragmentSource(processGLSLSource(fragmentShaderSource));
    const char * frgSrc = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &frgSrc, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        glDeleteShader(fragmentShader);
        THROW(string("Error: failed to compile fragment shader: ") + infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
        THROW(string("Error: failed to link shader program: ") + infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glAvailable = true;
}

Visualizer::~Visualizer() {
    if(shaderProgram) glDeleteProgram(shaderProgram);
    glfwTerminate();
}

int Visualizer::loop() {
    auto currentTimeStamp = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::microseconds>(currentTimeStamp - lastTimeStamp).count();
    if(elapsed >= 8333) {
        if (glAvailable) {
            if (glfwWindowShouldClose(window)) return 0;

            glClearColor(0.6f, 0.4f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);
            
            render();

            glfwSwapBuffers(window);

            glfwPollEvents();

            processInput();
        }
        frequency = 1000000.0 / double(elapsed);
        lastTimeStamp = currentTimeStamp;
    }   

    return 1;
}

void Visualizer::update() {
    OccupancyGrid *grid = nullptr;
    occupancyQueueMutex.lock();
    if(occupancyQueue.size() > 0) {
        grid = occupancyQueue.front();
        occupancyQueue.pop_front();
    }
    occupancyQueueMutex.unlock();

    if(grid) {
        grid->map(*this);
        delete grid;
    }
}

void Visualizer::render() {
}

void Visualizer::framebufferSizeCallback(size_t iWidth, size_t iHeight) {
    glViewport(0, 0, width, height);
}

void Visualizer::processInput() {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

double Visualizer::getFrequency() const {
    return frequency;
}

void Visualizer::operator()(size_t x, size_t y, size_t z) {
    
}

string Visualizer::processGLSLSource(const char * iSource) {
    string output("#version 450 core\n\n");
    output += embeddedShaderData;
    output += "\n\n";
    output += iSource;
    output += "\n";
    return output;
}