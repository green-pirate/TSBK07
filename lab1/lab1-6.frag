#version 150

out vec4 out_Color;
in vec4 color_2_frag;
in vec3 normal_to_frag;

void main(void)
{

	out_Color = vec4(normal_to_frag, 1.0f);

}
