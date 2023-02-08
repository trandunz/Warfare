#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Warfare/Interfaces/GunInterface.h"
#include "AK47.generated.h"

UCLASS()
class WARFARE_API AAK47 : public AActor, public IGunInterface
{
	GENERATED_BODY()
	
public:	
	AAK47();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Fire() override;
	virtual void Reload() override;

protected:
	UPROPERTY(EditAnywhere)
	class UGunComponent* GunComponent;
};
