#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class CRAFT_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

	void SetDefinition(UItemDefinition* ItemDefinition);
	UItemDefinition* GetDefinition() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	UItemDefinition* Definition;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
