# Microprocessor

기본 미션

p1.1 스위치를 누르면 모터가 "정지-느리게-빠르게" 순으로 시계 방향으로 회전,
p2.1 스위치를 누르면 모터가 "정지-느리게-빠르게" 순으로 반시계 방향 회전.
만약 모터가 시계방향으로 회전하고 있을 경우 p2.1 스위치를 누르면 모터가 정지.
반대의 상황도 동일.

추가 미션 1

모터가 시계 방향으로 움직일 때 p4.7의 LED만 켜지고,
반시계 방향 방향으로 움직일 때 p1.0의 LED만 켜진다.
모터가 모두 정지한 경우 LED는 켜지지 않는다.

추가 미션 2

초음파 센서로 측정한 거리 값이 7-세그먼크에 출력되게 하고,
거리 15cm 이하에 물체가 감지되면 무조건 모터가 일시정지된다.

추가 미션 3

가변저항을 위로 돌리면 추가 미션 2의 15cm 감지를 30cm로 올린다.
즉 30cm 이하에 물체가 감지되면 모터와 LED가 멈춘다.

추가 미션 4

키패드로 비밀번호를 입력하면 기본 미션과 추가 미션들이 실행되게 한다.
한 자리 숫자를 입력하고 *을 눌러서 4자리 숫자를 만들게 하고, 
마지막 *을 눌러서 비밀번호가 맞다면 미션들이 실행된다.
입력 중에 #을 누른다면 비밀번호 입력을 처음부터 다시 한다.
비밀번호가 틀린 경우에도 처음부터 다시 비밀번호를 입력한다.