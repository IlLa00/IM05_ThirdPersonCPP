#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CKeyIconWidget.generated.h"

class UImage;

UCLASS()
class THIRDPERSONCPP_API UCKeyIconWidget : public UUserWidget
{
	GENERATED_BODY()	

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetRedKey();

	UFUNCTION(BlueprintImplementableEvent)
		void SetGreenKey();

	UFUNCTION(BlueprintImplementableEvent)
		void SetBlueKey();
};
