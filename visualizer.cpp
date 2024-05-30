#include "visualizer.h"
#include "exception.h"
#include "vis/shaderData.h"
#include "vis/geometry.h"
#include "vis/node.h"
#include "vis/embeddedShaderData.h"
#include "vis/grid.h"

using namespace glm;

#include "vis/shaders/shaders.h"

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
  gridBox(0.02f, 0.02f, 0.02f),
  grid(nullptr),
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

    // pointVertexShader
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

    // pointFragmentShader
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

    // pointShaderProgram
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

    // litVertexShaderSource
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexSource = processGLSLSource(litVertexShaderSource);
    vtxSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vtxSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        THROW(string("Error: failed to compile lit vertex shader: ") + infoLog + "\n\n" + vertexSource);
    }

    // litFragmentShaderSource
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentSource = processGLSLSource(litFragmentShaderSource);
    frgSrc = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &frgSrc, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        glDeleteShader(fragmentShader);
        THROW(string("Error: failed to compile lit fragment shader: ") + infoLog);
    }
    
    // litShaderProgram
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

    // litInstanceShaderSource
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexSource = processGLSLSource(litInstanceShaderSource);
    vtxSrc = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vtxSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        THROW(string("Error: failed to compile lit instance shader: ") + infoLog + "\n\n" + vertexSource);
    }

    // litInstanceShaderProgram
    litInstanceShaderProgram = glCreateProgram();
    glAttachShader(litInstanceShaderProgram, vertexShader);
    glAttachShader(litInstanceShaderProgram, fragmentShader);
    glLinkProgram(litInstanceShaderProgram);

    glGetProgramiv(litInstanceShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(litInstanceShaderProgram, 512, NULL, infoLog);
        glDeleteProgram(litInstanceShaderProgram);
        litInstanceShaderProgram = 0;
        THROW(string("Error: failed to link lit instance shader program: ") + infoLog);
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

    /*
    Box * box = new Box(0.1f, 0.1f, 0.02f, Box::full, Box::full, Box::positive);
    Geometry<Box> * boxGeom = new Geometry<Box>(box, litShaderProgram);
    Node * node = new Node;
    // mat4 yRot = ::rotate(mat4(1.0f), (float) radians(30.0), vec3(0.0, 1.0, 0.0));
    // node->setTransform(yRot);
    node->setTransform(zOffset);
    node->addGeometry(boxGeom);
    rootNode.addChild(node);
    */

    UnmanagedGeometry<PointCloudAccumulator> * pointCloudGeom = new UnmanagedGeometry<PointCloudAccumulator>(pcAccum, pointShaderProgram);
    Node * node = new Node;
    // mat4 xRot = ::rotate(mat4(1.0f), (float) radians(180.0), vec3(1.0, 0.0, 0.0));
    // node->setTransform(xRot);
    // node->setTransform(zOffset);
    node->addGeometry(pointCloudGeom);
    rootNode.addChild(node);

    grid = new Grid(gridBox, 4, false, litInstanceShaderProgram);
    Grid::StorageBuffer & gridInstanceDataBuffer = grid->getInstanceDataBuffer();
    InstanceData instanceVertex;
    const void * ptr = nullptr;
    for(int k = 0; k < 10; k++) {
        instanceVertex.pos.z = -1.0f + float(k) * 0.222222222f;
        instanceVertex.scale.z = 1.0f;
        for(int j = 0; j < 10; j++) {
            instanceVertex.pos.y = -1.0f + float(j) * 0.222222222f;
            instanceVertex.scale.y = 1.0f;
            for(int i = 0; i < 10; i++) {
                instanceVertex.pos.x = -1.0f + float(i) * 0.222222222f;
                instanceVertex.scale.x = 1.0f;
                gridInstanceDataBuffer.addVertex(instanceVertex);
                ptr = gridInstanceDataBuffer.getData();
            }
        }
    }
    node = new Node;
    node->addGeometry(grid);
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
