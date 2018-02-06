#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

uniform mat4 mdlMatrix;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

out vec3 normal_to_frag;
out vec2 TexCoord_to_frag;
out vec3 exSurface; // Phong (specular)

void main(void)
{
	mat3 normalTransform = mat3(mdlMatrix);
	normal_to_frag = normalTransform * in_Normal; // Send in world coordinates

	exSurface = vec3(mdlMatrix * vec4(in_Position, 1.0)); // Send in world coordinates

	TexCoord_to_frag = inTexCoord;

	gl_Position = projectionMatrix * worldToViewMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
