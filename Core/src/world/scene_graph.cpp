#include "world/scene_graph.hpp"

#include <Maths/matrix.hpp>

#include "utils/logger.hpp"

using namespace XnorCore;

Matrix GetTrsOfParents(const Entity& parent)
{
	const Matrix parentMatrix = Matrix::Trs(parent.transform.GetPosition(), parent.transform.GetRotation(), parent.transform.GetScale());

	if (parent.HasParent())  
	{
		return GetTrsOfParents(*parent.GetParent()) * parentMatrix;
	}
	
	return parentMatrix;
}

void SceneGraph::UpdateTransform(Entity& entity)
{
	Transform& t = entity.transform;

	if (t.m_Changed == false)
		return;
	
	t.m_Changed = false;
	t.m_Rotation = Quaternion::FromEuler(t.m_EulerRotation).Normalized();
	t.worldMatrix = Matrix::Trs(t.m_Position, t.m_Rotation, t.m_Scale);
		
	if (entity.HasParent())
	{
		t.worldMatrix = GetTrsOfParents(*entity.GetParent()) *  t.worldMatrix;
	}

	if (!entity.HasChildren())
		return;
		
	for (size_t i = 0; i < entity.GetChildCount(); i++)
	{
		Entity& ent = *entity.GetChild(i);
		ent.transform.m_Changed = true;
		UpdateTransform(ent);
	}
}

void SceneGraph::	Update(const List<Entity*>& entities)
{
    for (size_t i = 0; i < entities.GetSize(); i++)
    {
        UpdateTransform(*entities[i]);
    }
}

void SceneGraph::OnAttachToParent(Entity& entity)
{
    Transform& transform = entity.transform;

    Matrix trs = transform.worldMatrix;
    const Matrix parent = GetTrsOfParents(*entity.GetParent());
    
    trs = parent.Inverted() * trs;
	Vector3 skew;
	Vector4 perspective;
	
    trs.Decompose(&transform.m_Position, &transform.m_Rotation, &transform.m_Scale, &skew, &perspective);
	transform.m_Position = static_cast<Vector3>(trs[3]);
	transform.m_EulerRotation = Quaternion::ToEuler(transform.m_Rotation);
}
