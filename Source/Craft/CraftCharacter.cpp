#include "CraftCharacter.h"

#include "AbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "InputActionValue.h"
#include "Abilities/BaseGameplayAbility.h"
#include "Abilities/InputID.h"
#include "Abilities/PlayerAttributeSet.h"
#include "Items/EquipmentComponent.h"
#include "Items/ItemContainerComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACraftCharacter

ACraftCharacter::ACraftCharacter()
	: AbilitySystemComponent{CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"))}
	, AttributeSet{CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"))}
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InventoryContainer = CreateDefaultSubobject<UItemContainerComponent>(TEXT("InventoryContainer"));
	HotbarContainer = CreateDefaultSubobject<UItemContainerComponent>(TEXT("HotbarContainer"));

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
}

void ACraftCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAbilities();
	InitializeEffects();
}

void ACraftCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &ACraftCharacter::OnStaminaAttributeChanged);
}

UAbilitySystemComponent* ACraftCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UEquipmentComponent* ACraftCharacter::GetEquipmentComponent() const
{
	return EquipmentComponent;
}

void ACraftCharacter::AnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("PickaxeHit"))
	{
		
	}
}

void ACraftCharacter::OnStaminaAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged(Data.OldValue, Data.NewValue);
}

void ACraftCharacter::InitializeAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent) return;

	for (TSubclassOf<UBaseGameplayAbility>& Ability : DefaultAbilities)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this));
	}
}

void ACraftCharacter::InitializeEffects()
{
	if (!AbilitySystemComponent) return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACraftCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACraftCharacter::Move);

		// Looking
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
	if (!AbilitySystemComponent) return;

	Value.Get<bool>()
		? AbilitySystemComponent->AbilityLocalInputPressed(InputID)
		: AbilitySystemComponent->AbilityLocalInputReleased(InputID);
}

void ACraftCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACraftCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
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
	FSlotActivationResult Result = HotbarContainer->TryActivateSlot(SlotIndex);
	if (Result.Success)
	{
		if (Result.ItemDefinition && Result.ItemDefinition->IsEquippable)
		{
			EquipmentComponent->EquipMainHandItem(Result.ItemDefinition);
		}
		else
		{
			EquipmentComponent->UnequipMainHandItem();
		}
	}
}

