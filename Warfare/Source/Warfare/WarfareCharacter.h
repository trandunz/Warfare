#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WarfareCharacter.generated.h"


class IGunInterface;
UCLASS(config=Game)
class AWarfareCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AWarfareCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void TryEquip(AActor* _weapon);
	
	void TryInteract();
	UFUNCTION(BlueprintAuthorityOnly, Server, Reliable)
	void Server_TryInteract(AActor* _hitResult);
	void Server_TryInteract_Implementation(AActor* _hitResult);
	UFUNCTION(BlueprintAuthorityOnly, NetMulticast, Reliable)
	void Multi_TryInteract(AActor* _hitResult);
	void Multi_TryInteract_Implementation(AActor* _hitResult);
	UFUNCTION(BlueprintAuthorityOnly)
	void EquipedWeapon_OnRep();
	
	void InteractRaycast();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prefabs, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerHUD> PlayerHUDPrefab;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	FHitResult LastHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	class UPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPSArms;

public:
	UPROPERTY(ReplicatedUsing= EquipedWeapon_OnRep, BlueprintReadOnly)
	AActor* EquipedWeapon;
};
