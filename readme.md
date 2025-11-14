# Range-query

В репозитории реализована библиотека красно-чёрного дерева. Проверка работы осуществляется программой range_query, формат которой таков:

* На вход поступают ключи (каждый ключ это целое число, все ключи
разные) и запросы (каждый запрос это пара из двух целых чисел, второе
больше первого).
* Нужно для каждого запроса подсчитать в дереве количество ключей, таких,
что все они лежат строго между его левой и правой границами
включительно.
* Вход: k 10 k 20 q 8 31 q 6 9 k 30 k 40 q 15 40.
* Результат: 2 0 3.

## Сборка
```bash
git clone --recurse-submodules git@github.com:71frukt/Red-BlackTree.git
cd Red-BlackTree
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
```


## Запуск

### Исполняемый файл
```bash
❯ build/range_query
```

### e2e тесты
```
❯ python3 tests/run.py --task-dir tests/tasks --key-dir tests/answers --bin ./build/range_query
```