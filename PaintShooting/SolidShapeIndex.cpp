#define _USE_MATH_DEFINES

#include "SolidShapeIndex.h"
#include "GL\glew.h"
#include "MathAndPhysic.h"
#include "Vector3.h"
#include <vector>
#include <array>
#include <cmath>  



SolidShapeIndex::SolidShapeIndex(GLint size, GLsizei vertexcount, const Vertex * vertex, GLsizei indexcount, const GLuint * index):
	ShapeIndex(size,vertexcount,vertex,indexcount,index)
{
}

SolidShapeIndex::~SolidShapeIndex()
{
}

void SolidShapeIndex::execute() const
{
	glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
}

std::shared_ptr<Shape> SolidShapeIndex::createSphereShape(int slices, int stacks,float radius)
{

	constexpr double pi = M_PI;
	std::vector<Vertex> solidSphereVertex((slices + 1)*(stacks + 1));
	std::vector<std::array<double,11> > doubleContener((slices + 1)*(stacks + 1));
	for (int j = 0; j <= stacks; ++j)
	{
		const double t(static_cast<double>(j) / static_cast<double>(stacks));
		const double y(cos(pi*t)), r(sin(pi*t));
		for (int i = 0; i <= slices; ++i)
	{
			const double s(static_cast<double>(i) / static_cast<double>(slices));
			const double z(r*cos(2.0*pi*s)), x(r*sin(2.0*pi*s));
			//vertex's position
		
			//const Object::Vertex v = { x,y,z,x,y,z,s,t,0,0,0};
			const std::array<double,11> d = { x,y,z,x,y,z,s,t,0,0,0};

			
			//add vertex's attribute
			//solidSphereVertex[j*(slices + 1) + i] = v;
			doubleContener[j*(slices + 1) + i] = d;

		
		}
	}



	std::vector<GLuint> solidSphereIndex(slices*stacks * 6);
	for (int j = 0; j < stacks; ++j) {
		const int k((slices + 1) * j);



		for (int i = 0; i < slices; ++i)
		{
			const GLuint k0(k + i);
			const GLuint k1(k0 + 1);
			const GLuint k2(k1 + slices);
			const GLuint k3(k2 + 1);
			std::array<GLuint, 4> indexes = { k0,k1,k2,k3 };
			solidSphereIndex.emplace_back(k0);
			solidSphereIndex.emplace_back(k2);
			solidSphereIndex.emplace_back(k3);
			Vector3d test1 = MyMath::calcTangent(doubleContener[k0], doubleContener[k2],doubleContener[k3]);

			// 右上の三角形
			solidSphereIndex.emplace_back(k0);
			solidSphereIndex.emplace_back(k3);
			solidSphereIndex.emplace_back(k1);
			Vector3d test2 = MyMath::calcTangent(doubleContener[k0],doubleContener[k3], doubleContener[k1]);
			
			Vector3d result(test1 + test2);
			//result /= 2.0f;
			result.normalize();
			for (auto index : indexes)
			{
				Vector3d tangent(doubleContener[index][8], doubleContener[index][9], doubleContener[index][10]);
		
				tangent = result;
				tangent.normalize();
				for (int i = 0; i < 3; ++i)
				{
					if (doubleContener[index][8 + i] == 0)
					{
						doubleContener[index][8 + i] = tangent.getByIndex(i);
					}
					else
					{
						doubleContener[index][8 + i] += tangent.getByIndex(i);
						doubleContener[index][8 + i] /= 2.0;
					}
				
				}

			}
		}
	}

	//ここ書き換えたい
	for (int c0 = 0; c0 < solidSphereVertex.size(); ++c0)
	{

		solidSphereVertex[c0].position[0]= doubleContener[c0][0];
		solidSphereVertex[c0].position[1] = doubleContener[c0][1];
		solidSphereVertex[c0].position[2] = doubleContener[c0][2];
		solidSphereVertex[c0].normal[0] = doubleContener[c0][3];
		solidSphereVertex[c0].normal[1] = doubleContener[c0][4];
		solidSphereVertex[c0].normal[2] = doubleContener[c0][5];
		solidSphereVertex[c0].uv[0] = doubleContener[c0][6];
		solidSphereVertex[c0].uv[1] = doubleContener[c0][7];
		solidSphereVertex[c0].tangent[0] = doubleContener[c0][8];
		solidSphereVertex[c0].tangent[1] = doubleContener[c0][9];
		solidSphereVertex[c0].tangent[2] = doubleContener[c0][10];		
	}

	return std::shared_ptr<Shape>(new SolidShapeIndex(3,
		static_cast<GLsizei>(solidSphereVertex.size()), solidSphereVertex.data(),
		static_cast<GLsizei>(solidSphereIndex.size()), solidSphereIndex.data()));

}
std::shared_ptr<Shape> SolidShapeIndex::createBoxShape()
{
	// 六面体の頂点の位置
	// 面ごとに色を変えた六面体の頂点属性
	
	Vertex solidCubeVertex[] =
	{
		//p n uv tan
		// 左
		{ -1.0f, -1.0f, -1.0f,  -1.0f, 0.0f, 0.0f , 0.0f,1.0f },
		{ -1.0f, -1.0f, 1.0f,   -1.0f, 0.0f, 0.0f , 0.0f,0.0f },
		{ -1.0f, 1.0f, 1.0f,    -1.0f,  0.0f, 0.0f , 1.0f,0.0f},
		{ -1.0f, 1.0f, -1.0f,   -1.0f, 0.0f, 0.0f ,  1.0f,1.0f},
	// 裏
		{ 1.0f, -1.0f, -1.0f,  0.0, 1.1f, 0.8f , 0.0f,1.0f},
		{ -1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f , 0.0f,0.0f },
		{ -1.0f, 1.0f, -1.0f,  0.8f, 0.1f, 0.8f ,1.0f,0.0f },
		{ 1.0f, 1.0f, -1.0f,   0.8f, 0.1f, 0.8f , 1.0f,1.0f },
	// 下
		{ -1.0f, -1.0f, -1.0f,  0.1f, 0.8f, 0.8f , 0.0f,1.0f },
		{ 1.0f, -1.0f, -1.0f,   0.1f, 0.8f, 0.8f ,0.0f,0.0f },
		{ 1.0f, -1.0f, 1.0f,   0.1f, 0.8f, 0.8f ,1.0f,0.0f },
		{ -1.0f, -1.0f, 1.0f,   0.1f, 0.8f, 0.8f ,  1.0f,1.0f },
	// 右
		{ 1.0f, -1.0f, 1.0f,   0.1f, 0.1f, 0.8f , 0.0f,1.0f },
		{ 1.0f, -1.0f, -1.0f,  0.1f, 0.1f, 0.8f , 0.0f,0.0f },
		{ 1.0f, 1.0f, -1.0f,   0.1f, 0.1f, 0.8f  ,1.0f,0.0f },
		{ 1.0f, 1.0f, 1.0f,   0.1f, 0.1f, 0.8f ,  1.0f,1.0f },
		// 上
		{ -1.0f, 1.0f, -1.0f,   0.0f, 1.0f, 0.0f , 0.0f,0.0f },
		{ -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f ,  0.0f,1.0f },
		{ 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f , 1.0f,1.0f },
		{ 1.0f, 1.0f, -1.0f,   0.0f, 1.0f, 0.0f ,  1.0f,0.0f },
		// 前
		{ -1.0f, -1.0f, 1.0f,  0.0f, 0.8f, 0.1f , 1.0f,0.0f },
		{ 1.0f, -1.0f, 1.0f,   0.0f, 0.8f, 0.1f , 0.0f,0.0f },
		{ 1.0f, 1.0f, 1.0f,    0.0f, 0.8f, 0.1f  ,0.0f,1.0f },
		{ -1.0f, 1.0f, 1.0f,   0.0f, 0.8f, 0.1f,  1.0f,1.0f }
	};
	// 六面体の面を塗りつぶす三角形の頂点のインデックス
	constexpr GLuint solidCubeIndex[] =
	{
		0, 1, 2, 0, 2, 3, // 左
		4, 5, 6, 4, 6, 7, // 裏
		8, 9, 10, 8, 10, 11, // 下
		12, 13, 14, 12, 14, 15, // 右
		16, 17, 18, 16, 18, 19, // 上
		20, 21, 22, 20, 22, 23 // 前
	};
	Vertex* scv = solidCubeVertex;
	const GLuint* sci = solidCubeIndex;
	for (int i = 0; i < 12; ++i)
	{
		int index0 = sci[i * 3];
		int index1 = sci[i * 3 + 1];
		int index2 = sci[i * 3 + 2];

		Vector3f n = MyMath::calcNormal(scv[index0], scv[index1], scv[index2]);

		Vector3f tan = MyMath::calcTangent(scv[index0], scv[index1], scv[index2]);
		
		scv[index0].normal =n.getArray();
		scv[index1].normal = n.getArray();
		scv[index2].normal = n.getArray();
		
		scv[index0].tangent = tan.getArray();
		scv[index1].tangent= tan.getArray();
		scv[index2].tangent = tan.getArray();
	}

	return 	std::shared_ptr<Shape>(new SolidShapeIndex(3,
		static_cast<GLsizei>(24), solidCubeVertex, static_cast<GLsizei>(36), solidCubeIndex));

}
std::shared_ptr<Shape> SolidShapeIndex::createBoxShape(float uvScaleX, float uvScaleY, float uvScaleZ)
{
	// 六面体の頂点の位置
	// 面ごとに色を変えた六面体の頂点属性

	Vertex solidCubeVertex[] =
	{
		//p n uv tan
		// 左
	{ -1.0f, -1.0f, -1.0f,  -1.0f, 0.0f, 0.0f , 0.0f,uvScaleZ},
	{ -1.0f, -1.0f, 1.0f,   -1.0f, 0.0f, 0.0f , 0.0f,0.0f },
	{ -1.0f, 1.0f, 1.0f,    -1.0f,  0.0f, 0.0f , uvScaleY,0.0f },
	{ -1.0f, 1.0f, -1.0f,   -1.0f, 0.0f, 0.0f ,  uvScaleY,uvScaleZ },
	// 裏
	{ 1.0f, -1.0f, -1.0f,  0.0, 1.1f, 0.8f , 0.0f,uvScaleX},
	{ -1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f , 0.0f,0.0f },
	{ -1.0f, 1.0f, -1.0f,  0.8f, 0.1f, 0.8f ,uvScaleY,0.0f },
	{ 1.0f, 1.0f, -1.0f,   0.8f, 0.1f, 0.8f ,uvScaleY,uvScaleZ },
	// 下
	{ -1.0f, -1.0f, -1.0f,  0.1f, 0.8f, 0.8f , 0.0f,uvScaleZ},
	{ 1.0f, -1.0f, -1.0f,   0.1f, 0.8f, 0.8f ,0.0f,0.0f },
	{ 1.0f, -1.0f, 1.0f,   0.1f, 0.8f, 0.8f ,uvScaleY,0.0f },
	{ -1.0f, -1.0f, 1.0f,   0.1f, 0.8f, 0.8f ,uvScaleY,uvScaleZ },
	// 右
	{ 1.0f, -1.0f, 1.0f,   0.1f, 0.1f, 0.8f , 0.0f,1.0f*uvScaleZ },
	{ 1.0f, -1.0f, -1.0f,  0.1f, 0.1f, 0.8f , 0.0f,0.0f },
	{ 1.0f, 1.0f, -1.0f,   0.1f, 0.1f, 0.8f  ,uvScaleY,0.0f },
	{ 1.0f, 1.0f, 1.0f,   0.1f, 0.1f, 0.8f ,  uvScaleY,1.0f*uvScaleZ },
	// 上
	{ -1.0f, 1.0f, -1.0f,   0.0f, 1.0f, 0.0f , 0.0f,0.0f },
	{ -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f ,  0.0f,uvScaleZ },
	{ 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f , uvScaleX,uvScaleZ },
	{ 1.0f, 1.0f, -1.0f,   0.0f, 1.0f, 0.0f ,  uvScaleX,0.0f },
	// 前
	{ -1.0f, -1.0f, 1.0f,  0.0f, 0.8f, 0.1f , uvScaleX,0.0f },
	{ 1.0f, -1.0f, 1.0f,   0.0f, 0.8f, 0.1f , 0.0f,0.0f },
	{ 1.0f, 1.0f, 1.0f,    0.0f, 0.8f, 0.1f  ,0.0f,uvScaleY },
	{ -1.0f, 1.0f, 1.0f,   0.0f, 0.8f, 0.1f,  uvScaleX,uvScaleY }
	};
	// 六面体の面を塗りつぶす三角形の頂点のインデックス
	constexpr GLuint solidCubeIndex[] =
	{
		0, 1, 2, 0, 2, 3, // 左
		4, 5, 6, 4, 6, 7, // 裏
		8, 9, 10, 8, 10, 11, // 下
		12, 13, 14, 12, 14, 15, // 右
		16, 17, 18, 16, 18, 19, // 上
		20, 21, 22, 20, 22, 23 // 前
	};
	Vertex* scv = solidCubeVertex;
	const GLuint* sci = solidCubeIndex;
	for (int i = 0; i < 12; ++i)
	{
		int index0 = sci[i * 3];
		int index1 = sci[i * 3 + 1];
		int index2 = sci[i * 3 + 2];

		Vector3f n = MyMath::calcNormal(scv[index0], scv[index1], scv[index2]);

		Vector3f tan = MyMath::calcTangent(scv[index0], scv[index1], scv[index2]);

		scv[index0].normal = n.getArray();
		scv[index1].normal = n.getArray();
		scv[index2].normal = n.getArray();

		scv[index0].tangent = tan.getArray();
		scv[index1].tangent = tan.getArray();
		scv[index2].tangent = tan.getArray();
	}

	return 	std::shared_ptr<Shape>(new SolidShapeIndex(3,
		static_cast<GLsizei>(24), solidCubeVertex, static_cast<GLsizei>(36), solidCubeIndex));
}
std::shared_ptr<Shape> SolidShapeIndex::createPlaneShape(float width, float depth)
{
	Vertex cubeVertex[] =
	{
		{ -width/2.0f, -0.0f, depth/2.0f,   0.0f, 1.0f, 0.0f ,   0.0f,3.0f,   0.0f,0.0f,1.0f}, // (2)
		{ -width/2.0f-0.0f, -depth/2.0f,   0.0f, 1.0f, 0.0f ,  0.0f,0.0f ,   0.0f,0.0f,1.0f }, // (3)
		{ width/2.0f, -0.0f, -depth/2.0f,   0.0f, 1.0f, 0.0f ,	3.0f,0.0f ,  0.0f,0.0f,1.0f }, // (4)
		{ width/2.0f, -0.0f, depth/2.0f,    0.0f, 1.0f, 0.0f ,  3.0f,3.0f,    0.0f,00.0f,1.0f } // (7)
	};
	constexpr GLuint solidCubeIndex[] =
	{
		3, 2, 1, 3, 1, 0, // 上
	};
	Vertex* scv = cubeVertex;
	const GLuint* sci = solidCubeIndex;
	for (int i = 0; i < 2; ++i)
	{
		int index0 = sci[i * 3];
		int index1 = sci[i * 3 + 1];
		int index2 = sci[i * 3 + 2];

		Vector3f n = MyMath::calcNormal(scv[index0], scv[index1], scv[index2]);
		Vector3f tan = MyMath::calcTangent(scv[index0], scv[index1], scv[index2]);
		scv[index0].normal = n.getArray();
		scv[index1].normal = n.getArray();
		scv[index2].normal = n.getArray();

		scv[index0].tangent = tan.getArray();
		scv[index1].tangent = tan.getArray();
		scv[index2].tangent = tan.getArray();
	}

	return 	std::shared_ptr<Shape>(new SolidShapeIndex(3,
		static_cast<GLsizei>(4), cubeVertex, static_cast<GLsizei>(6), solidCubeIndex));
}

