#include "CActionData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CEquipment.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter)
{
	FTransform TM;

	if (EqipmentClass)
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EqipmentClass, TM, InOwnerCharacter);
		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		Equipment->SetActorLabel(MakeLabel(InOwnerCharacter,"Equipment"));
		Equipment->FinishSpawning(TM);
	}
	
}

FString UCActionData::MakeLabel(ACharacter* InOwnerCharacter, FString InMiddleName)
{
	FString Label;
	Label.Append(InOwnerCharacter->GetActorLabel());
	Label.Append("_");
	Label.Append(InMiddleName);
	Label.Append("_");
	Label.Append(GetName().Replace(TEXT("DA_"), TEXT("")));

	return Label;
}
