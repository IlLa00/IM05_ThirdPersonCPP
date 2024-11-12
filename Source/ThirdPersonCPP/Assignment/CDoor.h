#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDoor.generated.h"

class UCDoorWidget;

UCLASS()
class THIRDPERSONCPP_API ACDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	bool CheckColor(FHitResult Hit);

public:
	UFUNCTION(BlueprintNativeEvent)
		void OpenDoor();
public:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UStaticMeshComponent* DoorFrameComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UStaticMeshComponent* DoorMeshComp;

	UPROPERTY(EditInstanceOnly, Category = "Color")
		FLinearColor Color;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<UCDoorWidget> WidgetClass;

private:
	UCDoorWidget* Widget;
};
