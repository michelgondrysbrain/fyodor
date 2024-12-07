
#include <random>
#include <iostream>

// Базовый класс, который определяет общий интерфейс для броска кости.
// В данном случае, по умолчанию метод roll() возвращает 10.
class General_Dice {
public:
  virtual unsigned roll() {
    return 10;
  }
};

// Класс Dice моделирует кость с указанным максимальным значением (max)
// и инициализируется с определённым seed для генератора случайных чисел.
// Метод roll() возвращает случайное число от 1 до max.
class Dice: public General_Dice {
 public:
   Dice(unsigned max, unsigned seed) :
     max(max), dstr(1, max), reng(seed) { }

   // Переопределение метода roll для возвращения случайного значения.
   unsigned roll() override {
     return dstr(reng);
  }

 private:
   unsigned max;  // максимальное значение на гранях кости
   std::uniform_int_distribution<unsigned> dstr;  // распределение от 1 до max
   std::default_random_engine reng;  // генератор случайных чисел
};

class ThreeDicePool : public General_Dice {
public:
    
    ThreeDicePool(General_Dice& dice1, General_Dice& dice2, General_Dice& dice3) 
        : d1(dice1), d2(dice2), d3(dice3) {}

    // Метод roll возвращает сумму бросков трёх кубиков
    unsigned roll() override {
        return d1.roll() + d2.roll() + d3.roll();
    }

private:
    General_Dice& d1; 
    General_Dice& d2; 
    General_Dice& d3;
};

// Класс PenaltyDice реализует логику штрафного броска.
// Для двух бросков возвращается наименьшее из двух значений.
class PenaltyDice : public General_Dice {
 public:
   PenaltyDice(General_Dice& object) : object(object) { }

   // Переопределённый метод roll возвращает минимальное значение из двух бросков.
   unsigned roll() override {
     unsigned roll1 = object.roll();
     unsigned roll2 = object.roll();
     return std::min(roll1, roll2);
   }

 private:
   General_Dice &object;  // ссылка на объект, который выполняет базовый бросок
};

// Класс BonusDice реализует логику бонусного броска.
// Для двух бросков возвращается наибольшее из двух значений.
class BonusDice : public General_Dice {
 public:
   BonusDice(General_Dice& object) : object(object) { }

   // Переопределённый метод roll возвращает максимальное значение из двух бросков.
   unsigned roll() override {
     unsigned roll1 = object.roll();
     unsigned roll2 = object.roll();
     return std::max(roll1, roll2);
   }

 private:
   General_Dice &object;  // ссылка на объект, который выполняет базовый бросок
};

// Класс DoubleDiceNaslednik демонстрирует множественное наследование.
// Он наследует и PenaltyDice, и BonusDice, что позволяет выполнить оба типа бросков.
// Метод roll возвращает сумму результатов PenaltyDice и BonusDice.
class DoubleDiceNaslednik : public BonusDice, public PenaltyDice {
 public:
   DoubleDiceNaslednik(Dice& d) : PenaltyDice(d), BonusDice(d) { }

   // Переопределённый метод roll возвращает сумму бросков для PenaltyDice и BonusDice.
   unsigned roll() override {
     return PenaltyDice::roll() + BonusDice::roll();
   }
};

// Класс DoubleDice реализует схожую логику без множественного наследования.
// В нём используется два объекта: один для бонусного броска, другой для штрафного.
class DoubleDice : public General_Dice {
 public:
   DoubleDice(Dice& d) : bonus(d), penalty(d) { }

   // Метод roll возвращает сумму результатов бонусного и штрафного бросков.
   unsigned roll() override {
     return bonus.roll() + penalty.roll();
   }
 private:
   BonusDice bonus;
   PenaltyDice penalty;
};

// Функция для вычисления математического ожидания случайной величины
// на основе переданного объекта и количества бросков.
double expected_value(General_Dice& tdp, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
      accum += tdp.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}
// Функция для вычисления вероятности выпадения определённого значения.
double value_probability(unsigned value, General_Dice& d, unsigned number_of_rolls = 1) {
   unsigned counter = 0;
   for (int i = 0; i < number_of_rolls; i++) {
     if (d.roll() == value) {
       counter++;
     }
   }
   return static_cast<double>(counter) / static_cast<double>(number_of_rolls);
}

int main() {
   // Создаём объект Dice с диапазоном значений от 1 до 100 и seed 511.
   Dice object(100, 511);

   // Создаём объект DoubleDice, который выполняет и бонусные, и штрафные броски.
   DoubleDice d(object);

   // Выводим вероятность каждого значения от 1 до 200 на основе 1000 бросков.
   for (int i = 1; i < 201; i++) {
     std::cout << i << ' ' << value_probability(i, d, 1000) << ' ';
   }
}
// i love the smiths they are the best band ever existed 
