#include "InitShader.h"
#include <vector>
#include <iostream>
#include <fstream>
#include<GL\glew.h>
#include <GLFW/glfw3.h>
//the result of shader object compiled
// shader: the name is shader object
// str: the string that indicates where compile erorr generated
GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
	//display the result of compiled shader;
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in" << str << std::endl;
	//get Log size
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

GLboolean printProgramInfoLog(GLuint program)
{
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Link erorr" << std::endl;
	//get Log size
	GLsizei bufSize;
	glGetShaderiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);

}
//create program
// vsrc: the string  in source program for vertex shader
// fsrc: the string in source program for fragment shader
GLuint createProgram(const char* vsrc, const char * fsrc)
{
	const GLuint program(glCreateProgram());

	if (vsrc != nullptr)
	{
		//create the shader object for vertex shader
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, nullptr);
		glCompileShader(vobj);
		//include the shader object for vertex shader in the program object
		if (printShaderInfoLog(vobj, " vertex shader"))
		{
			glAttachShader(program, vobj);
		}
		glDeleteShader(vobj);
	}

	if (fsrc != nullptr)
	{
		//create the shader object for fragment shader
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, nullptr);
		glCompileShader(fobj);
		//include the shader object for fragment shader in program object
		if (printShaderInfoLog(fobj, " fragment shader"))
		{
			glAttachShader(program, fobj);
		}
		//glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	//link program object

	//glBindAttribLocation(program, 0, "position");
	//glBindAttribLocation(program, 1, "normal");
	//glBindAttribLocation(program, 2, "uv");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	if (printProgramInfoLog(program)) {
		return program;
	}

	glDeleteProgram(program);
	return 0;
}

GLuint createProgram(const char * vsrc, const char * gsrc, const char * fsrc)
{
	const GLuint program(glCreateProgram());

	if (vsrc != nullptr)
	{
		//create the shader object for vertex shader
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, nullptr);
		glCompileShader(vobj);
		//include the shader object for vertex shader in the program object
		if (printShaderInfoLog(vobj, " vertex shader"))
		{
			glAttachShader(program, vobj);
		}
		glDeleteShader(vobj);
	}
	if (gsrc != nullptr)
	{
		const GLuint gobj(glCreateShader(GL_GEOMETRY_SHADER));
		glShaderSource(gobj, 1, &gsrc, nullptr);
		glCompileShader(gobj);
		if (printShaderInfoLog(gobj, "geometry shader"))
		{
			glAttachShader(program, gobj);
		}
		glDeleteShader(gobj);
	}
	if (fsrc != nullptr)
	{
		//create the shader object for fragment shader
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, nullptr);
		glCompileShader(fobj);
		//include the shader object for fragment shader in program object
		if (printShaderInfoLog(fobj, " fragment shader"))
		{
			glAttachShader(program, fobj);
		}
		//glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	if (printProgramInfoLog(program)) {
		return program;
	}

	glDeleteProgram(program);
	return 0;
}

//file input for shader
//name: file name
//buffer: source text
bool readShaderSource(const char* name, std::vector<GLchar> &buffer)
{
	if (name == nullptr) return false;

	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		std::cerr << "Erorr: Can't open source file:" << name << std::endl;
		return false;
	}
	//succceded file open

	// move file pointer to end to get file size
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());
	//allocate memory of file size(resize vector array
	buffer.resize(length + 1);
	//file read from the head
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if (file.fail())
	{
		std::cerr << "Erorr: Could not read source file" << name << std::endl;
		file.close();
		return false;
	}

	//succeeded file read
	file.close();
	return true;
}

//create a program object object from the source file for the shader
// vert: source file for the vertex shader
// frag: source file for the fragment shader
GLuint loadProgram(const char *vert, const char *frag)
{
	//load shader source
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));

	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));

	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

GLuint loadProgram(const char * vert, const char * geom, const char * frag)
{
	//load shader source
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));

	std::vector<GLchar> gsrc;
	const bool gstat(readShaderSource(geom, gsrc));

	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));

	return vstat && gstat && fstat ? createProgram(vsrc.data(), gsrc.data(),fsrc.data()) : 0;
}
