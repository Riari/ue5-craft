#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemRarity.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

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
	int32 Grade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize{1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class AActor> ActorClass;
};
