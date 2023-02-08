#include "WarfareCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/GunComponent.h"
#include "Interfaces/GunInterface.h"
#include "Warfare/Components/GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/PlayerHUD.h"

AWarfareCharacter::AWarfareCharacter()
{
	SetReplicates(true);
	bReplicates = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh());
	FollowCamera->bUsePawnControlRotation = false;

	FPSArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS Arms"));
	FPSArms->SetupAttachment(FollowCamera);
	FPSArms->SetRelativeLocation({47.0f, 0.0f, -139.0f});
	FPSArms->SetRelativeRotation({0.0f, 30.0f, 0.0f});
	FPSArms->SetOnlyOwnerSee(true);
}

void AWarfareCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (!PlayerHUD && PlayerHUDPrefab)
	{
		if (APlayerController* controller = Cast<APlayerController>(Controller))
		{
			UE_LOG(LogTemp, Warning, TEXT("Make Player HUD") );	
			PlayerHUD = CreateWidget<UPlayerHUD>(controller, PlayerHUDPrefab);
			PlayerHUD->AddToViewport();
		}
	}
}

void AWarfareCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	InteractRaycast();
}

void AWarfareCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWarfareCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWarfareCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AWarfareCharacter::TryInteract);
	}

}

void AWarfareCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWarfareCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWarfareCharacter::TryEquip(AActor* _weapon)
{
	if (!EquipedWeapon && FPSArms)
	{
		if (IGunInterface* gun = Cast<IGunInterface>(_weapon))
		{
			EquipedWeapon = _weapon;
			EquipedWeapon->SetReplicates(true);
			EquipedWeapon->AttachToComponent(FPSArms, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Rifle_Socket"));
		}
	}
}

void AWarfareCharacter::Server_TryInteract_Implementation(AActor* _hitResult)
{
	if (_hitResult)
	{
		TryEquip(_hitResult);
		Multi_TryInteract(_hitResult);
	}
		
}

void AWarfareCharacter::TryInteract()
{
	if (LastHitResult.GetActor())
	{
		if (auto* gun = Cast<IGunInterface>(LastHitResult.GetActor()))
		{
			if (!HasAuthority())
			{
				Server_TryInteract(LastHitResult.GetActor());
			}
			else
			{
				TryEquip(LastHitResult.GetActor());
			}
		}
	}
	
	
}

void AWarfareCharacter::Multi_TryInteract_Implementation(AActor* _hitResult)
{
	TryEquip(_hitResult);
}

void AWarfareCharacter::EquipedWeapon_OnRep()
{
}

void AWarfareCharacter::InteractRaycast()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetInteractText();
		if (GetWorld()->LineTraceSingleByChannel(LastHitResult, FollowCamera->GetComponentLocation(), FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 500, ECC_Visibility))
		{
			if (auto* gun = Cast<IGunInterface>(LastHitResult.GetActor()))
			{
				PlayerHUD->SetInteractText("Pickup Gun");
			}
		}
	}
}

void AWarfareCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( AWarfareCharacter, EquipedWeapon );
}




