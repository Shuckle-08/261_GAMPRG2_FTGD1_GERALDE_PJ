
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UStaticMeshComponent;

UCLASS()
class ENDLESSRUNNER_API APickup : public AActor
{
    GENERATED_BODY()

public:
    APickup();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
    UStaticMeshComponent* staticMesh;

    UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
    void OnGet();

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
