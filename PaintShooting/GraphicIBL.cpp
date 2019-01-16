#include "GraphicIBL.h"


#include "Shape.h"
#include "Shader.h"
#include "Camera.h"
#include "PointLights.h"
#include "Matrix.h"

#include "MaterialPBR.h"
#include "TexturePackPBR.h"
#include "IBLRenderPack.h"
#include "Particle3D.h"

#include "AdditionalAttributeSetter.h"


GraphicIBL::GraphicIBL( const std::shared_ptr<Camera>&camera, const std::shared_ptr<PointLights>&light, const std::shared_ptr<IBLRenderPack>& iblPack):
	mainShader(std::make_shared<Shader>("Shader\\common.vert","Shader\\IBLPBR.frag")),
	particleShader(std::make_shared<Shader>("Shader\\particle3D.vert", "Shader\\IBLPBR.frag")),
	camera(camera),
	light(light),
	iblPack(iblPack),
	hdrExposure(1.0f)
{

}

GraphicIBL::~GraphicIBL()
{
}

void GraphicIBL::setCamera(const std::shared_ptr<Camera>& camera)
{
	this->camera = camera;
}

void GraphicIBL::setLight(const std::shared_ptr<PointLights>& light)
{
	this->light = light;
}

void GraphicIBL::setIBLRenderPack(const std::shared_ptr<IBLRenderPack>& iblPack)
{
	this->iblPack = iblPack;
}

void GraphicIBL::setHDRExposure(float exposure)
{
	hdrExposure = exposure;
}


void GraphicIBL::draw(const Shape &shape, const Matrix &matrix, const MaterialPBR &material) const
{
	commonShaderSetting(*mainShader, *light, *camera,hdrExposure);

	glUniformMatrix4fv(mainShader->getUniformLoc("model"), 1, GL_FALSE, matrix.data());
	
	setIBLPackUniforms(*mainShader, *iblPack);
	setMaterialPBRUniforms(*mainShader, material);

	shape.draw();

	unsetIBLPackUniforms(*iblPack);
}

void GraphicIBL::draw(const Shape &shape, const Matrix &matrix, const TexturePackPBR &texturePack) const
{
	commonShaderSetting(*mainShader, *light, *camera,hdrExposure);

	glUniformMatrix4fv(mainShader->getUniformLoc("model"), 1, GL_FALSE, matrix.data());

	setIBLPackUniforms(*mainShader, *iblPack);
	setTexturePackUniforms(*mainShader, texturePack);

	shape.draw();

	unsetIBLPackUniforms(*iblPack);
	unsetTexturePackUniforms(texturePack);

	
}

void GraphicIBL::draw(const Particle3D &particle, const MaterialPBR &material) const
{


	commonShaderSetting(*particleShader, *light, *camera,hdrExposure);


	setIBLPackUniforms(*particleShader, *iblPack);
	setMaterialPBRUniforms(*particleShader, material);

	particle.draw();

	unsetIBLPackUniforms(*iblPack);

}

void GraphicIBL::draw(const Particle3D &particle, int drawParticleNum, const MaterialPBR &material) const
{

	commonShaderSetting(*particleShader, *light, *camera, hdrExposure);


	setIBLPackUniforms(*particleShader, *iblPack);
	setMaterialPBRUniforms(*particleShader, material);

	particle.draw(drawParticleNum);

	unsetIBLPackUniforms(*iblPack);
}

void GraphicIBL::draw(const Particle3D &particle, const TexturePackPBR &texturePack) const
{
	commonShaderSetting(*particleShader, *light, *camera,hdrExposure);

	setIBLPackUniforms(*particleShader, *iblPack);
	setTexturePackUniforms(*particleShader, texturePack);

	particle.draw();

	unsetIBLPackUniforms(*iblPack);
	unsetTexturePackUniforms(texturePack);

}

void GraphicIBL::draw(const Particle3D &particle, int drawParticleNum, const TexturePackPBR &texturePack) const
{
	commonShaderSetting(*particleShader, *light, *camera, hdrExposure);

	setIBLPackUniforms(*particleShader, *iblPack);
	setTexturePackUniforms(*particleShader, texturePack);

	particle.draw();

	unsetIBLPackUniforms(*iblPack);
	unsetTexturePackUniforms(texturePack);

}

