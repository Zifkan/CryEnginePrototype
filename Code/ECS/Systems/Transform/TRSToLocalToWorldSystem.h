#pragma once

class TRSToLocalToWorldSystem  final : public SystemBase<LocalToWorld,Rotation,Translation>
{
public:
    void OnCreate() override;
};
