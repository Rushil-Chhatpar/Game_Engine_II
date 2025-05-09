#version 450

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

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
    vec4 position;
    vec4 color;
    float radius;
} push;

const float LIGHT_RADIUS = 0.2;

void main()
{
    fragOffset = OFFSETS[gl_VertexIndex];

    // calculate the light pos in cam space first
    vec4 lightPosInCameraSpace = ubo.viewMatrix * vec4(push.position.xyz, 1.0);
    // then apply offset in cam space
    vec4 positionInCameraSpace = lightPosInCameraSpace + LIGHT_RADIUS * vec4(fragOffset, 0.0, 0.0);

    gl_Position = ubo.projectionMatrix * positionInCameraSpace;

    /* Alternative way to calculate the position of the light in world space
    vec3 cameraRightWorld = {ubo.viewMatrix[0][0], ubo.viewMatrix[1][0], ubo.viewMatrix[2][0]};
    vec3 cameraUpWorld = {ubo.viewMatrix[0][1], ubo.viewMatrix[1][1], ubo.viewMatrix[2][1]};

    vec3 positionWorld = ubo.lightPosition.xyz 
        + LIGHT_RADIUS * fragOffset.x * cameraRightWorld
        + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;
    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * vec4(positionWorld, 1.0);
    */
}