void GraphicIBL::drawWithAdditonalAttribute(const Shape &shape, const Matrix &matrix, const MaterialPBR &material, const Shader & uniqueShader, const AdditionalAttributeSetter & addtionalAttribe)const
{
	commonShaderSetting(uniqueShader, *light, *camera, hdrExposure);

	glUniformMatrix4fv(uniqueShader.getUniformLoc("model"), 1, GL_FALSE, matrix.data());

	setIBLPackUniforms(uniqueShader, *iblPack);
	setMaterialPBRUniforms(uniqueShader, material);
	addtionalAttribe.setAttributes(uniqueShader);


	shape.draw();

	unsetIBLPackUniforms(*iblPack);
	addtionalAttribe.unbindAttributes();
}



void GraphicIBL::drawWithAdditonalAttribute(const Shape &shape, const Matrix &matrix, const TexturePackPBR &texturePack, const Shader & uniqueShader, const AdditionalAttributeSetter & addtionalAttribe)const
{

	commonShaderSetting(uniqueShader, *light, *camera, hdrExposure);

	glUniformMatrix4fv(uniqueShader.getUniformLoc("model"), 1, GL_FALSE, matrix.data());

	setIBLPackUniforms(uniqueShader, *iblPack);
	setTexturePackUniforms(uniqueShader, texturePack);
	addtionalAttribe.setAttributes(uniqueShader);

	shape.draw();

	unsetIBLPackUniforms(*iblPack);
	unsetTexturePackUniforms(texturePack);
	addtionalAttribe.unbindAttributes();
}

void GraphicIBL::commonShaderSetting(const Shader &shader, const PointLights &pointLights, const Camera &camera,float exposure)
{
	shader.use();
	//カメラ(プロジェクション、ビュー変換)
	GLuint viewLoc = shader.getUniformLoc("view");
	GLuint projectionLoc = shader.getUniformLoc("projection");
	camera.setCamera(viewLoc, projectionLoc);

	camera.setViewPos(shader.getUniformLoc("viewPos"));
	//点光源
	glUniform3fv(shader.getUniformLoc("PLpos"), PointLights::MAX_LIGHT_NUMBER, pointLights.getPositionData().data());
	glUniform3fv(shader.getUniformLoc("PLcolor"), PointLights::MAX_LIGHT_NUMBER, pointLights.getColorData().data());
	glUniform1fv(shader.getUniformLoc("PLintensity"), PointLights::MAX_LIGHT_NUMBER, pointLights.getIntensityData().data());

	glUniform1f(shader.getUniformLoc("exposure"),exposure);
}

void GraphicIBL::setIBLPackUniforms(const Shader & shader, const IBLRenderPack & iblPack)
{

	iblPack.bindIrradiace(0);
	glUniform1i(shader.getUniformLoc("irradianceMap"), 0);
	iblPack.bindPrefilter(1);
	glUniform1i(shader.getUniformLoc("prefilterMap"), 1);
	iblPack.bindBRDFLut(2);
	glUniform1i(shader.getUniformLoc("brdfLUT"), 2);
}

void GraphicIBL::unsetIBLPackUniforms(const IBLRenderPack & iblPack)
{
	iblPack.unbindIrradiance(0);
	iblPack.unbindPrefilter(0);
	iblPack.unbindBRDFLut(0);
}

void GraphicIBL::setTexturePackUniforms(const Shader & shader, const TexturePackPBR & texturePack)
{
	glUniform1i(shader.getUniformLoc("useMaterialTexture"), true);
	texturePack.bindAlbedo(3);
	glUniform1i(shader.getUniformLoc("albedoMap"), 3);
	texturePack.bindNormal(4);
	glUniform1i(shader.getUniformLoc("normalMap"), 4);
	texturePack.bindMetallic(5);
	glUniform1i(shader.getUniformLoc("metallicMap"), 5);
	texturePack.bindRoughness(6);
	glUniform1i(shader.getUniformLoc("roughnessMap"), 6);
	texturePack.bindAo(7);
	glUniform1i(shader.getUniformLoc("aoMap"), 7);

}

void GraphicIBL::unsetTexturePackUniforms( const TexturePackPBR & texPack)
{
	texPack.unbindAlbedo(3);
	texPack.unibindNormal(4);
	texPack.unbindMetallic(5);
	texPack.unbindRoughness(6);
	texPack.unbindAo(7);
}

void GraphicIBL::setMaterialPBRUniforms(const Shader & shader, const MaterialPBR & material)
{
	glUniform1i(shader.getUniformLoc("useMaterialTexture"), false);
	glUniform3fv(shader.getUniformLoc("albedoMaterial"), 1, material.getAlbedo().data());
	glUniform1f(shader.getUniformLoc("roughnessMaterial"), material.getRoughness());
	glUniform1f(shader.getUniformLoc("metallicMaterial"), material.getMetallic());
	glUniform1f(shader.getUniformLoc("aoMaterial"), material.getAo());

}
