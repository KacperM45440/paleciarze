#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleBlocker.generated.h"

UCLASS()
class YOURGAME_API AObstacleBlocker : public AActor
{
    GENERATED_BODY()

public:
    // Konstruktor
    AObstacleBlocker();

protected:
    // Wywo³ywane przy rozpoczêciu gry
    virtual void BeginPlay() override;

public:
    // Wywo³ywane w ka¿dej klatce
    virtual void Tick(float DeltaTime) override;

private:
    // Komponent kolizji (blokuje ruch gracza)
    UPROPERTY(VisibleAnywhere, Category = "Blocker")
    class UBoxComponent* BlockerCollision;

    // Komponent wizualny (np. Mesh)
    UPROPERTY(VisibleAnywhere, Category = "Blocker")
    class UStaticMeshComponent* BlockerMesh;
};