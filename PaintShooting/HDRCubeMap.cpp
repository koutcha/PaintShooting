#define _USE_MATH_DEFINES
#include "HDRCubeMap.h"
#include <Iostream>
#include "GL/glew.h"

#include "Shader.h"
#include "PerspectiveCamera.h"

#include "IBLSubroutine.h"

#include "stb_image.h"

 const std::string HDRCubeMap::hdrToCubePath("IBLShader\\renderHDRtoCube");

HDRCubeMap::HDRCubeMap(const std::string& hdrImage)
{

	unsigned int fbo;
	unsigned int rbo;
	
	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rbo);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	//ok
	stbi_set_flip_vertically_on_load(true);
	int width, height, component;

	unsigned hdrTexture;
	float *data = stbi_loadf(hdrImage.c_str(), &width, &height, &component,0);

	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		stbi_set_flip_vertically_on_load(false);
	}
	else
	{
		stbi_set_flip_vertically_on_load(false);
		std::cout << "ERORR HDR CUBE MAP: Failed to load "<< hdrImage << std::endl;
		return;
	}

	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//ok

	Shader hdrToCube(hdrToCubePath  + ".vert", hdrToCubePath + ".frag");

	PerspectiveCamera captureCamera(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), 0.1f, 10.0f, static_cast<float>(M_PI) / 2.0f, 1.0f);

	std::array<Vector3f,12> captureVeiw(
	{
		Vector3f(1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f),
		Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f),
		Vector3f(0.0f,  1.0f,  0.0f), Vector3f(0.0f,  0.0f,  1.0f),
		Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f, -1.0f),
		Vector3f(0.0f,  0.0f,  1.0f), Vector3f(0.0f, -1.0f,  0.0f),
		Vector3f(0.0f,  0.0f, -1.0f), Vector3f(0.0f, -1.0f,  0.0f)
	});

	hdrToCube.use();

	glUniform1i(hdrToCube.getUniformLoc("equirectanglarMap"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	for (unsigned int i = 0; i < 6; ++i)
	{
		captureCamera.setGaze(captureVeiw[2 * i]);
		captureCamera.setUp(captureVeiw[2 * i + 1]);
		captureCamera.setCamera(hdrToCube.getUniformLoc("view"), hdrToCube.getUniformLoc("projection"));

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ok
		IBLSuberoutan::renderCube();
		//ok
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &hdrTexture);

}

HDRCubeMap::~HDRCubeMap()
{
	glDeleteTextures(1, &cubeMap);
}

void HDRCubeMap::bind(unsigned int unit)const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
}

void HDRCubeMap::unbind(unsigned int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void HDRCubeMap::renderCube()
{
}
