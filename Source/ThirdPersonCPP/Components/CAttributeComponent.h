#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

UENUM(BlueprintType)
enum class EWalkSpeedType : uint8
{
	Sneak,Walk,Sprint,Max
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	FORCEINLINE float GetSneakSpeed() { return WalkSpeeds[(int32)EWalkSpeedType::Sneak]; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeeds[(int32)EWalkSpeedType::Walk]; }
	FORCEINLINE float GetSprintSpeed() { return  WalkSpeeds[(int32)EWalkSpeedType::Sprint]; }

	void SetSpeed(EWalkSpeedType InType);

	FORCEINLINE bool IsCanMove() { return bCanMove; }

	void SetMove();
	void SetStop();

	void DecreaseHealth(float InAmount);
	void IncreaseHealth(float InAmount);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
		float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
		float WalkSpeeds[(int32)EWalkSpeedType::Max];

	

private:
	float CurrentHealth;
	bool bCanMove;
		
};
