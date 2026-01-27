#include "HealthAttributeSet.h"

#include "GameplayEffectExtension.h"
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

		const float NewHealth = FMath::Clamp(OldHealth - DamageValue, 0.f, MaxHealthValue);
		SetHealth(NewHealth);

		SetDamage(0.f);
	}
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

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	 
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
}
