
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

class UStaticMeshComponent;

UCLASS()
class ENDLESSRUNNER_API AObstacle : public AActor
{
    GENERATED_BODY()

public:
    AObstacle();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
    UStaticMeshComponent* staticMesh;

    UFUNCTION(BlueprintImplementableEvent, Category = "Obstacle")
    
    void OnTrigger();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void handleMeshOverlap(
        UPrimitiveComponent* overlappedComponent,
        AActor* otherActor,
        UPrimitiveComponent* otherComponent,
        int32 otherBodyIndex,
        bool fromSweep,
        const FHitResult& sweepResult
    );
};
