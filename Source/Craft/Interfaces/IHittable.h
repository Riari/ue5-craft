#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "IHittable.generated.h"

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
	bool CanBeHitWith(class AEquippableItem* Item) const;

	static bool CanBeHitWith_Default(const UObject* Hittable, const class AEquippableItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hittable")
	void OnHit();
};
