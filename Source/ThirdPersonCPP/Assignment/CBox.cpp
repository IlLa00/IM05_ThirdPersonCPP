#include "CBox.h"
#include "Global.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "CInfoWidget.h"

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

	CHelpers::GetClass(&WidgetClass, "/Game/Assignment/WB_CInfoWidget");
	CheckNull(WidgetClass);


}

void ACBox::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(ChestTopComp->GetMaterial(0), nullptr);
	CheckNull(MaterialInstance);

	MaterialInstance->SetVectorParameterValue("SymbolColor", Color);
	ChestTopComp->SetMaterial(0, MaterialInstance);

	Widget = CreateWidget<UCInfoWidget>(GetWorld(), WidgetClass);
	CheckNull(Widget);

	Widget->AddToViewport();
	Widget->SetVisibility(ESlateVisibility::Hidden);

	OnOpenChest.AddUObject(this, &ACBox::Open);

}

void ACBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> Ignores;

	FHitResult Hit;

	if (!bOpen)
	{
		if (UKismetSystemLibrary::SphereTraceSingleForObjects
		(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + FVector(1),
			150.f,
			ObjectTypes,
			false,
			Ignores,
			EDrawDebugTrace::None,
			Hit,
			true
		))
		{
			FVector2D ScreenPosition;
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(GetActorLocation(), ScreenPosition);
			Widget->SetPositionInViewport(ScreenPosition);

			Widget->SetVisibility(ESlateVisibility::Visible);

		}
		else
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}

void ACBox::OpenChest(FHitResult Hit)
{
	OnOpenChest.Broadcast(Hit);
}

void ACBox::Open_Implementation(FHitResult Hit)
{
	bOpen = true;
	Widget->SetVisibility(ESlateVisibility::Hidden);
	ChestTopComp->SetRelativeRotation(FRotator(90, 0, 0));
}

