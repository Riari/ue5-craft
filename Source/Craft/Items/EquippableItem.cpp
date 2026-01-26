#include "EquippableItem.h"

#include "AbilitySystemComponent.h"
#include "Craft/CraftCharacter.h"
#include "Craft/Abilities/BaseGameplayAbility.h"

AEquippableItem::AEquippableItem()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(StaticMeshComponent);
}

void AEquippableItem::Equip(ACraftCharacter* CraftCharacter)
{
	if (UAbilitySystemComponent* ASC = CraftCharacter->GetAbilitySystemComponent())
	{
		for (TSubclassOf<UBaseGameplayAbility>& Ability : Abilities)
		{
			FGameplayAbilitySpec Spec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this);
			FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(Spec);
			GrantedAbilities.Add(AbilityHandle);
		}
	}

	StaticMeshComponent->AttachToComponent(CraftCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Socket_HandR"));
	Character = CraftCharacter;
}

void AEquippableItem::Unequip(ACraftCharacter* CraftCharacter)
{
	if (UAbilitySystemComponent* ASC = CraftCharacter->GetAbilitySystemComponent())
	{
		for (FGameplayAbilitySpecHandle& AbilityHandle : GrantedAbilities)
		{
			ASC->ClearAbility(AbilityHandle);
		}

		GrantedAbilities.Reset();
	}

	Character = nullptr;
}
