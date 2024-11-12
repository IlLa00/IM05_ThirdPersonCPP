#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	BodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	LogoMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), nullptr);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	ActionComp->SetUnarmedMode();
}

void ACEnemy::SetBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("LogoColor", InColor);
}
