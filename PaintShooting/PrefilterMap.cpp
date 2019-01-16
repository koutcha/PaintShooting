#define _USE_MATH_DEFINES
#include "PrefilterMap.h"
#include "GL/glew.h"
#include "HDRCubeMap.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "IBLSubroutine.h"

const std::string PrefilterMap::shaderPath("IBLShader\\CubeToPrefilter");
PrefilterMap::PrefilterMap(const HDRCubeMap & envMap)
{


	unsigned int fbo;
	unsigned int rbo;

	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rbo);

	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 128, 128);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	Shader shader(shaderPath + ".vert", shaderPath + ".frag");

	PerspectiveCamera captureCamera(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), 0.1f, 10.0f, M_PI / 2.0f, 1.0f);

	std::array<Vector3f, 12> captureVeiw(
		{
			Vector3f(1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f),
			Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f),
			Vector3f(0.0f,  1.0f,  0.0f), Vector3f(0.0f,  0.0f,  1.0f),
			Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f, -1.0f),
			Vector3f(0.0f,  0.0f,  1.0f), Vector3f(0.0f, -1.0f,  0.0f),
			Vector3f(0.0f,  0.0f, -1.0f), Vector3f(0.0f, -1.0f,  0.0f)
		});
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	shader.use();
	glUniform1i(shader.getUniformLoc("environmentMap"), 0);
	envMap.bind(0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		unsigned int mipSize = 128 * std::pow(0.5f, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,mipSize,mipSize);
		glViewport(0, 0,mipSize, mipSize);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		glUniform1f(shader.getUniformLoc("roughness"), roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			captureCamera.setGaze(captureVeiw[2 * i]);
			captureCamera.setUp(captureVeiw[2 * i + 1]);

			captureCamera.setCamera(shader.getUniformLoc("view"),shader.getUniformLoc("projection"));

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			IBLSuberoutan::renderCube();

		}

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}


PrefilterMap::~PrefilterMap()
{
	glDeleteTextures(1, &prefilterMap);
}

void PrefilterMap::bind(unsigned int unit)const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
}

void PrefilterMap::unbind(unsigned int unit) const
{

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
