#include "visualizer.h"
#include "exception.h"
#include "vis/shaderData.h"
#include "vis/embeddedShaderData.h"

const char *Visualizer::pointVertexShaderSource = R"0B3R0N(
    layout (std140, binding = 0) uniform Camera {
        CameraData data;
    } uiCamera;

    layout (location = 0) in vec3 viPos;
    layout (location = 1) in vec4 viColor;

    /*
    vec4 hamiltonion(vec4 a, vec4 b) {
        vec4 result;
        result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
        result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
        result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
        result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
        return result;
    }

    vec4 invertQuaternion(vec4 q) {
        vec4 result;
        result.x = -q.x;
        result.y = -q.y;
        result.z = -q.z;
        result.w = q.w;
        return result;
    }

    vec4 conjugate(vec4 p, vec4 q) {
        return hamiltonion(hamiltonion(q, p), invertQuaternion(q));
    }
    */

    out vec4 fiColor;
    void main() {
        
        vec4 imuPoint = conjugate(vec4(viPos.x, viPos.y, viPos.z, 0.0), uiCamera.data.imuQuat);
        imuPoint.w = 1.0;
        gl_Position = uiCamera.data.projMatrix * uiCamera.data.mvMatrix * imuPoint;
        
        float d = (length(viPos) - 0.5);
        if(d > 1.0) d = 1.0;

        float r = 2.0 - 4.0 * d;
        if(r < 0.0) r = 0.0;
        if(r > 1.0) r = 1.0;
        
        float g = 1.0;
        if(d < 0.25) g = 4.0 * d;
        if(d > 0.75) g = 4.0 - 4.0 * d;
        if(g < 0.0) g = 0.0;
        if(g > 1.0) g = 1.0;
        
        float b = 4.0 * d - 2.0;
        if(b < 0.0) b = 0.0;
        if(b > 1.0) b = 1.0;
        
        fiColor = vec4(r * (1.0 - d * d * d), g * (1.0 - d * d * d), b * (1.0 - d * d * d), 1.0 - d * d * d);
        gl_PointSize = 1.0 + (0.5 - 0.5 * (gl_Position.z / gl_Position.w)) * 100.0;
    }
)0B3R0N";

const char *Visualizer::pointFragmentShaderSource = R"0B3R0N(
    in vec4 fiColor;
    layout (location = OUT_COLOR) out vec4 fragColor;
    void main() {
        
        vec2 coord = gl_PointCoord - vec2(0.5, 0.5);
        float radius = 0.5;
        float distance = length(coord);

        if(distance > radius) {
            discard;
        }
        

        fragColor = fiColor;
    }
)0B3R0N";

const char *Visualizer::vertexLitShaderSource = R"0B3R0N(
    layout (std140, binding = 0) uniform Camera {
        CameraData data;
    } uiCamera;
    layout (std140, binding = 1) uniform Light {
        LightData data;
    } uiLight0;

    layout (location = 0) in vec3 viPos;
    layout (location = 1) in vec4 viNorm;
    layout (location = 2) in vec4 viColor;
    layout (location = 3) in vec2 vUV;

    /*
    vec4 hamiltonion(vec4 a, vec4 b) {
        vec4 result;
        result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
        result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
        result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
        result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
        return result;
    }

    vec4 invertQuaternion(vec4 q) {
        vec4 result;
        result.x = -q.x;
        result.y = -q.y;
        result.z = -q.z;
        result.w = q.w;
        return result;
    }

    vec4 conjugate(vec4 p, vec4 q) {
        return hamiltonion(hamiltonion(q, p), invertQuaternion(q));
    }
    */

    out vec4 fiColor;
    void main() {
        
        vec4 imuPoint = conjugate(vec4(viPos.x, viPos.y, viPos.z, 0.0), uiCamera.data.imuQuat);
        imuPoint.w = 1.0;
        gl_Position = uiCamera.data.projMatrix * uiCamera.data.mvMatrix * imuPoint;
        
        vec3 ePos = vec3(uiCamera.data.mvMatrix * vec4(viPos.x, viPos.y, viPos.z, 1.0f));
        vec3 eNorm = vec3(uiCamera.data.mvMatrix * vec4(viNorm.z, viNorm.y, viNorm.z, 0.0f));

        vec3 diffuse = vec3(0.0f);
        vec3 specular = vec3(0.0f);

        const vec3 n = normalize(eNorm);
        const vec3 l = normalize(-ePos);
        
        float diffuseIntensity = dot(n, l);

        if (diffuseIntensity > 0.0f) {
            diffuse = vec3(uiLight0.data.diffuse) * diffuseIntensity;

            const vec3 r = reflect(-l, n);
            const vec3 e = l;
            const float specularIntensity = max(dot(r, e), 0.0f);
            specular = vec3(uiLight0.data.specular) * pow(specularIntensity, 32.0f);
        }

        fiColor = vec4(vec3(viColor) * (vec3(uiLight0.data.ambient) + diffuse, + specular), 1.0f);
    }
)0B3R0N";

