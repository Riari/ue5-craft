#pragma once

#include "CoreMinimal.h"
#include "Craft/Items/BaseItem.h"
#include "UObject/Interface.h"
#include "IHarvestable.generated.h"

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
	TSubclassOf<ABaseItem> GetResourceItemActorClass() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Harvestable")
	int32 GetResourceQuantityPerHit() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Harvestable")
	void OnHarvest(AActor* Harvester);
};
