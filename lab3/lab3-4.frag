#version 150

out vec4 out_Color;

in vec3 normal_to_frag;
in vec2 TexCoord_to_frag;
in vec3 exSurface; // Phong (specular)

uniform float elapsedTime;
uniform sampler2D texUnit;
uniform int use_tex;
uniform vec3 color;
uniform mat4 worldToViewMatrix;
uniform vec3 cameraPos;


uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

void main(void)
{
	vec3 lightPos = vec3(20, 0, 0);

	vec3 light = normalize(lightPos - exSurface);

	ÄNDRA SÅ ATT LJUSFLÄCKARNA FLYTTAR SIG MED RIKTNINGEN PÅ BETRAKTNINGSVEKTORN 

	float shade = dot(normalize(light), normalize(normal_to_frag));
	shade = clamp(shade, 0, 1);

	if(use_tex == 1)
	{
		out_Color = texture(texUnit, TexCoord_to_frag);
	}
	else
	{
		float diffuse;
		float specular = 0;
		float shade;

		// Diffuse
		diffuse = dot(normalize(normal_to_frag), light);
		diffuse = max(0.0, diffuse); // No negative light

		// Specular
		vec3 reflectedLightDirection = reflect(-light, normalize(normal_to_frag));
		vec3 eyeDirection = normalize(cameraPos - exSurface); // View direction
		if(dot(light, normal_to_frag) > 0.0)
		{
			specular = dot(reflectedLightDirection, eyeDirection);
			specular = max(specular, 0.01);
			specular = 1.0 * pow(specular, 10.0);
		}
		specular = max(specular, 0.01);
		shade = 0.5*diffuse + 0.5*specular;
		out_Color = vec4(shade, shade, shade, 1.0);
	}
}

float positionalLight(vec3 lightPos)
{
	vec3 light = normalize(lightPos - exSurface);

	float diffuse;
	float specular = 0;
	float shade;

	// Diffuse
	diffuse = dot(normalize(normal_to_frag), light);
	diffuse = max(0.0, diffuse); // No negative light

	// Specular
	if(dot(light, normal_to_frag) > 0.0)
	{
		specular = 1.0 * pow(diffuse, 300.0);
	}
	specular = max(specular, 0.01);
	shade = 0.5*diffuse + 0.5*specular;

	return shade;
}

float directionalLight(vec3 lightDirection)
{
	vec3 light = normalize(lightDirection);

	float diffuse;
	float specular = 0;
	float shade;

	// Diffuse
	diffuse = dot(normalize(normal_to_frag), light);
	diffuse = max(0.0, diffuse); // No negative light

	// Specular
	vec3 reflectedLightDirection = reflect(-light, normalize(normal_to_frag));
	vec3 eyeDirection = normalize(-exSurface); // View direction
	if(dot(light, normal_to_frag) > 0.0)
	{
		specular = dot(reflectedLightDirection, eyeDirection);
		specular = max(specular, 0.01);
		specular = 1.0 * pow(specular, 10.0);
	}
	specular = max(specular, 0.01);
	shade = 0.5*diffuse + 0.5*specular;

	return shade;
}
