#include "CKeyEquipComponent.h"
#include "Global.h"
#include "CBox.h"

UCKeyEquipComponent::UCKeyEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCKeyEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Boxes;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACBox::StaticClass(), Boxes);

	if (Boxes.Num() > 0)
	{
		for (const auto& b : Boxes)
			Cast<ACBox>(b)->OnOpenChest.AddUObject(this, &UCKeyEquipComponent::GetColor);
	}
		

}


void UCKeyEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCKeyEquipComponent::GetColor(FHitResult Hit)
{
	ACBox* HitBox = Cast<ACBox>(Hit.Actor);
	FLinearColor Color = HitBox->GetColor();
	PrintLine();
	if (Color == FLinearColor(50, 0, 0, 0))
		SetRedKey(true);
	else if (Color == FLinearColor(0, 50, 0, 0))
		SetGreenKey(true);
	else if (Color == FLinearColor(0, 0, 50, 0))
		SetBlueKey(true);
	else
		PrintLine(); 
}

void UCKeyEquipComponent::SetRedKey(bool InState)
{
	bRedKey = InState;
	PrintLine();
}

void UCKeyEquipComponent::SetGreenKey(bool InState)
{
	bGreenKey = InState;
	PrintLine();
}

void UCKeyEquipComponent::SetBlueKey(bool InState)
{
	bBlueKey = InState;
	PrintLine();
}

