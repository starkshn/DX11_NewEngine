#pragma once

class CameraScript : public MonoBehaviour
{
public:
	virtual void Start() override;
	virtual void Update() override;

	float _speed = 10.f;
};

