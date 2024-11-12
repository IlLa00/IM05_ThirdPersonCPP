#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBox.generated.h"

class UBoxComponent;
class UCInfoWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOpenChest, FHitResult);

UCLASS()
class THIRDPERSONCPP_API ACBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE FLinearColor GetColor() { return Color; }

public:
	UFUNCTION(BlueprintNativeEvent)
		void Open(FHitResult Hit);

	UFUNCTION()
		void OpenChest(FHitResult Hit);

public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* ChestTopComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* ChestBottomComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UBoxComponent* BoxComp;

	UPROPERTY(EditInstanceOnly, Category = "Color")
		FLinearColor Color;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<UCInfoWidget> WidgetClass;

public:
	FOpenChest OnOpenChest;

private:
	UCInfoWidget* Widget;
	bool bOpen;
};
