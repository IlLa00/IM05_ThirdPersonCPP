#include "CAnimNotify_Backstepped.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Backstepped::GetNotifyName_Implementation() const
{
	return "BackStepped";
}

void UCAnimNotify_Backstepped::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(Player);

	Player->End_Backstep();
}
