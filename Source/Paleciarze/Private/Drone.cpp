// :P

#include "Drone.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>

AAIController* AIController;
AActor* PlayerRef;
UCharacterMovementComponent* MovementRef;

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AIController = Cast<AAIController>(GetComponentByClass(AAIController::StaticClass()));

    if (PlayerRef)
    {
        MovementRef = PlayerRef->FindComponentByClass<UCharacterMovementComponent>();
    }
}

void ADrone::Seek(const FVector& TargetLocation)
{
    if (AIController)
    {
        AIController->MoveToLocation(TargetLocation);
    }
}

void ADrone::Pursue()
{
    if (!PlayerRef || !MovementRef) return;

    FVector TargetDirection = PlayerRef->GetActorLocation() - GetActorLocation();
    float FwdAngle = UKismetMathLibrary::Acos(FVector::DotProduct(GetActorForwardVector(), PlayerRef->GetActorForwardVector())) * (180.f / PI);
    float DirAngle = UKismetMathLibrary::Acos(FVector::DotProduct(GetActorForwardVector(), TargetDirection.GetSafeNormal())) * (180.f / PI);

    if ((DirAngle > 90.f && FwdAngle < 20.f) || (MovementRef->Velocity.X < 0.01f && MovementRef->Velocity.Z < 0.01f))
    {
        Seek(PlayerRef->GetActorLocation());
        return;
    }

    //float PredictedLocation = TargetDirection.Size() / (AIController->GetMovementSpeed() + MovementRef->Velocity); // <-- To trzeba naprawic zeby dzialalo, dolna linijka jest jako placeholder
    float PredictedLocation = TargetDirection.Size() / MovementRef->Velocity.X;
    Seek(PlayerRef->GetActorLocation() + PlayerRef->GetActorForwardVector() * PredictedLocation);
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Pursue();
}

