#define _USE_MATH_DEFINES
#include "CreatePrimitiveShapes.h"

#include "MathAndPhysic.h"
#include "Vector3.h"

void CreatePrimitiveShapes::createSphere(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& indexBuffer, int slices, int stacks)
{

	constexpr double pi = M_PI;
	vertexBuffer.resize((slices + 1)*(stacks + 1));
	std::vector<std::array<double, 11> > doubleContener((slices + 1)*(stacks + 1));
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
			const std::array<double, 11> d = { x,y,z,x,y,z,s,t,0,0,0 };


			//add vertex's attribute
			//solidSphereVertex[j*(slices + 1) + i] = v;
			doubleContener[j*(slices + 1) + i] = d;


		}
	}



	indexBuffer.resize(slices*stacks * 6);
	for (int j = 0; j < stacks; ++j) {
		const int k((slices + 1) * j);



		for (int i = 0; i < slices; ++i)
		{
			const GLuint k0(k + i);
			const GLuint k1(k0 + 1);
			const GLuint k2(k1 + slices);
			const GLuint k3(k2 + 1);
			std::array<GLuint, 4> indexes = { k0,k1,k2,k3 };
			indexBuffer.emplace_back(k0);
			indexBuffer.emplace_back(k2);
			indexBuffer.emplace_back(k3);
			Vector3d test1 = MyMath::calcTangent(doubleContener[k0], doubleContener[k2], doubleContener[k3]);

			// 右上の三角形
			indexBuffer.emplace_back(k0);
			indexBuffer.emplace_back(k3);
			indexBuffer.emplace_back(k1);
			Vector3d test2 = MyMath::calcTangent(doubleContener[k0], doubleContener[k3], doubleContener[k1]);

			Vector3d result(test1 + test2);
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
	for (unsigned int c0 = 0; c0 < vertexBuffer.size(); ++c0)
	{

		vertexBuffer[c0].position[0] = doubleContener[c0][0];
		vertexBuffer[c0].position[1] = doubleContener[c0][1];
		vertexBuffer[c0].position[2] = doubleContener[c0][2];
		vertexBuffer[c0].normal[0] = doubleContener[c0][3];
		vertexBuffer[c0].normal[1] = doubleContener[c0][4];
		vertexBuffer[c0].normal[2] = doubleContener[c0][5];
		vertexBuffer[c0].uv[0] = doubleContener[c0][6];
		vertexBuffer[c0].uv[1] = doubleContener[c0][7];
		vertexBuffer[c0].tangent[0] = doubleContener[c0][8];
		vertexBuffer[c0].tangent[1] = doubleContener[c0][9];
		vertexBuffer[c0].tangent[2] = doubleContener[c0][10];
	}

}

void CreatePrimitiveShapes::createBox(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& indexBuffer)
{

	vertexBuffer.resize(36);

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

	for (int i = 0; i < 24; ++i)
	{
		vertexBuffer[i] = solidCubeVertex[i];
	}


	indexBuffer.resize(36);

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
	for (int i = 0; i < 36; ++i)
	{
		indexBuffer[i] = solidCubeIndex[i];
	}

	for (int i = 0; i < 12; ++i)
	{
		int index0 = indexBuffer[i * 3];
		int index1 = indexBuffer[i * 3 + 1];
		int index2 = indexBuffer[i * 3 + 2];

		Vector3f n = MyMath::calcNormal(vertexBuffer[index0], vertexBuffer[index1], vertexBuffer[index2]);

		Vector3f tan = MyMath::calcTangent(vertexBuffer[index0], vertexBuffer[index1], vertexBuffer[index2]);

		vertexBuffer[index0].normal = n.getArray();
		vertexBuffer[index1].normal = n.getArray();
		vertexBuffer[index2].normal = n.getArray();

		vertexBuffer[index0].tangent = tan.getArray();
		vertexBuffer[index1].tangent = tan.getArray();
		vertexBuffer[index2].tangent = tan.getArray();
	}

}
