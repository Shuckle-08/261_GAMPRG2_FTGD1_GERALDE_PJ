
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class USceneComponent;
class UArrowComponent;
class UBoxComponent;
class AObstacle;
class ATile;
class APickup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FTileExitedSignature,
    ATile*,
    exitedTile
);

UCLASS()
class ENDLESSRUNNER_API ATile : public AActor
{
    GENERATED_BODY()

public:
    ATile();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
    USceneComponent* root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
    UArrowComponent* attachPoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
    UBoxComponent* exitTrigger;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
    UBoxComponent* obstacleSpawnArea;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    TArray<TSubclassOf<AObstacle>> obstacleClasses;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
    UBoxComponent* pickupSpawnArea;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    TArray<TSubclassOf<APickup>> pickupClasses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float pickupSpawnChance = 1.0f;

    UPROPERTY(BlueprintAssignable, Category = "Tile")
    FTileExitedSignature Exited;

    void destroyAfterDelay(float delay);



protected:
    virtual void BeginPlay() override;

    virtual void Destroyed() override;

private:
    UPROPERTY()
    bool hasExited = false;

    UPROPERTY()
    TArray<AObstacle*> spawnedObstacles;

    FTimerHandle destroyTimerHandle;

    UFUNCTION()
    void handleExitOverlap(
        UPrimitiveComponent* overlappedComponent,
        AActor* otherActor,
        UPrimitiveComponent* otherComponent,
        int32 otherBodyIndex,
        bool fromSweep,
        const FHitResult& sweepResult
    );

    void spawnObstacle();
    void destroySpawnedObstacles();
    void destroyTile();

    UPROPERTY()
TArray<APickup*> spawnedPickups;

void spawnPickup();
void destroySpawnedPickups();
};
