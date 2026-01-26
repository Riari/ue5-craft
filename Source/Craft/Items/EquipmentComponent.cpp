#include "EquipmentComponent.h"

#include "EquippableItem.h"
#include "Craft/CraftCharacter.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACraftCharacter>(GetOwner());
}

void UEquipmentComponent::EquipMainHandItem(UItemDefinition* ItemDefinition)
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	
	if (MainHandItem)
	{
		MainHandItem->Destroy();
	}
	
	AEquippableItem* Item = Cast<AEquippableItem>(GetWorld()->SpawnActor(ItemDefinition->EquipmentItemClass, &Location, &Rotation, SpawnInfo));
	if (Item)
	{
		Item->SetDefinition(ItemDefinition);
		Item->Equip(Character);
		MainHandItem = Item;
	}
}

void UEquipmentComponent::UnequipMainHandItem()
{
	if (MainHandItem)
	{
		MainHandItem->Unequip(Character);
		MainHandItem->Destroy();
		MainHandItem = nullptr;
	}
}

void UEquipmentComponent::ExecutePrimaryAction()
{
	MainHandItem->ExecutePrimaryAction();
}

void UEquipmentComponent::ExecuteSecondaryAction()
{
	MainHandItem->ExecuteSecondaryAction();
}
