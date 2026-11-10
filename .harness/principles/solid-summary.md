# 객체지향 설계 원칙 (SOLID)

> **요약본**이다. 이 저장소에서 설계 판단의 기준으로 쓴다.

| 원칙 | 이름 | 판별 질문 |
|---|---|---|
| SRP | 단일 책임 | 이 클래스·모듈이 **바뀌는 이유가 하나**인가? |
| OCP | 개방 폐쇄 | 기존 코드를 **수정하지 않고** 기능을 확장할 수 있는가? |
| LSP | 리스코프 치환 | 부모 타입을 자식 타입으로 바꿔도 동작이 깨지지 않는가? |
| ISP | 인터페이스 분리 | **쓰지 않는** 메서드에 의존하도록 강제하지 않는가? |
| DIP | 의존성 역전 | 구체 구현이 아니라 **추상화**에 의존하는가? |

---

## SRP — 단일 책임

> There should never be more than one reason for a class to change.

책임을 나눌 때는 책임 간 결합도를 최소로 한다(Low Coupling, High Cohesion).

**위반 신호**
- 한 파일을 서로 다른 이유로 자주 고치게 된다. 무엇이 바뀌면 여기를 고치는지 한 문장으로 못 쓴다.
- **Shotgun surgery**: 반대 상황 — 하나의 책임이 여러 곳에 흩어져, 한 번 고치려면 여러 파일을 같이 고쳐야 한다. 이때는 나누는 게 아니라 **모아야** 한다.

**실천**
- 각 책임은 그 책임에 필요한 데이터와 함수만 갖는다. 다른 책임의 사정을 알지 않는다.
- 나눌 곳을 찾을 때는 "이 파일이 바뀌는 이유"를 열거해 본다. 둘 이상이면 그것이 경계다.

## OCP — 개방 폐쇄

> Software entities should be open for extension, but closed for modification.

확장에는 열려 있고 변경에는 닫혀 있어야 한다. 가능케 하는 수단은 **추상화와 다형성**이다.

**실천**
- 기능을 추가할 때 기존 코드를 고쳐야 한다면 경계가 잘못 그어진 것이다.
- 다만 추상화를 미리 깔지 않는다 — 확장 지점이 실제로 두 번 이상 나타난 뒤에 뽑는다(YAGNI).

## LSP — 리스코프 치환

> Subtypes must be substitutable for their base types.

**하위 타입이 지켜야 할 것**
- 선행조건을 강화하지 않는다 / 후행조건을 약화하지 않는다
- 상위 타입의 불변조건을 유지한다
- 상위 타입이 명시하지 않은 예외를 던지지 않는다
- 상위 타입의 기능을 없애거나 뜻을 바꾸지 않는다

**위반 신호**: 호출부에서 "이 객체가 실제로 어느 파생형인지" 검사해야 동작이 맞는다.

## ISP — 인터페이스 분리

> Clients should not be forced to depend upon interfaces that they do not use.

**위반 신호**: 구현체가 쓰지 않는 메서드를 빈 몸통이나 "지원 안 함"으로 채운다.

**실천**: 인터페이스가 커지면 클라이언트가 실제로 쓰는 단위로 쪼갠다. 인터페이스가 바뀔 때 영향받는 범위가 그 기능을 쓰는 곳으로 한정된다.

## DIP — 의존성 역전

> High-level modules should not depend on low-level modules. Both should depend on abstractions.
> Abstractions should not depend on details. Details should depend on abstractions.

고수준(정책·업무 규칙)이 저수준(파일 입출력·DB·화면)에 직접 의존하지 않게 한다. 고수준·저수준 **모두** 추상화를 향하게 만든다.

**주의**: 고수준/저수준은 상대적이다. 절대적인 계층이 아니라 "누가 누구의 세부사항인가"로 판단한다.

**실천**: 의존 방향이 한쪽으로만 흐르는지 본다. 순환하거나, 상위 모듈이 하위 모듈의 자료구조를 알고 있으면 뒤집을 자리다.
