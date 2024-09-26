#include "world/world.hpp"

#include "input/time.hpp"
#include "physics/physics_world.hpp"
#include "world/scene_graph.hpp"

using namespace XnorCore;

void World::Update()
{
    if (!hasStarted && isPlaying)
    {
        scene->Awake();
        scene->Begin();
        hasStarted = true;
    }

    if (isPlaying)
    {
        scene->Update();
        scene->PrePhysics();
        PhysicsWorld::Update(Time::GetDeltaTime());
        scene->PostPhysics();
    }
    
    SceneGraph::Update(scene->GetEntities());

    scene->OnRendering();
}

