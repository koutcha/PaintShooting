#pragma once
#include <array>
#include "Vector3.h"
class Shader;
class OrthoCamera;
class PaintMarkTexture;
class StageObstacle;
class Shape;
class PaintBufferTexture
{
public:
	PaintBufferTexture(float scale);
	~PaintBufferTexture();
	

	//scale = 1.0のとき各辺2.0の四角形サイズでテクスチャをレンダリング
	void inputPos(float positionX, float positonY, float scale,const Vector3f& inputDirection, int uvID, int teamIndex);

	void refresh();
	void renderNotPaintArea(float positionX,float positionZ,const Vector3f& scale,const Shape& shape);
	void renderInput(const PaintMarkTexture&);
	int  getBufferResolution()const;
	std::array<int, 4> getPixcelData()const;
	void bind(unsigned int unit)const;
	void unbind(unsigned int unit)const;
private:
	struct TransformData
	{
		float posX;
		float posZ;
		float scale;
		float rotation;
	};
	struct IndexData
	{
		int textureID;
		int teamIndex;
	};
	static constexpr int INPUT_BUFFER_SIZE = 100;
	static constexpr int TEXTURE_BUFFER_SIZE = 1024;

	unsigned int texture;
	unsigned int fbo;
	unsigned int rbo;

	unsigned int pbo;
	
	int inputCount;
	
	std::array<Vector3f,INPUT_BUFFER_SIZE> positionScaleData;
	std::array<TransformData,INPUT_BUFFER_SIZE> transformData;
	std::array<IndexData, INPUT_BUFFER_SIZE> indicesData;


	std::unique_ptr<OrthoCamera> inputCam;
	std::unique_ptr<Shader> inputShader;
	std::unique_ptr<Shader> noPaintShader;

	unsigned int vao;
	unsigned int vbo;
	unsigned int positionScaleBuffer;
	unsigned int indexDataBuffer;

};

