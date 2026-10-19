
#include "RunCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunCharacterController.h"

ARunCharacter::ARunCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    springArm = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("SpringArm")
    );
    springArm->SetupAttachment(RootComponent);
    springArm->TargetArmLength = 500.0f;
    springArm->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));

    camera = CreateDefaultSubobject<UCameraComponent>(
        TEXT("Camera")
    );
    camera->SetupAttachment(springArm);

    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = forwardSpeed;
    GetCharacterMovement()->GravityScale = 0.0f;
}

void ARunCharacter::Die()
{
    if (isDead)
    {
        return;
    }

    isDead = true;

    // Stop character movement immediately.
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();

    // Disable the player's movement controls.
    ARunCharacterController* runController =
        Cast<ARunCharacterController>(GetController());

    if (runController)
    {
        runController->disableControls();
    }

    // Hide the character mesh.
    GetMesh()->SetVisibility(false, true);

    // Notify listeners that the character has died.
    OnDeath.Broadcast();
}

void ARunCharacter::addCoin()
{
    coins++;

    UE_LOG(LogTemp, Warning, TEXT("Coins: %d"), coins);
}