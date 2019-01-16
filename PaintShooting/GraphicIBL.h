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

	//カメラリソースを共有します
	void setCamera(const std::shared_ptr<Camera>&);
	//ライトリソースを共有します(とりあえず点光源)
	void setLight(const std::shared_ptr<PointLights>&);

	void setIBLRenderPack(const std::shared_ptr<IBLRenderPack>&);

	//hdr補正値　大きければ少ない光量に対応して、小さければ大きい光量に対応する
	void setHDRExposure(float exposure);

	//形状情報、変換情報,マテリアル(diffuse,specular,ao,emission)に基づいてレンダリングを行います
	void draw(const Shape&, const Matrix&, const MaterialPBR&)const;
	//形状情報、変換情報、マテリアルテクスチャ(diffuse,specular,normal,ao)に基づいてレンダリングを行います
	void draw(const Shape&, const Matrix&, const TexturePackPBR&)const;

	void draw(const Particle3D&, const MaterialPBR&)const;
	void draw(const Particle3D&,int drawParticleNum , const MaterialPBR&)const;

	void draw(const Particle3D&, const TexturePackPBR&)const;
	void draw(const Particle3D&, int drawParticleNum, const TexturePackPBR&)const;

	//アトリビュートを追加したシェーダー用(0~2 texture unit is used)
	void drawWithAdditonalAttribute(const Shape&, const Matrix&,const MaterialPBR& ,const Shader& uniqueShader, const AdditionalAttributeSetter& addtionalAttribe)const;

	//アトリビュートを追加したシェーダー用(0~7 texture unit is used)
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

	//hdr補正値
	float hdrExposure;
};

