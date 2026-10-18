
#include "RunCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ARunCharacter::ARunCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    springArm->SetupAttachment(RootComponent);
    springArm->TargetArmLength = 500.0f;
    springArm->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));

    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    camera->SetupAttachment(springArm);

    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = forwardSpeed;
    GetCharacterMovement()->GravityScale = 0.0f;
}
