#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PropertyExtractorComponent.generated.h"

class URealityPropertyComponent;

/**
 * First-person property routing logic. Blueprint events can bind controls to
 * ExtractMassFromView and ApplyMassToView without replacing template input.
 */
UCLASS(ClassGroup=(UnrealityServices), meta=(BlueprintSpawnableComponent))
class UNREALITY_SERVICES_API UPropertyExtractorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPropertyExtractorComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reality|Extractor", meta=(ClampMin="1.0"))
    float InteractionRangeCm = 650.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reality|Extractor", meta=(ClampMin="0.01"))
    float TransferAmountKg = 5.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Reality|Extractor")
    float HeldMassKg = 0.0f;

    UFUNCTION(BlueprintPure, Category="Reality|Extractor")
    bool HasPayload() const { return HeldMassKg > KINDA_SMALL_NUMBER; }

    /** Only succeeds if the reservoir can supply the whole requested quantity. */
    UFUNCTION(BlueprintCallable, Category="Reality|Extractor")
    bool ExtractMassFromView();

    /** Only succeeds if the target can receive the whole carried quantity. */
    UFUNCTION(BlueprintCallable, Category="Reality|Extractor")
    bool ApplyMassToView();

    /** Targeting helper for UI: nullptr on missing/invalid hit. */
    UFUNCTION(BlueprintCallable, Category="Reality|Extractor")
    URealityPropertyComponent* GetTargetPropertyComponent() const;

private:
    /** Preserve payload until applied; a failed action never discards it. */
    TWeakObjectPtr<URealityPropertyComponent> LastSource;
};
