#pragma once

#include "IExecute.h"
#include "Geometry.h"

class TextureDemo : public IExecute
{
public:
	// Inherited via IExecute
	void Init() override;
	void Update() override;
	void Render() override;

public:
	shared_ptr<Shader> _shader;

	// Object
	// 아래 세개를 묶은게 Mesh라는 개념이다.
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	Matrix _world = Matrix::Identity;
	Matrix _view = Matrix::Identity;
	Matrix _projection = Matrix::Identity;

	// Camera
	shared_ptr<GameObject> _camera;

	// Texture
	shared_ptr<Texture> _texture;
};

