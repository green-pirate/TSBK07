#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

uniform mat4 myMatrix;

out vec3 normal_to_frag;
out vec2 TexCoord_to_frag;
void main(void)
{
	TexCoord_to_frag = inTexCoord;
	normal_to_frag = in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
}
