#pragma once

#include "core.hpp"
#include "data_structure/object_bounding.hpp"
#include "rendering/draw_gizmo.hpp"

BEGIN_XNOR_CORE

enum Octans : uint8_t
{
    Zero = 0x00,// 0b00000000
    Q1 = 0x01, // 0b00000001
    Q2 = 0x02, // 0b00000010
    Q3 = 0x04, // 0b00000100
    Q4 = 0x08, // 0b00001000
    Q5 = 0x10, // 0b00010000
    Q6 = 0x20, // 0b00100000
    Q7 = 0x40, // 0b01000000
    Q8 = 0x80, // 0b10000000
};


template <typename T>
class OctreeNode
{
public:
    using Type = T;

    static constexpr size_t NbrOfChild = 8;

    static bool_t IsOctanValid(Octans octans, int32_t bitIndex);

    std::vector<T*> handles;

    Bound boudingBox;
    
    OctreeNode* parent = nullptr;
    
    DEFAULT_COPY_MOVE_OPERATIONS(OctreeNode)
    
    OctreeNode(ObjectBounding<T> objectBounding);
    
    OctreeNode() = default; 

    ~OctreeNode();
    
    void CreateBoundChild(Octans octans, Bound* outBound);
    
    void AddObject(ObjectBounding<T>& objectBounding);

    void Draw();
    
    Octans GetActiveOctans() const;
    
    bool_t GetChildNode(uint8_t octan,const OctreeNode<T>* outNode) const;
    
    bool_t SetPtrToChildNode(uint32_t bitIndex, OctreeNode<T>** outNode) const;

    void Clear();

    bool_t IsOctanNull() const;

private:    
    Octans m_ActiveOctans = Zero;
    std::array<OctreeNode*, NbrOfChild> m_Child;
    
    
    void DivideAndAdd(ObjectBounding<T>& objectBounding);
};


template <class T>
void OctreeNode<T>::AddObject(ObjectBounding<T>& objectBounding)
{
    DivideAndAdd(objectBounding);
}

template <class T>
bool_t OctreeNode<T>::IsOctanValid(Octans octans, int32_t bitIndex)
{
    return (octans & (1 << bitIndex));
}

template <class T>
bool_t OctreeNode<T>::IsOctanNull() const
{
    for (size_t i = 0; i < NbrOfChild; i++)
    {
        if (IsOctanValid(m_ActiveOctans,i))
            return false;
    }

    return true;
}

template <class T>
OctreeNode<T>::OctreeNode(ObjectBounding<T> objectBounding)
{
    boudingBox = objectBounding.bound;
    handles.push_back(objectBounding.handle);
}


template <class T>
OctreeNode<T>::~OctreeNode()
{
    for (size_t i = 0; i < m_Child.size(); i++)
    {
        if (IsOctanValid(m_ActiveOctans, static_cast<uint32_t>(i)))
        {
            delete m_Child[i];
            m_Child[i] = nullptr;
        }
    }
}
template <class T>
void OctreeNode<T>::CreateBoundChild(Octans octans, Bound* outBound)
{
    
    const float_t quarter = boudingBox.GetSize().x * 0.25f;
    const Vector3 childLength = boudingBox.GetSize() * 0.5f;
    const Vector3 childSize = Vector3(childLength);
    const Vector3 center = boudingBox.center;
    
    switch (octans)
    {
        case Q1:
            *outBound = Bound(center + Vector3(-quarter,quarter,-quarter), childSize ); 
            break;
            
        case Q2:
            *outBound = Bound(center + Vector3(quarter,quarter,-quarter), childSize ); 
            break;
            
        case Q3:
            *outBound = Bound(center + Vector3(-quarter,quarter,quarter), childSize ); 
            break;
            
        case Q4:
            *outBound = Bound(center + Vector3(quarter,quarter,quarter), childSize ); 
            break;
            
        case Q5:
            *outBound = Bound(center + Vector3(-quarter,-quarter,-quarter), childSize ); 
            break;
            
        case Q6:
            *outBound = Bound(center + Vector3(quarter,-quarter,-quarter), childSize ); 
            break;
            
        case Q7:
            *outBound = Bound(center + Vector3(-quarter,-quarter,quarter), childSize ); 
            break;
            
        case Q8:
            *outBound = Bound(center + Vector3(quarter,-quarter,quarter), childSize );
            break;
        case Zero:
            break;
        default: ;
    }
}

template <class T>
void OctreeNode<T>::Draw()
{
    Color color = Color::Green();
    for (size_t i = 0; i < m_Child.size(); i++)
    {
        if (m_ActiveOctans & (1 << i))
        {
            if (!m_Child[i]->handles.empty())
            {
                color = Color::Yellow();
                break;
            }
        }
    }
    DrawGizmo::Rectangle(boudingBox.center, boudingBox.extents, handles.empty() ? color : Color::Red());

    for (size_t i = 0; i < m_Child.size(); i++)
    {
        if (m_ActiveOctans & (1 << i))
        {
             m_Child[i]->Draw();
        }
    }
}


template <class T>
Octans OctreeNode<T>::GetActiveOctans() const
{
    return m_ActiveOctans;
}

template <class T>
void OctreeNode<T>::Clear()
{
    for (size_t i = 0; i < m_Child.size(); i++)
    {
        if (IsOctanValid(m_ActiveOctans, static_cast<uint32_t>(i)))
        {
            m_Child[i]->Clear();
        }
    }

    m_ActiveOctans = Zero;
    handles.clear();
    boudingBox = Bound();
}

template <class T>
bool_t OctreeNode<T>::GetChildNode(uint8_t octan, const OctreeNode<T>* outNode) const
{
    if (IsOctanValid(m_ActiveOctans, octan))
    {
        outNode = m_Child.at(octan);
        return true;
    }
    return false;
}

template <typename T>
bool_t OctreeNode<T>::SetPtrToChildNode(uint32_t bitIndex, OctreeNode<T>** outNode) const
{
    if (IsOctanValid(m_ActiveOctans, bitIndex))
    {
        *outNode = m_Child.at(bitIndex);
        return true;
    }
    return false;
}


template <class T>
void OctreeNode<T>::DivideAndAdd(ObjectBounding<T>& objectBounding)
{
    // If current bound is less than min size return 
    if (boudingBox.GetSize().x < objectBounding.bound.GetSize().x)
    {
        handles.push_back(objectBounding.handle);
        return;
    }
    
    bool_t hasDivide = false;
    
    for (size_t i = 0 ; i < m_Child.size(); i++)
    {
        uint32_t current = (0 | (1 << i));
        Bound octanBound;
        CreateBoundChild(static_cast<Octans>(current),&octanBound);

        // if the current octan countain the object bound
        if (octanBound.Countain(objectBounding.bound))
        {
            m_ActiveOctans = static_cast<Octans>(m_ActiveOctans | (1 << i));
            
            if (m_Child[i] == nullptr)
                m_Child[i] = new OctreeNode();

            m_Child[i]->boudingBox = octanBound;
            m_Child[i]->parent = this;
            // try adding the current object bound in the valid octan
            hasDivide = true;
            m_Child[i]->AddObject(objectBounding);
            break;
            
        }
      
    }

    if (!hasDivide)
        handles.push_back(objectBounding.handle);
    
}


END_XNOR_CORE
