#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CKeyEquipComponent.generated.h"

class ACBox;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCKeyEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCKeyEquipComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE bool IsRedKey() { return bRedKey; }
	FORCEINLINE bool IsGreenKey() { return bGreenKey; }
	FORCEINLINE bool IsBlueKey() { return bBlueKey; }

	void GetColor(FHitResult Hit);

	void SetRedKey(bool InState);
	void SetGreenKey(bool InState);
	void SetBlueKey(bool InState);

private:
	bool bRedKey;
	bool bGreenKey;
	bool bBlueKey;

	ACBox* Box;
		
};
