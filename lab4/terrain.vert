#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;

// NY
uniform mat4 projMatrix;
uniform mat4 modelToViewMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(modelToViewMatrix);
	texCoord = inTexCoord;
	gl_Position = projMatrix * modelToViewMatrix * vec4(inPosition, 1.0);
}
