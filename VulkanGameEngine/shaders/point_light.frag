#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 0) out vec4 outColor;

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

void main()
{
    float distance = sqrt(dot(fragOffset, fragOffset));
    if(distance > 1.0f)
    {
        discard;
    }
    outColor = vec4(push.color.xyz, 1.0f);
}