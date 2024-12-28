#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
// Default values to translation and rotation uniform
uniform mat4 translation_mat;
uniform mat4 rotation_mat;

out vec2 uv;


void main()
{
        uv = aTexCoord;
        gl_Position = projection_mat * view_mat * model_mat * rotation_mat * vec4(aPos,1.0) ;
}
