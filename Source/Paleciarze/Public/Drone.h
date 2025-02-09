#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Drone.generated.h"

UCLASS()
class PALECIARZE_API ADrone : public ACharacter
{
    GENERATED_BODY()

public:
    ADrone(); // Konstruktor

protected:
    virtual void BeginPlay() override;

    // Timer do teleportacji
    FTimerHandle TeleportTimerHandle;
    bool bIsTeleporting = false;

    // Funkcja do teleportowania drona
    void TeleportDrone();

    // Komponent dŸwiêkowy
    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* BackgroundMusic;

    UPROPERTY(VisibleAnywhere, Category = "Audio")
    UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* TeleportSound;

public:
    virtual void Tick(float DeltaTime) override;

    void Seek(const FVector& TargetLocation);
    void Pursue();
};
