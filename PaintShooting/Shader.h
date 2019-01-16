#pragma once
#include "GL\glew.h"
#include <string>
class Shader
{
public:
	Shader(const std::string& vert, const std::string& frag);
	Shader(const std::string& vert, const std::string& geom, const std::string& frag);
	virtual ~Shader();
	void set(const std::string& vert, const std::string& frag);
	void set(const std::string& vert, const std::string& geom, const std::string& frag);
	

	void use()const;
	void unUse()const;
	GLuint getUniformLoc(const std::string& name)const;
	//uniromBlock‚ðƒoƒCƒ“ƒh
	void blockBinding(const std::string& name,unsigned int unit)const;
private:
	static GLuint beforeUse;
	GLuint programID;
};

