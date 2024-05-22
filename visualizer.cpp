#include "visualizer.h"
#include "exception.h"
#include "vis/shaderData.h"
#include "vis/geometry.h"
#include "vis/node.h"
#include "vis/embeddedShaderData.h"

using namespace glm;

const char *Visualizer::pointVertexShaderSource = R"0B3R0N(
    layout (std140, binding = 0) uniform Camera {
        CameraData data;
    } uiCamera;
    layout (std140, binding = 1) uniform Context {
        ContextData data;
    } uiContext;

    layout (location = 0) in vec3 viPos;
    layout (location = 1) in vec4 viColor;

    out vec4 fiColor;
    void main() {
        mat4 qMatrix = quatToMat(uiCamera.data.imuQuat);
        mat4 mvMatrix = uiCamera.data.viewMatrix * qMatrix * uiContext.data.modelMatrix;
        mat4 matrix = uiCamera.data.projMatrix * mvMatrix;

        gl_Position = matrix * vec4(viPos, 1.0f);
        
        float d = (length(viPos) - 0.1) / 5.0;
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
        
        vec4 tint = uiContext.data.tint;
        float comp = 1.0 - d;

        d = (length(viPos) - 0.1) / 10.0;
        if(d > 1.0) d = 1.0;
        
        float logComp = 1.0 - d * d * d;
        fiColor = vec4(r * comp, g * comp * comp, b * logComp, 1.0); // comp);
        
        // fiColor = vec4(1.0, 1.0, 1.0, 1.0);

        gl_PointSize = 1.0 + (0.5 - 0.5 * (gl_Position.z / gl_Position.w)) * 20.0;
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
    layout (std140, binding = 1) uniform Context {
        ContextData data;
    } uiContext;
    layout (std140, binding = 2) uniform Light0 {
        LightData data;
    } uiLight0;
    layout (std140, binding = 3) uniform Light1 {
        LightData data;
    } uiLight1;

    layout (location = 0) in vec3 viPos;
    layout (location = 1) in vec4 viNorm;
    layout (location = 2) in vec4 viColor;
    layout (location = 3) in vec2 vUV;

    out vec4 fiColor;
    void main() {
        mat4 qMatrix = quatToMat(uiCamera.data.imuQuat);
        mat4 mvMatrix = uiCamera.data.viewMatrix * qMatrix * uiContext.data.modelMatrix;
        mat4 matrix = uiCamera.data.projMatrix * mvMatrix;
        
        gl_Position = matrix * vec4(viPos, 1.0f);
        
        vec3 sNorm = vec3(mvMatrix * vec4(viNorm.x, viNorm.y, viNorm.z, 0.0f));
        vec3 l0Norm = vec3(mvMatrix * uiLight0.data.position);
        vec3 l1Norm = vec3(mvMatrix * uiLight1.data.position);

        const vec3 n = normalize(sNorm);
        const vec3 l0 = normalize(l0Norm);
        const vec3 l1 = normalize(l1Norm);
        
        float cosAngle0 = clamp(dot(n, l0), 0.0f, 1.0f);
        float cosAngle1 = clamp(dot(n, l1), 0.0f, 1.0f);

        float lightContrib = clamp(cosAngle0 + cosAngle1, 0.0f, 1.0f);

        vec3 lightContrib0 = vec3(uiLight0.data.ambient) + vec3(uiLight0.data.diffuse) * cosAngle0;
        vec3 lightContrib1 = vec3(uiLight1.data.ambient) + vec3(uiLight1.data.diffuse) * cosAngle1;
        vec3 totalContrib = clamp(lightContrib0 + lightContrib1, vec3(0.0f), vec3(1.0f));

        fiColor = 
            // vec4(0.4, 0.0, 1.0, 1.0);
            // vec4(viColor.x, viColor.y, viColor.z, 1.0f);
            // vec4(vec3(viColor) * lightContrib, 1.0f);
            vec4(vec3(viColor) * totalContrib, 1.0f);
    }
)0B3R0N";

