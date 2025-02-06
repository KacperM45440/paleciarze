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

public:
    UFUNCTION(BlueprintCallable, Category = "Blocker")
    void SetNewMesh(UStaticMesh* NewMesh); // Umo¿liwia zmianê mesha w kodzie / Blueprintach

private:
    UPROPERTY(VisibleAnywhere, Category = "Blocker")
    class UBoxComponent* BlockerCollision;

    UPROPERTY(VisibleAnywhere, Category = "Blocker")
    class UStaticMeshComponent* BlockerMesh;

    FVector StartLocation;
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveDistance = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 1.5f;

    float ElapsedTime;

    void UpdateMaterialScale();
    void UpdateCollisionSize(); // NOWA METODA do automatycznego skalowania kolizji
};