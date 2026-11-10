// Copyright Epic Games, Inc. All Rights Reserved.

#include "UNREALITY_SERVICESCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UNREALITY_SERVICES.h"
#include "RealityProperties/PropertyExtractorComponent.h"
#include "RealityProperties/RealityPropertyComponent.h"
#include "Prototype/RealityMassBlock.h"
#include "Prototype/RealityPrototypeHUD.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "InputCoreTypes.h"

AUNREALITY_SERVICESCharacter::AUNREALITY_SERVICESCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
    GetCharacterMovement()->AirControl = 0.5f;

    PrimaryActorTick.bCanEverTick = true;
    RealityExtractor = CreateDefaultSubobject<UPropertyExtractorComponent>(TEXT("PropertyExtractor"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    ToolBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExtractorHousing"));
    ToolBody->SetupAttachment(FirstPersonCameraComponent);
    ToolBody->SetOnlyOwnerSee(true);
    ToolBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ToolBody->SetRelativeLocation(FVector(42.f, 21.f, -24.f));
    ToolBody->SetRelativeScale3D(FVector(0.33f, 0.11f, 0.11f));

    ToolBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExtractorBarrel"));
    ToolBarrel->SetupAttachment(ToolBody);
    ToolBarrel->SetOnlyOwnerSee(true);
    ToolBarrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ToolBarrel->SetRelativeLocation(FVector(56.f, 0.f, 10.f));
    ToolBarrel->SetRelativeScale3D(FVector(0.60f, 0.55f, 0.55f));

    ToolGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExtractorGrip"));
    ToolGrip->SetupAttachment(ToolBody);
    ToolGrip->SetOnlyOwnerSee(true);
    ToolGrip->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ToolGrip->SetRelativeLocation(FVector(-24.f, 4.f, -66.f));
    ToolGrip->SetRelativeScale3D(FVector(0.27f, 0.55f, 0.95f));

    if (CubeMesh.Succeeded())
    {
        ToolBody->SetStaticMesh(CubeMesh.Object);
        ToolBarrel->SetStaticMesh(CubeMesh.Object);
        ToolGrip->SetStaticMesh(CubeMesh.Object);
    }
}

void AUNREALITY_SERVICESCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
    // Prototype keyboard controls are bound without changing the template's .uasset mappings.
    PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::ExtractMass);
    PlayerInputComponent->BindKey(EKeys::R, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::RouteMass);
    PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::PushTarget);
    PlayerInputComponent->BindKey(EKeys::F1, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::ToggleGraphicsMenu);
    PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::SetQualityLow);
    PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::SetQualityMedium);
    PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::SetQualityHigh);
    PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::SetQualityEpic);
    PlayerInputComponent->BindKey(EKeys::LeftBracket, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::RenderScaleDown);
    PlayerInputComponent->BindKey(EKeys::RightBracket, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::RenderScaleUp);
    PlayerInputComponent->BindKey(EKeys::V, IE_Pressed, this, &AUNREALITY_SERVICESCharacter::ToggleVSync);

    // Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUNREALITY_SERVICESCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUNREALITY_SERVICESCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUNREALITY_SERVICESCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUNREALITY_SERVICESCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AUNREALITY_SERVICESCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogUNREALITY_SERVICES, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AUNREALITY_SERVICESCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AUNREALITY_SERVICESCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AUNREALITY_SERVICESCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AUNREALITY_SERVICESCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AUNREALITY_SERVICESCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AUNREALITY_SERVICESCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}


void AUNREALITY_SERVICESCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (IsLocallyControlled())
    {
        SpawnTestBlocks();
        ShowStatus(TEXT("TRANSFER MASS: E EXTRACT / R ROUTE / F PUSH"));
    }
}

