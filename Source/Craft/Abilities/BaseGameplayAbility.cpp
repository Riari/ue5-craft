#include "BaseGameplayAbility.h"

#include "Craft/CraftCharacter.h"

bool UBaseGameplayAbility::IsInputPressed() const
{
	const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	return Spec && Spec->InputPressed;
}

UEquipmentComponent* UBaseGameplayAbility::GetEquipmentComponent()
{
	if (!EquipmentComponent)
	{
		ACraftCharacter* Character = Cast<ACraftCharacter>(GetAvatarActorFromActorInfo());
		if (Character)
		{
			EquipmentComponent = Character->GetEquipmentComponent();
		}
	}

	return EquipmentComponent;
}
