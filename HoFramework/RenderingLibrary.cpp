
#include "RenderingLibrary.h"

#include <string.h>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <directxtk/SimpleMath.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

vector<Mesh> HRenderingLibrary::LoadMeshFromFile(string InDir,string InFileName)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(InDir+ InFileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	Matrix mat;
	vector<Mesh> meshes;
	meshes.reserve(scene->mNumMeshes);
	ProcessAINode(meshes, scene->mRootNode, scene, mat, InDir);

	return meshes;
}

void HRenderingLibrary::MakeBox(Mesh* InMesh, float InSize)
{
	InMesh->vertices.reserve(24);

	for (int i = 0; i < 24; ++i)
	{
		InMesh->vertices.push_back(Vertex());
	}

	//���� �Ʒ����� �ð����

	//����

	Vector3 BiTangent = Vector3(0, 1, 0);

	InMesh->vertices[0].position = Vector3(-1.0f, 1.0f, -1.0f) * InSize;
	InMesh->vertices[1].position = Vector3(-1.0f, 1.0f, 1.0f) * InSize;
	InMesh->vertices[2].position = Vector3(1.0f, 1.0f, 1.0f) * InSize;
	InMesh->vertices[3].position = Vector3(1.0f, 1.0f, -1.0f) * InSize;
	InMesh->vertices[0].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[1].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[2].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[3].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[0].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[1].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[2].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[3].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[0].normal = Vector3(0.0f, 1.f, 0.0f);
	InMesh->vertices[1].normal = Vector3(0.0f, 1.f, 0.0f);
	InMesh->vertices[2].normal = Vector3(0.0f, 1.f, 0.0f);
	InMesh->vertices[3].normal = Vector3(0.0f, 1.f, 0.0f);

	{
		Vector3 NormalOrth = InMesh->vertices[0].normal - InMesh->vertices[0].normal.Dot(BiTangent) * InMesh->vertices[0].normal;
		NormalOrth.Normalize();
		Vector3 TangentModel = BiTangent.Cross(NormalOrth);
		InMesh->vertices[0].tangent = TangentModel;
		InMesh->vertices[1].tangent = TangentModel;
		InMesh->vertices[2].tangent = TangentModel;
		InMesh->vertices[3].tangent = TangentModel;
	}



	InMesh->indices.push_back(0);
	InMesh->indices.push_back(1);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(0);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(3);


	//�Ʒ���
	InMesh->vertices[4].position = Vector3(-1.0f, -1.0f, -1.0f) * InSize;
	InMesh->vertices[5].position = Vector3(1.0f, -1.0f, -1.0f) * InSize;
	InMesh->vertices[6].position = Vector3(1.0f, -1.0f, 1.0f) * InSize;
	InMesh->vertices[7].position = Vector3(-1.0f, -1.0f, 1.0f) * InSize;
	InMesh->vertices[4].color = Vector3(0, 0, 0);
	InMesh->vertices[5].color = Vector3(0, 0, 0);
	InMesh->vertices[6].color = Vector3(0, 0, 0);
	InMesh->vertices[7].color = Vector3(0, 0, 0);
	InMesh->vertices[4].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[5].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[6].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[7].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[4].normal = Vector3(0.0f, -1.0f, 0.0f);
	InMesh->vertices[5].normal = Vector3(0.0f, -1.0f, 0.0f);
	InMesh->vertices[6].normal = Vector3(0.0f, -1.0f, 0.0f);
	InMesh->vertices[7].normal = Vector3(0.0f, -1.0f, 0.0f);


	{
		Vector3 NormalOrth = InMesh->vertices[4].normal - InMesh->vertices[4].normal.Dot(BiTangent) * InMesh->vertices[4].normal;
		NormalOrth.Normalize();
		Vector3 TangentModel = BiTangent.Cross(NormalOrth);
		InMesh->vertices[4].tangent = TangentModel;
		InMesh->vertices[5].tangent = TangentModel;
		InMesh->vertices[6].tangent = TangentModel;
		InMesh->vertices[7].tangent = TangentModel;
	}

	InMesh->indices.push_back(4);
	InMesh->indices.push_back(5);
	InMesh->indices.push_back(6);
	InMesh->indices.push_back(4);
	InMesh->indices.push_back(6);
	InMesh->indices.push_back(7);


	// �ո�
	InMesh->vertices[8].position = Vector3(-1.0f, -1.0f, -1.0f) * InSize;
	InMesh->vertices[9].position = Vector3(-1.0f, 1.0f, -1.0f) * InSize;
	InMesh->vertices[10].position = Vector3(1.0f, 1.0f, -1.0f) * InSize;
	InMesh->vertices[11].position = Vector3(1.0f, -1.0f, -1.0f) * InSize;
	InMesh->vertices[8].color = Vector3(0, 0, 0);
	InMesh->vertices[9].color = Vector3(0, 0, 0);
	InMesh->vertices[10].color = Vector3(0, 0, 0);
	InMesh->vertices[11].color = Vector3(0, 0, 0);
	InMesh->vertices[8].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[9].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[10].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[11].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[8].normal = Vector3(0.0f, 0.0f, -1.0f);
	InMesh->vertices[9].normal = Vector3(0.0f, 0.0f, -1.0f);
	InMesh->vertices[10].normal = Vector3(0.0f, 0.0f, -1.0f);
	InMesh->vertices[11].normal = Vector3(0.0f, 0.0f, -1.0f);

	{
		Vector3 NormalOrth = InMesh->vertices[8].normal - InMesh->vertices[8].normal.Dot(BiTangent) * InMesh->vertices[8].normal;
		NormalOrth.Normalize();
		Vector3 TangentModel = BiTangent.Cross(NormalOrth);
		InMesh->vertices[8].tangent = TangentModel;
		InMesh->vertices[9].tangent = TangentModel;
		InMesh->vertices[10].tangent = TangentModel;
		InMesh->vertices[11].tangent = TangentModel;
	}

	InMesh->indices.push_back(8);
	InMesh->indices.push_back(9);
	InMesh->indices.push_back(10);
	InMesh->indices.push_back(8);
	InMesh->indices.push_back(10);
	InMesh->indices.push_back(11);

	// �޸�
	InMesh->vertices[12].position = Vector3(-1.0f, -1.0f, 1.0f) * InSize;
	InMesh->vertices[13].position = Vector3(1.0f, -1.0f, 1.0f) * InSize;
	InMesh->vertices[14].position = Vector3(1.0f, 1.0f, 1.0f) * InSize;
	InMesh->vertices[15].position = Vector3(-1.0f, 1.0f, 1.0f) * InSize;
	InMesh->vertices[12].color = Vector3(0, 0, 0);
	InMesh->vertices[13].color = Vector3(0, 0, 0);
	InMesh->vertices[14].color = Vector3(0, 0, 0);
	InMesh->vertices[15].color = Vector3(0, 0, 0);
	InMesh->vertices[12].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[13].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[14].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[15].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[12].normal = Vector3(0.0f, 0.0f, 1.0f);
	InMesh->vertices[13].normal = Vector3(0.0f, 0.0f, 1.0f);
	InMesh->vertices[14].normal = Vector3(0.0f, 0.0f, 1.0f);
	InMesh->vertices[15].normal = Vector3(0.0f, 0.0f, 1.0f);

	{
		Vector3 NormalOrth = InMesh->vertices[12].normal - InMesh->vertices[12].normal.Dot(BiTangent) * InMesh->vertices[12].normal;
		NormalOrth.Normalize();
		Vector3 TangentModel = BiTangent.Cross(NormalOrth);
		InMesh->vertices[12].tangent = TangentModel;
		InMesh->vertices[13].tangent = TangentModel;
		InMesh->vertices[14].tangent = TangentModel;
		InMesh->vertices[15].tangent = TangentModel;
	}

	InMesh->indices.push_back(12);
	InMesh->indices.push_back(13);
	InMesh->indices.push_back(14);
	InMesh->indices.push_back(12);
	InMesh->indices.push_back(14);
	InMesh->indices.push_back(15);


	////����
	InMesh->vertices[16].position = Vector3(-1.0f, -1.0f, 1.0f) * InSize;
	InMesh->vertices[17].position = Vector3(-1.0f, 1.0f, 1.0f) * InSize;
	InMesh->vertices[18].position = Vector3(-1.0f, 1.0f, -1.0f) * InSize;
	InMesh->vertices[19].position = Vector3(-1.0f, -1.0f, -1.0f) * InSize;
	InMesh->vertices[16].color = Vector3(0, 0, 0);
	InMesh->vertices[17].color = Vector3(0, 0, 0);
	InMesh->vertices[18].color = Vector3(0, 0, 0);
	InMesh->vertices[19].color = Vector3(0, 0, 0);
	InMesh->vertices[16].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[17].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[18].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[19].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[16].normal = Vector3(-1.0f, 0.0f, 0.0f);
	InMesh->vertices[17].normal = Vector3(-1.0f, 0.0f, 0.0f);
	InMesh->vertices[18].normal = Vector3(-1.0f, 0.0f, 0.0f);
	InMesh->vertices[19].normal = Vector3(-1.0f, 0.0f, 0.0f);

	{
		Vector3 NormalOrth = InMesh->vertices[16].normal - InMesh->vertices[16].normal.Dot(BiTangent) * InMesh->vertices[16].normal;
		NormalOrth.Normalize();
		Vector3 TangentModel = BiTangent.Cross(NormalOrth);
		InMesh->vertices[16].tangent = TangentModel;
		InMesh->vertices[17].tangent = TangentModel;
		InMesh->vertices[18].tangent = TangentModel;
		InMesh->vertices[19].tangent = TangentModel;
	}

	InMesh->indices.push_back(16);
	InMesh->indices.push_back(17);
	InMesh->indices.push_back(18);
	InMesh->indices.push_back(16);
	InMesh->indices.push_back(18);
	InMesh->indices.push_back(19);

	////������	
	InMesh->vertices[20].position = Vector3(1.0f, -1.0f, 1.0f) * InSize;
	InMesh->vertices[21].position = Vector3(1.0f, -1.0f, -1.0f) * InSize;
	InMesh->vertices[22].position = Vector3(1.0f, 1.0f, -1.0f) * InSize;
	InMesh->vertices[23].position = Vector3(1.0f, 1.0f, 1.0f) * InSize;
	InMesh->vertices[20].color = Vector3(0, 0, 0);
	InMesh->vertices[21].color = Vector3(0, 0, 0);
	InMesh->vertices[22].color = Vector3(0, 0, 0);
	InMesh->vertices[23].color = Vector3(0, 0, 0);
	InMesh->vertices[20].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[21].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[22].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[23].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[20].normal = Vector3(1.0f, 0.0f, 0.0f);
	InMesh->vertices[21].normal = Vector3(1.0f, 0.0f, 0.0f);
	InMesh->vertices[22].normal = Vector3(1.0f, 0.0f, 0.0f);
	InMesh->vertices[23].normal = Vector3(1.0f, 0.0f, 0.0f);

	{
		Vector3 NormalOrth = InMesh->vertices[20].normal - InMesh->vertices[20].normal.Dot(BiTangent) * InMesh->vertices[20].normal;
		NormalOrth.Normalize();
		Vector3 TangentModel = BiTangent.Cross(NormalOrth);
		InMesh->vertices[20].tangent = TangentModel;
		InMesh->vertices[21].tangent = TangentModel;
		InMesh->vertices[22].tangent = TangentModel;
		InMesh->vertices[23].tangent = TangentModel;
	}


	//�� �޽��� �߽���

	//Vector3 center = Vector3(0.f);

	//for (int i = 0 ; i < InMesh->vertices.size() ;++i)
	//{
	//	center += InMesh->vertices[i].position;
	//}

	//center /= InMesh->vertices.size();


	////�޽� to ���ؽ��� �븻
	//for (int i = 0; i < InMesh->vertices.size(); ++i)
	//{
	//	InMesh->vertices[i].normal = (InMesh->vertices[i].position - center);
	//	InMesh->vertices[i].normal.Normalize();
	//}

	InMesh->indices.push_back(20);
	InMesh->indices.push_back(21);
	InMesh->indices.push_back(22);
	InMesh->indices.push_back(20);
	InMesh->indices.push_back(22);
	InMesh->indices.push_back(23);
}

