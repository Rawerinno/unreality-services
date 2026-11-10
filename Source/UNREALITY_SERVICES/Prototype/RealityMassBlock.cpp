#include "RealityMassBlock.h"

#include "Components/StaticMeshComponent.h"
#include "RealityProperties/RealityPropertyComponent.h"
#include "UObject/ConstructorHelpers.h"

ARealityMassBlock::ARealityMassBlock()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsCube"));
    SetRootComponent(Mesh);
    Mesh->SetSimulatePhysics(true);
    Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
    Mesh->SetNotifyRigidBodyCollision(true);
    Mesh->SetLinearDamping(0.2f);
    Mesh->SetAngularDamping(1.0f);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        Mesh->SetStaticMesh(CubeAsset.Object);
    }

    Properties = CreateDefaultSubobject<URealityPropertyComponent>(TEXT("RealityProperties"));
    Properties->PhysicsBody = Mesh;
    Properties->MinimumMassKg = 1.0f;
    Properties->MaximumMassKg = 80.0f;
}

void ARealityMassBlock::BeginPlay()
{
    Properties->StartingMassKg = InitialMassKg;
    Super::BeginPlay();
}
