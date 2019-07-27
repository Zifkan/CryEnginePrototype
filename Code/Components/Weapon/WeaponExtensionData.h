#pragma once

struct SWeaponHitStruct
{

    Vec3 Hitpoint;
    Vec3 HitDirection;
    float Damage;
    int PartId;

};

enum class EWeaponHandType
{
    LeftHand,
    RightHand,
    TwoHanded,
};

static void ReflectType(Schematyc::CTypeDesc<EWeaponHandType>& desc)
{
    desc.SetGUID("{C78A4852-45BF-4D94-BB4B-48D99EF18CF6}"_cry_guid);
    desc.SetDescription("Hand for attachment");
    desc.SetLabel("Weapon Hand Type Enum");

    desc.SetDefaultValue(EWeaponHandType::LeftHand);
    desc.AddConstant(EWeaponHandType::LeftHand, "LeftHand", "LeftHand");
    desc.AddConstant(EWeaponHandType::RightHand, "RightHand", "RightHand");
}

static void ReflectType(Schematyc::CTypeDesc<pe_type>& desc)
{
    desc.SetGUID("{273F3BAD-2293-4846-8612-93682853F324}"_cry_guid);
    desc.SetDescription("Phys type");
    desc.SetLabel("Phys Type of object");

    desc.SetDefaultValue(pe_type::PE_NONE);
    desc.AddConstant(pe_type::PE_NONE, "PE_NONE", "PE_NONE");
    desc.AddConstant(pe_type::PE_RIGID, "PE_RIGID", "PE_RIGID");
    desc.AddConstant(pe_type::PE_STATIC, "PE_STATIC", "PE_STATIC");
}