void HRenderingLibrary::MakeBoxNormal(Mesh* InBoxMesh, Mesh* OutNormalMesh)
{
	OutNormalMesh->vertices.reserve(InBoxMesh->vertices.size() * 2);
	OutNormalMesh->indices.reserve(InBoxMesh->indices.size() / 2);

	for (int i = 0; i < InBoxMesh->vertices.size(); ++i)
	{
		Vertex NewVertex = InBoxMesh->vertices[i];
		NewVertex.texCoord.x = 0;
		NewVertex.texCoord.y = 0;
		OutNormalMesh->vertices.push_back(NewVertex);
		NewVertex.texCoord.x = 1;
		NewVertex.texCoord.y = 1;
		OutNormalMesh->vertices.push_back(NewVertex);

		OutNormalMesh->indices.push_back(2 * i);
		OutNormalMesh->indices.push_back(2 * i + 1);
	}
}

void HRenderingLibrary::MakeGrid(Mesh* InBoxMesh, int InHorizontalGridCnt, int InVerticalGridCnt, int InGridSize)
{
	InBoxMesh->vertices.reserve(InVerticalGridCnt * InHorizontalGridCnt * 4);
	InBoxMesh->indices.reserve(InVerticalGridCnt * InHorizontalGridCnt * 6);
	//����� �ϴ� -1�� ����


	//Left Down ������� �׸���
	float VerticalAcc = 0.f;
	for (int y = 0; y <= InVerticalGridCnt; ++y)
	{
		for (int x = 0; x <= InHorizontalGridCnt; ++x)
		{
			Vertex NewVert1;
			NewVert1.position = Vector3(x * InGridSize, VerticalAcc, 0);
			NewVert1.normal = Vector3(0, 0, -1.f);
			NewVert1.texCoord = Vector2((float)x / InHorizontalGridCnt, (float)y / InVerticalGridCnt);
			InBoxMesh->vertices.push_back(NewVert1);
		}
		//���� y��ǥ�� �Ѿ
		VerticalAcc -= InGridSize;
	}

	for (int y = 0; y < InVerticalGridCnt; ++y)
	{
		const int startPoint = (y * (InHorizontalGridCnt + 1));
		for (int x = 0; x < InHorizontalGridCnt; ++x)
		{
			InBoxMesh->indices.push_back(startPoint + x);
			InBoxMesh->indices.push_back(startPoint + (x + 1) );
			InBoxMesh->indices.push_back(startPoint + x + (InHorizontalGridCnt + 1));

			InBoxMesh->indices.push_back(startPoint + x + (InHorizontalGridCnt + 1));
			InBoxMesh->indices.push_back(startPoint + (x + 1));
			InBoxMesh->indices.push_back(startPoint + (x + 1) + (InHorizontalGridCnt + 1) );
		}
	}

}

