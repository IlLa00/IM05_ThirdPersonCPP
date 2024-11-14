# 구현내용
- 보물상자 (CBox)
  - 공개변수로 컬러 지정.
  - 상자와 캐릭터와 가까워지면 위젯 띄움.
  - 상자가 열릴 시, 멀티캐스트 델리게이트 실행.
    - DECLARE_MULTICAST_DELEGATE_OneParam(FOpenChest, FHitResult);
    - 장비(CEquipment)에 어떤색깔의 상자가 열렸는지 알려줌 + 상자 열리는 모션(BlueprintNative) + 파티클 스폰(BP)
- 문 (CDoor)
  - 공개변수로 컬러 지정.
  - Tick함수로 플레이어가 근처에오고 조건에 맞으면 문이 자동으로 열림. (SphereTrace)
  - 조건에 맞지 않으면(=색깔에 맞는 키가 없으면) 위젯띄움.
- 플레이어
  - F키로 보물상자와 상호작용(Sphere Trace)
- 장비 (CKeyEquipment)
  - ActorComponent 상속, 플레이어가 가짐.
  - 무슨 색의 열쇠를 가졌는지.
  
