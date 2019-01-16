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

	//関数内でテクスチャをバインドします
	void drawSkyBox(const Camera& camera, const HDRCubeMap& cubeTexture);
	//呼び出す前に必ずテクスチャーをバインドしてください（bindした番号を第2引数に渡す）
	void drawSkyBox(const Camera& camera, unsigned int cubeMapTextureUnitID);

private:

	float hdrExposure;

	static const std::string shaderName;
	std::shared_ptr<Shader> shader;

	unsigned int vbo;
	unsigned int vao;
};

