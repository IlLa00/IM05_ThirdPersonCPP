#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"

void ACDoAction_Melee::PrimaryAction()
{
	Super::PrimaryAction();

	CheckFalse(Datas.Num() > 0);

	if (bCanCombo)
	{
		bCanCombo = false;
		bSuccessCombo = true;

		return;
	}

	CheckFalse(StateComp->IsIdleMode());
	StateComp->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].Montage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();
}

void ACDoAction_Melee::Begin_PrimaryAction()
{
	Super::Begin_PrimaryAction();

	CheckFalse(bSuccessCombo);

	bSuccessCombo = false;

	OwnerCharacter->StopAnimMontage();

	ComboCount++;
	FMath::Clamp(ComboCount, 0, Datas.Num() - 1);

	OwnerCharacter->PlayAnimMontage(Datas[ComboCount].Montage, Datas[ComboCount].PlayRate, Datas[ComboCount].StartSection);

	Datas[ComboCount].bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();
}

void ACDoAction_Melee::End_PrimaryAction()
{
	Super::End_PrimaryAction();

	OwnerCharacter->StopAnimMontage(Datas[ComboCount].Montage);

	ComboCount = 0;
	StateComp->SetIdleMode();
	AttributeComp->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCauser, InOtherCharacter);

	int32 PrevHitted = HittedCharacters.Num();
	HittedCharacters.AddUnique(InOtherCharacter);

	CheckFalse(PrevHitted < HittedCharacters.Num());

	float HitStop = Datas[ComboCount].HitStop;
	if (!FMath::IsNearlyZero(HitStop))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.02f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", HitStop * 0.02f, false);
	}

	UParticleSystem* ImpactEffect = Datas[ComboCount].Effect;
	if (ImpactEffect)
	{
		FTransform TM = Datas[ComboCount].EffectTransform;
		TM.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, TM);
	}

	TSubclassOf<UCameraShake> ShakeClass = Datas[ComboCount].ShakeClass;
	if (ShakeClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if(PC)
			PC->PlayerCameraManager->PlayCameraShake(ShakeClass);
	}
	

	FDamageEvent DamageEvent;
	InOtherCharacter->TakeDamage(Datas[ComboCount].Damage, DamageEvent, InAttacker->GetController(), InCauser);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InCauser, InOtherCharacter);
}

void ACDoAction_Melee::EnableCombo()
{
	bCanCombo = true;
}

void ACDoAction_Melee::DisableCombo()
{
	bCanCombo = false;
}

void ACDoAction_Melee::ClearHittedCharacters()
{
	HittedCharacters.Empty();
}

void ACDoAction_Melee::RestoreGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}