#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleBlocker.generated.h"

UCLASS()
class PALECIARZE_API AObstacleBlocker : public AActor
{
    GENERATED_BODY()

public:
    AObstacleBlocker();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Blocker")
    class UBoxComponent* BlockerCollision;

    UPROPERTY(VisibleAnywhere, Category = "Blocker")
    class UStaticMeshComponent* BlockerMesh;

    //  Referencja do gracza
    AActor* Player;

    //  Wsp�czynnik pr�dko�ci pod��ania (mniejsza pr�dko�� ni� gracz)
    UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.1", ClampMax = "1.0"))
    float FollowSpeedMultiplier = 0.5f; // 50% pr�dko�ci gracza

    FVector StartLocation;
    float ElapsedTime;
};