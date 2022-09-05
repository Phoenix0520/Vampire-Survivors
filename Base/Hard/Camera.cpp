#include "framework.h"
#include "Camera.h"
#include "Texture.h"

Camera::Camera()
{
	Vector3 eye = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 at = Vector3(0.0f, 0.0f, 1.0f);
	D3DXMatrixLookAtLH(&view, &eye, &(eye + at), &up);

	D3DXMatrixOrthoOffCenterLH(&proj,
		-(float)MAIN->GetWidth()*0.5f,
		(float)MAIN->GetWidth()*0.5f,
		-(float)MAIN->GetHeight()*0.5f,
		(float)MAIN->GetHeight()*0.5f,
		-1.0f, 1.0f);
}

Camera::~Camera()
{
}

////////////////////////////////////////////////////
//  Keyboard입력이, 카메라 제한
////////////////////////////////////////////////////
void Camera::PreUpdate()
{
	Vector2 position = this->position;

	if (PRESS(VK_CONTROL) && PRESS(VK_LEFT))
	{
		position.x = position.x - DELTA * moveSpeed.x;
	}
	if (PRESS(VK_CONTROL) && PRESS(VK_RIGHT))
	{
		position.x = position.x + DELTA * moveSpeed.x;
	}
	if (PRESS(VK_CONTROL) && PRESS(VK_UP))
	{
		position.y = position.y - DELTA * moveSpeed.y;
	}
	if (PRESS(VK_CONTROL) && PRESS(VK_DOWN))
	{
		position.y = position.y + DELTA * moveSpeed.y;
	}

	if (PRESS(VK_CONTROL) && PRESS(VK_DELETE))
	{
		position.x = 0.0f;
		position.y = 0.0f;
	}

	// Limit Check
	SetPosition(position);
}

void Camera::Update()
{
	if (!object)
		PreUpdate();
	else
	{
		Vector2 position = object->GetPosition();

		position = Vector2(position.x + offset.x, position.y + offset.y - 22.5f);

		if (centerXLock)
			position.x = 0.0f;

		SetPosition(position);
	}

	// 기본

	Vector3 eye = Vector3(position.x, position.y, 0.0f);
	Vector3 up = Vector3(0.0f, changeX, 0.0f);
	Vector3 at = Vector3(0.0f, 0.0f, changeY);
	D3DXMatrixLookAtLH(&view, &eye, &(eye + at), &up);

	// Window가 크기 변경
	D3DXMatrixOrthoOffCenterLH(&proj,
		-(float)MAIN->GetWidth()*0.5f,
		(float)MAIN->GetWidth()*0.5f,
		-(float)MAIN->GetHeight()*0.5f,
		(float)MAIN->GetHeight()*0.5f,
		-changeX, changeY);

	//	Matrix S;
	//	D3DXMatrixScaling(&S, 0.7f,0.7f,0.0f);
	//
	//	m_Projection = m_Projection * S;
}

void Camera::Update(Matrix V, Matrix P)
{
	
}

void Camera::Render()
{
	
}
/////////////////////////////////////////////////////////
// View(실제좌표) Window좌표로 변환
// 2D로 Text, 2D Line ( DirectX:: 이용)
////////////////////////////////////////////////////////
void Camera::VCToWC(Vector2 & posotion)
{
	Matrix vp = view * proj;
	Matrix S, T, W;

	D3DXMatrixScaling(&S, MAIN->GetWidth()*0.5f, -MAIN->GetHeight()*0.5f, 0.0f);
	D3DXMatrixTranslation(&T, MAIN->GetWidth()*0.5f, MAIN->GetHeight()*0.5f, 0.0f);

	W = vp * S*T;
	D3DXVec2TransformCoord(&posotion, &posotion, &W);
}

/////////////////////////////////////////////////////
//   Window좌표를 View좌표
//   Mouse Position(윈도우)
/////////////////////////////////////////////////////

void Camera::WCToVC(Vector2 & position)
{
	Matrix vp = view * proj;
	// inverse matrix
	D3DXMatrixInverse(&vp, NULL, &vp);

	position.x = (position.x / MAIN->GetWidth())*2.0f - 1.0f;
	position.y = -((position.y / MAIN->GetHeight())*2.0f - 1.0f);

	D3DXVec2TransformCoord(&position, &position, &vp);
}

void Camera::ChangeXY(float x, float y)
{
	static float firstX = (float)MAIN->GetWidth();
	static float firstY = (float)MAIN->GetHeight();

	changeX = x;
	changeY = y;

	//MAIN->SetWindowSize((int)firstX * changeX, (int)firstY * changeY);
}

void Camera::ChangeXY(Vector2 xy)
{
	Vector2 size = xy;
	static float firstX = (float)MAIN->GetWidth();
	static float firstY = (float)MAIN->GetHeight();

	changeX = size.x;
	changeY = size.y;

	//MAIN->SetWindowSize((int)firstX * changeX, (int)firstY * changeY);
}
