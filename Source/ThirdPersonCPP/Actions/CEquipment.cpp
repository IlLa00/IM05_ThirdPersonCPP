#include "CEquipment.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/CCharacterInterface.h"

ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	AttributeComp = CHelpers::GetComponent<UCAttributeComponent>(OwnerCharacter);
}

void ACEquipment::SetData(FEquipmentData InData)
{
	Data = InData;
}

void ACEquipment::SetColor(FLinearColor InColor)
{
	Color = InColor;
}

void ACEquipment::Equip_Implementation()
{
	StateComp->SetEquipMode();

	Data.bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();

	if (Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate, Data.StartSection);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bUseControlRotation)
	{
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		OwnerCharacter->bUseControllerRotationYaw = true;
	}

	ICCharacterInterface* CharacterInterface = Cast<ICCharacterInterface>(OwnerCharacter);
	CheckNull(CharacterInterface);

	CharacterInterface->SetBodyColor(Color);
}

void ACEquipment::Begin_Equip_Implementation()
{
	OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	bEquipped = true;

	StateComp->SetIdleMode();
	AttributeComp->SetMove();
}

void ACEquipment::Unequip_Implementation()
{
	bEquipped = false;

	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerCharacter->bUseControllerRotationYaw = false;

	OnUnequipmentDelegate.Broadcast();
}


