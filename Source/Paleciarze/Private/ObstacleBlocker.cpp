#include "ObstacleBlocker.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
}

void AObstacleBlocker::BeginPlay()
{
    Super::BeginPlay();

    //  Znajdź gracza w świecie gry
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    StartLocation = GetActorLocation();
}

void AObstacleBlocker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Player)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector BlockerLocation = GetActorLocation();

        //  Blocker teraz może być szybszy od gracza
        float NewY = FMath::Lerp(BlockerLocation.Y, PlayerLocation.Y, FollowSpeedMultiplier * DeltaTime);
        SetActorLocation(FVector(BlockerLocation.X, NewY, BlockerLocation.Z));
    }
}