void HRenderingLibrary::MakeCylinder(Mesh* InBoxMesh, float InRadius, int InRadialSliceCount, float InCylinderHeight)
{

	InBoxMesh->vertices.reserve(size_t((InRadialSliceCount + 1) * 2));
	InBoxMesh->indices.reserve(size_t((InRadialSliceCount + 1) * 6));

	float AngleInRad = XM_2PI / InRadialSliceCount;
	float ExtentHeight = InCylinderHeight / 2.f;

	//�߽��� 0,0,0�̶�� ����
	//����
	for (int x = 0; x <= InRadialSliceCount; ++x)
	{
		Vertex NewVert;
		NewVert.position = Vector3(InRadius  * cos(AngleInRad * x), ExtentHeight, InRadius  * sin(AngleInRad * x));
		NewVert.texCoord = Vector2(float(x) / InRadialSliceCount, 0);
		NewVert.normal = (NewVert.position - Vector3(0.f));
		NewVert.normal.Normalize();

		InBoxMesh->vertices.push_back(NewVert);
	}

	//�Ʒ���
	for (int x = 0; x <= InRadialSliceCount; ++x)
	{
		Vertex NewVert;
		NewVert.position = Vector3(InRadius * cos(AngleInRad * x), -ExtentHeight, InRadius * sin(AngleInRad * x));
		NewVert.texCoord = Vector2(float(x) / InRadialSliceCount, 1.f);
		NewVert.normal = (NewVert.position - Vector3(0.f));
		NewVert.normal.Normalize();
		InBoxMesh->vertices.push_back(NewVert);
	}


	//�δ��̽�
	for (int i = 0; i < InRadialSliceCount; ++i)
	{
		InBoxMesh->indices.push_back(i);
		InBoxMesh->indices.push_back(i + 1);
		InBoxMesh->indices.push_back(i + (InRadialSliceCount + 1));

		InBoxMesh->indices.push_back(i + (InRadialSliceCount + 1));
		InBoxMesh->indices.push_back(i + 1);
		InBoxMesh->indices.push_back((i + 1) + (InRadialSliceCount + 1));
	}



}

