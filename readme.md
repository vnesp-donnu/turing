# Эмулятор машины Тьюринга

Программа позволяет эмулировать работу машины Тьюринга

## Компиляция

``` bash
g++ turing.cpp -o turing
```

## Иcходные данные

Исходные данные помещаются в файл `input.txt` в следующем формате

```
[количество состояний (без учета финального) QSize]
[Строка, задающая алфавит]

[Таблица, задающая функцию переходов]

[кол-во тестов NTest]

[данные для теста 1]
[данные для теста 2]
...
[данные для теста NTest]

[Произвольное содержимое -- игнорируется программой]
```

Состояния нумеруются последовательными натуральными числами 0, 1, 2, ..., QSize-1, QSize. При этом состояние 0 считается начальным, а состояние QSize - финальным. Количество состояний не должно превышать 100 (константа `MaxQSize`).

В строке, задающей алфавит, последовательно перечисляются все символы ленточного алфавита. При этом первый символ считается пробельным (blank, lambda) для машины Тьюринга. Пробелы, табуляции и прочие пробельные символы ASCII не могут использоваться в алфавите машины Тьюринга. Длина строки не должна превышать 100 (`константа MaxASize`).

Таблица, задающая функцию переходов, имеет следующий формат:
```
[значение delta для состояния 0 и символа 0] [значение delta для состояния 0 и символа 1] ... [значение delta для состояния 0 и символа ASize-1]
[значение delta для состояния 1 и символа 0] [значение delta для состояния 1 и символа 1] ... [значение delta для состояния 1 и символа ASize-1]
... ... ...
[значение delta для состояния QSize-1 и символа 0] [значение delta для состояния QSize-1 и символа 1] ... [значение delta для состояния QSize-1 и символа ASize-1]
```

Значение delta представляет собой тройку, отдельные элементы которой разделяются запятыми, и имеет вид `[char],[shift],[state]`. Здесь `[char]` - символ, который печатается на ленту вместо наблюдаемого, `[shift]` - один из символов смещения головки (`L` - влево, `R` - вправо, любой другой - не смещать), `[state]` - новое состояние. Любой из элементов тройки может быть опущен. В этом случае предполагается, что символ и/или состояние совпадают с соответствующим аргументов функции перехода. Если же опущен символ смещения головки, предполагается, что головка остается в той же позиции на ленте.

Данные одного теста имеют следующий формат:
```
[левая граница ленты] [правая граница ленты]
[позиция, где размещена строка с исходными данными на ленте] [строка In, задающая исходные данные на ленте]
[начальная позиция головки]
```

Границы задают ту часть ленты, которую может обрабатывать машина Тьюринга для данного теста. При выходе за пределы этого диапазона Машина Тьюринга будет остановлена. Границы быть целыми числами и не должны превышать по модулю 10000 (константа `MaxTapeLen`).

Вся лента изначально заполняется пробельным символом (нулевой символ строки, задающей алфавит), кроме той части ленты, которая определяет исходные данные. С позиции, указанной во второй строке теста, записывается последовательность символов строки In.

## Выходные данные

В файл `output.txt` будут помещены результаты работы заданной МТ для каждого теста (начальное и конечное состояние ленты в указанных границах).
В случае, если `machine.readAndRun` запускается с параметром `debug` равным `true`, на каждом шаге будет выводиться дополнительная информация о текущем содержимом ленты, позиции головки на ленте, текущем состоянии, наблюдаемом символе и соответствующем им значении функции перехода.

## Запуск

``` bash
./turing
```
