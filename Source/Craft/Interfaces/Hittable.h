#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHittable : public UInterface
{
	GENERATED_BODY()
};

class CRAFT_API IHittable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hittable")
	FGameplayTagQuery GetValidItemTagQuery() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hittable")
	int32 GetMinimumValidItemGrade() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hittable")
	void OnHit();
};