void HRenderingLibrary::MakeSquare(Mesh* InBoxMesh)
{
	vector<Vector3> positions;
	vector<Vector3> colors;
	vector<Vector3> normals;
	vector<Vector2> texcoords; // �ؽ��� ��ǥ

	const float scale = 1.0f;

	// �ո�
	positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

	// Texture Coordinates (Direct3D 9)
	// https://learn.microsoft.com/en-us/windows/win32/direct3d9/texture-coordinates
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));


	for (size_t i = 0; i < positions.size(); i++) {
		Vertex v;
		v.position = positions[i];
		v.normal = normals[i];
		v.texCoord = texcoords[i];

		v.tangent = Vector3(1, 0, 0);

		// v.color = colors[i];

		InBoxMesh->vertices.push_back(v);
	}
	InBoxMesh->indices = {
		0, 1, 2, 0, 2, 3, // �ո�
	};
}

//VerticalDivision - ���η� �ɰ��� ����(slice) , HorizontalDivision - ���η� �ɰ��� ����(stack)
void HRenderingLibrary::MakeSphere(Mesh* InBoxMesh , float InRadius,Vector3 InCenterPos,int InVerticalDivision, int InHorizontalDivision)
{
	float ZAnglePerPiece = XM_PI / InHorizontalDivision;
	float YAnglePerPiece = XM_2PI / InVerticalDivision;

	Vector3 StartPt = Vector3::Transform(Vector3(0, InRadius, 0), Matrix::CreateRotationZ(ZAnglePerPiece));
	Vector3 Dx = Vector3::Transform(StartPt, Matrix::CreateRotationY(YAnglePerPiece)) - StartPt;
	Vector3 Dy = Vector3::Transform(Vector3(0, InRadius, 0), Matrix::CreateRotationZ(ZAnglePerPiece * 2.f)) - StartPt;
	Dx.Normalize();
	Dy.Normalize();

	//������ �Ʒ��� �����.
	for (int y = 0; y <= InHorizontalDivision; ++y)
	{
		Vector3 StartPoint = Vector3::Transform(Vector3(0, InRadius, 0),Matrix::CreateRotationZ(ZAnglePerPiece * y));
		for (int x = 0; x <= InVerticalDivision; ++x)
		{
			Vertex NewVert;
			NewVert.position = Vector3::Transform(StartPoint, Matrix::CreateRotationY(YAnglePerPiece * float(x)));
			NewVert.texCoord = Vector2(float(x) / InVerticalDivision, float(y) / InHorizontalDivision);
			NewVert.normal = NewVert.position - InCenterPos;
			NewVert.normal.Normalize();

			{
				//���⼭ ����ź��Ʈ�� �Ʒ��� �������ٰ� ��������. �ֳĸ� ���� ������ �Ʒ��� ä���ִ� ���̱� ����.
				Vector3 BiTangent = Vector3(0, -1, 0);

				//BiTangent�� Normal�� �����̿��� �Ѵ�. �׷��� ���� ź��Ʈ�� normal���� ���� projection �� ���� ���ش�. ; //���������� ����� BiTangent�� �´°� ����.
				BiTangent = BiTangent - BiTangent.Dot(NewVert.normal) * NewVert.normal;
				BiTangent.Normalize();
				//BiTangent�� ������ ������ ���, Normal�� ũ�ν� �ϸ� �޼���ǥ�� �ǰ� ȭ�������� ���ƿ��� CrossProduct Vector�� ���������. 3D������ �뺸�� �̰� ����� ���±�.
				NewVert.tangent = BiTangent.Cross(NewVert.normal);
				NewVert.tangent.Normalize();
			}


			InBoxMesh->vertices.push_back(NewVert);
		}
	}

	for (int j = 0; j < InHorizontalDivision; j++) {

		const int offset = (InVerticalDivision + 1) * j;

		for (int i = 0; i < InVerticalDivision; i++) {

			InBoxMesh->indices.push_back(offset + i);
			InBoxMesh->indices.push_back(offset + i + InVerticalDivision + 1);
			InBoxMesh->indices.push_back(offset + i + 1 + InVerticalDivision + 1);

			InBoxMesh->indices.push_back(offset + i);
			InBoxMesh->indices.push_back(offset + i + 1 + InVerticalDivision + 1);
			InBoxMesh->indices.push_back(offset + i + 1);
		}
	}

}

