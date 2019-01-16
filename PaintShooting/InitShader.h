#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include<GL\glew.h>
#include <GLFW/glfw3.h>
//the result of shader object compiled
// shader: the name is shader object
// str: the string that indicates where compile erorr generated
GLboolean printShaderInfoLog(GLuint shader, const char *str);
GLboolean printProgramInfoLog(GLuint program);
//create program
// vsrc: the string  in source program for vertex shader
// fsrc: the string in source program for fragment shader
GLuint createProgram(const char* vsrc, const char * fsrc);
GLuint createProgram(const char* vart, const char* geom, const char* frag);
//file input for shader
//name: file name
//buffer: source text
bool readShaderSource(const char* name, std::vector<GLchar> &buffer);

//create a program object object from the source file for the shader
// vert: source file for the vertex shader
// frag: source file for the fragment shader
GLuint loadProgram(const char *vert, const char *frag);
GLuint loadProgram(const char* vart, const char* geom, const char* frag);