#version 330
layout (location = 0) in vec3 position;

out vec3 Wpos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    Wpos = vec3(model * vec4(position, 1.0));
    gl_Position =  projection * view * vec4(Wpos, 1.0);
}