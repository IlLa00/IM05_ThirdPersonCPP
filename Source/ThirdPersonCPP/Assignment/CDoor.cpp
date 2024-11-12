#include "CDoor.h"
#include "Global.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CDoorWidget.h"
#include "Characters/CPlayer.h"
#include "CKeyEquipComponent.h"

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

	CHelpers::GetClass(&WidgetClass, "/Game/Assignment/WB_CDoorWidget");
	CheckNull(WidgetClass);
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

	Widget = CreateWidget<UCDoorWidget>(GetWorld(), WidgetClass);
	CheckNull(Widget);

	Widget->AddToViewport();
	Widget->SetVisibility(ESlateVisibility::Hidden);
}

void ACDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> Ignores;

	FHitResult Hit;
	
	if (UKismetSystemLibrary::SphereTraceSingleForObjects
	(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + FVector(1),
		150.f,
		ObjectTypes,
		false,
		Ignores,
		EDrawDebugTrace::ForOneFrame,
		Hit,
		true
	))
	{
		bool Result = CheckColor(Hit);

		if (Result)
		{
			PrintLine();
			OpenDoor();
		}
		else
		{
			FVector2D ScreenPosition;
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(GetActorLocation(), ScreenPosition);
			Widget->SetPositionInViewport(ScreenPosition);

			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}

}

bool ACDoor::CheckColor(FHitResult Hit)
{
	UCKeyEquipComponent* KeyEquip = Cast<UCKeyEquipComponent>((Cast<ACPlayer>(Hit.Actor))->GetComponentByClass(UCKeyEquipComponent::StaticClass()));

	if (Color == FLinearColor(50, 0, 0,0))
	{
		bool Result = KeyEquip->IsRedKey();
		return Result;
	}
	else if (Color == FLinearColor(0, 50, 0,0))
	{
		bool Result = KeyEquip->IsGreenKey();
		return Result;
	}
	else if (Color == FLinearColor(0, 0, 50,0))
	{
		bool Result = KeyEquip->IsBlueKey();
		return Result;
	}

	return false;
}

void ACDoor::OpenDoor_Implementation()
{
	PrintLine();
	DoorMeshComp->SetRelativeRotation(FRotator(0, 100, 0));
}

