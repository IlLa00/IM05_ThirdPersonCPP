#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDoor.generated.h"

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

private:

};
