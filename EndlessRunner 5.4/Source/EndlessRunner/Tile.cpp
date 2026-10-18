
#include "Tile.h"

#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "RunCharacter.h"
#include "TimerManager.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = false;

    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(root);

    attachPoint = CreateDefaultSubobject<UArrowComponent>(
        TEXT("AttachPoint")
    );
    attachPoint->SetupAttachment(root);

    exitTrigger = CreateDefaultSubobject<UBoxComponent>(
        TEXT("ExitTrigger")
    );
    exitTrigger->SetupAttachment(root);

    exitTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    exitTrigger->SetCollisionObjectType(ECC_WorldDynamic);
    exitTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    exitTrigger->SetCollisionResponseToChannel(
        ECC_Pawn,
        ECR_Overlap
    );
    exitTrigger->SetGenerateOverlapEvents(true);
}

void ATile::BeginPlay()
{
    Super::BeginPlay();

    exitTrigger->OnComponentBeginOverlap.AddDynamic(
        this,
        &ATile::handleExitOverlap
    );
}

void ATile::handleExitOverlap(
    UPrimitiveComponent* overlappedComponent,
    AActor* otherActor,
    UPrimitiveComponent* otherComponent,
    int32 otherBodyIndex,
    bool fromSweep,
    const FHitResult& sweepResult
)
{
    if (hasExited || !Cast<ARunCharacter>(otherActor))
    {
        return;
    }

    hasExited = true;
    Exited.Broadcast(this);
}

void ATile::destroyAfterDelay(float delay)
{
    GetWorldTimerManager().SetTimer(
        destroyTimerHandle,
        this,
        &ATile::destroyTile,
        delay,
        false
    );
}

void ATile::destroyTile()
{
    Destroy();
}
