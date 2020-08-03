#pragma once
#include "BaseAction.h"

class IBaseLifeResource;

class EnemyMovementAction : public BaseAction
{
public:

    EnemyMovementAction(IEntity* characterEntity,   
        
        int priority,
        FragmentID fragmentID = FRAGMENT_ID_INVALID,
        const TagState& fragTags = TAG_STATE_EMPTY,
        uint32 flags = 0,
        ActionScopes scopeMask = 0,
        uint32 userToken = 0);

    ~EnemyMovementAction();

    EStatus Update(float timePassed) override;
private:    

    Vec2 m_movePoint;
   

    float m_sprintRatio;
    float m_sprintAnimRatio;
};
