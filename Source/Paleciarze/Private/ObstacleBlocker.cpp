#include "ObstacleBlocker.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Konstruktor
AObstacleBlocker::AObstacleBlocker()
{
    // Inicjalizacja komponentu kolizji
    BlockerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockerCollision"));
    RootComponent = BlockerCollision;

    // Ustawienie rozmiaru kolizji
    BlockerCollision->SetBoxExtent(FVector(50.f, 50.f, 100.f));

    // Inicjalizacja komponentu Mesh (opcjonalnie)
    BlockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockerMesh"));
    BlockerMesh->SetupAttachment(RootComponent);

    // Mo¿na dodaæ mesh z zasobów UE w Blueprintach
}

// Wywo³ywane przy rozpoczêciu gry
void AObstacleBlocker::BeginPlay()
{
    Super::BeginPlay();
}

// Wywo³ywane w ka¿dej klatce
void AObstacleBlocker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}