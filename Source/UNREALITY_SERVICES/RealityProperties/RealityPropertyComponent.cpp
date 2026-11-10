#include "RealityPropertyComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

URealityPropertyComponent::URealityPropertyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URealityPropertyComponent::BeginPlay()
{
    Super::BeginPlay();

    MinimumMassKg = FMath::Max(0.01f, MinimumMassKg);
    MaximumMassKg = FMath::Max(MinimumMassKg, MaximumMassKg);
    StartingMassKg = FMath::Clamp(StartingMassKg, MinimumMassKg, MaximumMassKg);

    if (!ApplyMass(StartingMassKg))
    {
        UE_LOG(LogTemp, Warning, TEXT("Reality property: %s needs a simulating primitive body for mass routing."),
            *GetNameSafe(GetOwner()));
    }
}

UPrimitiveComponent* URealityPropertyComponent::ResolvePhysicsBody() const
{
    if (PhysicsBody)
    {
        return PhysicsBody;
    }

    const AActor* Owner = GetOwner();
    return Owner ? Cast<UPrimitiveComponent>(Owner->GetRootComponent()) : nullptr;
}

bool URealityPropertyComponent::CanApplyMass(float ProposedMassKg) const
{
    const UPrimitiveComponent* Body = ResolvePhysicsBody();
    return FMath::IsFinite(ProposedMassKg)
        && ProposedMassKg >= MinimumMassKg - KINDA_SMALL_NUMBER
        && ProposedMassKg <= MaximumMassKg + KINDA_SMALL_NUMBER
        && IsValid(Body)
        && Body->IsSimulatingPhysics();
}

bool URealityPropertyComponent::ApplyMass(float NewMassKg)
{
    if (!CanApplyMass(NewMassKg))
    {
        return false;
    }

    UPrimitiveComponent* Body = ResolvePhysicsBody();
    Body->SetMassOverrideInKg(NAME_None, NewMassKg, true);
    Body->WakeAllRigidBodies();
    CurrentMassKg = NewMassKg;
    return true;
}

float URealityPropertyComponent::GetAvailableMassKg() const
{
    return FMath::Max(0.0f, CurrentMassKg - MinimumMassKg);
}

float URealityPropertyComponent::GetMassCapacityKg() const
{
    return FMath::Max(0.0f, MaximumMassKg - CurrentMassKg);
}

bool URealityPropertyComponent::TryExtractMass(float AmountKg)
{
    return FMath::IsFinite(AmountKg)
        && AmountKg > KINDA_SMALL_NUMBER
        && AmountKg <= GetAvailableMassKg() + KINDA_SMALL_NUMBER
        && ApplyMass(CurrentMassKg - AmountKg);
}

bool URealityPropertyComponent::TryReceiveMass(float AmountKg)
{
    return FMath::IsFinite(AmountKg)
        && AmountKg > KINDA_SMALL_NUMBER
        && AmountKg <= GetMassCapacityKg() + KINDA_SMALL_NUMBER
        && ApplyMass(CurrentMassKg + AmountKg);
}

void URealityPropertyComponent::ResetMass()
{
    ApplyMass(StartingMassKg);
}
