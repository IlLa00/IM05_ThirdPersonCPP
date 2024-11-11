#include "CDoor.h"
#include "Global.h"
#include "Materials/MaterialInstanceDynamic.h"

ACDoor::ACDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &DoorFrameComp, "DoorFrameComp");
	CheckNull(DoorFrameComp);

	UStaticMesh* FrameMeshAsset;
	CHelpers::GetAsset(&FrameMeshAsset, "/Game/Assignment/StaticMesh/SM_DoorFrame");
	CheckNull(FrameMeshAsset);

	DoorFrameComp->SetStaticMesh(FrameMeshAsset);

	CHelpers::CreateSceneComponent(this, &DoorMeshComp, "DoorMeshComp", DoorFrameComp);
	CheckNull(DoorMeshComp);

	UStaticMesh* DoorMeshAsset;
	CHelpers::GetAsset(&DoorMeshAsset, "/Game/Assignment/StaticMesh/SM_Door");
	CheckNull(DoorMeshAsset);

	DoorMeshComp->SetStaticMesh(DoorMeshAsset);
	DoorMeshComp->SetRelativeLocation(FVector(0, 45, 0));
}

void ACDoor::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* FrameMaterialInstance = UMaterialInstanceDynamic::Create(DoorFrameComp->GetMaterial(0), nullptr);
	CheckNull(FrameMaterialInstance);

	FrameMaterialInstance->SetVectorParameterValue("BaseColor", Color);
	DoorFrameComp->SetMaterial(0, FrameMaterialInstance);

	UMaterialInstanceDynamic* DoorMaterialInstance = UMaterialInstanceDynamic::Create(DoorMeshComp->GetMaterial(0), nullptr);
	CheckNull(DoorMaterialInstance);

	DoorMaterialInstance->SetVectorParameterValue("BaseColor", Color);
	DoorMeshComp->SetMaterial(0, DoorMaterialInstance);
}

void ACDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACDoor::OpenDoor_Implementation()
{

}

