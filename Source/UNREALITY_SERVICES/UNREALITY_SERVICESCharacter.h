// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UNREALITY_SERVICESCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UPropertyExtractorComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AUNREALITY_SERVICESCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;
	
public:
    AUNREALITY_SERVICESCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reality|Extractor")
    TObjectPtr<UPropertyExtractorComponent> RealityExtractor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reality|Tool")
    TObjectPtr<UStaticMeshComponent> ToolBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reality|Tool")
    TObjectPtr<UStaticMeshComponent> ToolBarrel;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Reality|Tool")
    TObjectPtr<UStaticMeshComponent> ToolGrip;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reality|Profile")
    FString TechnicianName = TEXT("TECH-001");

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Reality|Profile")
    int32 ExperienceXP = 0;

    UPROPERTY(BlueprintReadOnly, Category="Reality|UI")
    FString StatusMessage;

    UPROPERTY(BlueprintReadOnly, Category="Reality|UI")
    float StatusUntilTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Reality|UI")
    bool bGraphicsMenuOpen = false;

    UPropertyExtractorComponent* GetRealityExtractor() const { return RealityExtractor; }
    FString GetGraphicsDescription() const;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    void ExtractMass();
    void RouteMass();
    void PushTarget();
    void ToggleGraphicsMenu();
    void SetQualityLow();
    void SetQualityMedium();
    void SetQualityHigh();
    void SetQualityEpic();
    void RenderScaleDown();
    void RenderScaleUp();
    void ToggleVSync();
    void ApplyQuality(int32 Quality);
    void ChangeRenderScale(float Delta);
    void ShowStatus(const FString& Message);
    void SpawnTestBlocks();

    float ToolPulse = 0.0f;
    bool bPrototypeInitialized = false;
    float RenderScalePercent = 80.0f;

protected:

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

