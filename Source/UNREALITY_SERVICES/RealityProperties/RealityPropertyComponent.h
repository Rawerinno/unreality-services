#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RealityPropertyComponent.generated.h"

class UPrimitiveComponent;

/**
 * Prototype 0.0.1: a mass reservoir attached to a physics-enabled actor.
 * Mass is measured in kilograms, and transfers are conservative by design.
 */
UCLASS(ClassGroup=(UnrealityServices), meta=(BlueprintSpawnableComponent))
class UNREALITY_SERVICES_API URealityPropertyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URealityPropertyComponent();

    /** Defaults to the root primitive; may be assigned explicitly in the editor. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reality|Mass")
    TObjectPtr<UPrimitiveComponent> PhysicsBody;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reality|Mass", meta=(ClampMin="0.01"))
    float StartingMassKg = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reality|Mass", meta=(ClampMin="0.01"))
    float MinimumMassKg = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reality|Mass", meta=(ClampMin="0.01"))
    float MaximumMassKg = 100.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Reality|Mass")
    float CurrentMassKg = 10.0f;

    /** Returns how many kg can currently be extracted without exceeding minimum mass. */
    UFUNCTION(BlueprintPure, Category="Reality|Mass")
    float GetAvailableMassKg() const;

    /** Returns how many kg can be received without exceeding maximum mass. */
    UFUNCTION(BlueprintPure, Category="Reality|Mass")
    float GetMassCapacityKg() const;

    /** Validation and application occur atomically; no change on failure. */
    UFUNCTION(BlueprintCallable, Category="Reality|Mass")
    bool TryExtractMass(float AmountKg);

    UFUNCTION(BlueprintCallable, Category="Reality|Mass")
    bool TryReceiveMass(float AmountKg);

    /** Restores initial mass for rapid puzzle iteration. */
    UFUNCTION(BlueprintCallable, Category="Reality|Mass")
    void ResetMass();

protected:
    virtual void BeginPlay() override;

private:
    UPrimitiveComponent* ResolvePhysicsBody() const;
    bool CanApplyMass(float ProposedMassKg) const;
    bool ApplyMass(float NewMassKg);
};
