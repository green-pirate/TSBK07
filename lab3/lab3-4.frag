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

float positionalLight(vec3 lightPos);
float directionalLight(vec3 lightDirection);


void main(void)
{
	if(use_tex == 1)
	{
		out_Color = texture(texUnit, TexCoord_to_frag);
	}
	else
	{
        float shade = positionalLight(vec3(20,0,0));
        out_Color = vec4(shade, shade, shade, 1.0);
	}
}

float positionalLight(vec3 lightPos)
{
    // lightPos in world coordinates
    vec3 lightDirection = normalize(lightPos - exSurface);

    return directionalLight(lightDirection);
}

float directionalLight(vec3 lightDirection)
{
    // vertex shader sends all in world coordinates
    vec3 light = normalize(lightDirection);
    float diffuse, specular, shade;

    // Diffuse
    diffuse = dot(normalize(normal_to_frag), light);
    diffuse = max(0.0, diffuse); // No negative light

    // Specular
    vec3 r = reflect(-light, normalize(normal_to_frag));
    vec3 v = normalize(cameraPos - exSurface); // View direction
    specular = dot(r, v);
    if (specular > 0.0)
        specular = 1.0 * pow(specular, 400.0);
    specular = max(specular, 0.0);
    shade = 0.7*diffuse + 1.0*specular;

    return shade;
}
