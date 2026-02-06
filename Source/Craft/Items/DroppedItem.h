#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroppedItem.generated.h"

UCLASS()
class CRAFT_API ADroppedItem : public AActor
{
	GENERATED_BODY()
	
	ADroppedItem();
	
public:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dropped Item", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CollisionCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dropped Item", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dropped Item", meta = (AllowPrivateAccess = true))
	FRotator NiagaraRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dropped Item", meta = (AllowPrivateAccess = true))
	TObjectPtr<class ULevelSequence> LevelSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dropped Item", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UItemDefinition> ItemDefinition;

	UPROPERTY(ReplicatedUsing = OnRep_Item)
	TObjectPtr<class ABaseItem> Item;

	UPROPERTY()
	TObjectPtr<class ULevelSequencePlayer> SequencePlayer;

	UPROPERTY()
	class ALevelSequenceActor* SequenceActor;
	
	class UNiagaraComponent* NiagaraComponent;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SpawnItemFromDefinition();
	
	void ApplyLevelSequence();
	
	void SpawnNiagaraSystem();
	
	UFUNCTION()
	void OnRep_Item();
	
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
