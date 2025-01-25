// :P

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Drone.generated.h"

UCLASS()
class PALECIARZE_API ADrone : public AActor
{
	GENERATED_BODY()
	
	
	public:	
		// Sets default values for this actor's properties
		ADrone();
		
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	public:	
		// Called every frame
		void Seek(const FVector& TargetLocation);
		void Pursue();
		virtual void Tick(float DeltaTime) override;
};
