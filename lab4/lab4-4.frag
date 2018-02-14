#version 150

out vec4 outColor;

in vec2 texCoord;
in vec3 normal;
in vec3 exSurface;

uniform vec3 cameraPos;
uniform sampler2D tex;

float positionalLight(vec3 lightPos, float specularExponent);
float directionalLight(vec3 lightDirection, float specularExponent);


void main(void)
{
    vec3 lightDir = vec3(1,1,1);
    float shade = directionalLight(lightDir, 0.0);

	outColor = vec4(vec3(2 * shade), 1.0)  * texture(tex, texCoord);
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
    diffuse = dot(normalize(normal), light);
    diffuse = max(0.0, diffuse); // No negative light

    // Specular
    vec3 r = reflect(-light, normalize(normal));
    vec3 v = normalize(cameraPos - exSurface); // View direction
    specular = dot(r, v);
    if (specular > 0.0)
        specular = 1.0 * pow(specular, specularExponent);
    specular = max(specular, 0.0);
    shade = 0.7*diffuse + 0.0*specular;
    return shade;
}
