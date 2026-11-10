# 프로그래밍 원칙

## 개요

소프트웨어를 단순하고 읽기 쉽게 유지하기 위한 대표 원칙을 정리한 문서이다.

## 빠른 요약

| 원칙 | 핵심 |
| --- | --- |
| KISS | 가능한 한 단순하게 만든다. |
| DRY | 같은 지식을 여러 곳에 반복하지 않는다. |
| YAGNI | 지금 필요하지 않은 기능을 미리 만들지 않는다. |
| PIE | 코드가 의도를 표현하게 작성한다. |
| SLAP | 한 함수나 문단 안의 추상화 수준을 맞춘다. |

## KISS

### Keep It Simple, Stupid

간단하게 해 어리석은 놈아
코드를 쓸 때 “단순성”이나 “간결성”을 최대 중요 항목으로 한다.
코드는 자연에 맡겨 수정해 가면 무질서하게 되고 복잡해진다.
그 결과, 읽기 어렵고 수정하기 어려워진다.
코드를 간단하게 유지하는 것으로 수정하기 쉬워 진다.

구체적으로는 프로그래밍 할 때 “동작하기 위해서 가장 단순한 것은 무엇인가”라고 자주 물어보는 것이 필요하다.

## DRY

### Don’t Repeat Yourself.

되풀이하지 마라.
같은 코드를 중복해서 쓰지 말라는 것.
왜냐하면, 코드의 개선이 어려워지기 때문이다.
우선 같은 코드가 여럿 있는 것으로 양적으로 “더 많은” 질적으로 “보다 복잡한 “이 되기 때문에 코드를 읽는 작업이 어려워진다.
또 수정할 때도 하나 고칠 때도 중복해서 쓴 코드에 대해서도 빠짐없이 고쳐야 하니 큰일이다.

구체적으로는 처리의 모음은 “함수화” “모듈화”를 하고 데이터는 이름 있는 정수를 정의하는 것이 중요하다.

## YAGNI

### You Aren’t Going to Need it.

그것은 꼭 필요하게 되지 않는다
코드를 필요 최저한으로 한다.
미리 여러가지 사태에 대비하여 코드를 써도 결국은 이용되지 않는 것이 많다.
그리고 그것에 의해서 코드에 “쓸데없는” 복잡성을 담아 버리게 되는, KISS에 사상에도 역행한다.
즉, 예상이 빗나간 경우 시간이 헛되게 사용하는 것에 더해 방해도 되는 것이다.
그리고 단순 코드가, 범용성이 높다.
구체적으로는 보편성보다는 단순성을 생각하는 것이 중요.
코드는 “지금”필요한 것만 하기.

## PIE

### Program Intently and Expressively

의도를 표현하는 프로그래밍을 해라
코드는 의도를 분명히 표현하게 써야 한다는 것이다.
코드가 “사람”이 읽기 위한 것이며 코드만이 소프트웨어의 동작을 “정확히” “완전히” 알기 위한 단서이다.
따라서 소프트웨어의 동작을 파악하는 데는 쉬운 코드를 쓰고 코드에서 의도를 전하는 수밖에 없다.

구체적으로는 코드를 쓸 때는 “쓰기 쉬움” 보다 “읽기 쉬움”을 중시하는 것.
코드는 “쓰는 것” 보다 “읽는 것”이 더 많은 것이기 때문이다.

## SLAP

### Single Level of Abstraction Principle

추상화 수준의 통일
코드를 쓸 때 높은 수준의 추상화 개념과 낮은 수준의 추상화 개념을 분리하도록 할 것.
왜냐하면, 코드가 수준에 어울리는 함수로 분할됨으로써 “요약성” “열람성”을 동시에 충족시킬 수 있다.
같은 곳에는 같은 추상도의 처리와 같이 코드가 통일됨으로써 코드는 거침 없이 흐르고 이해하기 쉽다.

구체적으로는 함수를 구조화하는 것이 중요하다.
하나의 함수에서 어느 부분에서는 데이터베이스 접속은 저 수준의 처리를 하고 다른 부분에서는 비즈니스 로직의 실행이라는 높은 수준 처리를 하도록 하지 않는 것이 중요하다.

## 출처

- KISS — **원저작물 없음.** Kelly Johnson(Lockheed Skunk Works 수석 엔지니어)이 만든 격언. 1960년 미 해군에서 처음 기록되고 1970년대에 대중화. 정리된 글: Wikipedia, "KISS principle" — https://en.wikipedia.org/wiki/KISS_principle
- DRY — Andrew Hunt · David Thomas, *The Pragmatic Programmer* (1999). 원문: "Every piece of knowledge must have a single, unambiguous, authoritative representation within a system"
- YAGNI — Kent Beck, *Extreme Programming Explained* (1999). 문구 자체는 XP 최초 프로젝트(C3)에서 Beck이 반복해 말한 데서 유래. 정리된 글: Martin Fowler, "Yagni" — https://martinfowler.com/bliki/Yagni.html
- PIE — Venkat Subramaniam · Andy Hunt, *Practices of an Agile Developer* (2006)
- SLAP — 개념은 Kent Beck의 Composed Method 패턴(*Smalltalk Best Practice Patterns*, 1997). SLAP이라는 이름은 Glenn Vanderburg가 붙였고, Neal Ford의 *The Productive Programmer* (2008)와 Robert C. Martin의 *Clean Code* (2008, "함수는 한 가지 추상화 수준")로 널리 알려짐
