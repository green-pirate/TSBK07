#version 150

out vec4 out_Color;

in vec3 normal_to_frag;
in vec2 TexCoord_to_frag;

uniform float elapsedTime;

void main(void)
{
	float a = sin(TexCoord_to_frag.s*30)/2+0.5;
	float b = sin(TexCoord_to_frag.t*30)/2+0.5;
	out_Color = vec4(a, b, sin(0.001*elapsedTime), 0.0);

}
