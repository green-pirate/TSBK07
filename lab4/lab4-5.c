// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


#include <time.h>
#include <stdlib.h>

void handleKeyboardEvent();
void draw(Model* model, mat4 mdlMatrix);
void drawLake(Model* model, mat4 mdlMatrix);
void drawSkybox();
void drawTerrain();

mat4 projectionMatrix;
vec3 camPos = {10.0f, 0.0f, 0.0f};
vec3 camLookAt = {0.0f, 0.0f, 0.0f};
vec3 camUp = {0.0f, 1.0f, 0.0f};

GLfloat scaling_factor = 20.0;


float GetHeight(TextureData *tex, float x, float z)
{
    int x_floored = floor(x);
    int z_floored = floor(z);
    int x_ceiled = ceil(x);
    int z_ceiled = ceil(z);



    float height = 0;


    if((x_floored >= 0 && x_ceiled < tex->width ) && (z_floored >= 0 && z_ceiled < tex->height))
    {
        float x_floor_z_floor_height = tex->imageData[(x_floored + z_floored * tex->width) * (tex->bpp/8)] / scaling_factor;
        float x_floor_z_ceil_height = tex->imageData[(x_floored + z_ceiled * tex->width) * (tex->bpp/8)] / scaling_factor;
        float x_ceil_z_floor_height = tex->imageData[(x_ceiled + z_floored * tex->width) * (tex->bpp/8)] / scaling_factor;
        float x_ceil_z_ceil_height = tex->imageData[(x_ceiled + z_ceiled * tex->width) * (tex->bpp/8)] / scaling_factor;


        // Bottom triangle
        if(x - x_floored <= z - z_floored)
        {
            float x_incline = x_ceil_z_floor_height - x_floor_z_floor_height;
            float z_incline = x_floor_z_ceil_height - x_floor_z_floor_height;

            height = ((x - x_floored)*x_incline + (z - z_floored)*z_incline) + x_floor_z_floor_height;
        }
        // Top triangle
        else
        {
            float x_incline = x_ceil_z_ceil_height - x_floor_z_ceil_height;
            float z_incline = x_ceil_z_ceil_height - x_ceil_z_floor_height;

            height = x_ceil_z_ceil_height - ((x_ceiled - x)*x_incline + (z_ceiled - z)*z_incline);
        }

        //return tex->imageData[(x_int + z_int * tex->width) * (tex->bpp/8)] / scaling_factor;
    }

    // Lack of error handling...
    return height;
}


Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

    srand(time(NULL));

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / scaling_factor;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
            GLfloat x_prev_intensity = 0;
            GLfloat x_next_intensity = 0;
            GLfloat z_prev_intensity = 0;
            GLfloat z_next_intensity = 0;
            GLfloat intensity = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / scaling_factor;
            if(x > 0)
            {
                x_prev_intensity = tex->imageData[((x-1) + z * tex->width) * (tex->bpp/8)] / scaling_factor;
            }
            if(x < tex->width - 1)
            {
                x_next_intensity = tex->imageData[((x+1) + z * tex->width) * (tex->bpp/8)] / scaling_factor;
            }
            if(z > 0)
            {
                z_prev_intensity = tex->imageData[(x + (z-1) * tex->width) * (tex->bpp/8)] / scaling_factor;
            }
            if(z < tex->height - 1)
            {
                z_next_intensity = tex->imageData[(x + (z+1) * tex->width) * (tex->bpp/8)] / scaling_factor;
            }
            if(x == 0)
            {
                x_prev_intensity = intensity - (x_next_intensity - intensity);
            }
            else if(x == tex->width - 1)
            {
                x_next_intensity = intensity - (x_prev_intensity - intensity);
            }
            if(z == 0)
            {
                z_prev_intensity = intensity - (z_next_intensity - intensity);
            }
            else if(z == tex->height -1)
            {
                z_next_intensity = intensity - (z_prev_intensity - intensity);
            }

            vec3 normal = {x_prev_intensity - x_next_intensity, 1, z_prev_intensity - z_next_intensity};
            normal = Normalize(normal);

			normalArray[(x + z * tex->width)*3 + 0] = normal.x;
			normalArray[(x + z * tex->width)*3 + 1] = normal.y;
			normalArray[(x + z * tex->width)*3 + 2] = normal.z;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

    printf("Triangle count: %d\n", triangleCount);

	return model;
}


