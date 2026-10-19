
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRunCharacterDeathSignature);

UCLASS()
class ENDLESSRUNNER_API ARunCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ARunCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* springArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float forwardSpeed = 800.0f;

    UPROPERTY(BlueprintAssignable, Category = "Character")
    FRunCharacterDeathSignature OnDeath;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    bool isDead = false;

    UFUNCTION(BlueprintCallable, Category = "Character")
    void Die();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
    int32 coins = 0;

    UFUNCTION(BlueprintCallable, Category = "Pickup")
    void addCoin();
};
