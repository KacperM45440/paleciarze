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
// Konstruktor
ADrone::ADrone()
{
    PrimaryActorTick.bCanEverTick = true; // W³¹cza Tick()
}

// Funkcja wywo³ywana po rozpoczêciu gry
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

    // Znalezienie najbli¿szego AI Pawna do powi¹zania z kontrolerem
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

    // Pobranie komponentu ruchu AI
    MovementRef = AICharacter->GetCharacterMovement();
}

// Funkcja teleportuj¹ca drona
void ADrone::TeleportDrone()
{
    FVector NewLocation(180.f, 0.f, 310.f);
    SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
    UE_LOG(LogTemp, Warning, TEXT("Drone teleported to 180,0,310"));

    bIsTeleporting = false; // Reset flagi po teleportacji
}

// Funkcja œledzenia celu
void ADrone::Seek(const FVector& TargetLocation)
{
    if (!AIController) return;

    AIController->MoveToLocation(TargetLocation);
    UE_LOG(LogTemp, Warning, TEXT("SEEK"));
}

// Funkcja œcigania gracza
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

// Funkcja wywo³ywana co klatkê
void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerRef) return;

    float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerRef->GetActorLocation());
    UE_LOG(LogTemp, Warning, TEXT("Distance to player: %f"), DistanceToPlayer);

    if (DistanceToPlayer < 145.0f && !bIsTeleporting)
    {
        UFunction* DeathFunction = PlayerRef->FindFunction(FName("Death"));
        PlayerRef->ProcessEvent(DeathFunction, nullptr);
        bIsTeleporting = true;
        UE_LOG(LogTemp, Warning, TEXT("Teleport in 1 second..."));
        GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &ADrone::TeleportDrone, 1.0f, false);
    }
    else if (DistanceToPlayer >= 145.0f)
    {
        bIsTeleporting = false;
        Pursue();
    }
}
