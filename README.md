# Клеточные автоматы

### Старт

1. Перед первым запуском приложения установите OpenGL. Для этого введите: «make install» (Нужны sudoers-права и предустановленные «gcc», «make»)
2. В том случае, если возникли какие-то проблемы в 1 пункте, то введите «make install2». Возможно поможет...
3. Для компиляции игры введите: «make»
4. Если надо и скомпилировать и запустить, то введите: «make run»
5. Для тестировани введите: «make test»

## Пролог (история и правила)

&#8195;В 1970 году британский математик Джон Конвей такую игру. Допустим у нас есть поле, состоящие из клеточек. Эти клеточки мы можем либо закрашивать, либо стирать. При этом у каждой клетки есть 8 соседних клеток, которые её окружают, и для каждой клетки мы подсчитываем количество закрашенных соседних клеток, ровно также, как в игре «Сапёр» цифры показывают количество окружающих клетку мин.

&#8195;Дальше мы действуем по следующим правилам: если клетка ещё не закрашена и у неё есть ровно 3 закрашенных соседа, то её мы тоже закрашиваем. А если клетка уже закрашена, то и её мы тоже закрашиваем, если у неё есть 2 или 3 закрашенных соседа. В остальных случаях мы её стираем. У этих правил есть даже специальное название: B3 / S23, что означает, что клетка рождается, если у неё ровно 3 соседа, а выживает, если у неё ровно 2 или 3 соседа.

&#8195;Про эти клетки говорят, что они рождаются, выживают, умирают, по тому, что их часто сравнивают с биологическими клетками или какими-нибудь микробами, которые рождаются, если вокруг них есть ещё микробы. Если вокруг микрома других микробов становиться слишком много, то он умирает от перенаселения, а если их слишком мало, то микроб умирает от одиночества. Из-за некоторой схожестью с биологической жизнью, эта игра получила название Игра «Жизнь», правда это не совсем игра, ведь мы играть в неё не можем. Всё то, что мы можем - это воссоздать изначальное положение клеточек и посмотреть, как они будут развиваться. Иногда её ещё называют игрой для 0 игроков.

&#8195;Есть целые сообщества, которые занимаются поиском новых фигур в Игре «Жизнь». Особый интерес представляют космически коробли. И эти космические корабли, т.е. фигуры, которые движутся постоянно в какую-то сторону, не просто так называют. Многие из них действительно похоже на какие-то космические корабли. При этом их нельзя просто так взять и нарисовать. Это очень сложная задача - придумать новую фигуру, которая будет работать по этим правилам. Это примерно, как решить очень сложное судоку. И каждый такой корабль, каждая такая фигура - это очень точный механизм. Если мы уберём хотябы одну клетку, то всё сразу же развалится.

&#8195;Помимо устойчивых фигур, осцилляторов, космических кораблей, интерес ещё представляют так называемые ружья, т.е. фигуры похожие на осцилляторы, но при этом они постоянно создают космически корабли. Также есть ещё пароходы и пожиратели. Пароходы за собой создают некоторые устойчивые фигуры, а пожиратели наоборот - съедают фигуры на своём пути.

&#8195;И это не всё, в Игре «Жизнь» даже сделали машину Тьюринга, т.е. полноценный компьютер. Получается, что мы можем в Игре «Жизнь» построить компьютер, в который мы можем загрузить программу в виде нулей и единиц, т.е. последовательностей глайдеров и получить какой-то ответ. Т.е. фактически мы можем подсчитать любой алгоритм в Игре «Жизнь». Т.е. мы можем в Игре «Жизнь» можем просимулировать другую Игру «Жизнь» (метавселенная), или даже возможно всю вселенную мы можем запустить, как симуляцию внутри Игры «Жизнь». По этому Игру «Жизнь» называют Тьюринг Полной, что означает, что с помощью неё мы можем подсчитать что угодно. Но всё-таки самой интересной особенностью Игры «Жизнь» является то, что из таких простых правил следует такое сложное поведение.

&#8195;Просто из-за того, чтобы определённым образом раскрашиваем клеточки, получаются все эти осцелляторы, космические корабли, даже компьютеры. Такие вот штуки с клеточками называются клеточными автоматами. Игра «Жизнь» является определённом самым популярным клеточным автоматом, но далеко не единственным. Существует ещё огромное множество других клеточных автоматов.

&#8195;Даже Джон Конвей придумал игру не на пустом месте. Он вдохнавлялся работами Джона Фон-Неймана, Истанислова Улома, которые во время работы на Манхеттанском проекте, в свободное от создания ядерной бомбы время, эспериментировали с клеточными автоматами, так что здесь реализована не только Игра «Жизнь» по правилам Конвея, а ещё и другие клеточные автоматы, в том числе, и с другим количеством измерений.

&#8195;Жизнеподобные клеточные автоматы - клеточные автоматы, которые работают по правилам, похожим на правила Игры «Жизнь».

&#8195;Тьюринговые болота - вычисления на некоторых клеточных автоматов, со свойством Тьюринг Полная, произвести хоть и можно, но это слишком сложно, по тому, что сконструировать в них какой-то компьютер или хотябы калькулятор очень-очень трудно.

