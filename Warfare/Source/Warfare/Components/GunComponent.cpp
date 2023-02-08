#include "GunComponent.h"

UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	Skeletal_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletel Mesh"));
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

