#pragma once

#include "ResourceBase.h"

class Shader;
class Texture;
class Mesh;

#define PATH_MAX_LENGTH 1024

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);
public:
	void Init();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	template<typename T>
	ResourceType GetResourceType();

public:
	const wstring GetAbsolutePath(wstring textureName)
	{
		if (_absolutePath != L"") return _absolutePath + textureName;

		wchar_t path[PATH_MAX_LENGTH] = {};
		GetModuleFileName(NULL, path, PATH_MAX_LENGTH);
		_absolutePath = wstring(path);

		auto pos = _absolutePath.find(L"Binaries\\");

		wstring subPath = _absolutePath.substr(0, pos);
		subPath += L"Resources\\Texture\\";

		_absolutePath = subPath;

		return _absolutePath + textureName;
	}

private:
	void CreateDefaultMesh();

private:
	wstring _resourcePath;
	wstring _absolutePath; // 내가 추가한거

private:
	using KeyObjMap = map<wstring/*key*/, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

template<typename T>
shared_ptr<T>
ResourceManager::Load(const wstring& key, const wstring& path)
{
	auto objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

template<typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;
	return true;
}

template<typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;

	assert(false);
	return ResourceType::None;
}

