#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;

layout(location = 0) out vec4 outColor;

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

void main()
{
    vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 surfaceNormal = normalize(fragWorldNormal);

    for(int i = 0; i < ubo.numPointLights; i++)
    {
        PointLight light = ubo.pointLights[i];
        vec3 directionToLight = light.position.xyz - fragWorldPos;
        
        // light falloff based on the distance to the light
        // Light color is the color multiplied by the light intensity (alpha)
        float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

        // Calculate the light intensity based on the normal and the direction to the light
        // The dot product gives us the cosine of the angle between the normal and the light direction
        // which is used to calculate the intensity of the light on the surface
        // The max function ensures that we don't get negative values
        // when the light is behind the surface
        // The result is a value between 0.0 and 1.0
        // This is a simple Lambertian lighting model
        // light intensity is basically the dot product of the surface normal and the light direction
        float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0.0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;
        diffuseLight += intensity * cosAngIncidence;
    }
    
    // Set the color of the fragment
    outColor = vec4(diffuseLight * fragColor, 1.0);
}