#include "ResourceNode.h"

#include "AbilitySystemComponent.h"
#include "CraftCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/HealthAttributeSet.h"
#include "Kismet/GameplayStatics.h"

AResourceNode::AResourceNode()
	: AbilitySystemComponent{CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"))}
	, HealthAttributeSet{CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"))}
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AResourceNode::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
}

UAbilitySystemComponent* AResourceNode::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AResourceNode::CanBeHitWith_Implementation(AEquippableItem* Item) const
{
	return CanBeHitWith_Default(this, Item);
}

TSubclassOf<ABaseItem> AResourceNode::GetResourceItemActorClass_Implementation() const
{
	return ResourceItemActorClass;
}

int32 AResourceNode::GetResourceQuantityPerHit_Implementation() const
{
	return 1;
}

void AResourceNode::OnHarvest_Implementation(AActor* Harvester)
{
	if (ACraftCharacter* Character = Cast<ACraftCharacter>(Harvester))
	{
		TSubclassOf<ABaseItem> ActorClass = Execute_GetResourceItemActorClass(this);
		int32 Quantity = Execute_GetResourceQuantityPerHit(this);
		Character->TrySpawnItemToInventory(ActorClass, Quantity);
	}
}

void AResourceNode::InitializeAttributes()
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
}

void AResourceNode::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		Multicast_OnDestroy();
	}
}

void AResourceNode::Multicast_OnDestroy_Implementation()
{
	if (DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			DestroySound,
			GetActorLocation()
		);
	}

	if (DestroyParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DestroyParticles,
			GetActorLocation()
		);
	}

	Destroy();
}
