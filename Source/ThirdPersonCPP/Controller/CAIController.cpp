#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/CBehaviorComponent.h"
#include "Characters/CPlayer.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &PerceptionComp, "PerceptionComp");
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "BlackBoard");

	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &BehaviorComp, "BehaviorComp");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.f;
	Sight->LoseSightRadius = 800.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->SetMaxAge(2.f);

	PerceptionComp->ConfigureSense(*Sight);

	TeamID = 1;
	bDrawRange = true;
	AdjustHeight = 64.f;
	BehaivorRange = 150.f;
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedEnemy = Cast<ACEnemy_AI>(InPawn);

	if (PossessedEnemy && ensure(PossessedEnemy->GetBehaviorTree()))
	{
		UseBlackboard(PossessedEnemy->GetBehaviorTree()->GetBlackboardAsset(), Blackboard);
		RunBehaviorTree(PossessedEnemy->GetBehaviorTree());
	}

	SetGenericTeamId(TeamID);
	BehaviorComp->SetBlackboardComponent(Blackboard);

	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	PerceptionComp->OnPerceptionUpdated.Clear();
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(bDrawRange);

	FVector Center = PossessedEnemy->GetActorLocation();
	Center.Z += AdjustHeight;

	DrawDebugCircle(GetWorld(), Center, Sight->SightRadius, 64, FColor::Green, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);

	DrawDebugCircle(GetWorld(), Center, BehaivorRange, 64, FColor::Red, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> PerceivedActors;

	PerceptionComp->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);

	ACPlayer* Player = nullptr;
	for (const auto& Actor : PerceivedActors)
	{
		Player = Cast<ACPlayer>(Actor);

		if (Player)
			break;
	}

	Blackboard->SetValueAsObject("PlayerKey", Player);
}
