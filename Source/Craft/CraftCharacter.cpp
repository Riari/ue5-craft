#include "CraftCharacter.h"

#include "AbilitySystemComponent.h"
#include "CraftPlayerState.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Abilities/InputID.h"
#include "Items/EquippableItem.h"
#include "Items/ItemContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ACraftCharacter::ACraftCharacter()
	: ActionMontageComponent{CreateDefaultSubobject<UActionMontageComponent>("ActionMontageComponent")}
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
		GetCharacterMovement()->JumpZVelocity = 700.f;
		GetCharacterMovement()->AirControl = 0.35f;
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
		GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
		GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	}
	
	bReplicates = true;
	SetReplicateMovement(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACraftCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	Initialize();
}

void ACraftCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	Initialize();
}

UAbilitySystemComponent* ACraftCharacter::GetAbilitySystemComponent() const
{
	return GetPlayerState<ACraftPlayerState>()->GetAbilitySystemComponent();
}

UEquipmentComponent* ACraftCharacter::GetEquipmentComponent() const
{
	return GetPlayerState<ACraftPlayerState>()->GetEquipmentComponent();
}

float ACraftCharacter::PlayActionMontageForItem(const TObjectPtr<ABaseItem> Item)
{
	if (!HasAuthority()) return 0.f;

	UItemDefinition* ItemDefinition = Item->GetDefinition();
	if (UAnimMontage* Montage = ActionMontageComponent->GetNextMontageByTag(ItemDefinition->Tags.First()))
	{
		Multicast_PlayMontage(Montage);
		return Montage->GetPlayLength();
	}
	
	return 0.f;
}

void ACraftCharacter::AnimNotify(FName NotifyName)
{
}

bool ACraftCharacter::TrySpawnItemToInventory(TSubclassOf<ABaseItem> ItemClass, int32 Quantity)
{
	ABaseItem* Item = GetWorld()->SpawnActor<ABaseItem>(ItemClass);
	return Item && TryAddItemToInventory(Item, Quantity);
}

void ACraftCharacter::OnItemPickUp(ABaseItem* Item)
{
	Item->SetOwner(this);
	Item->SetActorHiddenInGame(true);
	TryAddItemToInventory(Item);
	Client_OnItemPickUp(Item);
}

void ACraftCharacter::Client_OnItemPickUp_Implementation(ABaseItem* Item)
{
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		ItemPickUpSound,
		GetActorLocation()
	);

	TryAddItemToInventory(Item);
}

bool ACraftCharacter::TryAddItemToInventory(ABaseItem* Item, int32 Quantity)
{
	ACraftPlayerState* PS = GetPlayerState<ACraftPlayerState>();
	if (!PS) return false;

	UItemContainerComponent* HotbarContainer = PS->GetHotbarContainer();
	int HotbarSlotIndex = HotbarContainer->FindFirstUsableSlotIndex(Item->GetDefinition(), Quantity);
	if (HotbarSlotIndex != -1)
	{
		HotbarContainer->AddItem(Item, Quantity, HotbarSlotIndex);
		return true;
	}

	return false;
}

bool ACraftCharacter::CanBeHitWith_Implementation(AEquippableItem* Item) const
{
	return !bIsDead && CanBeHitWith_Default(this, Item);
}

void ACraftCharacter::OnDeath()
{
	bIsDead = true;
	GetCharacterMovement()->DisableMovement();
	DisableInput(GetWorld()->GetFirstPlayerController());
	Multicast_PlayMontage(DeathMontage);
}

void ACraftCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACraftCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACraftCharacter::Look);

		EnhancedInputComponent->BindActionValueLambda(Hotbar01Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(0); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar02Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(1); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar03Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(2); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar04Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(3); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar05Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(4); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar06Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(5); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar07Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(6); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar08Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(7); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar09Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(8); });
		EnhancedInputComponent->BindActionValueLambda(Hotbar10Action, ETriggerEvent::Started, [&](const FInputActionValue& Value) { ActivateHotbar(9); });
		
		EnhancedInputComponent->BindAction(PrimaryActionAction, ETriggerEvent::Triggered, this, &ACraftCharacter::PrimaryAction);
		EnhancedInputComponent->BindAction(SecondaryActionAction, ETriggerEvent::Triggered, this, &ACraftCharacter::SecondaryAction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACraftCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	Value.Get<bool>()
		? AbilitySystemComponent->AbilityLocalInputPressed(InputID)
		: AbilitySystemComponent->AbilityLocalInputReleased(InputID);
}

void ACraftCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACraftCharacter, ReplicatedRemoteRotation);
}

void ACraftCharacter::Initialize()
{
	ACraftPlayerState* PS = GetPlayerState<ACraftPlayerState>();
	if (PS)
	{
		UAbilitySystemComponent* AbilitySystemComponent = Cast<UAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		PS->Initialize();
	}
}

void ACraftCharacter::Move(const FInputActionValue& Value)
{
	if (bIsDead) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (!MovementVector.IsNearlyZero())
		{
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw = Rotation.Yaw;

			if (IsLocallyControlled())
			{
				SetActorRotation(NewRotation);
				Server_SetActorRotation(NewRotation);
			}
		}
	}
}

void ACraftCharacter::Look(const FInputActionValue& Value)
{
	if (bIsDead) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACraftCharacter::PrimaryAction(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::PrimaryAction));
}

void ACraftCharacter::SecondaryAction(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::SecondaryAction));
}

void ACraftCharacter::ActivateHotbar(int32 SlotIndex)
{
	if (!HasAuthority())
		Server_ActivateHotbar(SlotIndex);
	
	ACraftPlayerState* PS = GetPlayerState<ACraftPlayerState>();
	check(PS);

	UItemContainerComponent* HotbarContainer = PS->GetHotbarContainer();
	FSlotActivationResult Result = HotbarContainer->TryActivateSlot(SlotIndex);

	if (Result.Success)
	{
		UEquipmentComponent* EquipmentComponent = PS->GetEquipmentComponent();

		EquipmentComponent->UnequipMainHandItem();

		if (Result.ItemActor)
		{
			EquipmentComponent->TryEquipMainHandItem(Result.ItemActor);
		}
	}
}

void ACraftCharacter::OnRep_RemoteRotation()
{
	if (!IsLocallyControlled())
	{
		SetActorRotation(ReplicatedRemoteRotation);
	}
}

void ACraftCharacter::Server_SetActorRotation_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
	ReplicatedRemoteRotation = NewRotation;
	ForceNetUpdate();
}

void ACraftCharacter::Multicast_PlayMontage_Implementation(UAnimMontage* Montage)
{
	check(Montage);

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ACraftCharacter::Server_ActivateHotbar_Implementation(int32 SlotIndex)
{
	ActivateHotbar(SlotIndex);
}