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
ACharacter* AICharacter;
AActor* PlayerRef;
UCharacterMovementComponent* MovementRef;

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
    Super::BeginPlay();

    PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // Find the AI controller that controls this actor
    AIController = Cast<AAIController>(UGameplayStatics::GetActorOfClass(GetWorld(), AAIController::StaticClass()));
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController not found! Ensure an AIController exists in the level."));
        return;
    }

    // Find the closest AI Pawn to associate with the AIController
    TArray<AActor*> FoundPawns;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundPawns);

    for (AActor* Actor : FoundPawns)
    {
        APawn* PotentialPawn = Cast<APawn>(Actor);
        if (PotentialPawn && AIController->GetPawn() == PotentialPawn)
        {
            AICharacter = Cast<ACharacter>(PotentialPawn);
            break;
        }
    }

    if (!AICharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("AICharacter not found! Make sure the AIController is possessing a Character."));
        return;
    }

    // Get the movement component from the AICharacter
    MovementRef = AICharacter->GetCharacterMovement();
}

void ADrone::Seek(const FVector& TargetLocation)
{
    if (!AIController) return;

    AIController->MoveToLocation(TargetLocation);
    UE_LOG(LogTemp, Warning, TEXT("SEEK"));
}

void ADrone::Pursue()
{
    if (!PlayerRef || !MovementRef || !AICharacter) return;

    FVector TargetDirection = PlayerRef->GetActorLocation() - GetActorLocation();
    float FwdAngle = UKismetMathLibrary::Acos(FVector::DotProduct(GetActorForwardVector(), PlayerRef->GetActorForwardVector())) * (180.f / PI);
    float DirAngle = UKismetMathLibrary::Acos(FVector::DotProduct(GetActorForwardVector(), TargetDirection.GetSafeNormal())) * (180.f / PI);

    UE_LOG(LogTemp, Warning, TEXT("PURSUE 1"));

    float PlayerSpeed = MovementRef->Velocity.Size();
    float AISpeed = AICharacter->GetCharacterMovement()->Velocity.Size();

    if ((DirAngle > 90.f && FwdAngle < 20.f) || PlayerSpeed < 0.01f)
    {
        Seek(PlayerRef->GetActorLocation());
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("PURSUE 2"));

    float PredictedLocation = TargetDirection.Size() / (AISpeed + PlayerSpeed);
    Seek(PlayerRef->GetActorLocation() + PlayerRef->GetActorForwardVector() * PredictedLocation);

    UE_LOG(LogTemp, Warning, TEXT("PURSUING %f"), PredictedLocation);
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Pursue();
}
