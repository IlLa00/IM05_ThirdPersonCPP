#include "CAnimNotify_BeginPrimaryAction.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionObject.h"
#include "Actions/CDoAction.h"

FString UCAnimNotify_BeginPrimaryAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}

void UCAnimNotify_BeginPrimaryAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UCActionComponent* ActionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(ActionComp);

	UCActionObject* ActionData = ActionComp->GetCurrentDataObject();
	CheckNull(ActionData);

	ACDoAction* DoAction = ActionData->GetDoAction();
	CheckNull(DoAction);

	DoAction->Begin_PrimaryAction();
}