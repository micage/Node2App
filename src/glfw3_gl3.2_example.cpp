// #include <OpenGL/GL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
	GLfloat x, y, z, nx, ny, nz, u0, v0;
	Vertex() : x(0.0), y(0.0), z(0.0), nx(0.0), ny(0.0), nz(0.0), u0(0.0), v0(0.0){};
};

struct Mesh {
	Vertex *vertecies;
	int num_verts;
	int num_idx;
	unsigned short* indicies;

	void setVertices(Vertex *verts, int num){
		vertecies = verts; num_verts = num;
	}
	void setIndices(unsigned short* idx, int num){
		indicies = idx; num_idx = num;
	}
};

GLuint _vertexBufferObjectID = 0, _indexBufferObjectID = 0, _vertexArrayObjectID = 0;
GLuint vertex_shader = 0, fragment_shader = 0, _programID = 0;
glm::mat4 mvp_mat;

GLFWwindow*_mainWindow;
int _mainWindowWidth;
int _mainWindowHeight;
const char *_openGLVersion;

Mesh *_mesh;
int _vertexCount = 6;

void create_buffers();
void setup_shaders();

static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main(void)
{
	int width = 600, height = 600;
	const char *title = "glfwtest";
	GLFWmonitor *monitor = NULL;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	////////////////////////////////////////////////////////////////
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	_mainWindow = glfwCreateWindow(width, height, title, monitor, NULL);
	if (_mainWindow == NULL)
	{
		return false;
	}
	_mainWindowWidth = width;
	_mainWindowHeight = height;
	glfwSetKeyCallback(_mainWindow, key_callback);
	glfwMakeContextCurrent(_mainWindow);

	glewExperimental = GL_TRUE;
	glewInit();

	_openGLVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	//////////////////////////////////////////////////////////////////////
	printf("version:%s", _openGLVersion);


	if (!_mainWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(_mainWindow);

	setup_shaders();
	create_buffers();

	glClearColor(0.5, 0.5, 0.5, 0.0);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(_mainWindow))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(_mainWindow, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Gets completely ignored under core profile
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		//glBegin(GL_TRIANGLES);
		//glColor3f(1.f, 0.f, 0.f);
		//glVertex3f(-0.6f, -0.4f, 0.f);
		//glColor3f(0.f, 1.f, 0.f);
		//glVertex3f(0.6f, -0.4f, 0.f);
		//glColor3f(0.f, 0.f, 1.f);
		//glVertex3f(0.f, 0.6f, 0.f);
		//glEnd();

		float rotval = 0.0f;
		//to test motion:
		//rotval = (float) glfwGetTime() * 80.f;
		glm::mat4 mvp_final = mvp_mat * glm::rotate(glm::mat4(1.0),rotval , glm::vec3(0.0, 0.0, 1.0));
		glUseProgram(_programID);
		GLuint mvp = glGetUniformLocation(_programID, "ModelViewProjection");
		glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(mvp_final));

		glBindVertexArray(_vertexArrayObjectID);

		// Draw
		glDrawRangeElements(GL_TRIANGLES, 0, 4, _vertexCount, GL_UNSIGNED_SHORT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);


		glfwSwapBuffers(_mainWindow);
		glfwPollEvents();
	}
	glfwDestroyWindow(_mainWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}





void create_buffers(){
	float width = 500.0, height = 500.0;

	_mesh = new Mesh();
	Vertex* vertices = new Vertex[6];

	vertices[0].nz = 1.0f;
	vertices[1].nz = 1.0f;
	vertices[2].nz = 1.0f;
	vertices[3].nz = 1.0f;
	vertices[4].nz = 1.0f;
	vertices[5].nz = 1.0f;

	vertices[1].y = height;
	vertices[1].v0 = 1.0f;

	vertices[2].x = width;
	vertices[2].y = height;
	vertices[2].u0 = 1.0f;
	vertices[2].v0 = 1.0f;

	vertices[4].x = width;
	vertices[4].u0 = 1.0f;

	vertices[5].x = width;
	vertices[5].y = height;
	vertices[5].u0 = 1.0f;
	vertices[5].v0 = 1.0f;

	_mesh->setVertices(vertices, 6);
	vertices = NULL;

	unsigned short * indices = new unsigned short[6];

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 4;
	indices[5] = 2;

	//
	//indices[0] = 0;
	//indices[1] = 2;
	//indices[2] = 3;
	//indices[3] = 0;
	//indices[4] = 1;
	//indices[5] = 3;

	_mesh->setIndices(indices, 6);
	indices = NULL;

	//////////////////////////////////////////////////////////
	Vertex *_vertices = _mesh->vertecies;
	unsigned short*_indices = _mesh->indicies;
	//////////////////////////////////////////////////////////
	// Update VBO
	if (_vertexBufferObjectID == 0)
	{
		glGenBuffers(1, &_vertexBufferObjectID);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);

	float* data = new float[_vertexCount * sizeof(Vertex) / sizeof(float) ];
	long int begin = 0;
	for (int i = 0; i < _vertexCount; i++)
	{
		begin = i * 8;
		data[begin] = _vertices[i].x;
		data[begin + 1] = _vertices[i].y;
		data[begin + 2] = _vertices[i].z;
		data[begin + 3] = _vertices[i].nx;
		data[begin + 4] = _vertices[i].ny;
		data[begin + 5] = _vertices[i].nz;
		data[begin + 6] = _vertices[i].u0;
		data[begin + 7] = _vertices[i].v0;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertexCount, &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete [] data;
	data = NULL;

	// Update IBO
	if (_indexBufferObjectID == 0)
	{
		glGenBuffers(1, &_indexBufferObjectID);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * _vertexCount, &_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Update VAO
	if (_vertexArrayObjectID == 0)
	{
		glGenVertexArrays(1, &_vertexArrayObjectID);
	}
	glBindVertexArray(_vertexArrayObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);
	// Vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*) NULL) + (0));
	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*) NULL) + (12));
	// TexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*) NULL) + (24));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObjectID);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void printMat(glm::mat4 &mat){
	int i, j;
	for (j = 0; j < 4; j++){
		for (i = 0; i < 4; i++){
			printf("%f ", mat[i][j]);
		}
		printf("\n");
	}
}

