#include "ObstacleBlocker.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AObstacleBlocker::AObstacleBlocker()
{
    PrimaryActorTick.bCanEverTick = true;

    BlockerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockerCollision"));
    RootComponent = BlockerCollision;

    BlockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockerMesh"));
    BlockerMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshAsset.Succeeded())
    {
        BlockerMesh->SetStaticMesh(MeshAsset.Object);
    }

    BlockerCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BlockerCollision->SetCollisionObjectType(ECC_WorldStatic);
    BlockerCollision->SetCollisionResponseToAllChannels(ECR_Block);

    ElapsedTime = 0.0f;
}

void AObstacleBlocker::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    TargetLocation = StartLocation + FVector(MoveDistance, 0.0f, 0.0f);

    UpdateMaterialScale();
    UpdateCollisionSize(); // Ustawienie poprawnego rozmiaru Box Collision
}

void AObstacleBlocker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ElapsedTime += DeltaTime * MoveSpeed;

    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, FMath::Sin(ElapsedTime) * 0.5f + 0.5f);
    SetActorLocation(NewLocation);
}

// 🔹 Metoda do zmiany Mesh'a i dopasowania kolizji
void AObstacleBlocker::SetNewMesh(UStaticMesh* NewMesh)
{
    if (NewMesh)
    {
        BlockerMesh->SetStaticMesh(NewMesh);
        UpdateCollisionSize(); // Automatyczne dostosowanie kolizji do nowego Mesh'a
    }
}

// 🔹 Skalowanie kolizji do kształtu Mesh'a
void AObstacleBlocker::UpdateCollisionSize()
{
    if (BlockerMesh && BlockerMesh->GetStaticMesh())
    {
        FVector MeshSize = BlockerMesh->GetStaticMesh()->GetBoundingBox().GetSize(); // Pobiera wymiary Mesh’a
        FVector MeshScale = BlockerMesh->GetComponentScale(); // Pobiera skalę ustawioną w edytorze
        FVector FinalSize = MeshSize * MeshScale * 0.5f; // Dopasowanie Box Collision

        BlockerCollision->SetBoxExtent(FinalSize); // Aktualizacja rozmiaru kolizji
    }
}

// 🔹 Skalowanie tekstury do Mesh'a
void AObstacleBlocker::UpdateMaterialScale()
{
    if (BlockerMesh)
    {
        UMaterialInstanceDynamic* DynMaterial = BlockerMesh->CreateAndSetMaterialInstanceDynamic(0);
        if (DynMaterial)
        {
            FVector Scale = GetActorScale3D();
            DynMaterial->SetVectorParameterValue(FName("ObjectScale"), Scale);
        }
    }
}