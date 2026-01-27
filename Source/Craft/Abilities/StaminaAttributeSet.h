#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "StaminaAttributeSet.generated.h"

UCLASS(BlueprintType)
class CRAFT_API UStaminaAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

public:
	UStaminaAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, StaminaRegen);

protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
};