// vertex array object
Model *m, *m2, *tm, *boll, *lake_model, *skyModel, *borg1, *borg2;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex, lake_texture; // terrain
GLuint skyTex;
GLuint dirtTex;

Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                                 {0.0f, 1.0f, 0.0f}, // Green light
                                 {0.0f, 0.0f, 1.0f}, // Blue light
                                 {1.0f, 1.0f, 1.0f} }; // White light
GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};
Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                                       {0.0f, 5.0f, 10.0f}, // Green light, positional
                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                       {0.0f, 0.0f, -1.0f} }; // White light along Z


void init(void)
{
	// GL inits
	glClearColor(1,0,0,1);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printError("GL inits");

    boll = LoadModelPlus("./groundsphere.obj");
    borg1 = LoadModelPlus("./rooftops.obj");
    borg2 = LoadModelPlus("./walls.obj");
    skyModel = LoadModelPlus("./skybox.obj");

    LoadTGATextureSimple("SkyBox512.tga", &skyTex);
    LoadTGATextureSimple("grassplus.tga", &tex1);
    LoadTGATextureSimple("dirt.tga", &dirtTex);

	// Load and compile shader
	program = loadShaders("lab4-5.vert", "lab4-5.frag");
	glUseProgram(program);
	printError("init shader");

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);


    // Load terrain data
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

    // Load lake
    LoadTGATextureData("lake_bottom.tga", &lake_texture);
	lake_model = GenerateTerrain(&lake_texture);
	printError("init lake");

    camPos.y = GetHeight(&ttex, camPos.x, camPos.z) + 5;

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, skyTex);
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dirtTex);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handleKeyboardEvent();
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	printError("pre display");


	// Build matrix
    mat4 worldToView = lookAtv(camPos, camLookAt, camUp);
    glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, &camPos.x);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToViewMatrix"), 1, GL_TRUE, worldToView.m);


    // Draw world
    drawSkybox();
    drawLake(lake_model, T(0,-GetHeight(&lake_texture, 0, 0),0));
    drawTerrain();


    // Draw spheres
    glUniform1i(glGetUniformLocation(program, "drawing_objects"), 1);
    draw(borg1, Mult(T(107,GetHeight(&ttex,107,215),215),S(0.1,0.1,0.1)));
    draw(borg2, Mult(T(107,GetHeight(&ttex,107,215),215),S(0.1,0.1,0.1)));
    //draw(boll, Mult(T(127,GetHeight(&ttex,127,235),235),S(3,3,3)));
    draw(boll, Mult(T(67,GetHeight(&ttex,67,89),89),S(3,3,3)));
    draw(boll, Mult(T(200,GetHeight(&ttex,200,127),127),S(3,3,3)));
    draw(boll, Mult(T(100 + 100*sin(0.0001*t),GetHeight(&ttex,100 + 100*sin(0.0001*t),75),75),S(3,3,3)));
    glUniform1i(glGetUniformLocation(program, "drawing_objects"), 0);

	printError("display 2");

	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}

void draw(Model* model, mat4 mdlMatrix)
{
	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorldMatrix"), 1, GL_TRUE, mdlMatrix.m);
	DrawModel(model, program, "inPosition", "inNormal", "inTexCoord");
}

void drawLake(Model* model, mat4 mdlMatrix)
{
    glUniform1i(glGetUniformLocation(program, "drawing_lake_bottom"), 1);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorldMatrix"), 1, GL_TRUE, mdlMatrix.m);
    glUniform1i(glGetUniformLocation(program, "tex"), 0);
	DrawModel(model, program, "inPosition", "inNormal", "inTexCoord");
    glUniform1i(glGetUniformLocation(program, "drawing_lake_bottom"), 0);
}

