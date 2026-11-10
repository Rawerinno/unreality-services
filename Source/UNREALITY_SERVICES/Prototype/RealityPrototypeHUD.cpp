#include "RealityPrototypeHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UNREALITY_SERVICESCharacter.h"
#include "RealityProperties/PropertyExtractorComponent.h"
#include "RealityProperties/RealityPropertyComponent.h"

void ARealityPrototypeHUD::Label(const FString& Value, float X, float Y, const FLinearColor& Color, float Scale)
{
    if (GEngine)
    {
        DrawText(Value, Color, X, Y, GEngine->GetSmallFont(), Scale, false);
    }
}

void ARealityPrototypeHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas || !PlayerOwner)
    {
        return;
    }

    const AUNREALITY_SERVICESCharacter* Character = Cast<AUNREALITY_SERVICESCharacter>(PlayerOwner->GetPawn());
    if (!Character)
    {
        return;
    }

    const float W = Canvas->SizeX;
    const float H = Canvas->SizeY;
    const FLinearColor Ink(0.76f, 0.91f, 0.95f, 1.0f);
    const FLinearColor Cyan(0.07f, 0.91f, 0.96f, 1.0f);
    const FLinearColor Amber(1.0f, 0.70f, 0.21f, 1.0f);
    const FLinearColor Dark(0.012f, 0.041f, 0.060f, 0.84f);

    // Municipal technician identity + XP.
    DrawRect(Dark, 24, 24, 290, 98);
    DrawRect(Cyan, 24, 24, 4, 98);
    Label(TEXT("REALITY MAINTENANCE / SECTOR 01"), 42, 35, Cyan, 1.0f);
    Label(Character->TechnicianName, 42, 57, Ink, 1.35f);
    DrawRect(FLinearColor(0.08f,0.18f,0.22f,1), 42, 94, 248, 8);
    DrawRect(Cyan, 42, 94, 248.f * FMath::Clamp(Character->ExperienceXP / 100.f, 0.f, 1.f), 8);
    Label(FString::Printf(TEXT("XP %d / 100"), Character->ExperienceXP), 207, 74, Ink, 0.85f);

    // Crosshair; target data under the reticle.
    const float CX = W * 0.5f;
    const float CY = H * 0.5f;
    DrawRect(Cyan, CX - 13, CY, 8, 2);
    DrawRect(Cyan, CX + 5, CY, 8, 2);
    DrawRect(Cyan, CX - 1, CY - 13, 2, 8);
    DrawRect(Cyan, CX - 1, CY + 5, 2, 8);

    const UPropertyExtractorComponent* Extractor = Character->GetRealityExtractor();
    if (Extractor)
    {
        if (const URealityPropertyComponent* Target = Extractor->GetTargetPropertyComponent())
        {
            const FString TargetName = GetNameSafe(Target->GetOwner());
            Label(FString::Printf(TEXT("%s | %.1f KG"), *TargetName, Target->CurrentMassKg),
                CX - 105, CY + 27, Cyan, 1.0f);
        }

        DrawRect(Dark, 24, H - 146, 330, 118);
        DrawRect(Amber, 24, H - 146, 4, 118);
        Label(TEXT("PROPERTY EXTRACTOR // MASS"), 42, H - 130, Amber, 1.1f);
        Label(FString::Printf(TEXT("RESERVOIR  %.1f KG"), Extractor->HeldMassKg), 42, H - 103, Ink, 1.45f);
        Label(TEXT("E EXTRACT  /  R ROUTE  /  F PUSH"), 42, H - 70, Ink, 1.0f);
    }

    DrawRect(Dark, W - 241, 24, 217, 60);
    Label(TEXT("UNREALITY SERVICES"), W - 225, 36, Cyan, 1.1f);
    Label(TEXT("PROTOTYPE 0.0.1"), W - 225, 61, Ink, 0.9f);

    if (Character->GetWorld() && Character->GetWorld()->GetTimeSeconds() < Character->StatusUntilTime)
    {
        DrawRect(Dark, CX - 195, H - 194, 390, 40);
        Label(Character->StatusMessage, CX - 175, H - 183, Amber, 1.15f);
    }

    if (Character->bGraphicsMenuOpen)
    {
        const float PX = CX - 228;
        const float PY = CY - 180;
        DrawRect(FLinearColor(0.008f,0.024f,0.034f,0.96f), PX, PY, 456, 346);
        DrawRect(Cyan, PX, PY, 456, 4);
        Label(TEXT("DISPLAY CONFIGURATION"), PX + 24, PY + 22, Cyan, 1.5f);
        Label(TEXT("1 LOW       2 MEDIUM"), PX + 24, PY + 76, Ink, 1.15f);
        Label(TEXT("3 HIGH      4 EPIC"), PX + 24, PY + 113, Ink, 1.15f);
        Label(TEXT("[ / ]  RENDER SCALE - / +"), PX + 24, PY + 155, Ink, 1.05f);
        Label(TEXT("V  TOGGLE V-SYNC"), PX + 24, PY + 190, Ink, 1.05f);
        Label(Character->GetGraphicsDescription(), PX + 24, PY + 233, Amber, 1.02f);
        Label(TEXT("F1 CLOSE / SETTINGS SAVED"), PX + 24, PY + 291, Cyan, 1.0f);
    }
    else
    {
        Label(TEXT("F1 GRAPHICS"), W - 141, H - 39, Ink, 0.9f);
    }
}
