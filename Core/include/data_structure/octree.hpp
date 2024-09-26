#pragma once


#include <rendering/draw_gizmo.hpp>

#include "scene/entity.hpp"

#include <vector>

#include "octree_iterator.hpp"
#include "stack"
#include "octree_node.hpp"

BEGIN_XNOR_CORE

template<class T>
class Octree
{
public:
    bool_t draw;
    
    void Update(std::vector<ObjectBounding<T>>& data);
    
    Octree() = default;

    ~Octree() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Octree)
    
    void Draw();

    size_t GetHandleSize() const
    {
        return m_HandleSize;
    }

    OctreeIterator<OctreeNode<const T>> GetIterator() const
    {
        return OctreeIterator<OctreeNode<const T>>(&m_MotherNode);
    }


    OctreeIterator<OctreeNode<T>> GetIterator()
    {
        return OctreeIterator<OctreeNode<T>>(&m_MotherNode);
    }

    Bound GetMotherBound() const
    {
        return m_MotherNode.boudingBox;        
    }
    
private:
    void Clear();
    
    OctreeNode<T> m_MotherNode;

    size_t m_HandleSize;
};

template <class T>
void Octree<T>::Update(std::vector<ObjectBounding<T>>& data)
{
    Clear();
    
    m_HandleSize = data.size();
    // Reset mother bound
    m_MotherNode.boudingBox = Bound();

    // Get the bounding box who contain all the data
    for (ObjectBounding<T>& element : data)
    {
        m_MotherNode.boudingBox.Encapsulate(element.bound);
    }

    // normalize the mother box with his max to be a cube
    const Vector3 previousSize = m_MotherNode.boudingBox.GetSize();
    const float_t maxSize = std::max( { previousSize.x , previousSize.y ,previousSize.z });
    const Vector3 size = Vector3(maxSize) * 0.5f;
    m_MotherNode.boudingBox.SetMinMax(m_MotherNode.boudingBox.center - size,m_MotherNode.boudingBox.center + size);
    
    for (ObjectBounding<T>& element : data)
    {
        m_MotherNode.AddObject(element);
    }
}

template <class T>
void Octree<T>::Draw()
{
    if (draw)
        m_MotherNode.Draw();
}



template <class T>
void Octree<T>::Clear()
{
    m_MotherNode.Clear();
}


END_XNOR_CORE
