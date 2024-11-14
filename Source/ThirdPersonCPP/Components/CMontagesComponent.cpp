#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{

}


void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!DataTable)
	{
		CLog::Log("DataTable asset is not set!!");
		return;
	}
	
	TArray<FMontageData*> ReadDatas;
	DataTable->GetAllRows<FMontageData>("", ReadDatas);

	for (int32 i = 0; i < int32(EStateType::Max); i++)
	{
		for (const auto& It : ReadDatas)
		{
			if ((EStateType)i == It->Type)
			{
				Datas[i] = It;
				break;
			}
			
		}
	}

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		if (!Datas[i])
		{
			CLog::Log("[" + FString::FromInt(i) + "] is not set");
			continue;
		}

		CLog::Log(Datas[i]->AnimMontage->GetPathName());
	}
}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackstep()
{
	PlayAnimMontage(EStateType::Backstep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	const FMontageData* Data = Datas[(int32)InType];
	CheckNull(Data);

	OwnerCharacter->PlayAnimMontage(Data->AnimMontage, Data->PlayRate, Data->StartSection);
}