void drawSkybox()
{
	glDisable(GL_DEPTH_TEST);
    glUniform1i(glGetUniformLocation(program, "drawing_skyBox"), 1);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorldMatrix"), 1, GL_TRUE, T(camPos.x,camPos.y - 0.2,camPos.z).m);
	glUniform1i(glGetUniformLocation(program, "tex"), 1);
	DrawModel(skyModel, program,"inPosition","inNormal","inTexCoord");
    glUniform1i(glGetUniformLocation(program, "drawing_skyBox"), 0);
	glEnable(GL_DEPTH_TEST);
}

void drawTerrain()
{
	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorldMatrix"), 1, GL_TRUE, IdentityMatrix().m);
    glUniform1i(glGetUniformLocation(program, "tex"), 0);
    glUniform1i(glGetUniformLocation(program, "dirtTex"), 2);
    DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord"); // Draw terrain
}

void handleKeyboardEvent()
{

	if (glutKeyIsDown('w'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorAdd(camLookAt, stepDirection);
		camPos = VectorAdd(camPos, stepDirection);
        float camPos_y = camPos.y;
        camPos.y = GetHeight(&ttex, camPos.x, camPos.z) + 5;
        camLookAt.y = camLookAt.y - (camPos.y - camPos_y);
	}

	if (glutKeyIsDown('s'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorSub(camLookAt, stepDirection);
		camPos = VectorSub(camPos, stepDirection);
        float camPos_y = camPos.y;
        camPos.y = GetHeight(&ttex, camPos.x, camPos.z) + 5;
        camLookAt.y = camLookAt.y - (camPos.y - camPos_y);
	}

	if (glutKeyIsDown('d'))
	{
		vec3 stepDirection = CrossProduct(VectorSub(camLookAt, camPos), camUp);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorAdd(camLookAt, stepDirection);
		camPos = VectorAdd(camPos, stepDirection);
        float camPos_y = camPos.y;
        camPos.y = GetHeight(&ttex, camPos.x, camPos.z) + 5;
        camLookAt.y = camLookAt.y - (camPos.y - camPos_y);
	}

	if (glutKeyIsDown('a'))
	{
		vec3 stepDirection = CrossProduct(VectorSub(camLookAt, camPos), camUp);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorSub(camLookAt, stepDirection);
		camPos = VectorSub(camPos, stepDirection);
        float camPos_y = camPos.y;
        camPos.y = GetHeight(&ttex, camPos.x, camPos.z) + 5;
        camLookAt.y = camLookAt.y - (camPos.y - camPos_y);
	}

	if (glutKeyIsDown('q'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(camUp, 0.05)), stepDirection);
				camLookAt = VectorAdd(camPos, camLookAt);
	}

	if (glutKeyIsDown('e'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(camUp, -0.05)), stepDirection);
		camLookAt = VectorAdd(camPos, camLookAt);
	}

	/*if (glutKeyIsDown('r'))
	{
		camLookAt = VectorAdd(camLookAt, Normalize(camUp));
		camPos = VectorAdd(camPos, Normalize(camUp));
	}

	if (glutKeyIsDown('f'))
	{
		camLookAt = VectorSub(camLookAt, Normalize(camUp));
		camPos = VectorSub(camPos, Normalize(camUp));
	}*/

	if (glutKeyIsDown('t'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		vec3 planarComp = CrossProduct(stepDirection, camUp);

		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(planarComp, 0.05)), stepDirection);
		camLookAt = VectorAdd(camPos, camLookAt);

		camUp = Normalize(CrossProduct(planarComp, VectorSub(camLookAt, camPos)));
	}

	if (glutKeyIsDown('g'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		vec3 planarComp = CrossProduct(stepDirection, camUp);

		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(planarComp, -0.05)), stepDirection);
		camLookAt = VectorAdd(camPos, camLookAt);

		camUp = Normalize(CrossProduct(planarComp, VectorSub(camLookAt, camPos)));
	}
}
