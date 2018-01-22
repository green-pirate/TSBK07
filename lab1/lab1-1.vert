#version 150

in  vec3 in_Position;
in vec4 in_color;
out vec4 color_2_frag;

void main(void)
{
	color_2_frag = in_color;
	gl_Position = vec4(in_Position, 1.0);
}
