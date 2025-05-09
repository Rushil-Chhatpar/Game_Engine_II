#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPos; // linearly interpolated to be the fragment position in the world space. We need this to calculate the light direction for each fragment
layout(location = 2) out vec3 fragWorldNormal; // linearly interpolated to be the fragment normal in the world space.

struct PointLight
{
    vec4 position;
    vec4 color;
};

layout(set = 0, binding = 0) uniform GlobalUBO
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec4 ambientLightColor;
    PointLight pointLights[20];
    int numPointLights;
} ubo;

layout(push_constant) uniform Push
{
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

const float AMBIENT_LIGHT_INTENSITY = 0.2;

void main()
{
    vec4 worldPosion = push.modelMatrix * vec4(inPosition, 1.0);
    // Set the position of the vertex
    //gl_Position = vec4(push.transform * inPosition + push.offset, 0.0, 1.0);
    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPosion;

    // transform normals from model space to world space to properly calculate lighting
    // since lighting is done in world space
    // Only works if the scale is uniform (sx == sy == sz)
    //vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * inNormal);

    fragWorldNormal = normalize(mat3(push.normalMatrix) * inNormal);
    fragWorldPos = worldPosion.xyz;
    fragColor = inColor;
}