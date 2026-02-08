#include "HealthAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Craft/CraftCharacter.h"
#include "Craft/Interfaces/IHarvestable.h"
#include "Net/UnrealNetwork.h"

UHealthAttributeSet::UHealthAttributeSet()
	: Health{100.f}
	, MaxHealth{100.f}
	, HealthRegen{5.f}
{
}

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	 
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageValue = GetDamage();
		const float OldHealth = GetHealth();
		const float MaxHealthValue = GetMaxHealth();

		// TODO: Not the best place to put this
		AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator();
		AActor* Owner = GetOwningActor();
		if (Owner->Implements<UHarvestable>())
		{
			IHarvestable::Execute_OnHarvest(Owner, Instigator);
		}

		const float NewHealth = FMath::Clamp(OldHealth - DamageValue, 0.f, MaxHealthValue);
		SetHealth(NewHealth);

		SetDamage(0.f);
	}
}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAttributeSet, Health);
	DOREPLIFETIME(UHealthAttributeSet, MaxHealth);
	DOREPLIFETIME(UHealthAttributeSet, HealthRegen);
	DOREPLIFETIME(UHealthAttributeSet, Damage);
}

float UHealthAttributeSet::GetPercentage()
{
	return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
}

void UHealthAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldHealth);
}