void AUNREALITY_SERVICESCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!IsLocallyControlled())
    {
        return;
    }

    if (!bPrototypeInitialized)
    {
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            bPrototypeInitialized = true;
            if (!Cast<ARealityPrototypeHUD>(PC->GetHUD()))
            {
                PC->ClientSetHUD(ARealityPrototypeHUD::StaticClass());
            }
        }
    }

    ToolPulse = FMath::Max(0.f, ToolPulse - DeltaSeconds * 3.3f);
    if (ToolBody)
    {
        const float Recoil = FMath::Sin(ToolPulse * PI) * 6.0f;
        ToolBody->SetRelativeLocation(FVector(42.f - Recoil, 21.f, -24.f + Recoil * 0.4f));
    }
}

void AUNREALITY_SERVICESCharacter::ShowStatus(const FString& Message)
{
    StatusMessage = Message;
    StatusUntilTime = GetWorld() ? GetWorld()->GetTimeSeconds() + 2.5f : 0.f;
}

void AUNREALITY_SERVICESCharacter::ExtractMass()
{
    if (bGraphicsMenuOpen || !RealityExtractor)
    {
        return;
    }

    if (RealityExtractor->ExtractMassFromView())
    {
        ToolPulse = 1.f;
        ShowStatus(TEXT("MASS CAPTURED +5 KG"));
    }
    else
    {
        ShowStatus(TEXT("EXTRACTION FAILED / CHECK TARGET"));
    }
}

void AUNREALITY_SERVICESCharacter::RouteMass()
{
    if (bGraphicsMenuOpen || !RealityExtractor)
    {
        return;
    }

    if (RealityExtractor->ApplyMassToView())
    {
        ToolPulse = 1.f;
        ExperienceXP = FMath::Min(100, ExperienceXP + 25);
        ShowStatus(TEXT("MASS ROUTED / +25 XP"));
    }
    else
    {
        ShowStatus(TEXT("ROUTING FAILED / CHECK DESTINATION"));
    }
}

void AUNREALITY_SERVICESCharacter::PushTarget()
{
    if (bGraphicsMenuOpen || !GetWorld() || !GetController())
    {
        return;
    }

    FVector ViewOrigin;
    FRotator ViewRotation;
    GetController()->GetPlayerViewPoint(ViewOrigin, ViewRotation);

    FCollisionQueryParams Params(SCENE_QUERY_STAT(RealityPush), false);
    Params.AddIgnoredActor(this);
    FHitResult Hit;
    if (GetWorld()->LineTraceSingleByChannel(Hit, ViewOrigin,
        ViewOrigin + ViewRotation.Vector() * 550.f, ECC_Visibility, Params))
    {
        UPrimitiveComponent* Body = Hit.GetComponent();
        if (IsValid(Body) && Body->IsSimulatingPhysics())
        {
            // Fixed impulse, not velocity: heavier blocks accelerate less.
            Body->AddImpulseAtLocation(ViewRotation.Vector() * 4200.f, Hit.ImpactPoint);
            ToolPulse = 0.65f;
            ShowStatus(TEXT("FORCE TEST / IMPULSE APPLIED"));
            return;
        }
    }
    ShowStatus(TEXT("PUSH TEST / NO PHYSICAL TARGET"));
}