bool HRenderingLibrary::CreateVertexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer> &pVertexBuffer)
{
    D3D11_BUFFER_DESC VertexBufferDesc = {};
    VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    VertexBufferDesc.ByteWidth = sizeof(Vertex) * pDrawingMesh->vertices.size();
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA VertexBufferData = {};
    VertexBufferData.pSysMem = pDrawingMesh->vertices.data();

    pDeviceContext->CreateBuffer(&VertexBufferDesc, &VertexBufferData, pVertexBuffer.GetAddressOf());

    if (pVertexBuffer.Get() == nullptr)
    {
        cout << "No Vertex Buffer!" << endl;
        return false;
    }
    return true;
}

void HRenderingLibrary::MakeLcosahedron(Mesh* InMesh)
{
	// Luna DX12 ���� ����
   // ��20��ü
   // https://mathworld.wolfram.com/Isohedron.html

	const float X = 0.525731f;
	const float Z = 0.850651f;

	vector<Vector3> pos = {
		Vector3(-X, 0.0f, Z), Vector3(X, 0.0f, Z),   Vector3(-X, 0.0f, -Z),
		Vector3(X, 0.0f, -Z), Vector3(0.0f, Z, X),   Vector3(0.0f, Z, -X),
		Vector3(0.0f, -Z, X), Vector3(0.0f, -Z, -X), Vector3(Z, X, 0.0f),
		Vector3(-Z, X, 0.0f), Vector3(Z, -X, 0.0f),  Vector3(-Z, -X, 0.0f) };

	for (size_t i = 0; i < pos.size(); i++) {
		Vertex v;
		v.position = pos[i];
		v.normal = v.position;
		v.normal.Normalize();

		InMesh->vertices.push_back(v);
	}

	InMesh->indices = { 1,  4,  0, 4,  9, 0, 4, 5,  9, 8, 5, 4,  1,  8, 4,
					   1,  10, 8, 10, 3, 8, 8, 3,  5, 3, 2, 5,  3,  7, 2,
					   3,  10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6,  1, 0,
					   10, 1,  6, 11, 0, 9, 2, 11, 9, 5, 2, 9,  11, 2, 7 };
}

