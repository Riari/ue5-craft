// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimNotifiable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimNotifiable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAFT_API IAnimNotifiable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AnimNotify(FName NotifyName) = 0;
};
