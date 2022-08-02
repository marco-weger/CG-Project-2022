#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 mMat;
	mat4 nMat;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragTexCoord;

void main()
{
    fragTexCoord = inPosition;
    vec4 pos = ubo.mvpMat * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
    
//    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(pos, 1.0);
//    fragViewDir  = (ubo.view[3]).xyz - (ubo.model * vec4(pos,  1.0)).xyz;
//    fragNorm     = (ubo.model * vec4(norm, 0.0)).xyz;
//    fragTexCoord = texCoord;
}  
