#include "visualizer.h"
#include "exception.h"
#include "vis/shaderData.h"
#include "vis/embeddedShaderData.h"

const char *Visualizer::vertexShaderSource = R"0B3R0N(
    layout (location = 0) in vec3 viPos;
    layout (location = 1) in vec4 viColor;

    out vec4 fiColor;
    void main() {
        gl_Position = vec4(viPos.x, viPos.y, viPos.z, 1.0);
        fiColor = viColor;
    }
)0B3R0N";

const char *Visualizer::fragmentShaderSource = R"0B3R0N(
    in vec4 fiColor;
    layout (location = OUT_COLOR) out vec4 fragColor;
    void main() {
        fragColor = fiColor;
    }
)0B3R0N";

const char *Visualizer::vertexShaderSource2 = R"0B3R0N(

layout(std140, binding = UB_CAMERA) uniform Camera {
    CameraData data;
} ubCamera;

layout(std140, binding = UB_LIGHT) uniform Light {
    LightData data;
} ubLight;

layout(location = IN_POSITION) in vec3 aPos;
layout(location = IN_NORMAL) in vec4 aNorm;
layout(location = IN_COLOR) in vec3 aColor;
layout(location = IN_TEXCOORD) in vec3 aUV;

out Frag {
    vec3 color;
} outFrag;

void main() {
    gl_Position = ubCamera.data.projMatrix * ubCamera.data.mvMatrix * vec4(aPos, 1.0f);
    outFrag.color = aColor;
}

)0B3R0N";

const char *Visualizer::fragmentShaderSource2 = R"0B3R0N(

in Frag {
    vec3 color;
} inFrag;

layout(location = OUT_COLOR) out vec4 outColor;

void main() {
    outColor = vec4(inFrag.color, 1.0f);
}

)0B3R0N";

void gFramebufferSizeCallback(GLFWwindow * iWindow, int iWidth, int iHeight) {
    Visualizer *ptr = static_cast<Visualizer *>(glfwGetWindowUserPointer(iWindow));
    ptr->framebufferSizeCallback(iWidth, iHeight);
}

void gDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam) {
    const Visualizer *ptr = static_cast<const Visualizer *>(userParam);
    if(ptr) ptr->debugMessageCallback(source, type, id, severity, length, message);

}

Visualizer::Visualizer(deque<OccupancyGrid *> & ioOccupancyQueue, mutex & ioOccupancyQueueMutex, const string &iWindowTitle, size_t iWidth, size_t iHeight)
: glAvailable(false), 
  width(iWidth), height(iHeight), 
  lastTimeStamp(), frequency(0.0), 
  occupancyQueue(ioOccupancyQueue), 
  occupancyQueueMutex(ioOccupancyQueueMutex),
  framebuffer(800, 600),
  camera(800, 600, 0, true),
  light(1, false),
  testTriangle() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    THROW_IF_NULL(window = glfwCreateWindow(width, height, iWindowTitle.c_str(), NULL, NULL), "Failed to create GLFW window");

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, gFramebufferSizeCallback);

    THROW_IF_NOT(gladLoadGL(glfwGetProcAddress), "Failed to initialize GLAD");

    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageCallback(gDebugMessageCallback, static_cast<void *>(this));

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

    /*
    framebuffer.init();

    testBox.expand(boundingBox);
    testBox.init();

    camera.setFocus(boundingBox);
    camera.init();

    light.init();
    */

    testTriangle.init();

    float vertices[] = {
        -1.0f, -0.5f,  0.0f, 1.0, 0.0, 0.0, 1.0,
         0.0f, -0.5f,  0.0f, 0.0, 1.0, 0.0, 1.0,
        -0.5f,  0.5f,  0.0f, 0.0, 0.0, 1.0, 1.0
    };

    glGenVertexArrays(1, &testTriangleVAO);
    glBindVertexArray(testTriangleVAO);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) uint64_t(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glAvailable = true;
}

Visualizer::~Visualizer() {
    if(shaderProgram) glDeleteProgram(shaderProgram);
    framebuffer.cleanUp();
    camera.cleanUp();
    light.cleanUp();
    testTriangle.cleanUp();
    glfwTerminate();
}

void Visualizer::debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message) const {
    cerr << message << endl;
}

int Visualizer::loop() {
    auto currentTimeStamp = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::microseconds>(currentTimeStamp - lastTimeStamp).count();
    if(elapsed >= 16666) {
        if (glAvailable) {
            if (glfwWindowShouldClose(window)) return 0;
            glViewport(0, 0, width, height);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(shaderProgram);

            // camera.bind();
            // light.bind();
            
            render();

            // light.unbind();
            // camera.unbind();

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
    // camera.update();
    // light.update();
    
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
    // testBox.draw();
    
    glBindVertexArray(testTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    
    testTriangle.draw();
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

bool Visualizer::isDSACompatible() {
    const char * version = (const char *) glGetString(GL_VERSION);
    if (version) {
        float openGLVersion = atof(version);
        return openGLVersion >= 4.5;
    }
    return false;
}

bool Visualizer::isDSAExtensionAvailable() {
    GLint numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for (int i = 0; i < numExtensions; i++) {
        const char * ext = (const char *) glGetStringi(GL_EXTENSIONS, i);
        if (string(ext) == string("GL_ARB_direct_state_access"))
            return true;
    }

    return false;
}
