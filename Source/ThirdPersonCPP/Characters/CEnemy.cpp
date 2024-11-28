#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/CNameWidget.h"
#include "UI/CHealthWidget.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

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
	
	CHelpers::GetAsset(&DissolveCurve, "/Game/Curves/Curve_Dissolve");

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
	//ActionComp->SetUnarmedMode();

	NameWidgetComp->InitWidget();
	UCNameWidget* NameWidgetObject = Cast<UCNameWidget>(NameWidgetComp->GetUserWidgetObject());
	if (NameWidgetObject)
		NameWidgetObject->SetNameText(GetController()->GetName(), GetName());

	HealthWidgetComp->InitWidget();
	UCHealthWidget* HealthWidgetObject = Cast<UCHealthWidget>(HealthWidgetComp->GetUserWidgetObject());
	if (HealthWidgetObject)
		HealthWidgetObject->Update(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());

	UMaterialInstanceConstant* DissolveMaterialAsset;
	CHelpers::GetAssetDynamic(&DissolveMaterialAsset, "/Game/Materials/MI_Dissolve");
	DissolveMaterial = UMaterialInstanceDynamic::Create(DissolveMaterialAsset, nullptr);

	FOnTimelineFloat DissolveTimelineDelegate;
	DissolveTimelineDelegate.BindUFunction(this, "OnProgressDissolve");
	DissolveTimeline.AddInterpFloat(DissolveCurve, DissolveTimelineDelegate);

	FOnTimelineEvent DissolveTimelineFinish;
	DissolveTimelineFinish.BindUFunction(this, "OnFinishDissolve");
	DissolveTimeline.SetTimelineFinishedFunc(DissolveTimelineFinish);
		
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DissolveTimeline.TickTimeline(DeltaTime);
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
	CheckTrue(StateComp->IsDeadMode());

	if (StateComp->IsHittedMode())
	{
		LogoMaterial->SetScalarParameterValue("bHitted", 1.f);
		LogoMaterial->SetVectorParameterValue("LogoColor", InColor);
	}

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

	SetBodyColor(FLinearColor(100,0,0));
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreBodyColor", 0.5f, false);
}

void ACEnemy::Dead()
{
	NameWidgetComp->SetVisibility(false);
	HealthWidgetComp->SetVisibility(false);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	FVector Start = GetActorLocation();
	FVector Target = DamageInstigator->GetPawn()->GetActorLocation();
	FVector Direction = Start - Target;
	Direction.Normalize();

	GetMesh()->AddImpulseAtLocation(Direction * DamageValue * 3000.f, Start);

	FLinearColor EquipmentColor = FLinearColor::White;
	if (ActionComp->GetCurrentDataAsset())
	{
		EquipmentColor = ActionComp->GetCurrentDataAsset()->EquipmentColor;
	}
	
	DissolveMaterial->SetVectorParameterValue("BaseColor", EquipmentColor);

	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); i++)
	{
		GetMesh()->SetMaterial(i, DissolveMaterial);
	}

	DissolveTimeline.PlayFromStart();
}

void ACEnemy::RestoreBodyColor()
{
	LogoMaterial->SetScalarParameterValue("bHitted", 0.f);

	if (ActionComp->GetCurrentDataAsset())
	{
		FLinearColor EquipmentColor = ActionComp->GetCurrentDataAsset()->EquipmentColor;
		LogoMaterial->SetVectorParameterValue("LogoColor", EquipmentColor);
		BodyMaterial->SetVectorParameterValue("BodyColor", EquipmentColor);
	}

}

void ACEnemy::OnProgressDissolve(float Output)
{
	CheckNull(DissolveMaterial);

	DissolveMaterial->SetScalarParameterValue("Amount", Output);
}

void ACEnemy::OnFinishDissolve()
{
	Destroy();
}