const char *Visualizer::instanceVertexLitShaderSource = R"0B3R0N(
    layout (std140, binding = 0) uniform Camera {
        CameraData data;
    } uiCamera;
    layout (std140, binding = 1) uniform Context {
        ContextData data;
    } uiContext;
    layout (std140, binding = 2) uniform Light0 {
        LightData data;
    } uiLight0;
    layout (std140, binding = 3) uniform Light1 {
        LightData data;
    } uiLight1;
    layout (std140, binding = 4) uniform Grid {
        GridData data;
    } uiGrid;

    layout (location = 0) in vec3 viPos;
    layout (location = 1) in vec4 viNorm;
    layout (location = 2) in vec4 viColor;
    layout (location = 3) in vec2 vUV;

    out vec4 fiColor;
    void main() {
        mat4 qMatrix = quatToMat(uiCamera.data.imuQuat);
        mat4 mvMatrix = uiCamera.data.viewMatrix * qMatrix * uiContext.data.modelMatrix;
        mat4 matrix = uiCamera.data.projMatrix * mvMatrix;
        
        int instanceID = gl_InstanceID;
        float k = float(instanceID / (uiGrid.data.sx * uiGrid.data.sy));
        int ij = instanceID % (uiGrid.data.sx * uiGrid.data.sy);
        float j = float(ij / uiGrid.data.sx);
        float i = float(ij % uiGrid.data.sx);

        vec3 instOff = vec3(uiGrid.data.ox + uiGrid.data.dx * i, uiGrid.data.oy + uiGrid.data.dy * j, uiGrid.data.oz + uiGrid.data.dz * k);
        gl_Position = matrix * vec4(viPos + instOff, 1.0f);

        vec3 sNorm = vec3(mvMatrix * vec4(viNorm.x, viNorm.y, viNorm.z, 0.0f));
        vec3 l0Norm = vec3(mvMatrix * uiLight0.data.position);
        vec3 l1Norm = vec3(mvMatrix * uiLight1.data.position);

        const vec3 n = normalize(sNorm);
        const vec3 l0 = normalize(l0Norm);
        const vec3 l1 = normalize(l1Norm);
        
        float cosAngle0 = clamp(dot(n, l0), 0.0f, 1.0f);
        float cosAngle1 = clamp(dot(n, l1), 0.0f, 1.0f);

        float lightContrib = clamp(cosAngle0 + cosAngle1, 0.0f, 1.0f);

        vec3 lightContrib0 = vec3(uiLight0.data.ambient) + vec3(uiLight0.data.diffuse) * cosAngle0;
        vec3 lightContrib1 = vec3(uiLight1.data.ambient) + vec3(uiLight1.data.diffuse) * cosAngle1;
        vec3 totalContrib = clamp(lightContrib0 + lightContrib1, vec3(0.0f), vec3(1.0f));

        fiColor = 
            // vec4(0.4, 0.0, 1.0, 1.0);
            // vec4(viColor.x, viColor.y, viColor.z, 1.0f);
            // vec4(vec3(viColor) * lightContrib, 1.0f);
            vec4(vec3(viColor) * totalContrib, 1.0f);
    }
)0B3R0N";

const char *Visualizer::fragmentLitShaderSource = R"0B3R0N(
    in vec4 fiColor;
    layout (location = OUT_COLOR) out vec4 fragColor;
    void main() {
        fragColor = fiColor;
    }
)0B3R0N";

Visualizer::Visualizer(
    /*
    deque<OccupancyGrid *> & ioOccupancyQueue,
    mutex & ioOccupancyQueueMutex,
    */
    OccupancyGrid & ioOccupancyGrid,
    mutex & ioOccupancyGridMutex,
    PointCloudAccumulator & ioPCAccum, 
    size_t iWidth, 
    size_t iHeight)
