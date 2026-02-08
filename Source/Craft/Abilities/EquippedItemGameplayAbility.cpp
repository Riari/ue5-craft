#include "EquippedItemGameplayAbility.h"

#include "Craft/Items/EquippableItem.h"

void UEquippedItemGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
										 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (CooldownType == EItemAbilityCooldownType::Default)
	{
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
		return;
	}

	if (UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGE->GetClass(), 1);
		if (SpecHandle.IsValid())
		{
			AEquippableItem* Item = Cast<AEquippableItem>(GetCurrentSourceObject());
			check(Item);

			float MontageLength{1.f};
			if (CooldownType == EItemAbilityCooldownType::ActionMontageLength)
			{
				MontageLength = Item->GetLatestActionMontagePlayLength();
			}

			SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(CooldownMagnitudeTagName), MontageLength + MontageCooldownAdjustment);
			SpecHandle.Data->SetStackCount(1);
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		}
	}
}