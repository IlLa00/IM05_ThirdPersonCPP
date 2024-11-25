#include "CGameMode.h"
#include "Global.h"
#include "GameFramework/HUD.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass(&PlayerControllerClass, "/Game/Player/BP_CPlayerConroller");
	CHelpers::GetClass(&HUDClass, "/Game/Game/BP_CHUD");
}

