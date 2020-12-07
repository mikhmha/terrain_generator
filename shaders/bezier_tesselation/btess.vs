#version 460 core
//#extension GL_EXT_geometry_shader4 : enable

layout (location = 0) in vec3 aPos;

uniform mat4 model;



//out vec4 WorldPos_CS_in;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    //gl_Position = projection * view * model * vec4(aPos, 1.0);

}