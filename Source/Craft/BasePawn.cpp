#include "BasePawn.h"

#include "AbilitySystemComponent.h"
#include "Abilities/HealthAttributeSet.h"

ABasePawn::ABasePawn()
	: AbilitySystemComponent{CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"))}
	, HealthAttributeSet{CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"))}
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ABasePawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABasePawn::InitializeEffects()
{
	if (!AbilitySystemComponent) return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

