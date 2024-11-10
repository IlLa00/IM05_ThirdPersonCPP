#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass(&PlayerControllerClass, "/Game/Player/BP_CPlayerConroller");

}

void ACGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}