void HRenderingLibrary::MakeSphereSubdivision(Mesh* InMesh,float InSphereRadius)
{

	using namespace DirectX;
	using DirectX::SimpleMath::Matrix;
	using DirectX::SimpleMath::Vector3;

	// ������ �߽��̶�� ����
	// �Է� ���� �� ���� ������ ����
	for (auto& v : InMesh->vertices) {
		v.position = v.normal * InSphereRadius;
	}

	// ���ؽ��� �ߺ��Ǵ� ������ ����
	Mesh newMesh;
	uint32_t count = 0;
	for (size_t i = 0; i < InMesh->indices.size(); i += 3) {
		size_t i0 = InMesh->indices[i];
		size_t i1 = InMesh->indices[i + 1];
		size_t i2 = InMesh->indices[i + 2];

		Vertex v0 = InMesh->vertices[i0];
		Vertex v1 = InMesh->vertices[i1];
		Vertex v2 = InMesh->vertices[i2];

		Vertex v3;

		v3.position = (v0.position + v2.position) / 2.f;
		v3.texCoord = (v0.texCoord + v2.texCoord) / 2.f;

		// ��ġ�� �ؽ��� ��ǥ ����

		Vertex v4;

		v4.position = (v0.position + v1.position) / 2.f;
		v4.texCoord = (v0.texCoord + v1.texCoord) / 2.f;
		// ��ġ�� �ؽ��� ��ǥ ����

		Vertex v5;
		v5.position = (v1.position + v2.position) / 2.f;
		v5.texCoord = (v1.texCoord + v2.texCoord) / 2.f;
		// ��ġ�� �ؽ��� ��ǥ ����


		ProjectVertexToSphereSurface(v3, InSphereRadius);
		ProjectVertexToSphereSurface(v4, InSphereRadius);
		ProjectVertexToSphereSurface(v5, InSphereRadius);

		//VertexNormalToFaceNormal(v4, v1, v5);
		//VertexNormalToFaceNormal(v0, v4, v3);
		//VertexNormalToFaceNormal(v3, v4, v5);
		//VertexNormalToFaceNormal(v3, v5, v2);

		// ��� ���ؽ� ���� �߰�
		newMesh.vertices.push_back(v4);
		newMesh.vertices.push_back(v1);
		newMesh.vertices.push_back(v5);

		newMesh.vertices.push_back(v0);
		newMesh.vertices.push_back(v4);
		newMesh.vertices.push_back(v3);

		newMesh.vertices.push_back(v3);
		newMesh.vertices.push_back(v4);
		newMesh.vertices.push_back(v5);

		newMesh.vertices.push_back(v3);
		newMesh.vertices.push_back(v5);
		newMesh.vertices.push_back(v2);

		//�ε��� ������Ʈ
		for (uint32_t j = 0; j < 12; j++) {
			newMesh.indices.push_back(j + count);
		}
		count += 12;
	}
	*InMesh = newMesh;
}

void HRenderingLibrary::VertexNormalToFaceNormal(Vertex& v0,Vertex& v1,Vertex& v2)
{
	Vector3 NewNormal = (v1.position - v0.position).Cross(v2.position - v0.position);
	NewNormal.Normalize();
	v0.normal = NewNormal;
	v1.normal = NewNormal;
	v2.normal = NewNormal;
}

vector<D3D11_INPUT_ELEMENT_DESC> HRenderingLibrary::GetVSInputLayout()
{
	vector<D3D11_INPUT_ELEMENT_DESC> Result;

	Result.reserve(4);

	D3D11_INPUT_ELEMENT_DESC position;
	D3D11_INPUT_ELEMENT_DESC color;
	D3D11_INPUT_ELEMENT_DESC texCoord;
	D3D11_INPUT_ELEMENT_DESC normal;
	D3D11_INPUT_ELEMENT_DESC tangent;

	position.SemanticName = "POSITION";
	position.SemanticIndex = 0;
	position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	position.InputSlot = 0;
	position.AlignedByteOffset = 0;
	position.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	position.InstanceDataStepRate = 0;

	color.SemanticName = "COLOR";
	color.SemanticIndex = 0;
	color.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	color.InputSlot = 0;
	color.AlignedByteOffset = 4 * 3;
	color.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	color.InstanceDataStepRate = 0;

	texCoord.SemanticName = "TEXCOORD";
	texCoord.SemanticIndex = 0;
	texCoord.Format = DXGI_FORMAT_R32G32_FLOAT;
	texCoord.InputSlot = 0;
	texCoord.AlignedByteOffset = 4 * 3 + 4 * 3;
	texCoord.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	texCoord.InstanceDataStepRate = 0;

	normal.SemanticName = "NORMAL";
	normal.SemanticIndex = 0;
	normal.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	normal.InputSlot = 0;
	normal.AlignedByteOffset = 4 * 3 + 4 * 3 + 4 * 2;
	normal.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	normal.InstanceDataStepRate = 0;

	tangent.SemanticName = "TANGENT";
	tangent.SemanticIndex = 0;
	tangent.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tangent.InputSlot = 0;
	tangent.AlignedByteOffset = 4 * 3 + 4 * 3 + 4 * 2 + 4 * 3;
	tangent.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	tangent.InstanceDataStepRate = 0;

	Result.push_back(position);
	Result.push_back(color);
	Result.push_back(texCoord);
	Result.push_back(normal);
	Result.push_back(tangent);

	return Result;
}

