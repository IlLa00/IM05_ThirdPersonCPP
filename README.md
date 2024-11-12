# 구현내용
- 보물상자 (CBox)
  - 공개변수로 컬러 지정.
- 문 (CDoor)
  - 공개변수로 컬러 지정
  - 문이 열릴 때 타임라인 선형보간 사용
- 플레이어
  - F키로 보물상자와 상호작용(Sphere Trace)
- 장비 (CKeyEquipment)
  - 무슨 색의 열쇠를 가졌는지.

# Code Detail
## DECLARE_MULTICAST_DELEGATE_OneParam(FOpenChest, FHitResult);
보물상자에 델리게이트를 만들어 보물상자가 열리는 동시에
보물상자(CBox)에 작업한 BlueprintNative 함수인 상자가 열리는 모션(C++)과 위젯관리(BP)를,
장비(CKeyEquipment)에 FHitResult로 어떤 색깔을 가진 보물상자가 열렸는지 알려줌.

## SphereTrace
플레이어는 F키를 누를 때, 상자와 충돌하는지 검사.
if (UKismetSystemLibrary::SphereTraceSingleForObjects
	(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + FVector(1),
		100.f,
		ObjectTypes, <- WorldDynamic만 인식하게 함.
		false,
		Ignores,
		EDrawDebugTrace::ForDuration,
		Hit,
		true
	)
상자와 충돌하면 위에 언급한 델리게이트 호출.

문(CDoor)은 Tick함수에 SphereTrace 실행, <- 플레이어가 문과 근접하자마자 바로 조건검사(색깔에 맞는 키가 있는지?)에 들어가기 위함.
<- 지금와서 생각해보니 트리거를 사용했으면 Tick을 사용하지 않아도 됐을듯.