void AUNREALITY_SERVICESCharacter::SpawnTestBlocks()
{
    if (!GetWorld())
    {
        return;
    }

    const FVector Forward = GetActorForwardVector();
    const FVector Right = GetActorRightVector();
    const FVector Start = GetActorLocation();
    const FVector Centers[2] = {
        Start + Forward * 460.f - Right * 160.f,
        Start + Forward * 460.f + Right * 160.f
    };

    for (int32 Index = 0; Index < 2; ++Index)
    {
        FVector Position = Centers[Index] + FVector(0.f, 0.f, 90.f);
        FHitResult GroundHit;
        FCollisionQueryParams Query(SCENE_QUERY_STAT(RealityBlockPlacement), false);
        Query.AddIgnoredActor(this);
        if (GetWorld()->LineTraceSingleByChannel(
            GroundHit, Centers[Index] + FVector(0,0,500),
            Centers[Index] - FVector(0,0,1600), ECC_Visibility, Query))
        {
            Position.Z = GroundHit.ImpactPoint.Z + 75.f;
        }

        FTransform SpawnTransform(FRotator::ZeroRotator, Position);
        ARealityMassBlock* Block = GetWorld()->SpawnActorDeferred<ARealityMassBlock>(
            ARealityMassBlock::StaticClass(), SpawnTransform, this);
        if (Block)
        {
            Block->InitialMassKg = Index == 0 ? 20.0f : 5.0f;
            Block->BlockLabel = Index == 0 ? TEXT("BODY A") : TEXT("BODY B");
            UGameplayStatics::FinishSpawningActor(Block, SpawnTransform);
        }
    }
}

void AUNREALITY_SERVICESCharacter::ToggleGraphicsMenu()
{
    bGraphicsMenuOpen = !bGraphicsMenuOpen;
}

void AUNREALITY_SERVICESCharacter::ApplyQuality(int32 Quality)
{
    if (!bGraphicsMenuOpen || !GEngine) return;
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->SetOverallScalabilityLevel(Quality);
        Settings->ApplySettings(false);
        ShowStatus(TEXT("GRAPHICS PROFILE SAVED"));
    }
}
void AUNREALITY_SERVICESCharacter::SetQualityLow()    { ApplyQuality(0); }
void AUNREALITY_SERVICESCharacter::SetQualityMedium() { ApplyQuality(1); }
void AUNREALITY_SERVICESCharacter::SetQualityHigh()   { ApplyQuality(2); }
void AUNREALITY_SERVICESCharacter::SetQualityEpic()   { ApplyQuality(3); }

void AUNREALITY_SERVICESCharacter::ChangeRenderScale(float Delta)
{
    if (!bGraphicsMenuOpen || !GEngine) return;
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        float Normalized = 0.f, Actual = 100.f, Min = 50.f, Max = 100.f;
        Settings->GetResolutionScaleInformationEx(Normalized, Actual, Min, Max);
        RenderScalePercent = FMath::Clamp(Actual + Delta, FMath::Max(50.f, Min), FMath::Min(100.f, Max));
        Settings->SetResolutionScaleValueEx(RenderScalePercent);
        Settings->ApplySettings(false);
    }
}

void AUNREALITY_SERVICESCharacter::RenderScaleDown() { ChangeRenderScale(-10.f); }
void AUNREALITY_SERVICESCharacter::RenderScaleUp()   { ChangeRenderScale(10.f); }

void AUNREALITY_SERVICESCharacter::ToggleVSync()
{
    if (!bGraphicsMenuOpen || !GEngine) return;
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->SetVSyncEnabled(!Settings->IsVSyncEnabled());
        Settings->ApplySettings(false);
    }
}

FString AUNREALITY_SERVICESCharacter::GetGraphicsDescription() const
{
    if (!GEngine) return TEXT("GRAPHICS UNAVAILABLE");
    const UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (!Settings) return TEXT("GRAPHICS UNAVAILABLE");
    const int32 Quality = Settings->GetOverallScalabilityLevel();
    const TCHAR* QualityName =
        Quality == 0 ? TEXT("LOW") :
        Quality == 1 ? TEXT("MEDIUM") :
        Quality == 2 ? TEXT("HIGH") :
        Quality == 3 ? TEXT("EPIC") : TEXT("CUSTOM");
    float Normalized = 0.f, Actual = 100.f, Min = 50.f, Max = 100.f;
    Settings->GetResolutionScaleInformationEx(Normalized, Actual, Min, Max);
    return FString::Printf(TEXT("%s  |  SCALE %.0f%%  |  V-SYNC %s"),
        QualityName, Actual, Settings->IsVSyncEnabled() ? TEXT("ON") : TEXT("OFF"));
}
