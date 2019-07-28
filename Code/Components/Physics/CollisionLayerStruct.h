#pragma once

enum class ECollisionLayerFlags
{
    Default = 1 << 0,
    Player = 1 << 1,
    Weapon = 1 << 2,
    Enemy = 1 << 3,
};


static void ReflectType(Schematyc::CTypeDesc<ECollisionLayerFlags>& desc)
{
    desc.SetGUID("{C78A4852-45BF-4D94-BB4B-48D99EF18CF6}"_cry_guid);
    desc.SetDescription("Layer type");
    desc.SetLabel("Collision Layer");

    desc.SetDefaultValue(ECollisionLayerFlags::Default);
    desc.AddConstant(ECollisionLayerFlags::Enemy, "Enemy", "Enemy");
    desc.AddConstant(ECollisionLayerFlags::Player, "Player", "Player");
    desc.AddConstant(ECollisionLayerFlags::Weapon, "Weapon", "Weapon");
}


struct SCollisionLayerStruct
{   
    static void ReflectType(Schematyc::CTypeDesc<SCollisionLayerStruct>& desc)
    {
        desc.SetGUID("{01E6D49F-56C4-4CFC-8C26-EFBB94A43DB3}"_cry_guid);

       // desc.AddMember(&SCollisionLayerStruct::Name, 'type', "CollisionType", "Collision Type", "Defines the type collision classes of this object", Schematyc::CArray<SCollisionLayerStruct>());
      //  desc.AddMember(&SCollisionLayerStruct::BitMask, 'ignr', "CollisionIgnore", "Collision Ignore", "Defines the ignore collision classes of this object", Schematyc::CArray<SCollisionLayerStruct>());

    }


    string Name;
    uint32 BitMask;



};
