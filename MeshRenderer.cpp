#include "stdafx.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "Entity.h"
using namespace Core;

Core::MeshRenderer::MeshRenderer(Device& device, Entity& entity, string modelPath)
	:Component(entity), _device(device), _mesh(nullptr), _material(nullptr)
{
	SetMesh(modelPath);
	SetMaterial(MaterialType::SAMPLE);
}

Core::MeshRenderer::~MeshRenderer()
{
}

void Core::MeshRenderer::SetMesh(string modelPath)
{
	if (_mesh != nullptr)
		delete(_mesh);

	_mesh = MeshFactory::CreateMesh(_device, modelPath);
}

void Core::MeshRenderer::SetMaterial(MaterialType type)
{
	if (_material != nullptr)
		delete(_material);

	_material = MaterialFactory::CreateMaterial(_device, type);
}

void Core::MeshRenderer::UpdateFrame(float deltaTime)
{
}

std::type_index Core::MeshRenderer::GetType()
{
	return typeid(MeshRenderer);
}

void Core::MeshRenderer::Resize(uint32_t width, uint32_t height)
{
	//do nothing.
}
