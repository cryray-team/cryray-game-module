#pragma once

class IPhysicsShell;
class IPhysicsElement;
class ENGINE_API IObjectPhysicsCollision
{
public:
    IObjectPhysicsCollision() = default;
    IObjectPhysicsCollision(const IObjectPhysicsCollision&) = default;
    IObjectPhysicsCollision(IObjectPhysicsCollision&&) = default;

    virtual const IPhysicsShell *physics_shell()const = 0;
    virtual const IPhysicsElement *physics_character()const = 0;//depricated
};