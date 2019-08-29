#include "StdAfx.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include "ShieldWeapon.h"
#include "DefaultComponents/Physics/BoxPrimitiveComponent.h"
#include <CryCore/StaticInstanceList.h>


static void RegisterShieldWeaponComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CShieldWeaponComponent));
        // Functions
        {
        }
    }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterShieldWeaponComponent)



void CShieldWeaponComponent::SetBlock(bool isEnable)
{    
    m_pCollider->m_size = isEnable ? ColliderSize : ZERO;
    m_pCollider->CreateGeometry();

}

void CShieldWeaponComponent::StartGame()
{
    m_pCollider = m_pEntity->GetComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
    ColliderSize = m_pCollider->m_size;
    m_pCollider->m_size = ZERO;
}