const char *Visualizer::fragmentLitShaderSource = R"0B3R0N(
    in vec4 fiColor;
    layout (location = OUT_COLOR) out vec4 fragColor;
    void main() {
        fragColor = fiColor;
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

Visualizer::Visualizer(
    deque<OccupancyGrid *> & ioOccupancyQueue,
    mutex & ioOccupancyQueueMutex,
    const string &iWindowTitle,
    PointCloud &iPointCloud, 
    size_t iWidth, 
    size_t iHeight)
: glAvailable(false), 
  width(iWidth), height(iHeight), 
  lastTimeStamp(), frequency(0.0), 
  occupancyQueue(ioOccupancyQueue), 
  occupancyQueueMutex(ioOccupancyQueueMutex),
  framebuffer(800, 600),
  camera(800, 600, true),
  light(false),
  pointCloud(iPointCloud),
  testBox(),
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
    string vertexSource(processGLSLSource(pointVertexShaderSource));
    const char * vtxSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vtxSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        THROW(string("Error: failed to compile point vertex shader: ") + infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    string fragmentSource(processGLSLSource(pointFragmentShaderSource));
    const char * frgSrc = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &frgSrc, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        glDeleteShader(fragmentShader);
        THROW(string("Error: failed to compile point fragment shader: ") + infoLog);
    }

    pointShaderProgram = glCreateProgram();
    glAttachShader(pointShaderProgram, vertexShader);
    glAttachShader(pointShaderProgram, fragmentShader);
    glLinkProgram(pointShaderProgram);

    glGetProgramiv(pointShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pointShaderProgram, 512, NULL, infoLog);
        glDeleteProgram(pointShaderProgram);
        pointShaderProgram = 0;
        THROW(string("Error: failed to link point shader program: ") + infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexSource = processGLSLSource(vertexLitShaderSource);
    vtxSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vtxSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        THROW(string("Error: failed to compile lit vertex shader: ") + infoLog + "\n\n" + vertexSource);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentSource = processGLSLSource(fragmentLitShaderSource);
    frgSrc = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &frgSrc, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        glDeleteShader(fragmentShader);
        THROW(string("Error: failed to compile lit fragment shader: ") + infoLog);
    }

    litShaderProgram = glCreateProgram();
    glAttachShader(litShaderProgram, vertexShader);
    glAttachShader(litShaderProgram, fragmentShader);
    glLinkProgram(litShaderProgram);

    glGetProgramiv(litShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(litShaderProgram, 512, NULL, infoLog);
        glDeleteProgram(litShaderProgram);
        litShaderProgram = 0;
        THROW(string("Error: failed to link lit shader program: ") + infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // GLint bindPoint = glGetUniformLocation(shaderProgram, "camera");

    // camera.setFocus(boundingBox);
    camera.init(0);
    light.init(1);
    /*
    bindPoint = glGetUniformLocation(vertexShader, "light0");
    light.init(bindPoint);
    */
    /*
    framebuffer.init();

    testBox.expand(boundingBox);
    testBox.init();
    */

    pointCloud.init();
    testBox.init();
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);                                                                                                                                           
    glEnable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //glPointSize(5.0);

    glAvailable = true;
}

Visualizer::~Visualizer() {
    if(pointShaderProgram) glDeleteProgram(pointShaderProgram);
    if(litShaderProgram) glDeleteProgram(litShaderProgram);
    framebuffer.cleanUp();
    camera.cleanUp();
    light.cleanUp();
    testBox.cleanUp();
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
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.bind();
            light.bind();
            
            render();

            light.unbind();
            camera.unbind();

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
    camera.update();
    pointCloud.update();
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
    /*
    glBindVertexArray(testTriangleVAO);
    glDrawArrays(GL_POINTS, 0, 3);
    glBindVertexArray(0);
    
    */
    // testTriangle.draw();
    glUseProgram(pointShaderProgram);
    pointCloud.draw();
    
    glUseProgram(litShaderProgram);
    testBox.draw();
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

void Visualizer::setImuQuat(const vec4 & iImuQuat) {
    camera.setImuQuat(iImuQuat);
}
