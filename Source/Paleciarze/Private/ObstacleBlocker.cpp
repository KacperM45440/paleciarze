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

    // Mo�na doda� mesh z zasob�w UE w Blueprintach
}

// Wywo�ywane przy rozpocz�ciu gry
void AObstacleBlocker::BeginPlay()
{
    Super::BeginPlay();
}

// Wywo�ywane w ka�dej klatce
void AObstacleBlocker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}