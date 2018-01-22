#version 150

in vec3 in_Position;
in vec4 in_color;
out vec4 color_2_frag;
uniform mat4 myMatrix;

void main(void)
{
	color_2_frag = in_color;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
}
