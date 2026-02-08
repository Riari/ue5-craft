#include "EquippableItem.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "NiagaraFunctionLibrary.h"
#include "Craft/CraftCharacter.h"
#include "Craft/Abilities/BaseGameplayAbility.h"
#include "Craft/Interfaces/IHittable.h"
#include "Kismet/GameplayStatics.h"

AEquippableItem::AEquippableItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetIsReplicated(true);
	SetRootComponent(StaticMesh);

	bReplicates = true;
	SetReplicateMovement(true);
}

void AEquippableItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AEquippableItem::OnComponentBeginOverlap);
	}
}

void AEquippableItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnActorEndOverlap.RemoveAll(this);
}

void AEquippableItem::OnEquip(ACraftCharacter* Character)
{
	SetActorHiddenInGame(false);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainHandSocketName);

	if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
	{
		
		for (TSubclassOf<UBaseGameplayAbility>& Ability : Abilities)
		{
			FGameplayAbilitySpec Spec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this);
			FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(Spec);
			GrantedAbilities.Add(AbilityHandle);
		}
	}

	Character->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AEquippableItem::OnMontageNotifyBegin);
}

void AEquippableItem::OnUnequip(ACraftCharacter* Character)
{
	SetActorHiddenInGame(true);

	if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
	{
		for (FGameplayAbilitySpecHandle& AbilityHandle : GrantedAbilities)
		{
			ASC->ClearAbility(AbilityHandle);
		}

		GrantedAbilities.Reset();
	}

	Character->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveAll(this);
}

void AEquippableItem::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;

	if (OtherActor->Implements<UHittable>())
	{
		if (IHittable::Execute_CanBeHitWith(OtherActor, this))
		{
			OnHit(OtherActor);
		}
	}

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEquippableItem::OnHit(AActor* OtherActor)
{
	IHittable::Execute_OnHit(OtherActor);

	ACraftCharacter* Character = GetOwner<ACraftCharacter>();
	check(Character);

	FVector HitLocation = StaticMesh->GetComponentLocation();
	FRotator HitRotation = Character ? Character->GetActorRotation() : FRotator::ZeroRotator;
	if (HitParticles || HitSound)
	{
		Multicast_PlayHitEffects(HitLocation, HitRotation);
	}

	if (HitCameraShakeClass && Character)
	{
		if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		{
			PC->ClientStartCameraShake(HitCameraShakeClass, 1.0f);
		}
	}

	UAbilitySystemComponent* OtherASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (OtherASC)
	{
		for (TSubclassOf<UGameplayEffect>& HitEffectClass : HitEffects)
		{
			FGameplayEffectContextHandle EffectContext = OtherASC->MakeEffectContext();
			EffectContext.AddInstigator(Character, this);
			FGameplayEffectSpecHandle SpecHandle = OtherASC->MakeOutgoingSpec(HitEffectClass, 1.0f, EffectContext);

			if (SpecHandle.IsValid())
			{
				UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
				ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), OtherASC);
			}
		}
	}
}

void AEquippableItem::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == SwingStartNotify)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else if (NotifyName == SwingEndNotify)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEquippableItem::Multicast_PlayHitEffects_Implementation(const FVector& Location, const FRotator& Rotation)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location);
	}

	if (HitParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, Location, Rotation);
	}
}

void AEquippableItem::ExecutePrimaryAction()
{
	ACraftCharacter* Character = GetOwner<ACraftCharacter>();
	LastActionMontagePlayLength = Character->PlayActionMontageForItem(this);
}

void AEquippableItem::ExecuteSecondaryAction()
{
	ACraftCharacter* Character = GetOwner<ACraftCharacter>();
	LastActionMontagePlayLength = Character->PlayActionMontageForItem(this);
}
