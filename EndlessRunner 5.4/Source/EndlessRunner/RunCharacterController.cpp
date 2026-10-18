
#include "RunCharacterController.h"
#include "RunCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

ARunCharacterController::ARunCharacterController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ARunCharacterController::BeginPlay()
{
    Super::BeginPlay();

    runCharacter = Cast<ARunCharacter>(GetPawn());

    if (runCharacter)
    {
        centerY = runCharacter->GetActorLocation().Y;
    }

    if (ULocalPlayer* localPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem =
            localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (inputMappingContext)
            {
                inputSubsystem->AddMappingContext(inputMappingContext, 0);
            }
        }
    }
}

void ARunCharacterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* enhancedInput =
        Cast<UEnhancedInputComponent>(InputComponent);

    if (!enhancedInput)
    {
        return;
    }

    if (moveLeftAction)
    {
        enhancedInput->BindAction(
            moveLeftAction,
            ETriggerEvent::Started,
            this,
            &ARunCharacterController::moveLeft
        );
    }

    if (moveRightAction)
    {
        enhancedInput->BindAction(
            moveRightAction,
            ETriggerEvent::Started,
            this,
            &ARunCharacterController::moveRight
        );
    }
}

void ARunCharacterController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    if (!runCharacter)
    {
        return;
    }

    // Automatic forward movement.
    runCharacter->AddMovementInput(
        runCharacter->GetActorForwardVector(),
        1.0f
    );

    // Smoothly move toward the selected lane.
    FVector currentLocation = runCharacter->GetActorLocation();

    const float targetY = centerY + (currentLane * laneWidth);

    currentLocation.Y = FMath::FInterpTo(
        currentLocation.Y,
        targetY,
        deltaTime,
        laneChangeSpeed
    );

    runCharacter->SetActorLocation(currentLocation, true);
}

void ARunCharacterController::moveLeft()
{
    currentLane = FMath::Max(currentLane - 1, -1);
}

void ARunCharacterController::moveRight()
{
    currentLane = FMath::Min(currentLane + 1, 1);
}
