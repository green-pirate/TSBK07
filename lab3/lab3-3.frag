#version 150

out vec4 out_Color;

in vec3 normal_to_frag;
in vec2 TexCoord_to_frag;

uniform float elapsedTime;
uniform sampler2D texUnit;
uniform int use_tex;
uniform vec3 color;

void main(void)
{
	const vec3 light = vec3(580, 580, 580);

	float shade = dot(normalize(light), normalize(normal_to_frag));
	shade = clamp(shade, 0, 1);

	if(use_tex == 1)
	{
		out_Color = texture(texUnit, TexCoord_to_frag);
	}
	else
	{
		out_Color = vec4(color.x*shade, color.y*shade, color.z*shade, 1.0f);
	}
}
