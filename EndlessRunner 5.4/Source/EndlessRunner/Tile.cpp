
#include "Tile.h"

#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Obstacle.h"
#include "RunCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Pickup.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = false;

    root = CreateDefaultSubobject<USceneComponent>(
        TEXT("Root")
    );
    SetRootComponent(root);

    attachPoint = CreateDefaultSubobject<UArrowComponent>(
        TEXT("AttachPoint")
    );
    attachPoint->SetupAttachment(root);

    exitTrigger = CreateDefaultSubobject<UBoxComponent>(
        TEXT("ExitTrigger")
    );
    exitTrigger->SetupAttachment(root);

    exitTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    exitTrigger->SetCollisionObjectType(ECC_WorldDynamic);
    exitTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    exitTrigger->SetCollisionResponseToChannel(
        ECC_Pawn,
        ECR_Overlap
    );
    exitTrigger->SetGenerateOverlapEvents(true);

    obstacleSpawnArea = CreateDefaultSubobject<UBoxComponent>(
        TEXT("ObstacleSpawnArea")
    );
    obstacleSpawnArea->SetupAttachment(root);

    obstacleSpawnArea->SetCollisionEnabled(
        ECollisionEnabled::NoCollision
    );

    pickupSpawnArea = CreateDefaultSubobject<UBoxComponent>(
        TEXT("PickupSpawnArea")
    );
    pickupSpawnArea->SetupAttachment(root);

    pickupSpawnArea->SetCollisionEnabled(
        ECollisionEnabled::NoCollision
    );
}

void ATile::BeginPlay()
{
    Super::BeginPlay();

    exitTrigger->OnComponentBeginOverlap.AddDynamic(
        this,
        &ATile::handleExitOverlap
    );

    spawnObstacle();
    spawnPickup();
}

void ATile::handleExitOverlap(
    UPrimitiveComponent* overlappedComponent,
    AActor* otherActor,
    UPrimitiveComponent* otherComponent,
    int32 otherBodyIndex,
    bool fromSweep,
    const FHitResult& sweepResult
)
{
    if (hasExited || !Cast<ARunCharacter>(otherActor))
    {
        return;
    }

    hasExited = true;
    Exited.Broadcast(this);
}

void ATile::spawnObstacle()
{
    UE_LOG(LogTemp, Warning, TEXT("Tile BeginPlay: %s"), *GetName());

    UE_LOG(LogTemp, Warning, TEXT("Obstacle area: %s"),
        obstacleSpawnArea ? TEXT("Valid") : TEXT("Missing"));

    UE_LOG(LogTemp, Warning, TEXT("Obstacle classes: %d"),
        obstacleClasses.Num());

    if (obstacleClasses.IsEmpty() || !obstacleSpawnArea)
    {
        return;
    }

    const int32 randomIndex = FMath::RandRange(
        0,
        obstacleClasses.Num() - 1
    );

    TSubclassOf<AObstacle> obstacleClass =
        obstacleClasses[randomIndex];

    if (!obstacleClass)
    {
        return;
    }

    const FVector spawnAreaCenter =
        obstacleSpawnArea->GetComponentLocation();

    const FVector spawnAreaExtent =
        obstacleSpawnArea->GetScaledBoxExtent();

    const FVector spawnLocation =
        UKismetMathLibrary::RandomPointInBoundingBox(
            spawnAreaCenter,
            spawnAreaExtent
        );

    FActorSpawnParameters spawnParameters;
    spawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AObstacle* spawnedObstacle = GetWorld()->SpawnActor<AObstacle>(
        obstacleClass,
        spawnLocation,
        GetActorRotation(),
        spawnParameters
    );

    if (spawnedObstacle)
    {
        spawnedObstacle->AttachToActor(
            this,
            FAttachmentTransformRules::KeepWorldTransform
        );

        spawnedObstacles.Add(spawnedObstacle);
    }
}

void ATile::spawnPickup()
{
    if (!pickupSpawnArea ||
        pickupClasses.IsEmpty() ||
        FMath::FRand() > pickupSpawnChance)
    {
        return;
    }

    const int32 randomIndex = FMath::RandRange(
        0,
        pickupClasses.Num() - 1
    );

    TSubclassOf<APickup> pickupClass = pickupClasses[randomIndex];

    if (!pickupClass)
    {
        return;
    }

    const FVector spawnLocation =
        UKismetMathLibrary::RandomPointInBoundingBox(
            pickupSpawnArea->GetComponentLocation(),
            pickupSpawnArea->GetScaledBoxExtent()
        );

    FActorSpawnParameters spawnParameters;
    spawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    APickup* spawnedPickup = GetWorld()->SpawnActor<APickup>(
        pickupClass,
        spawnLocation,
        GetActorRotation(),
        spawnParameters
    );

    if (spawnedPickup)
    {
        spawnedPickup->AttachToActor(
            this,
            FAttachmentTransformRules::KeepWorldTransform
        );

        spawnedPickups.Add(spawnedPickup);
    }
}

void ATile::destroySpawnedObstacles()
{
    for (AObstacle* obstacle : spawnedObstacles)
    {
        if (IsValid(obstacle))
        {
            obstacle->Destroy();
        }
    }

    spawnedObstacles.Empty();
}

void ATile::destroySpawnedPickups()
{
    for (APickup* pickup : spawnedPickups)
    {
        if (IsValid(pickup))
        {
            pickup->Destroy();
        }
    }

    spawnedPickups.Empty();
}

void ATile::Destroyed()
{
    destroySpawnedObstacles();
    destroySpawnedPickups();

    Super::Destroyed();
}

void ATile::destroyAfterDelay(float delay)
{
    GetWorldTimerManager().SetTimer(
        destroyTimerHandle,
        this,
        &ATile::destroyTile,
        delay,
        false
    );
}

void ATile::destroyTile()
{
    Destroy();
}
