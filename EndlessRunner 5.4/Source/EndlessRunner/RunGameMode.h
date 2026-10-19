
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "RunGameMode.generated.h"

class ATile;

UCLASS()
class ENDLESSRUNNER_API ARunGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ARunGameMode();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course")
    TSubclassOf<ATile> tileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course")
    float tileDestroyDelay = 2.0f;

private:
    UFUNCTION()
    void handleTileExited(ATile* exitedTile);

    UFUNCTION()
    void handleCharacterDeath();

    void bindTile(ATile* tile);
    void restartCurrentLevel();

    FTimerHandle restartTimerHandle;

    bool isRestarting = false;
};
