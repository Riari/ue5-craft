#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "PlayerAttributeSet.generated.h"

UCLASS()
class CRAFT_API UPlayerAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, StaminaRegen);

protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
};
