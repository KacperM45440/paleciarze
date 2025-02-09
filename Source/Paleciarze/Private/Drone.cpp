#include "Drone.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/AIModule/Classes/AIController.h"

AAIController* AIController;
ACharacter* AICharacter;
AActor* PlayerRef;
UCharacterMovementComponent* MovementRef;

ADrone::ADrone()
{
    PrimaryActorTick.bCanEverTick = true;
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->bAutoActivate = false;
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();

    PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    AIController = Cast<AAIController>(UGameplayStatics::GetActorOfClass(GetWorld(), AAIController::StaticClass()));
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController not found! Ensure an AIController exists in the level."));
        return;
    }

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

    MovementRef = AICharacter->GetCharacterMovement();

    if (BackgroundMusic && AudioComponent)
    {
        AudioComponent->SetSound(BackgroundMusic);
        AudioComponent->Play();
    }
}

void ADrone::TeleportDrone()
{
    FVector NewLocation(180.f, 0.f, 310.f);
    SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
    bIsTeleporting = false;
}

void ADrone::Seek(const FVector& TargetLocation)
{
    if (!AIController) return;
    AIController->MoveToLocation(TargetLocation);
}

void ADrone::Pursue()
{
    if (!PlayerRef || !MovementRef || !AICharacter) return;

    FVector TargetDirection = PlayerRef->GetActorLocation() - GetActorLocation();
    float FwdAngle = UKismetMathLibrary::Acos(FVector::DotProduct(GetActorForwardVector(), PlayerRef->GetActorForwardVector())) * (180.f / PI);
    float DirAngle = UKismetMathLibrary::Acos(FVector::DotProduct(GetActorForwardVector(), TargetDirection.GetSafeNormal())) * (180.f / PI);
    float PlayerSpeed = MovementRef->Velocity.Size();
    float AISpeed = AICharacter->GetCharacterMovement()->Velocity.Size();

    if ((DirAngle > 90.f && FwdAngle < 20.f) || PlayerSpeed < 0.01f)
    {
        Seek(PlayerRef->GetActorLocation());
        return;
    }

    float PredictedLocation = TargetDirection.Size() / (AISpeed + PlayerSpeed);
    Seek(PlayerRef->GetActorLocation() + PlayerRef->GetActorForwardVector() * PredictedLocation);
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerRef) return;

    float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerRef->GetActorLocation());

    if (DistanceToPlayer < 150.0f && !bIsTeleporting)
    {
        UFunction* DeathFunction = PlayerRef->FindFunction(FName("Death"));
        PlayerRef->ProcessEvent(DeathFunction, nullptr);
        bIsTeleporting = true;
        GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &ADrone::TeleportDrone, 1.0f, false);
    }
    else if (DistanceToPlayer >= 150.0f)
    {
        bIsTeleporting = false;
        Pursue();
    }
}
