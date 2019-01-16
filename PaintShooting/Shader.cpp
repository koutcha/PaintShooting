#include "Shader.h"
#include "InitShader.h"
#include <iostream>

GLuint Shader::beforeUse = 0;

Shader::Shader(const std::string & vert, const std::string & frag):
	programID(0)
{
	set(vert, frag);

}

Shader::Shader(const std::string & vert, const std::string & geom, const std::string & frag)
{
	set(vert, geom, frag);
}

Shader::~Shader()
{
	if(programID != 0)
	glDeleteProgram(programID);
}

void Shader::set(const std::string & vert, const std::string & frag)
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
	}

	
	if (programID = loadProgram(vert.c_str(), frag.c_str()))
	{
		std::cout << vert << " :" << frag << "suceed" << std::endl;
	}
	else
	{

	}

}

void Shader::set(const std::string & vert, const std::string & geom, const std::string & frag)
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
	}

	if (programID = loadProgram(vert.c_str(),geom.c_str(), frag.c_str()))
	{
		std::cout << vert << " : " << geom<<" : "<<frag << "suceed" << std::endl;
	}
	else
	{
	
	}
}


void Shader::use() const
{
	if (programID != 0)
	{
		glUseProgram(programID);
	}
	
}

void Shader::unUse() const
{
	glUseProgram(0);
	beforeUse = 0;
}


GLuint Shader::getUniformLoc(const std::string & name)const
{
	return glGetUniformLocation(programID, name.c_str());
}

void Shader::blockBinding(const std::string & name, unsigned int unit) const
{
	glUniformBlockBinding(programID, glGetUniformBlockIndex(programID, name.c_str()), unit);
}


