#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"

class UAnimMontage;
class ACEquipment;
class ACharacter;

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bUseControlRotation = true;
};

UCLASS()
class THIRDPERSONCPP_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(ACharacter* InOwnerCharacter);

	FORCEINLINE ACEquipment* GetEquipment() { return Equipment; }

private:
	FString MakeLabel(ACharacter* InOwnerCharacter, FString InMiddleName);

public:
	UPROPERTY(EditAnywhere, Category = "Equipment")
		TSubclassOf<ACEquipment> EqipmentClass;

	UPROPERTY(EditAnywhere, Category = "Equipment")
		FLinearColor EquipmentColor;

	UPROPERTY(EditAnywhere, Category = "Equipment")
		FEquipmentData EquipmentData;

private:
	ACEquipment* Equipment;
	
};
