#define _USE_MATH_DEFINES
#include "PaintBufferTexture.h"
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include "OrthoCamera.h"
#include "PaintMarkTexture.h"
#include "StageObstacle.h"
#include "Matrix.h"

#include "Shape.h"

PaintBufferTexture::PaintBufferTexture(float scale):
	//中心(0,0,0),up = -zとしてキャプチャーする
	inputCam(std::make_unique<OrthoCamera>(Vector3f(0.0f,3.0f,0.0f),Vector3f(0.0f,0.0f,-1.0f),Vector3f(0,0,0),-scale,scale,scale,-scale,0.1f,20.0f)),
	inputShader(std::make_unique<Shader>("Shader\\paintInput.vert","Shader\\paintInput.frag")),
	noPaintShader(std::make_unique<Shader>("Shader\\nopaint.vert", "Shader\\nopaint.frag")),
	inputCount(0)
{
	//テクスチャバッファ作成
	glGenTextures(1, &texture);


	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_BUFFER_SIZE, TEXTURE_BUFFER_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, TEXTURE_BUFFER_SIZE, TEXTURE_BUFFER_SIZE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//塗りレンダリングのための矩形VBO
	float quadVertices[] = {
		// positions        // texture Coords
		 -1.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 1.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &positionScaleBuffer);
	glGenBuffers(1, &indexDataBuffer);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//変形データのバッファ
	glBindBuffer(GL_ARRAY_BUFFER, positionScaleBuffer);
	glBufferData(GL_ARRAY_BUFFER, INPUT_BUFFER_SIZE * sizeof(float) * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2); 
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);

	//インデクスデータ(塗り形状パターンのID、2つの色のID)
	glBindBuffer(GL_ARRAY_BUFFER,indexDataBuffer);
	glBufferData(GL_ARRAY_BUFFER, INPUT_BUFFER_SIZE *2*sizeof(int), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 1, GL_INT,sizeof(int)*2, (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 1, GL_INT, sizeof(int)*2 ,(void*)(sizeof(int)));
	

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	//position scale date
	glVertexAttribDivisor(2, 1);

	//textureIndex
	glVertexAttribDivisor(3,1);
	//teamIndex
	glVertexAttribDivisor(4,1);


	glBindVertexArray(0);

	// PBO作成とバインド
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo);

	glBufferData(GL_PIXEL_PACK_BUFFER_ARB ,TEXTURE_BUFFER_SIZE*TEXTURE_BUFFER_SIZE*4,0, GL_DYNAMIC_DRAW);    // バッファの作成と初期化

	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

	//テクスチャのリフレッシュ
	refresh();

}


PaintBufferTexture::~PaintBufferTexture()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &positionScaleBuffer); 
	glDeleteBuffers(1, &indexDataBuffer); 

	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteBuffers(1, &pbo);
	
}


void PaintBufferTexture::inputPos(float positionX, float positionZ, float scale,const  Vector3f& inputDirection, int uvID, int teamIndex)
{
	
	Vector3f direction = inputDirection;
	direction.normalize();
	direction.y = 0;
	
	
	Vector3f standard(0.0f, 0.0f, 1.0f);
	//入力方向とstandardの成す角を求める
	//上向きを(0.0,0.0,-1.0)としたいのでPIを加える
	float degree = acos(standard.dot(direction))+M_PI;
	//0<degree<pi なので入力方向のxの正負からpi以上かどうか判断する
	if (direction.x > 0)
	{
		degree = M_PI*2.0f - degree;
	}
	//入力情報をストックする
	transformData[inputCount].posX = positionX;
	transformData[inputCount].posZ = positionZ;
	transformData[inputCount].scale = scale;
	//シェーダー内で角度から変換行列をもとめているので角度だけ渡す
	transformData[inputCount].rotation = degree;

	indicesData[inputCount].textureID = uvID;
	indicesData[inputCount].teamIndex = teamIndex;


	inputCount++;
}

void PaintBufferTexture::refresh()
{
	//テクスチャのリフレッシュ　塗りつぶしてまっさらのテクスチャに戻す
	glViewport(0, 0, TEXTURE_BUFFER_SIZE, TEXTURE_BUFFER_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void PaintBufferTexture::renderNotPaintArea(float positionX, float positionZ, const Vector3f & scale, const Shape & shape)
{

	glViewport(0, 0, TEXTURE_BUFFER_SIZE, TEXTURE_BUFFER_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	Matrix m = Matrix::translate(positionX, 0.0f, positionZ)*Matrix::scale(scale);
	noPaintShader->use();
	inputCam->setCamera(noPaintShader->getUniformLoc("view"), noPaintShader->getUniformLoc("projection"));
	glUniformMatrix4fv(noPaintShader->getUniformLoc("model"),1,GL_FALSE,m.data());
	shape.draw();
	noPaintShader->unUse();
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}




void PaintBufferTexture::renderInput(const PaintMarkTexture& paintTexture)
{

	glViewport(0, 0, TEXTURE_BUFFER_SIZE, TEXTURE_BUFFER_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//入力のためにステージを真上から見下ろしたカメラをセット
	inputShader->use();
	inputCam->setCamera(inputShader->getUniformLoc("view"), inputShader->getUniformLoc("projection"));
	paintTexture.bind(0);
	this->bind(1);
	//塗りテクスチャと塗り模様テクスチャ、塗り模様テクスチャの分割数をバインド
	glUniform1i(inputShader->getUniformLoc("paintMark"), 0);
	glUniform1i(inputShader->getUniformLoc("bufferMask"), 1);
	glUniform1f(inputShader->getUniformLoc("uvDivision"),paintTexture.getDivisionNumber());

	//ストックした入力情報をGPUのバッファに渡す
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, positionScaleBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, inputCount * sizeof(float)*4, transformData.data());;


	glBindBuffer(GL_ARRAY_BUFFER, indexDataBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, inputCount * sizeof(int)*2, indicesData.data());

	//インスタンシングでレンダリング
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, inputCount);

	//unbind
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	this->unbind(1);
	inputShader->unUse();
	inputCount = 0;
}

int PaintBufferTexture::getBufferResolution() const
{
	return TEXTURE_BUFFER_SIZE;
}

std::array<int, 4> PaintBufferTexture::getPixcelData() const
{
	std::array<int, 4> rgba;
	rgba.fill(0);
	
	//pboにピクセルデータを書き込む
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo);
	glReadPixels(0, 0,TEXTURE_BUFFER_SIZE, TEXTURE_BUFFER_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//pboからピクセル情報を読み込む
	GLubyte *ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
	if (ptr)
	{
		for (int i = 0; i < TEXTURE_BUFFER_SIZE*TEXTURE_BUFFER_SIZE; ++i)
		{
			for (int j = 0; j < 4; j++)
			{
				if (ptr[4 * i + j] > 0)
				{
					rgba[j]++;
				}
			}
			
		}
		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
	}
	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return rgba;
}

void PaintBufferTexture::bind(unsigned int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D,texture);
}

void PaintBufferTexture::unbind(unsigned int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
