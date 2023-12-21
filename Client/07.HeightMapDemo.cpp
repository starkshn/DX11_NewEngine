#include "pch.h"
#include "07.HeightMapDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"

void HeightMapDemo::Init()
{
	_shader = make_shared<Shader>(L"06.Terrain.fx");

	// Texture
	// Texture Load (UV매핑을 위해)
	RESOURCES->Load<Texture>(L"grass.jpg", RESOURCES->GetAbsolutePath(L"grass.jpg"));
	RESOURCES->Load<Texture>(L"Heightmap.png", RESOURCES->GetAbsolutePath(L"Heightmap.png"));

	_texture = RESOURCES->Get<Texture>(L"grass.jpg");
	_heightMap = RESOURCES->Get<Texture>(L"Heightmap.png");

	const int32 width = _heightMap->GetSize().x;
	const int32 height = _heightMap->GetSize().y;

	const DirectX::ScratchImage& info = _heightMap->GetInfo();
	// uint8* pixelBuffer = info.GetPixels();
	 uint8* pixelBuffer = info.GetPixels();

	// Object
	_geometry = make_shared<Geometry<VertexTextureData>>();

	// GeometryHelper::CreateQuad(_geometry);
	// GeometryHelper::CreateCube(_geometry);
	// GeometryHelper::CreateSphere(_geometry);
	GeometryHelper::CreateGrid(_geometry, width, height);

	// CPU 에서 좌표 고치기
	{
		vector<VertexTextureData>& v = const_cast<vector<VertexTextureData>&>(_geometry->GetVertices());

		for (int32 z = 0; z < height; ++z)
		{
			for (int32 x = 0; x < width; ++x)
			{
				// idx -> 진행방향
				// 0 1 2 3 4 5 ... n
				// 0 1 2 3 4 5 ... n
				int32 idx = width * z + x;
				
				// 아래는 높이 보정 로직이다.
				// Pixel이 0~255사이의 숫자가 있다
				// 25.f는 최대 높이
				uint8 height = pixelBuffer[idx] / 255.f * 25.f;
				v[idx].position.y = height; // 높이 보정
			}
		}
	}

	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	_camera->GetTransform()->SetPosition(Vec3(25.f, 5.f, -2.f));
	_camera->GetTransform()->SetRotation(Vec3(25.f, 0.f, 0.f));


}

void HeightMapDemo::Update()
{
	_camera->Update();
}

void HeightMapDemo::Render()
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
	};

	// SamplerState 적용
	_shader->GetScalar("Address")->SetInt(ADDRESS_WRAP);

	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