&#8195;Сады Эдема - комбинация, которая не могла никак появиться, если мы её изначально сами задали в каком-либо клеточном автомате.

## Задачи

НЕ ФАКТ, ЧТО МЫ УСПЕЕМ СДЕЛАТЬ ВСЁЁЁЁЁЁЁЁЁЁ

- [ ] не выполнненая задача
- [x] выполненная задача
- 1D клеточные автоматы:
    - [ ] Сам автомат:
        - Правило 110 (01101110). Тьюринг Полная O_o
        - Правило  30 (00011110). Самое лучшее правило для генерации псевдослучайных чисел.
        - Правило  90 (01011010). Из одной клетки получаем фрактал «Треугольник Серпинского».
- 2D клеточные автоматы:
    - [ ] Игра «Жизнь»:
        - Соседство Мура (8 соседних клеток):
            - Правило Конвея: B3 / S23
            - Клеточный автомат «Жизнь Бессмертия», делающий из одной клетки кораллы: B3 / S012345678
            - Если раскидать закрашенные клетки случайно, что получится что-то типо шумов Перлина: B5678 / S45678
            - Квадратный фрактал, если начать с одной клетки: B1 / S012345678
        - Соседство фон Неймана (4 соседних клеток):
            - Ромбовый фрактал, если начать с одной клетки: B1 / S012345678
        - [ ] Ввести поддержку .rle формата файлов
    - [ ] Соседство Марголиса (чередуем квадратики 2x2 по диагонале):
        - Правила задаются массивами 16 числел от 0 до 15. Из каждого квадрата 2x2 находим число битовым путём.
        - Первые 2 квадрата сверху составляют биты 1 и 2, а следующие 4 и 8. Это число будет индексом массивом, что даёт следующий вариант квадрата
            - 0,2,8,3,1,5,6,7,4,9,10,11,12,13,14,15: Single Rotate
            - 0,8,4,3,2,5,9,7,1,6,10,11,12,13,14,15: Billiard Ball Machine
            - 0,8,4,3,2,5,9,14,1,6,10,13,12,11,7,15: Bounce gas
            - 0,8,4,12,2,10,9,14,1,6,5,13,3,11,7,15: HPP Gas
            - 0,2,8,12,1,10,9,11,4,6,5,14,3,7,13,15: Rotations
            - 0,2,8,12,1,10,9,13,4,6,5,7,3,14,11,15: Rotations II
            - 0,4,1,10,8,3,9,11,2,6,12,14,5,7,13,15: Rotations III
            - 0,4,1,12,8,10,6,14,2,9,5,13,3,11,7,15: Rotations IV
            - 0,1,2,12,4,10,9,7,8,6,5,11,3,13,14,15: String Thing
            - 0,1,2,12,4,10,6,7,8,9,5,11,3,13,14,15: String Thing II
            - 0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15: Swap On Diag
            - 15,14,13,3,11,5,6,1,7,9,10,2,12,4,8,0: Critters
            - 15,1,2,3,4,5,6,7,8,9,10,11,12,13,14,0: Tron
            - 0,2,8,3,1,5,6,13,4,9,10,7,12,14,11,15: Double Rotate
    - [ ] Реакция Белоусова Жаботинского:
        - 3 цвета, соседство Мура. Если определённую клетку окружают 3 или больше клеток следующего цвета, то она закрашивается в этот самый цвет
    - [ ] Wire world (проволочный мир):
        - 4 цвета:
            1. Пустые клетки
            2. Сами провода
            3. Клетка сигнала. Появляется, если есть рядом 1 или 2 сигнала, а сама клетка - провод.
            4. Хвост сигнала. На следующий ход всегда клетки сигнала превращаются в хвост, а потом на следующий - в клетку провода.
    - [ ] Муравей:
        - По клеткам ходит один единственный муравей. Всего 2 вида клеток (но можно сделать больше)
        - На каждом шаге муравей:
            1. Закрашивает клетку под собой на противоположную
            2. Идёт вперёд
            3. Если муравей наступает на белую клетку, то он поворачивается на 90 градусов по часовой стрелке, а если на чёрную, то против часовой.
        - [ ] Добавить систему шаблонов и настройку числа цветов с поворотами с них
        - [ ] Добавить функцию обратной перемотки, сделав автомат обратимым
- 3D клеточные автоматы:
    - [ ] Муравей:
        - По клеткам ходит один единственный муравей. Всего 2 вида клеток (но можно сделать больше)
        - На каждом шаге муравей:
            1. Закрашивает клетку под собой на противоположную
            2. Идёт вперёд
            3. В зависимости от цвета, муравей может повернуть либо вверх, либо влево, либо вправо, либо вниз. По факту, на каждую из 6 сторон есть по 4 состояния.
        - [ ] Добавить систему шаблонов и настройку числа цветов с поворотами с них
        - [ ] Добавить функцию обратной перемотки, сделав автомат обратимым
    - [ ] Игра «Жизнь»:
        - 3D вариации соседства Мура (26 соседей), фон Неймана (6 соседей) и средний вариант (18 соседей)
            - B5 / S45