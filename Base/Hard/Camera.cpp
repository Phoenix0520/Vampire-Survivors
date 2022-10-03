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

		SetPosition(position);
	}

	Vector3 eye = Vector3(position.x, position.y, 0.0f);
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

void Camera::VCToWC(Vector2 & posotion)
{
	Matrix vp = view * proj;
	Matrix S, T, W;

	D3DXMatrixScaling(&S, MAIN->GetWidth()*0.5f, -MAIN->GetHeight()*0.5f, 0.0f);
	D3DXMatrixTranslation(&T, MAIN->GetWidth()*0.5f, MAIN->GetHeight()*0.5f, 0.0f);

	W = vp * S*T;
	D3DXVec2TransformCoord(&posotion, &posotion, &W);
}

void Camera::WCToVC(Vector2 & position)
{
	Matrix vp = view * proj;
	// inverse matrix
	D3DXMatrixInverse(&vp, NULL, &vp);

	position.x = (position.x / MAIN->GetWidth())*2.0f - 1.0f;
	position.y = -((position.y / MAIN->GetHeight())*2.0f - 1.0f);

	D3DXVec2TransformCoord(&position, &position, &vp);
}