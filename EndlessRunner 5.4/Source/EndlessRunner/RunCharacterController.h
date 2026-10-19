
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RunCharacterController.generated.h"

class ARunCharacter;
class UInputAction;
class UInputMappingContext;

UCLASS()
class ENDLESSRUNNER_API ARunCharacterController : public APlayerController
{
    GENERATED_BODY()

public:
    ARunCharacterController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

public:
    virtual void Tick(float deltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* inputMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* moveLeftAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* moveRightAction;

    void disableControls();


private:
    UPROPERTY()
    ARunCharacter* runCharacter;

    int32 currentLane = 0;

    float laneWidth = 300.0f;
    float centerY = 0.0f;
    float laneChangeSpeed = 10.0f;

    void moveLeft();
    void moveRight();

    bool controlsDisabled = false;

};
