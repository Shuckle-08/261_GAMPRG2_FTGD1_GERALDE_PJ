
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class USceneComponent;
class UArrowComponent;
class UBoxComponent;
class ARunCharacter;
class ATile;

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

    UPROPERTY(BlueprintAssignable, Category = "Tile")
    FTileExitedSignature Exited;

    void destroyAfterDelay(float delay);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    bool hasExited = false;

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

    void destroyTile();
};
