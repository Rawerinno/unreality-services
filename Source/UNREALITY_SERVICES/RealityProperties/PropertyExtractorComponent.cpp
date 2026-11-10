#include "PropertyExtractorComponent.h"

#include "RealityPropertyComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UPropertyExtractorComponent::UPropertyExtractorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

URealityPropertyComponent* UPropertyExtractorComponent::GetTargetPropertyComponent() const
{
    const APawn* Pawn = Cast<APawn>(GetOwner());
    const APlayerController* Controller = Pawn ? Cast<APlayerController>(Pawn->GetController()) : nullptr;
    const UWorld* World = GetWorld();
    if (!Controller || !World)
    {
        return nullptr;
    }

    FVector Origin;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(Origin, ViewRotation);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(RealityExtractorTrace), false);
    Params.AddIgnoredActor(GetOwner());

    const FVector End = Origin + ViewRotation.Vector() * InteractionRangeCm;
    if (!World->LineTraceSingleByChannel(Hit, Origin, End, ECC_Visibility, Params))
    {
        return nullptr;
    }

    return IsValid(Hit.GetActor())
        ? Hit.GetActor()->FindComponentByClass<URealityPropertyComponent>()
        : nullptr;
}

bool UPropertyExtractorComponent::ExtractMassFromView()
{
    if (HasPayload() || !FMath::IsFinite(TransferAmountKg) || TransferAmountKg <= KINDA_SMALL_NUMBER)
    {
        return false;
    }

    URealityPropertyComponent* Target = GetTargetPropertyComponent();
    if (!IsValid(Target) || !Target->TryExtractMass(TransferAmountKg))
    {
        return false;
    }

    HeldMassKg = TransferAmountKg;
    LastSource = Target;
    return true;
}

bool UPropertyExtractorComponent::ApplyMassToView()
{
    if (!HasPayload())
    {
        return false;
    }

    URealityPropertyComponent* Target = GetTargetPropertyComponent();
    if (!IsValid(Target) || Target == LastSource.Get() || !Target->TryReceiveMass(HeldMassKg))
    {
        return false;
    }

    HeldMassKg = 0.0f;
    LastSource.Reset();
    return true;
}
