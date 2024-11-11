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

	Box = Cast<ACBox>(UGameplayStatics::GetActorOfClass(GetWorld(), Box->StaticClass()));
	CheckNull(Box);

	Box->OnOpenChest.AddUObject(this, &UCKeyEquipComponent::GetColor);	

}


void UCKeyEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCKeyEquipComponent::GetColor(FHitResult Hit)
{
	ACBox* HitBox = Cast<ACBox>(Hit.Actor);
	FVector Color = HitBox->GetColor();

	if (Color == FVector(50, 0, 0))
		SetRedKey(true);
	else if (Color == FVector(0, 50, 0))
		SetGreenKey(true);
	else if (Color == FVector(0, 0, 50))
		SetBlueKey(true);
	else
		PrintLine(); // 여기찍히는건 초록상자의 정보를 못가져오기 때문.

	
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

