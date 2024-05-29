const char *Visualizer::litInstanceShaderSource = R"0B3R0N(
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
    layout (std140, binding = 5) buffer Instances {
        InstanceData data[];
    } uiInstances;

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

