#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBox.generated.h"

class UBoxComponent;

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
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* ChestTopComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* ChestBottomComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UBoxComponent* BoxComp;

	UPROPERTY(EditInstanceOnly, Category = "Color")
		FVector Color;



};
