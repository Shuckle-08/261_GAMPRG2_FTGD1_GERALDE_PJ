
#include "Pickup.h"

#include "Components/StaticMeshComponent.h"
#include "RunCharacter.h"

APickup::APickup()
{
    PrimaryActorTick.bCanEverTick = false;

    staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("StaticMesh")
    );

    SetRootComponent(staticMesh);

    staticMesh->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly
    );

    staticMesh->SetCollisionObjectType(ECC_WorldDynamic);

    staticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

    staticMesh->SetCollisionResponseToChannel(
        ECC_Pawn,
        ECR_Overlap
    );

    staticMesh->SetGenerateOverlapEvents(true);
}

void APickup::BeginPlay()
{
    Super::BeginPlay();

    staticMesh->OnComponentBeginOverlap.AddDynamic(
        this,
        &APickup::handleMeshOverlap
    );
}

void APickup::handleMeshOverlap(
    UPrimitiveComponent* overlappedComponent,
    AActor* otherActor,
    UPrimitiveComponent* otherComponent,
    int32 otherBodyIndex,
    bool fromSweep,
    const FHitResult& sweepResult
)
{
    ARunCharacter* runCharacter = Cast<ARunCharacter>(otherActor);

    if (!runCharacter || runCharacter->isDead)
    {
        return;
    }

    OnGet();

    Destroy();
}
