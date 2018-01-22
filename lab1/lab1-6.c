// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include <math.h>
#include <stdio.h>

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f,-0.5f,-0.5f, // triangle 1 : begin
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
    0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
};

GLfloat a = 0.0f;

GLfloat myMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f,
	  									0.0f, 1.0f, 0.0f, 0.0f,
  										0.0f, 0.0f, 1.0f, 0.0f,
  										0.0f, 0.0f, 0.0f, 1.0f };

GLuint program;

Model *m;

// vertex array object
unsigned int vertexArrayObjID;

unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;

void OnTimer(int value)
{
		//GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
		//printf("%f\n", t);
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	m = LoadModel("bunny.obj");

	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID[2];
	// Reference to shader program



	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-6.vert", "lab1-6.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(2, vertexBufferObjID);

	glGenVertexArrays(1, &bunnyVertexArrayObjID);
  glGenBuffers(1, &bunnyVertexBufferObjID);
  glGenBuffers(1, &bunnyIndexBufferObjID);
  glGenBuffers(1, &bunnyNormalBufferObjID);

  glBindVertexArray(bunnyVertexArrayObjID);


	// VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

  // VBO for normal data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

/*
	GLfloat color[] = {
		0.583f,  0.771f,  0.014f, 1.0f,
    0.609f,  0.115f,  0.436f, 1.0f,
    0.327f,  0.483f,  0.844f, 1.0f,
    0.822f,  0.569f,  0.201f, 1.0f,
    0.435f,  0.602f,  0.223f, 1.0f,
    0.310f,  0.747f,  0.185f, 1.0f,
    0.597f,  0.770f,  0.761f, 1.0f,
    0.559f,  0.436f,  0.730f, 1.0f,
    0.359f,  0.583f,  0.152f, 1.0f,
    0.483f,  0.596f,  0.789f, 1.0f,
    0.559f,  0.861f,  0.639f, 1.0f,
    0.195f,  0.548f,  0.859f, 1.0f,
    0.014f,  0.184f,  0.576f, 1.0f,
    0.771f,  0.328f,  0.970f, 1.0f,
    0.406f,  0.615f,  0.116f, 1.0f,
    0.676f,  0.977f,  0.133f, 1.0f,
    0.971f,  0.572f,  0.833f, 1.0f,
    0.140f,  0.616f,  0.489f, 1.0f,
    0.997f,  0.513f,  0.064f, 1.0f,
    0.945f,  0.719f,  0.592f, 1.0f,
    0.543f,  0.021f,  0.978f, 1.0f,
    0.279f,  0.317f,  0.505f, 1.0f,
    0.167f,  0.620f,  0.077f, 1.0f,
    0.347f,  0.857f,  0.137f, 1.0f,
    0.055f,  0.953f,  0.042f, 1.0f,
    0.714f,  0.505f,  0.345f, 1.0f,
    0.783f,  0.290f,  0.734f, 1.0f,
    0.722f,  0.645f,  0.174f, 1.0f,
    0.302f,  0.455f,  0.848f, 1.0f,
    0.225f,  0.587f,  0.040f, 1.0f,
    0.517f,  0.713f,  0.338f, 1.0f,
    0.053f,  0.959f,  0.120f, 1.0f,
    0.393f,  0.621f,  0.362f, 1.0f,
    0.673f,  0.211f,  0.457f, 1.0f,
    0.820f,  0.883f,  0.371f, 1.0f,
    0.982f,  0.099f,  0.879f, 1.0f};
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_color"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_color"));
*/
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

	// End of upload of geometry

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

	//glBindVertexArray(vertexArrayObjID);	// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object

	printError("display");

	glutSwapBuffers();
/*
	GLfloat Rotx[] = {1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, cos(a), -sin(a), 0.0f,
										0.0f, sin(a), cos(a), 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f};

	GLfloat Roty[] = {cos(a), 0.0f, sin(a), 0.0f,
										0.0f, 1.0f, 0.0f, 0.0f,
										-sin(a), 0.0f, cos(a), 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f};

	GLfloat Rotz[] = {cos(a), -sin(a), 0.0f, 0.0f,
										sin(a), cos(a), 0.0f, 0.0f,
										0.0f, 0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f};
*/
	myMatrix[0] = cos(a)*cos(a);
	myMatrix[1] = -cos(a)*sin(a);
	myMatrix[2] = sin(a);
	myMatrix[4] = sin(a)*sin(a)*cos(a) + cos(a)*sin(a);
	myMatrix[5] = -sin(a)*sin(a)*sin(a) + cos(a)*cos(a);
	myMatrix[6] = -sin(a)*cos(a);
	myMatrix[8] = -cos(a)*sin(a)*cos(a) + sin(a)*sin(a);
	myMatrix[9] = cos(a)*sin(a)*sin(a) + sin(a)*cos(a);
	myMatrix[10] = cos(a)*cos(a);
	a += 0.05f;
	//glUniformMatrix4fv(glGetUniformLocation(program, "Rotx"), 1, GL_TRUE, Rotx);
	//glUniformMatrix4fv(glGetUniformLocation(program, "Roty"), 1, GL_TRUE, Roty);
	//glUniformMatrix4fv(glGetUniformLocation(program, "Rotz"), 1, GL_TRUE, Rotz);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
