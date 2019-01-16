#pragma once
#include <memory>
#include <string>
class Shader;
class HDRCubeMap;
class Camera;
class SkyboxRender
{
public:
	SkyboxRender();
	~SkyboxRender();

	void setHDRExposure(float exposure);

	//�֐����Ńe�N�X�`�����o�C���h���܂�
	void drawSkyBox(const Camera& camera, const HDRCubeMap& cubeTexture);
	//�Ăяo���O�ɕK���e�N�X�`���[���o�C���h���Ă��������ibind�����ԍ����2�����ɓn���j
	void drawSkyBox(const Camera& camera, unsigned int cubeMapTextureUnitID);

private:

	float hdrExposure;

	static const std::string shaderName;
	std::shared_ptr<Shader> shader;

	unsigned int vbo;
	unsigned int vao;
};

