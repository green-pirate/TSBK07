#version 150

out vec4 out_Color;

in vec3 normal_to_frag;
in vec2 TexCoord_to_frag;
in vec3 exSurface; // Phong (specular)

uniform float elapsedTime;
uniform sampler2D texUnit;
uniform sampler2D secondTexUnit;
uniform int use_tex;
uniform vec3 color;
uniform mat4 worldToViewMatrix;
uniform vec3 cameraPos;


uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

float positionalLight(vec3 lightPos, float specularExponent);
float directionalLight(vec3 lightDirection, float specularExponent);


void main(void)
{
    int i;
    float shade[4];
    vec4 shaded_color[4];
    vec4 total_color = vec4(0,0,0,0);
    for(i = 0; i < 4; i++)
    {
        if(isDirectional[i])
        {
            shade[i] = directionalLight(lightSourcesDirPosArr[i], specularExponent[i]);
        }
        else
        {
            shade[i] = positionalLight(lightSourcesDirPosArr[i], specularExponent[i]);
        }

        shaded_color[i] = vec4(shade[i] * lightSourcesColorArr[i], 1.0f);
        total_color = total_color + shaded_color[i];
    }


	if(use_tex == 1)
	{
		out_Color = texture(texUnit, TexCoord_to_frag);
	}
    else if(use_tex == 2)
    {
        vec4 grass = sin(0.001 * elapsedTime) * texture(texUnit, TexCoord_to_frag);
        vec4 dirt = - sin(0.001 * elapsedTime) * texture(secondTexUnit, TexCoord_to_frag);
        grass = max(grass, 0);
        dirt = max(dirt, 0);
        out_Color = vec4(vec3(total_color * (grass + dirt)), 0.5);
    }
	else
	{
        out_Color = total_color;
	}
}

float positionalLight(vec3 lightPos, float specularExponent)
{
    // lightPos in world coordinates
    vec3 lightDirection = normalize(lightPos - exSurface);

    return directionalLight(lightDirection, specularExponent);
}

float directionalLight(vec3 lightDirection, float specularExponent)
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
        specular = 1.0 * pow(specular, specularExponent);
    specular = max(specular, 0.0);
    shade = 0.7*diffuse + 1.0*specular;

    return shade;
}
