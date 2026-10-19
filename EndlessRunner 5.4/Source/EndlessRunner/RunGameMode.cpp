
#include "RunGameMode.h"

#include "Tile.h"
#include "RunCharacter.h"
#include "EngineUtils.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

ARunGameMode::ARunGameMode()
{
}

void ARunGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Find the first tile already placed in the level.
    for (TActorIterator<ATile> iterator(GetWorld()); iterator; ++iterator)
    {
        bindTile(*iterator);
        break;
    }

    // Bind to the player's death event.
    ARunCharacter* runCharacter = Cast<ARunCharacter>(
        UGameplayStatics::GetPlayerCharacter(this, 0)
    );

    if (runCharacter)
    {
        runCharacter->OnDeath.AddDynamic(
            this,
            &ARunGameMode::handleCharacterDeath
        );
    }
}

void ARunGameMode::bindTile(ATile* tile)
{
    if (tile)
    {
        tile->Exited.AddDynamic(
            this,
            &ARunGameMode::handleTileExited
        );
    }
}

void ARunGameMode::handleTileExited(ATile* exitedTile)
{
    if (!exitedTile || !tileClass)
    {
        return;
    }

    // Spawn the next tile at the previous tile's attach point.
    const FVector spawnLocation =
        exitedTile->attachPoint->GetComponentLocation();

    const FRotator spawnRotation =
        exitedTile->attachPoint->GetComponentRotation();

    const FTransform spawnTransform(
        spawnRotation,
        spawnLocation,
        FVector::OneVector
    );

    FActorSpawnParameters spawnParameters;
    spawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const FVector rootLocation = exitedTile->GetActorLocation();
    const FVector attachLocation = exitedTile->attachPoint->GetComponentLocation();

    UE_LOG(LogTemp, Warning, TEXT("Old tile root: %s"), *rootLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Attach point: %s"), *attachLocation.ToString());

    ATile* newTile = GetWorld()->SpawnActor<ATile>(
        tileClass,
        spawnTransform,
        spawnParameters
    );

    if (newTile)
    {
        bindTile(newTile);
    }

    // Remove the old tile after a delay.
    exitedTile->destroyAfterDelay(tileDestroyDelay);
}

void ARunGameMode::handleCharacterDeath()
{
    if (isRestarting)
    {
        return;
    }

    isRestarting = true;

    GetWorldTimerManager().SetTimer(
        restartTimerHandle,
        this,
        &ARunGameMode::restartCurrentLevel,
        2.0f,
        false
    );
}

void ARunGameMode::restartCurrentLevel()
{
    const FString currentLevelName =
        UGameplayStatics::GetCurrentLevelName(this, true);

    UGameplayStatics::OpenLevel(
        this,
        FName(*currentLevelName)
    );
}
