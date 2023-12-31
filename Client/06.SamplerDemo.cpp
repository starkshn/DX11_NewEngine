#include "pch.h"
#include "06.SamplerDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"

void SamplerDemo::Init()
{
	_shader = make_shared<Shader>(L"05.Sampler.fx");

	// Object
	_geometry = make_shared<Geometry<VertexTextureData>>();

	// GeometryHelper::CreateQuad(_geometry);
	// GeometryHelper::CreateCube(_geometry);
	// GeometryHelper::CreateSphere(_geometry);
	GeometryHelper::CreateGrid(_geometry, 256, 256);

	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	_camera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -2.f));

	// Texture Load (UV매핑을 위해)
	RESOURCES->Load<Texture>(L"Veigar.jpg", RESOURCES->GetAbsolutePath(L"Veigar.jpg"));

	// RESOURCES->Get<Texture>(L"Veigar.jpg");
	_texture = RESOURCES->Get<Texture>(L"Veigar.jpg");
}

void SamplerDemo::Update()
{
	_camera->Update();
}

void SamplerDemo::Render()
{
	// ?
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);

	// Shader의 Textur0과 연결
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	enum ADDRESS_VALUE
	{
		ADDRESS_WRAP = 0,
		ADDRESS_MIRROR = 1,
		ADDRESS_CLAMP = 2,
		ADDRESS_BORDER = 3,
	};

	// SamplerState 적용
	_shader->GetScalar("Address")->SetInt(ADDRESS_BORDER);

	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
