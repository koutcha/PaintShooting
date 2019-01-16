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
	//skybox用の箱の頂点を用意する

	//頂点データ
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
	//vao vboの設定
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	//vaoセット
	glBindVertexArray(vao);
	//vboに頂点データを渡す
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	//vaoの０番の有効化(position)
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
