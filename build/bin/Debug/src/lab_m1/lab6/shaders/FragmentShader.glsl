#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_normal;
// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color	= vec4(abs(frag_normal), 1.f);

}
