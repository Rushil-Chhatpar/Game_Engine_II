#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push
{
    mat4 transform; // projection * view * model
    mat4 modelMatrix;
} push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));

void main()
{
    // Set the position of the vertex
    //gl_Position = vec4(push.transform * inPosition + push.offset, 0.0, 1.0);
    gl_Position = push.transform * vec4(inPosition, 1.0);

    // transform normals from model space to world space to properly calculate lighting
    // since lighting is done in world space
    vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * inNormal);

    // Calculate the light intensity based on the normal and the direction to the light
    // The dot product gives us the cosine of the angle between the normal and the light direction
    // which is used to calculate the intensity of the light on the surface
    // The max function ensures that we don't get negative values
    // when the light is behind the surface
    // The result is a value between 0.0 and 1.0
    // This is a simple Lambertian lighting model
    float lightIntensity = max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0.0);
    
    // Set the color of the vertex
    fragColor = lightIntensity * inColor;
}