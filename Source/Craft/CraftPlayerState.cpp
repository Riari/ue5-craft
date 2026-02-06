#include "CraftPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Abilities/BaseGameplayAbility.h"
#include "Abilities/HealthAttributeSet.h"
#include "Abilities/StaminaAttributeSet.h"
#include "Items/EquipmentComponent.h"
#include "Items/ItemContainerComponent.h"
#include "Net/UnrealNetwork.h"

ACraftPlayerState::ACraftPlayerState()
	: AbilitySystemComponent{CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"))}
	, StaminaAttributeSet{CreateDefaultSubobject<UStaminaAttributeSet>(TEXT("StaminaAttributeSet"))}
	, HealthAttributeSet{CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"))}
	, InventoryContainer{CreateDefaultSubobject<UItemContainerComponent>(TEXT("InventoryContainer"))}
	, HotbarContainer{CreateDefaultSubobject<UItemContainerComponent>(TEXT("HotbarContainer"))}
	, EquipmentComponent{CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"))}
{
}

UAbilitySystemComponent* ACraftPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACraftPlayerState::Initialize()
{
	InitializeAbilitySystem();
	EquipmentComponent->Initialize();
}

void ACraftPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACraftPlayerState, HealthAttributeSet);
	DOREPLIFETIME(ACraftPlayerState, StaminaAttributeSet);
}

UItemContainerComponent* ACraftPlayerState::GetInventoryContainer() const
{
	return InventoryContainer;
}

UItemContainerComponent* ACraftPlayerState::GetHotbarContainer() const
{
	return HotbarContainer;
}

UEquipmentComponent* ACraftPlayerState::GetEquipmentComponent() const
{
	return EquipmentComponent;
}

void ACraftPlayerState::Client_OnAbilitySystemInitialized_Implementation()
{
	OnAbilitySystemInitialized.Broadcast();
}

void ACraftPlayerState::Client_OnStaminaChanged_Implementation(float OldValue, float NewValue, float Max)
{
	OnClientStaminaChanged.Broadcast(OldValue, NewValue, Max);
}

void ACraftPlayerState::Client_OnHealthChanged_Implementation(float OldValue, float NewValue, float Max)
{
	OnClientHealthChanged.Broadcast(OldValue, NewValue, Max);
}

void ACraftPlayerState::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent) return;

	if (HasAuthority())
	{
		// Abilities
		for (TSubclassOf<UBaseGameplayAbility>& Ability : DefaultAbilities)
		{
			FGameplayAbilitySpec Spec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this);
			AbilitySystemComponent->GiveAbility(Spec);
		}

		// Effects
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		
		UE_LOG(LogTemp, Warning, TEXT("Applying %d default effects to %s"), DefaultEffects.Num(), *GetName());

		for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				
				UE_LOG(LogTemp, Warning, TEXT("Applied effect %s to %s"), *Effect->GetName(), *GetName());
			}
		}
		
		Client_OnAbilitySystemInitialized();
	}

	// Ability delegates
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(StaminaAttributeSet->GetStaminaAttribute()).AddUObject(this, &ThisClass::OnStaminaAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
}

void ACraftPlayerState::OnStaminaAttributeChanged(const FOnAttributeChangeData& Data)
{
	Client_OnStaminaChanged(Data.OldValue, Data.NewValue, StaminaAttributeSet->GetMaxStamina());
}

void ACraftPlayerState::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	Client_OnHealthChanged(Data.OldValue, Data.NewValue, HealthAttributeSet->GetMaxHealth());
}
