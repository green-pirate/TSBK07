#version 150

out vec4 out_Color;

in vec3 normal_to_frag;
//in vec2 TexCoord_to_frag;
in vec3 surf;

uniform float elapsedTime;
//uniform sampler2D texUnit;

void main(void)
{
	const vec3 lightDirection = vec3(0.58, 0.58, 0.58);

	float shade = dot(lightDirection, normalize(normal_to_frag));
	shade = clamp(shade, 0, 1);


	/*
	// Specular
	float diffuseStrength = 1.5f;

	vec3 reflectedLightDirection = reflect(-lightDirection, normal_to_frag);
	vec3 eyeDirection = normalize(-surf);
	float specularStrength = 0.0;
	if (dot(lightDirection, normal_to_frag) > 0.0)
	{
		specularStrength = dot(reflectedLightDirection, eyeDirection);
		float exponent = 200.0;
		specularStrength = max(specularStrength, 0.01);
		specularStrength = pow(specularStrength, exponent);
	}

	out_Color = vec4(diffuseStrength*0.5 + specularStrength*0.5);
	*/


	out_Color = vec4(vec3(shade), 0.0f);

	//out_Color = texture(texUnit, TexCoord_to_frag);
}
