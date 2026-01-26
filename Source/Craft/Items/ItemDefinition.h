#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common = 0,
	Uncommon = 1,
	Rare = 2,
	Legendary = 3,
};

UCLASS(BlueprintType)
class CRAFT_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int MaxStackSize{1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool IsEquippable{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "IsEquippable"))
	TSubclassOf<class AEquippableItem> EquipmentItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool IsConsumable{false};
};
