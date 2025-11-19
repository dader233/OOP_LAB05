
# Лабораторная работа №5
**Палкин Даниил Дмитриевич**

## Требования
-  DynamicListMemoryResource (стратегия 4)
-  Переиспользование памяти в do_deallocate  
-  Очистка в деструкторе
-  Queue с polymorphic_allocator
-  Итератор std::forward_iterator_tag
-  Демонстрация с int и struct

## Сборка
```bash
mkdir build && cd build
cmake .. && make
```

## Запуск
```bash
./main    # Демо
./tests   # Тесты
```

**Вариант 10: Queue + стратегия 4**

