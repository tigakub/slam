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

