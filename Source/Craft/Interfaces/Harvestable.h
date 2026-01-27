#pragma once

#include "CoreMinimal.h"
#include "Craft/Items/ItemType.h"
#include "UObject/Interface.h"
#include "Harvestable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHarvestable : public UInterface
{
	GENERATED_BODY()
};

class CRAFT_API IHarvestable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Harvestable")
	EItemType GetRequiredToolType() const;
};
