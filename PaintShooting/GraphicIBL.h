#pragma once

#include<memory>
class Shader;
class Camera;
class Shape;
class Matrix;
class TexturePackPBR;
class MaterialPBR;

class IBLRenderPack;
class PointLights;

class Particle3D;

class AdditionalAttributeSetter;

class GraphicIBL
{
public:
	GraphicIBL(const std::shared_ptr<Camera>&, const std::shared_ptr<PointLights>&,const std::shared_ptr<IBLRenderPack>&);
	~GraphicIBL();

	//�J�������\�[�X�����L���܂�
	void setCamera(const std::shared_ptr<Camera>&);
	//���C�g���\�[�X�����L���܂�(�Ƃ肠�����_����)
	void setLight(const std::shared_ptr<PointLights>&);

	void setIBLRenderPack(const std::shared_ptr<IBLRenderPack>&);

	//hdr�␳�l�@�傫����Ώ��Ȃ����ʂɑΉ����āA��������Α傫�����ʂɑΉ�����
	void setHDRExposure(float exposure);

	//�`����A�ϊ����,�}�e���A��(diffuse,specular,ao,emission)�Ɋ�Â��ă����_�����O���s���܂�
	void draw(const Shape&, const Matrix&, const MaterialPBR&)const;
	//�`����A�ϊ����A�}�e���A���e�N�X�`��(diffuse,specular,normal,ao)�Ɋ�Â��ă����_�����O���s���܂�
	void draw(const Shape&, const Matrix&, const TexturePackPBR&)const;

	void draw(const Particle3D&, const MaterialPBR&)const;
	void draw(const Particle3D&,int drawParticleNum , const MaterialPBR&)const;

	void draw(const Particle3D&, const TexturePackPBR&)const;
	void draw(const Particle3D&, int drawParticleNum, const TexturePackPBR&)const;

	//�A�g���r���[�g��ǉ������V�F�[�_�[�p(0~2 texture unit is used)
	void drawWithAdditonalAttribute(const Shape&, const Matrix&,const MaterialPBR& ,const Shader& uniqueShader, const AdditionalAttributeSetter& addtionalAttribe)const;

	//�A�g���r���[�g��ǉ������V�F�[�_�[�p(0~7 texture unit is used)
	void drawWithAdditonalAttribute(const Shape&, const Matrix&, const TexturePackPBR&, const Shader& uniqueShader, const AdditionalAttributeSetter& addtionalAttribe)const;
private:
	static void commonShaderSetting(const Shader&, const PointLights&, const Camera&,float exposure);
	static void	 setIBLPackUniforms(const Shader& shader,const IBLRenderPack& iblPack);
	static void unsetIBLPackUniforms(const IBLRenderPack& iblPack);
	static void setTexturePackUniforms(const Shader& shader,const TexturePackPBR& texPack);
	static void unsetTexturePackUniforms(const TexturePackPBR& texPack);
	static void setMaterialPBRUniforms(const Shader& shader,const MaterialPBR& material);

	std::shared_ptr<Shader> mainShader;
	std::shared_ptr<Shader> particleShader;
	std::shared_ptr<PointLights> light;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<IBLRenderPack> iblPack;

	//hdr�␳�l
	float hdrExposure;
};

