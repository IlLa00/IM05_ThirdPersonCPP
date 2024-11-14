#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/CNameWidget.h"
#include "UI/CHealthWidget.h"

ACEnemy::ACEnemy()
{
	USkeletalMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/Character/Mesh/SK_Mannequin");
	GetMesh()->SetSkeletalMesh(MeshAsset);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> AnimClass;
	CHelpers::GetClass(&AnimClass, "/Game/Enemies/ABP_CEnemy");
	GetMesh()->SetAnimInstanceClass(AnimClass);

	CHelpers::CreateActorComponent(this, &ActionComp, "ActionComp");

	CHelpers::CreateActorComponent(this, &MontagesComp, "MontagesComp");

	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");

	CHelpers::CreateActorComponent(this, &StateComp, "StateComp");

	CHelpers::CreateSceneComponent(this, &NameWidgetComp, "NameWidgetComp",GetMesh());
	CHelpers::CreateSceneComponent(this, &HealthWidgetComp, "HealthWidgetComp", GetMesh());

	TSubclassOf<UCNameWidget> NameWidgetClass;
	CHelpers::GetClass(&NameWidgetClass, "/Game/UI/WB_Name");
	NameWidgetComp->SetWidgetClass(NameWidgetClass);
	NameWidgetComp->SetRelativeLocation(FVector(0, 0, 240));
	NameWidgetComp->SetDrawSize(FVector2D(240, 30));
	NameWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCHealthWidget> HealthWidgetClass;
	CHelpers::GetClass(&HealthWidgetClass, "/Game/UI/WB_Health");
	HealthWidgetComp->SetWidgetClass(HealthWidgetClass);
	HealthWidgetComp->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidgetComp->SetDrawSize(FVector2D(120, 20));
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	LaunchValue = 30.f;

}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	BodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	LogoMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), nullptr);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	StateComp->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
	ActionComp->SetUnarmedMode();

	NameWidgetComp->InitWidget();
	UCNameWidget* NameWidgetObject = Cast<UCNameWidget>(NameWidgetComp->GetUserWidgetObject());
	if (NameWidgetObject)
		NameWidgetObject->SetNameText(GetController()->GetName(), GetName());

	HealthWidgetComp->InitWidget();
	UCHealthWidget* HealthWidgetObject = Cast<UCHealthWidget>(HealthWidgetComp->GetUserWidgetObject());
	if (HealthWidgetObject)
		HealthWidgetObject->Update(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());
		
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageInstigator = EventInstigator;
	DamageValue = ActualDamage;

	AttributeComp->DecreaseHealth(Damage);

	if (AttributeComp->GetCurrentHealth() <= 0.f)
	{
		StateComp->SetDeadMode();
		return ActualDamage;
	}
	StateComp->SetHittedMode();

	return ActualDamage;
}

void ACEnemy::SetBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("LogoColor", InColor);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	
	case EStateType::Hitted:
	{
		Hitted();
		break;
	}
	case EStateType::Dead:
	{
		Dead();
		break;
	}
	default:
		break;
	}
}

void ACEnemy::Hitted()
{
	UCHealthWidget* HealthWidgetObject = Cast<UCHealthWidget>(HealthWidgetComp->GetUserWidgetObject());
	
	if (HealthWidgetObject)
	{
		HealthWidgetObject->Update(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());
	}

	MontagesComp->PlayHitted();

	FVector Start = DamageInstigator->GetPawn()->GetActorLocation();
	FVector Target = GetActorLocation();
	FVector LaunchDirection = (Target - Start).GetSafeNormal();
	LaunchCharacter(LaunchDirection * DamageValue * LaunchValue, true, false);
}

void ACEnemy::Dead()
{
	FString Message = GetName();
	Message.Append(" is dead.");
	CLog::Print(Message, -1, 2, FColor::Red);
}
