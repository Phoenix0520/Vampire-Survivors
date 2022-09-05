#include  "framework.h"
#include  "Collider.h"
#include  "clipper.hpp"
#include  "ColorShader.h"

#define MAX(x,y) ( ( ((x) >= (y)) ? (x):(y)) )
#define MIN(x,y) ( ( ((x) <  (y)) ? (x):(y)) )
#define ABS(x) ( ( ((x) > (0)) ? (x):(-(x))) )
#define PI 3.141592f

using namespace clipper;

enum poly_color_type { pctSubject, pctClip, pctSolution };
ClipType     CT = ctIntersection;      //  { ctIntersection, ctUnion, ctDifference, ctXor };
PolyFillType PFT = pftNonZero;


/////////////////////////////////////////////////////////////////////
//  ������
/////////////////////////////////////////////////////////////////////
Collider::Collider()
{
	wstring strShader = L"./Shader/HLSL/Color.hlsl";

	m_pShader = new ColorShader(strShader);
	CreateBuffer();
	D3DXMatrixIdentity(&m_World);
}

Collider::~Collider()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(m_pVertexBuffer);

	if (m_cvPolygons.size())
	{
		for (UINT i = 0; i < m_cvPolygons.size(); i++)
			delete m_cvPolygons[i];
		m_cvPolygons.erase(m_cvPolygons.begin(), m_cvPolygons.begin() + m_cvPolygons.size());

	}
}
void Collider::Update(Matrix V, Matrix P)
{
	Matrix W, S, T, R;

	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&R);

	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	D3DXMatrixScaling(&S, m_Scale.x, m_Scale.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotation.y*PI / 180.0f, m_Rotation.x*PI / 180.0f, m_Rotation.z*PI / 180.0f);
	W = S * R *  T;
	m_World = S * R * T;
	m_pShader->Update(W, V, P);

}

