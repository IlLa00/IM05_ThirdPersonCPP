#include "CBox.h"
#include "Global.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/BoxComponent.h"

ACBox::ACBox()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &ChestBottomComp, "ChestBottomComp");
	CheckNull(ChestBottomComp);

	UStaticMesh* ChestBottomAsset;
	CHelpers::GetAsset(&ChestBottomAsset, "/Game/Assignment/StaticMesh/SM_ChestBottom");
	CheckNull(ChestBottomAsset);

	CHelpers::CreateSceneComponent(this, &ChestTopComp, "ChestTopComp", ChestBottomComp);
	CheckNull(ChestTopComp);

	UStaticMesh* ChestTopAsset;
	CHelpers::GetAsset(&ChestTopAsset, "/Game/Assignment/StaticMesh/SM_ChestTop");
	CheckNull(ChestTopAsset);

	ChestBottomComp->SetStaticMesh(ChestBottomAsset);
	ChestTopComp->SetStaticMesh(ChestTopAsset);
	
	CHelpers::CreateSceneComponent(this, &BoxComp, "BoxComp", ChestBottomComp);
	CheckNull(BoxComp);
	
	BoxComp->SetRelativeLocation(FVector(40, 0, 0));
	BoxComp->SetRelativeScale3D(FVector(2));
	BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel4);

	Color = FVector(50, 0, 0);
	
}

void ACBox::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(ChestTopComp->GetMaterial(0),nullptr);
	CheckNull(MaterialInstance);

	MaterialInstance->SetVectorParameterValue("SymbolColor", Color);
	ChestTopComp->SetMaterial(0, MaterialInstance);
}

void ACBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

