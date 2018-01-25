#version 150

out vec4 out_Color;

in vec3 normal_to_frag;
in vec2 TexCoord_to_frag;

uniform float elapsedTime;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, TexCoord_to_frag);
}
