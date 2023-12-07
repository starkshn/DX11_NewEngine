#pragma once
#include "IExecute.h"
#include "Geometry.h"

class QuadDemo : public IExecute
{
public:
	// Inherited via IExecute
	void Init() override;
	void Update() override;
	void Render() override;

public:
	shared_ptr<Shader> _shader;

	shared_ptr<Geometry<VertexColorData>> _geometry;

	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
};

