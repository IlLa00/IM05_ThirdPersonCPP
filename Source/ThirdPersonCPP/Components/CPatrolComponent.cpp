#include "CPatrolComponent.h"
#include "Global.h"
#include "BTNode/CPatrolPath.h"
#include "Components/SplineComponent.h"

bool UCPatrolComponent::GetMoveto(FVector& OutLocation)
{
	OutLocation = FVector(-1);
	CheckFalseResult(IsPathValid(), false);

	OutLocation = PatrolPath->GetSplineComponent()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckFalse(IsPathValid());

	int32 Count = PatrolPath->GetSplineComponent()->GetNumberOfSplinePoints();

	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;
			return;
		}

		if (PatrolPath->GetSplineComponent()->IsClosedLoop())
		{
			Index = Count - 1;
			return;
		}

		Index = 1;
		bReverse = false;
		return;
	}
	
	if (Index < Count - 1)
	{
		Index++;
		return;
	}

	if (PatrolPath->GetSplineComponent())
	{
		Index = 0;
		return;
	}

	Index = Count - 2;
	bReverse = true;
}