void setup_shaders(){

	//Make sure you use float values here
	mvp_mat = glm::ortho(0.0f, (float) _mainWindowWidth, 0.0f, (float) _mainWindowHeight);
	//printf("\n MatOrthoNorm:\n"); printMat(mvp_mat);


	//mvp_mat = glm::perspective(60.0f, (float) _mainWindowWidth / (float) _mainWindowHeight, 0.1f, 100.0f);
	//printf("\n MatPersp:\n"); printMat(mvp_mat);

	//optional for orthographic projection, doesn't do much 
	//mvp_mat = mvp_mat * glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,0),glm::vec3(0,1,0));
	//printf("\n MatLookAt:\n"); printMat(mvp_mat);

	//mvp_mat = glm::mat4(1.0);


	const char *_vertexShaderSource =
		"#version 150 core\n"
		"in vec3 in_Position;\n"
		"in vec3 in_Normal;\n"
		"in vec2 in_TexCoord;\n"
		"uniform mat4 ModelViewProjection;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = ModelViewProjection * vec4(in_Position, 1.0);\n"
		"}\n";
	const char *_fragmentShaderSource =
		"#version 150 core\n"
		"out vec4 FColor;"
		"void main()\n"
		"{\n"
		"   FColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
		"}\n";

	GLint result, loglen;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex_shader, 1, &_vertexShaderSource, NULL);
	glShaderSource(fragment_shader, 1, &_fragmentShaderSource, NULL);

	glCompileShader(vertex_shader);
	glGetProgramiv(vertex_shader, GL_COMPILE_STATUS, &result);
	glGetProgramiv(vertex_shader, GL_INFO_LOG_LENGTH, &loglen);

	if (result == GL_FALSE && loglen > 0)
	{
		char* log = new char[loglen + 1];

		glGetProgramInfoLog(vertex_shader, loglen + 1, 0, log);
		//DANGER, passing pointer to content that is freed in the next lines
		//_lastInfoLog = log; 
		printf("vertex_log: %s", log);
		delete[] log;
		log = NULL;
	}

	glCompileShader(fragment_shader);
	glGetProgramiv(fragment_shader, GL_COMPILE_STATUS, &result);
	glGetProgramiv(fragment_shader, GL_INFO_LOG_LENGTH, &loglen);

	if (result == GL_FALSE && loglen > 0)
	{
		char* log = new char[loglen + 1];

		glGetProgramInfoLog(fragment_shader, loglen + 1, 0, log);
		//_lastInfoLog = log; //DANGER
		printf("fragment_log: %s", log);
		delete[] log;
		log = NULL;
	}

	_programID = glCreateProgram();
	glAttachShader(_programID, vertex_shader);
	glAttachShader(_programID, fragment_shader);

	//might have to be put here, I think this needs to be set before linking
	//otherwise it has no effect
	//glBindAttribLocation(_programID, 0, "in_Position");
	//glBindAttribLocation(_programID, 1, "in_Normal");
	//glBindAttribLocation(_programID, 2, "in_TexCoord");
	//glBindFragDataLocation(_programID, 0, "FColor");


	glLinkProgram(_programID);
	glGetProgramiv(_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &loglen);

	if (result == GL_FALSE && loglen > 0)
	{
		char* log = new char[loglen + 1];

		glGetProgramInfoLog(_programID, loglen + 1, 0, log);
		//_lastInfoLog = log; //DANGER
		printf("link_log: %s", log);
		delete[] log;
		log = NULL;
	}





	if (result == GL_FALSE)
	{
		glDeleteProgram(_programID);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return;
	}

	glUseProgram(_programID);
	glBindAttribLocation(_programID, 0, "in_Position");
	glBindAttribLocation(_programID, 1, "in_Normal");
	glBindAttribLocation(_programID, 2, "in_TexCoord");
	glBindFragDataLocation(_programID, 0, "FColor");
	//if you want the last 4 calls to do anything at all you need to link again or do it before linking
	//in this case it works out either way but just by luck
	//glLinkProgram(_programID);
	glUseProgram(0);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}