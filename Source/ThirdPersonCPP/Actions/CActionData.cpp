#include "CActionData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CEquipment.h"
#include "CAttachment.h"
#include "CDoAction.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter)
{
	FTransform TM;

	if (AttachmentClass)
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, TM, InOwnerCharacter);
		Attachment->SetActorLabel(MakeLabel(InOwnerCharacter, "Attachment"));
		Attachment->FinishSpawning(TM);
	}

	if (EqipmentClass)
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EqipmentClass, TM, InOwnerCharacter);
		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		Equipment->SetActorLabel(MakeLabel(InOwnerCharacter,"Equipment"));
		Equipment->FinishSpawning(TM);

		if (Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
			
	}
	
	if (DoActionClass)
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, TM, InOwnerCharacter);
		DoAction->SetDatas(DoActionDatas);
		DoAction->SetActorLabel(MakeLabel(InOwnerCharacter, "DoAction"));
		DoAction->FinishSpawning(TM);
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
