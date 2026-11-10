#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealityMassBlock.generated.h"

class UStaticMeshComponent;
class URealityPropertyComponent;

/** Spawnable block for the first mass-routing test; no Blueprint asset is required. */
UCLASS()
class UNREALITY_SERVICES_API ARealityMassBlock : public AActor
{
    GENERATED_BODY()

public:
    ARealityMassBlock();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reality")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reality")
    TObjectPtr<URealityPropertyComponent> Properties;

    /** Set before BeginPlay, preferably with SpawnActorDeferred. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reality")
    float InitialMassKg = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reality")
    FName BlockLabel = TEXT("TEST BLOCK");

    virtual void BeginPlay() override;
};