std::shared_ptr<Shape> SolidShapeIndex::createVerticalPlane(float width, float height)
{
	Vertex vertices[] = 
	{
		{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f , 1.0f,0.0f },
		{ 1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f , 0.0f,0.0f },
		{ 1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f  ,0.0f,1.0f },
		{ -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,  1.0f,1.0f }
	};

	constexpr GLuint indices[] =
	{
		0, 1, 2, 0, 2, 3 
	};

	Vertex* scv = vertices;
	const GLuint* sci = indices;
	for (int i = 0; i < 2; ++i)
	{
		int index0 = sci[i * 3];
		int index1 = sci[i * 3 + 1];
		int index2 = sci[i * 3 + 2];

		Vector3f n = MyMath::calcNormal(scv[index0], scv[index1], scv[index2]);
		Vector3f tan = MyMath::calcTangent(scv[index0], scv[index1], scv[index2]);
		scv[index0].normal = n.getArray();
		scv[index1].normal = n.getArray();
		scv[index2].normal = n.getArray();

		scv[index0].tangent = tan.getArray();
		scv[index1].tangent = tan.getArray();
		scv[index2].tangent = tan.getArray();
	}

	return 	std::shared_ptr<Shape>(new SolidShapeIndex(3,
		static_cast<GLsizei>(4),vertices, static_cast<GLsizei>(6),indices));

}

std::shared_ptr<Shape> SolidShapeIndex::createTriangleShape()
{



	constexpr Vertex triangleVertex[] =
	{
		{ 0.0f,0.1f,2.25f,0.0f,1.0f,0.0f,0.5f,0.0f,0.0f,0.0f,1.0f },
		{ -1.5f,0.1f,-0.75f,0.0f,1.0f,0.0f,0.5f,0.0f ,0.0f,0.0f,1.0f },
		{ 1.5f,0.1f,-0.75f,0.0f,1.0f,0.0f,0.5f,0.0f ,0.0f,0.0f,1.0f },

	};
	constexpr GLuint triangleIndex[] =
	{
		0,2,1,
	};

	return 	std::shared_ptr<Shape>(new SolidShapeIndex(3,
		static_cast<GLsizei>(3), triangleVertex,
		static_cast<GLsizei>(6), triangleIndex));
}