#include "AK47.h"

#include "Warfare/Components/GunComponent.h"

AAK47::AAK47()
{
	PrimaryActorTick.bCanEverTick = true;
	GunComponent = CreateDefaultSubobject<UGunComponent>(TEXT("Gun Component"));
	SetReplicates(true);
}

void AAK47::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAK47::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAK47::Fire()
{
}

void AAK47::Reload()
{
}

