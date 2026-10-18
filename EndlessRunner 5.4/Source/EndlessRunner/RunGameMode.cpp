
#include "RunGameMode.h"
#include "Tile.h"
#include "EngineUtils.h"
#include "Components/ArrowComponent.h"

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
    const FTransform spawnTransform =
        exitedTile->attachPoint->GetComponentTransform();

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