bool HRenderingLibrary::CreateIndexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer> &pIndexBuffer)
{
	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint32_t) * pDrawingMesh->indices.size();
	IndexBufferDesc.StructureByteStride = sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA IndexBufferData = {};
	IndexBufferData.pSysMem = pDrawingMesh->indices.data();

	pDeviceContext->CreateBuffer(&IndexBufferDesc, &IndexBufferData, pIndexBuffer.GetAddressOf());

	if (pIndexBuffer.Get() == nullptr)
	{
		cout << "No Index Buffer!" << endl;
		return false;
	}

	return true;
}

bool HRenderingLibrary::CreateVertexShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11VertexShader>& pVertexShader, ComPtr<ID3D11InputLayout>& pVertexInputLayout , LPCWSTR pShaderFileLocation, LPCSTR pShaderMainName, vector<D3D11_INPUT_ELEMENT_DESC> pInput)
{

	ComPtr<ID3DBlob> VSBlob;
	ComPtr<ID3DBlob> VSErrorBlob;

	HRESULT VShr = D3DCompileFromFile(pShaderFileLocation, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, pShaderMainName, "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &VSBlob, &VSErrorBlob);

	if (FAILED(VShr))
	{
		cout << (char*)VSErrorBlob->GetBufferPointer() << endl;
		return false;
	}

	pDeviceContext->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, pVertexShader.GetAddressOf());
	pDeviceContext->CreateInputLayout(pInput.data(), UINT(pInput.size()), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), pVertexInputLayout.GetAddressOf());

	return true;
}


bool HRenderingLibrary::CreatePixelShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11PixelShader>& pPixelShader, LPCWSTR pShaderFileLocation, LPCSTR pShaderMainName)
{
	ComPtr<ID3DBlob> PSBlob;
	ComPtr<ID3DBlob> PSErrorBlob;

	HRESULT PShr = D3DCompileFromFile(pShaderFileLocation, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, pShaderMainName, "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &PSBlob, &PSErrorBlob);

	if (FAILED(PShr))
	{
		cout << (char*)PSErrorBlob->GetBufferPointer() << endl;
		return false;
	}

	pDeviceContext->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, pPixelShader.GetAddressOf());
	return true;
}

bool HRenderingLibrary::CreateTexture(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext, string pTextureFileLocation, ComPtr<ID3D11Texture2D>& OutTexture, ComPtr<ID3D11ShaderResourceView>& OutResourceView, DXGI_FORMAT Format)
{
	int width, height, channels;
	unsigned char* img = stbi_load(pTextureFileLocation.c_str(), &width, &height, &channels,0);

	if (img == nullptr)
	{
		cout << "Image Doesn't exist!" << endl;
		return false;
	}

	std::vector<uint8_t> colors;

	// 4ä�η� ���� ����
	colors.resize(width * height * 4);

	if (channels == 1) {
		for (size_t i = 0; i < width * height; i++) {
			uint8_t g = img[i * channels + 0];
			for (size_t c = 0; c < 4; c++) {
				colors[4 * i + c] = g;
			}
		}
	}
	else if (channels == 2) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 2; c++) {
				colors[4 * i + c] = img[i * channels + c];
			}
			colors[4 * i + 2] = 255;
			colors[4 * i + 3] = 255;
		}
	}
	else if (channels == 3) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 3; c++) {
				colors[4 * i + c] = img[i * channels + c];
			}
			colors[4 * i + 3] = 255;
		}
	}
	else if (channels == 4) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 4; c++) {
				colors[4 * i + c] = img[i * channels + c];
			}
		}
	}
	else {
		std::cout << "Cannot read " << channels << " channels" << endl;
	}



	D3D11_TEXTURE2D_DESC txtDesc = {};
	ZeroMemory(&txtDesc, sizeof(txtDesc));
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = 1;
	txtDesc.ArraySize = 1;
	txtDesc.Format = Format;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE ;
	//txtDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS; // �Ӹ� ���
	//txtDesc.CPUAccessFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = colors.data();
	InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
	
	pDevice->CreateTexture2D(&txtDesc, &InitData, OutTexture.GetAddressOf());
	pDevice->CreateShaderResourceView(OutTexture.Get(), 0,	OutResourceView.GetAddressOf());


	
	//pDeviceContext->GenerateMips(OutResourceView.Get());

	return (OutTexture.Get() && OutResourceView.Get());
}

Matrix HRenderingLibrary::GLMatrixToDXMatrix(aiMatrix4x4 InMatrix)
{
	Matrix Return;


	//DX ��� �����Ϳ� GLMatrix�� ���� ���� ����
	float* DXMat11 = &Return._11;
	ai_real* GLMat11 = &InMatrix.a1;

	//4x4
	for (int i = 0; i < 16; ++i)
	{
		DXMat11[i] = float(GLMat11[i]);
	}

	Return.Transpose();
	
	return Return;
}