: glAvailable(false), 
  width(iWidth), height(iHeight), 
  lastTimeStamp(), frequency(0.0), 
  /*
  occupancyQueue(ioOccupancyQueue), 
  occupancyQueueMutex(ioOccupancyQueueMutex),
  */
  occupancyGrid(ioOccupancyGrid),
  occupancyGridMutex(ioOccupancyGridMutex),
  framebuffer(800, 600),
  camera(0, 800, 600, true),
  context(1),
  light0(2, false),
  light1(3, false),
  pcAccum(ioPCAccum),
  rootNode() {
  // cell(0.001, 0.001, 0.001),
  // cellGeom(nullptr) {
  // testBox(),
  // testTriangle() {
    /*
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
    */

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

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexSource = processGLSLSource(instanceVertexLitShaderSource);
    vtxSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vtxSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        THROW(string("Error: failed to compile instance lit vertex shader: ") + infoLog + "\n\n" + vertexSource);
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
        THROW(string("Error: failed to link instance lit shader program: ") + infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    /*
    camera.init(0);
    context.init(1);
    light.init(2);
    */

    /*
    cellGeom = new UnmanagedGeometry<Box>(cell, litShaderProgram);
    cellGeom->update();
    */

    LightData & lightData0 = light0.getLightData();
    lightData0.position = vec4(2.0f, 3.0f, 5.0f, 0.0f);
    lightData0.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    LightData & lightData1 = light1.getLightData();
    lightData1.position = vec4(-2.0f, -3.0f, -5.0f, 0.0f);
    lightData1.diffuse = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    mat4 zOffset = translate(mat4(1.0f), vec3(0.0, 0.0, 0.05));

    Box * box = new Box(0.1f, 0.1f, 0.02f, Box::full, Box::full, Box::positive);
    Geometry<Box> * boxGeom = new Geometry<Box>(box, litShaderProgram);
    Node * node = new Node;
    // mat4 yRot = ::rotate(mat4(1.0f), (float) radians(30.0), vec3(0.0, 1.0, 0.0));
    // node->setTransform(yRot);
    node->setTransform(zOffset);
    node->addGeometry(boxGeom);
    rootNode.addChild(node);

    UnmanagedGeometry<PointCloudAccumulator> * pointCloudGeom = new UnmanagedGeometry<PointCloudAccumulator>(pcAccum, pointShaderProgram);
    node = new Node;
    // mat4 xRot = ::rotate(mat4(1.0f), (float) radians(180.0), vec3(1.0, 0.0, 0.0));
    // node->setTransform(xRot);
    // node->setTransform(zOffset);
    node->addGeometry(pointCloudGeom);
    rootNode.addChild(node);

    /*
    boxGeom->init();
    pointCloudGeom->init();
    testTriangle.init();
    */
    /*
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
    */

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);                                                                                                                                           
    glEnable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glAvailable = true;
}

Visualizer::~Visualizer() {
    if(pointShaderProgram) glDeleteProgram(pointShaderProgram);
    if(litShaderProgram) glDeleteProgram(litShaderProgram);
    /*
    framebuffer.cleanUp();
    camera.cleanUp();
    light.cleanUp();
    // testBox.cleanUp();
    testTriangle.cleanUp();
    */
}

void Visualizer::setViewportSize(GLsizei iWidth, GLsizei iHeight) {
    width = iWidth;
    height = iHeight;
    camera.resize(iWidth, iHeight);
}

int Visualizer::loop() {
    auto currentTimeStamp = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::microseconds>(currentTimeStamp - lastTimeStamp).count();
    if(elapsed >= 16666) {
        if (glAvailable) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.bind();
            context.bind();
            light0.bind();
            light1.bind();
            
            render();

            light1.unbind();
            light0.unbind();
            context.unbind();
            camera.unbind();
        }
        frequency = 1000000.0 / double(elapsed);
        lastTimeStamp = currentTimeStamp;
    }   

    return 1;
}

void Visualizer::update() {
    camera.update();
    // context.update();
    // pointCloud.update();
    rootNode.update();
    light0.update();
    light1.update();
    
    /*
    OccupancyGrid *grid = nullptr;
    occupancyQueueMutex.lock();
    if(occupancyQueue.size() > 0) {
        grid = occupancyQueue.front();
        occupancyQueue.pop_front();
    }
    occupancyQueueMutex.unlock();
    */
}

void Visualizer::render() {
    // testBox.draw();
    /*
    glBindVertexArray(testTriangleVAO);
    glDrawArrays(GL_POINTS, 0, 3);
    glBindVertexArray(0);
    
    */
    // testTriangle.draw();
    // glUseProgram(pointShaderProgram);
    // pointCloud.draw();
    
    // glUseProgram(litShaderProgram);
    // testBox.draw();

    rootNode.draw(context);

    // Drawing up to 2000 separate boxes is too slow.
    // Have to do this with instancing.
    /*
    occupancyGridMutex.lock();
    occupancyGrid.map(*this);
    occupancyGridMutex.unlock();
    */
}

double Visualizer::getFrequency() const {
    return frequency;
}

void Visualizer::operator()(size_t x, size_t y, size_t z) {
    /*
    if(cellGeom) {
        const vec3 position(double(x) * 0.001, double(y) * 0.001, double(z) * 0.001);
        const mat4 identity(1.0f);
        auto trans = translate(identity, position);
        context.pushMatrix(trans);
        cellGeom->draw();
        context.popMatrix();
    }
    */
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
