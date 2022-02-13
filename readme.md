# Эмулятор машины Тьюринга

Программа позволяет эмулировать работу машины Тьюринга

## Компиляция

``` bash
g++ turing.cpp -o turing
```

## Иcходные данные

Исходные данные помещаются в текстовый файл в следующем формате (имя файла задается при запуске эмулятора в командной строке)

``` text
[количество состояний (без учета финального) QSize]
[Строка, задающая алфавит]

[Таблица, задающая функцию переходов]

[кол-во тестов NTest]

[данные и ожидаемый результат для теста 1]
[данные и ожидаемый результат для теста 2]
...
[данные и ожидаемый результат для теста NTest]

[Произвольное содержимое -- игнорируется программой]
```

Состояния нумеруются последовательными натуральными числами 0, 1, 2, ..., QSize-1, QSize. При этом состояние 0 считается начальным, а состояние QSize - финальным. Количество состояний не должно превышать 100 (константа `MaxQSize`).

В строке, задающей алфавит, последовательно перечисляются все символы ленточного алфавита. При этом первый символ считается пробельным (blank, lambda) для машины Тьюринга. Пробелы, табуляции и прочие пробельные символы ASCII не могут использоваться в алфавите машины Тьюринга. Длина строки не должна превышать 100 (`константа MaxASize`).

Таблица, задающая функцию переходов, имеет следующий формат:

``` text
[значение delta для состояния 0 и символа 0] [значение delta для состояния 0 и символа 1] ... [значение delta для состояния 0 и символа ASize-1]
[значение delta для состояния 1 и символа 0] [значение delta для состояния 1 и символа 1] ... [значение delta для состояния 1 и символа ASize-1]
... ... ...
[значение delta для состояния QSize-1 и символа 0] [значение delta для состояния QSize-1 и символа 1] ... [значение delta для состояния QSize-1 и символа ASize-1]
```

Значение delta представляет собой тройку, отдельные элементы которой разделяются запятыми, и имеет вид `[char],[shift],[state]`. Здесь `[char]` - символ, который печатается на ленту вместо наблюдаемого, `[shift]` - один из символов смещения головки (`L` - влево, `R` - вправо, любой другой - не смещать), `[state]` - новое состояние (допускается, что перед номером состояния может быть записан символ q). Любой из элементов тройки может быть опущен. В этом случае предполагается, что символ и/или состояние совпадают с соответствующим аргументов функции перехода. Если же опущен символ смещения головки, предполагается, что головка остается в той же позиции на ленте.

Вместо значения delta для какого-то состояния и символа может быть записано `-`, если гарантируется, что машина Тьюринга никогда в данном состоянии не обнаружит на соответствующий символ в наблюдаемой ячейке ленты. Если это все же произойдет, программа выдаст ошибку `Invalid state`.

Описание данных и ожидаемого результата для одного теста имеет следующий формат:

``` text
[левая граница ленты] [правая граница ленты]
[позиция, где размещена строка с исходными данными на ленте] [строка In, задающая исходные данные на ленте]
[начальная позиция головки]

[позиция головки относительно начала проверяемого фрагмента] [проверяемый фрагмент]
```

Границы задают ту часть ленты, которую может обрабатывать машина Тьюринга для данного теста. При выходе за пределы этого диапазона Машина Тьюринга будет остановлена. Границы быть целыми числами и не должны превышать по модулю 10000 (константа `MaxTapeLen`).

Вся лента изначально заполняется пробельным символом (нулевой символ строки, задающей алфавит), кроме той части ленты, которая определяет исходные данные. С позиции, указанной во второй строке теста, записывается последовательность символов строки In.

При проверке результата сравнивается реальное содержимое ленты с заданным фрагментом с учетом положения головки. При совпадении получившегося результата с ожидаемым выдается сообщение `OK!`, при несовпадении хотя бы одного символа -- `Wrong answer`.

## Выходные данные

В текстовый файл (имя определяется вторым параметром командной строки при запуске) будут помещены результаты работы заданной МТ для каждого теста (начальное и конечное состояние ленты в указанных границах) и результат проверки совпадения с ожидаемым фрагментом.
В случае, если `machine.readAndRun` запускается с параметром `debug` равным `true`, на каждом шаге будет выводиться дополнительная информация о текущем содержимом ленты, позиции головки на ленте, текущем состоянии, наблюдаемом символе и соответствующем им значении функции перехода.

## Запуск

Программа имеет два обязательных параметра командной строки и один опциональный:

``` bash
./turing <input_file> <output_file> [-d]
```

где `<input_file>` - имя входного файла (например, `input.txt`), `<output_file>` - имя входного файла (например, `output.txt`). В случае указания третьим параметром `-d` в выходной файл будет выводиться также дополнительная отладочная информация.
