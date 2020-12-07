#version 460 core
#extension GL_EXT_geometry_shader4 : enable
// define the number of CPs in the output patch
layout (vertices = 16) out;

uniform float tess_factor;

 

void main()
{
    // Set the control points of the output patch
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = tess_factor;
    gl_TessLevelOuter[1] = tess_factor;
    gl_TessLevelOuter[2] = tess_factor;
    gl_TessLevelOuter[3] = tess_factor;
    gl_TessLevelInner[0] = tess_factor;
    gl_TessLevelInner[1] = tess_factor;

}

