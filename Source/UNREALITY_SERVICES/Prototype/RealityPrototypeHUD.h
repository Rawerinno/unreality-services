#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RealityPrototypeHUD.generated.h"

/** Canvas HUD with no runtime asset dependencies; replace with authored UMG later. */
UCLASS()
class UNREALITY_SERVICES_API ARealityPrototypeHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    void Label(const FString& Value, float X, float Y, const FLinearColor& Color, float Scale = 1.0f);
};