void Collider::Render()
{
	UINT  stride = sizeof(Vertex);
	UINT  offset = 0;

	if (!visibleCollider)
		return;

	if (m_bCollisionCheck)
		UpdateBlock(Color(1, 0, 0, 1));
	else
		UpdateBlock(Color(0, 1, 0, 1));

	// IA�ܰ�  Input Assembly
	DeviceContext->IASetVertexBuffers(0, 1, &this->m_pVertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(m_pShader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VS-OM
	m_pShader->Draw(5, 0);

}
//////////////////////////////////////////////////////////////////////////
// CCW�� �̿��� Line Itersection Check
// https://gaussian37.github.io/math-algorithm-line_intersection/
//////////////////////////////////////////////////////////////////////////
bool Collider::InterSectionLine(Vector2 A, Vector2 B, Vector2 C, Vector2 D)
{
	int ret;
	// l1(A-B)�� �������� l2(C-D)�� �����ϴ� �� Ȯ���Ѵ�.
	// Check if l2 intersects based on l1.
	int l1_l2 = CCW(A, B, C) * CCW(A, B, D);
	// l2�� �������� l1�� �����ϴ� �� Ȯ���Ѵ�.
	// Check if l1 intersects based on l2.
	int l2_l1 = CCW(C, D, A) * CCW(C, D, B);


	// l1 �� l2�� ������ �� �ִ� ���
	// l1 and l2 are in same line.
	if (l1_l2 == 0 && l2_l1 == 0)
	{
		// Line1�� ���� ũ�� ������ p1 < p2 ������ �����.
		// Set the order of the points on Line1 in the order p1 < p2.
		if (IsLeft(B, A)) swap(A, B);
		// Line2�� ���� ũ�� ������ p1 < p2 ������ �����.
		// Set the order of the points on Line2 in the order p1 < p2.
		if (IsLeft(D, C)) swap(C, D);

		// A -----------B
		//         C -----------D
		// �� ������ �����ϴ� �� ���캻��.
		// See if the above conditions are met.
		ret = (IsLeft(C, B)) && (IsLeft(A, D));
	}
	// l1�� l2�� ������ �� ���� �ʴ� ���
	// l1 and l2 are not in same line.
	else {
		ret = (l1_l2 <= 0) && (l2_l1 <= 0);
	}
	return ret;
}
//////////////////////////////////////////////////////////////
// 1���� ���ο� �ִ°� Check
/////////////////////////////////////////////////////////////
bool Collider::PointInRect(Matrix world, Vector2 pos)
{
	// world1 ����
	float halfWidth = fabsf(world._11 * 0.5f);
	float halfHeight = fabsf(world._22 * 0.5f);

	float minX = world._41 - halfWidth;
	float maxX = world._41 + halfWidth;
	float maxY = world._42 + halfHeight;
	float minY = world._42 - halfHeight;

	if (!(maxX >= pos.x && minX <= pos.x && maxY >= pos.y && minY <= pos.y))
		return false;

	return true;
}
//////////////////////////////////////////////////////////////
// 1���� ���ο� �ִ°� Check
/////////////////////////////////////////////////////////////
bool Collider::Aabb(Matrix world, Vector2 pos)
{
	// world1 ����
	float halfWidth  = fabsf(world._11 * 0.5f);
	float halfHeight = fabsf(world._22 * 0.5f);

	float minX   = world._41 - halfWidth;
	float maxX   = world._41 + halfWidth;
	float maxY   = world._42 + halfHeight;
	float minY   = world._42 - halfHeight;

	if (!(maxX >= pos.x && minX <= pos.x && maxY >= pos.y && minY <= pos.y))
		return false;

	return true;
}
/////////////////////////////////////////////////////////////
// Polygon Operation 
////////////////////////////////////////////////////////////
bool Collider::InterSect(Collider * A, Collider * B, float advX, float advY)
{
	bool  return_value = false;

	vector<Vector2*> PolygonA = A->GetAreas();
	vector<Vector2*> PolygonB = B->GetAreas();

	// clipper �����
	// ������ ����
	Clipper  Engine;
	Polygons  clip, sub, solution;

	clip.resize(1);  // sA + sB -->2 �ټ��� size
	sub.resize(1);   // 

	clip[0].resize(PolygonA.size());  // ���� ������ Size
	sub[0].resize(PolygonB.size());   // ���� ������ Size

	// Clipper������ Ÿ�� integer
	for (UINT i = 0; i < PolygonA.size(); i++)
	{
		clip[0][i].X = (int)(PolygonA[i]->x * (1000.0f + advX));
		clip[0][i].Y = (int)(PolygonA[i]->y * (1000.0f + advY));

	}

	for (UINT i = 0; i < PolygonB.size(); i++)
	{
		sub[0][i].X = (int)(PolygonB[i]->x * (1000.0f + advX));
		sub[0][i].Y = (int)(PolygonB[i]->y * (1000.0f + advY));

	}

	Engine.AddPolygons(sub, ptSubject);        // Polygon Subject
	Engine.AddPolygons(clip, ptClip);          // Polygon Clip
	Engine.Execute(CT, solution, PFT, PFT);    // AND,OR


	int nCount = 0;

	if (solution.size() == 0)
		return false;

	return true;

	// ���� ������ ������ ���Ϸ���

	POINT  pt[1000];
	int    no = 0;

	for (UINT i = 0; i < solution.size(); ++i)
	{
		nCount = nCount + (int)(solution[i].size());
		no = 0;
		for (UINT j = 0; j < solution[i].size(); j++)
		{
			int x = (int)(solution[i][j].X);
			int y = (int)(solution[i][j].Y);

			pt[no].x = x;
			pt[no].y = y;
			no++;
		}
		if (no)
		{
			//Win2DGraphic  graphic;
			//for (int j = 0; j < (no - 1); j++)
			//{
			//	graphic.DrawLine(pt[j].x, pt[j].y, pt[j + 1].x, pt[j + 1].y, 5, RGB(255, 0, 0));
			//
			//}
		    // Draw Line
		}
	}
	return true;
}
/*
- input : p1 = (p1.x, p1.y), p2 = (p2.x, p2.y), p3 = (p3.x, p3.y)
- output : 1 (counter clockwise), 0 (collinear), -1 (clockwise)
�� vector v1 = (p2 - p1), vector v2 = (p3 - p1)
*/
int Collider::CCW(Vector2 p1, Vector2 p2, Vector2 p3)
{
	float cross_product = (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);

	if (cross_product > 0) 
		return 1;

	else if (cross_product < 0) 
		return -1;
	else 
		return 0;
}
// right�� left ���� ũ�� true�� ��ȯ�մϴ�.
// if right is greater than left, it is true
int Collider::IsLeft(Vector2 left, Vector2 right)
{
	int ret;
	if (left.x == right.x) {
		ret = (left.y <= right.y);
	}
	else {
		ret = (left.x <= right.x);
	}
	return ret;
}



vector<Vector2*> Collider::GetAreas()
{
	// ���������� ����
	if (m_cvPolygons.size())
	{
		for (UINT i = 0; i < m_cvPolygons.size(); i++)
			delete m_cvPolygons[i];
		m_cvPolygons.erase(m_cvPolygons.begin(), m_cvPolygons.begin() + m_cvPolygons.size());

	}

	Matrix  matrix = GetWorld();  // sacle, trans, rotate , position
	Vector2 posA[5];
	
	posA[0] = Vector2(-0.5f, -0.5f);
	posA[1] = Vector2(+0.5f, -0.5f);
	posA[2] = Vector2(+0.5f, +0.5f);
	posA[3] = Vector2(-0.5f, +0.5f);
	posA[4] = Vector2(-0.5f, -0.5f);

	for (int i = 0; i < 5; i++)
	{
		D3DXVec2TransformCoord(&posA[i], &posA[i], &matrix);
		m_cvPolygons.push_back(new Vector2(posA[i]));
	}
	return m_cvPolygons;
}
/////////////////////////////////////////////////////
//
//  ������ �ð�������� ���� --> OpenGL : ���ݴ� ����
// ������ ���������� IA�ܰ� : IA->VS->RS->PS->OM
//
//  OpenGL/DirectX  --> ������ǥ��  -0.5 ~ 0.5 �� : Normal��
//
//    ������ ����������  ( IA �ܰ�)  IA>VS->RS->PS->OM
//    * �⺻�����Ͱ� ���� �����ܰ�
//    
//     1    |   (3)
//          |
//    -------------
//     0    |    2         0 : -0.5f, -0.5f, 0.0f
//          |              1 : -0.5f, +0.5f, 0.0f
//                         2 : +0.5f, -0.5f, 0.0f
//
//  Rectangle ---> Triangle + Triangle
//                 0 1 2      3 4 5
/////////////////////////////////////////////////////////
void Collider::CreateBuffer()
{
	// 1. ������ �����  --> �ð��������
	Vertex vertices[5];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 2
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0

	vertices[0].Color = Color(1.0f,1.0f,1.0f,0.7f);
	vertices[1].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);
	vertices[2].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);
	vertices[3].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);
	vertices[4].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);


	// 2. Device(CPU) �����͸� ����
    // 2. CPU�ڿ��� GPU(IA�ܰ�)�� �Ѱ� �ֱ����� ��ȯ �۾��� �Ѵ�
	{

		D3D11_BUFFER_DESC  desc;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // �ʱ�ȭ 

		desc.Usage = D3D11_USAGE_DYNAMIC;             // Buffer�� ������ �� �ִ�
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(Vertex) * 5;          // Class�� �ȵ˴ϱ�? 
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // Buffer -> Constant ( Matrix )

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // �ʱ�ȭ 
		data.pSysMem = vertices;                       // vertices�� �ý��� ������

		// �ڿ��� ����̽��� ���� �����  ( Device --> CPU )
		HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Collider::UpdateBlock(Color color)
{
	Vertex  vertices[5];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 2
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0

	vertices[0].Color = color;
	vertices[1].Color = color;
	vertices[2].Color = color;
	vertices[3].Color = color;
	vertices[4].Color = color;
	// MAP�� UNMAP���̿��� ������ ����, CPU_ACCESS_WRITE

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceContext->Map(this->m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &mapped_subresource);

	// �����͸� ����
	memcpy(mapped_subresource.pData, vertices, sizeof(Vertex) * 5);
	DeviceContext->Unmap(this->m_pVertexBuffer, 0);



}

