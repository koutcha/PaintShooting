#include "SkyboxRender.h"

#include "GL/glew.h"
#include "Camera.h"
#include "Shader.h"
#include "HDRCubeMap.h"

const std::string SkyboxRender::shaderName("Shader\\skybox");

SkyboxRender::SkyboxRender():
	shader(std::make_unique<Shader>(shaderName+".vert",shaderName+".frag")),
	hdrExposure(1.0)
{
	//skybox�p�̔��̒��_��p�ӂ���

	//���_�f�[�^
	constexpr float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	//vao vbo�̐ݒ�
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	//vao�Z�b�g
	glBindVertexArray(vao);
	//vbo�ɒ��_�f�[�^��n��
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	//vao�̂O�Ԃ̗L����(position)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

	glBindVertexArray(0);
	
}


SkyboxRender::~SkyboxRender()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void SkyboxRender::setHDRExposure(float exposure)
{
	hdrExposure = exposure;
}

void SkyboxRender::drawSkyBox(const Camera & camera, const HDRCubeMap & cubeTexture)
{
	shader->use();

	cubeTexture.bind(0);
	camera.setCamera(shader->getUniformLoc("view"), shader->getUniformLoc("projection"));
	glUniform1f(shader->getUniformLoc("exposure"), hdrExposure);
	glUniform1i(shader->getUniformLoc("environmentMap"), 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	cubeTexture.unbind(0);
	glBindVertexArray(0);
}

void SkyboxRender::drawSkyBox(const Camera & camera, unsigned int cubeMapTextureUnitID)
{
	shader->use();
	camera.setCamera(shader->getUniformLoc("view"), shader->getUniformLoc("projection"));
	glUniform1f(shader->getUniformLoc("exposure"), hdrExposure);
	glUniform1i(shader->getUniformLoc("environmentMap"), cubeMapTextureUnitID);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}
