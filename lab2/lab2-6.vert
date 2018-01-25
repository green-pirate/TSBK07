#version 150

in vec3 in_Position;
in vec3 in_Normal;
//in vec2 inTexCoord;

uniform mat4 mdlMatrix;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

out vec3 normal_to_frag;
//out vec2 TexCoord_to_frag;
out vec3 exColor;

void main(void)
{
	mat3 normalTransform = mat3(mdlMatrix);
	normal_to_frag = normalTransform * in_Normal;

	const vec3 light = vec3(0.58, 0.58, 0.58);

	float shade = dot(light, normalize(normal_to_frag));
	shade = clamp(shade, 0, 1);

	exColor = vec3(shade);




	gl_Position = projectionMatrix*worldToViewMatrix*mdlMatrix*vec4(in_Position, 1.0);
}