void HRenderingLibrary::ProjectVertexToSphereSurface(Vertex& InVertex, const float InRadius)
{
	InVertex.normal = InVertex.position;
	InVertex.normal.Normalize();
	InVertex.position = InVertex.normal * InRadius;

	// ����: �ؽ��簡 �����ſ��� �����ϴ�.
	// atan vs atan2
	// https://stackoverflow.com/questions/283406/what-is-the-difference-between-atan-and-atan2-in-c
	const float theta = atan2f(InVertex.position.z, InVertex.position.x);
	const float phi = acosf(InVertex.position.y / InRadius);
	InVertex.texCoord.x = theta / XM_2PI;
	InVertex.texCoord.y = phi / XM_PI;
}

void HRenderingLibrary::ProcessAINode(vector<Mesh>& OutMesh, aiNode* InNode, const aiScene* InScene, Matrix InMatrix, string InDir)
{
	Matrix UsingMat;
	UsingMat = GLMatrixToDXMatrix(InNode->mTransformation) * InMatrix;

	for (int i = 0; i < InNode->mNumMeshes; ++i)
	{
		aiMesh* mesh = InScene->mMeshes[InNode->mMeshes[i]];

		Mesh NewMesh = ProcessAIMesh(mesh, InScene, InDir);

		for (int j = 0; j < NewMesh.vertices.size(); ++j)
		{
			NewMesh.vertices[j].position = Vector3::Transform(NewMesh.vertices[j].position, UsingMat);
		}


		OutMesh.push_back(NewMesh);
	}

	for (int i = 0; i < InNode->mNumChildren; ++i)
	{
		ProcessAINode(OutMesh, InNode->mChildren[i], InScene, InMatrix, InDir);
	}
}

Mesh HRenderingLibrary::ProcessAIMesh(aiMesh* InAIMesh, const aiScene* InScene, string InDir)
{
	Mesh NewMesh;
	//���ؽ�
	NewMesh.vertices.reserve(InAIMesh->mNumVertices);
	for (int j = 0; j < InAIMesh->mNumVertices; ++j)
	{
		Vertex NewVert;
		NewVert.position = Vector3(InAIMesh->mVertices[j].x, InAIMesh->mVertices[j].y, InAIMesh->mVertices[j].z);
		NewVert.normal = Vector3(InAIMesh->mNormals[j].x, InAIMesh->mNormals[j].y, InAIMesh->mNormals[j].z);
		NewVert.normal.Normalize();
		if (InAIMesh->mTextureCoords[0])
			NewVert.texCoord = Vector2(InAIMesh->mTextureCoords[0][j].x, InAIMesh->mTextureCoords[0][j].y);
		NewMesh.vertices.push_back(NewVert);
	}


	//�δ��̽�
	vector<uint32_t> indices;
	for (int j = 0; j < InAIMesh->mNumFaces; ++j)
	{
		aiFace face = InAIMesh->mFaces[j];
		indices.reserve(indices.size() + face.mNumIndices);
		for (int faceNum = 0; faceNum < face.mNumIndices; ++faceNum)
		{
			indices.push_back(face.mIndices[faceNum]);
		}
	}

	for (int i = 0; i < indices.size(); ++i)
	{
		NewMesh.indices.push_back(indices[i]);
	}

	//�ؽ��� ��Ī
	if (InAIMesh->mMaterialIndex >= 0) {
		aiMaterial* material = InScene->mMaterials[InAIMesh->mMaterialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString filepath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);
			
			NewMesh.textureNames[EModelTexture2DType::Albedo] = InDir + filesystem::path(filepath.C_Str()).filename().string();
		}

		if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString filepath;
			material->GetTexture(aiTextureType_NORMALS, 0, &filepath);

			NewMesh.textureNames[EModelTexture2DType::Normal] = InDir + filesystem::path(filepath.C_Str()).filename().string();
		}

		if (material->GetTextureCount(aiTextureType_HEIGHT) > 0) {
			aiString filepath;
			material->GetTexture(aiTextureType_HEIGHT, 0, &filepath);

			NewMesh.textureNames[EModelTexture2DType::Height] = InDir + filesystem::path(filepath.C_Str()).filename().string();
		}

		if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0) {
			aiString filepath;
			material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &filepath);

			NewMesh.textureNames[EModelTexture2DType::AmbientOcclusion] = InDir + filesystem::path(filepath.C_Str()).filename().string();
		}

		if (material->GetTextureCount(aiTextureType_METALNESS) > 0) {
			aiString filepath;
			material->GetTexture(aiTextureType_METALNESS, 0, &filepath);

			NewMesh.textureNames[EModelTexture2DType::Metallic] = InDir + filesystem::path(filepath.C_Str()).filename().string();
		}


		if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0) {
			aiString filepath;
			material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &filepath);

			NewMesh.textureNames[EModelTexture2DType::Roughness] = InDir + filesystem::path(filepath.C_Str()).filename().string();
		}

	}
	return NewMesh;
}
