int outputPin = 9; // С какой ножки считываем сигнал

int zeroDelay = 200; // Длина посылки "0" в миллисекундах
int oneDelay = 100; // Длина посылки "1" в миллисекундах
int waitDelay = 250; // Длина защитного интервала в миллисекундах
int frequency = 24000; // Частоты сигнала

bool activSession = false; // Флаг активной передачи сообщения
unsigned long currentTime; // Время начала передачи сообщения
int byteCountInSession = 0; // Количество байт в сообщении

void setup()
{
  Serial.begin(115200); // установка скорости для монитора порта
}

void loop()
{
  if (Serial.available() > 0)
  {
    if (!activSession)
    {
      activSession = true; // Поднятие флага активной передачи сообщения
      byteCountInSession = 0; // На начало передачи количество байт в сообщении 
      currentTime = micros(); // Запоминиаем время начала передачи сообщения
    }

    char charData = Serial.read(); // Считываем символ
    
    Serial.print("charData == "); // Выводим символ для пользователя
    Serial.println(charData);

    translateAndTransmit((int)charData); // Передаем символ на передачу
    byteCountInSession++; // Увеличиваем число переданных символов. 1 символ = 1 байту
  }
  else
  {
    if (activSession)
    {
      activSession = false; // Убераем флаг активной передачи сообщения

      unsigned long actualTime = micros() - currentTime; // Считаем время прошедшее с начала передачи сообщения
      double rightTime = actualTime / 1000000.0; // Переводим время в формат секунд для удобного понимания
      
      Serial.print("transmit Time: "); // Выводим пользователю время передачи сообщения
      Serial.print(rightTime);
      Serial.println(" sec");

      Serial.print("transmit speed: "); // Выводим пользователю скорость передачи
      Serial.print(byteCountInSession / rightTime);
      Serial.println(" byte/sec");
    }
  }
}

void translateAndTransmit(int data) // Функция перевода и передачи символа
{
  byte bin[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // Созаем массив посылок
  int i = 7; // Устанавливаем указатель на заполняемую ячейку массива

  while (data) // Пока число больше 0
  {
    if (i < 0) break; // Если указатель вышел за пределы массива - принудительно выходим

    bin[i] = data % 2; // заполняем ячейку массива посылкой "0" или "1"
    i--; // Смещаем указатель
    data /= 2; // Обновляем общее число
  }

  Serial.print("bin == "); // Выводим получившееся бинарное число

  for (int j = 0; j < 8; j++) // По порядку выводим все 8 посылок
  {
    Serial.print(bin[j]); // Выводим саму посылку пользователю

    switch (bin[j]) // Смотрим какая посылка
    {
      case 0:
        tone(outputPin, frequency, zeroDelay); // задаем на ножке тон заданной частоты и длительности
        delay(zeroDelay); // Ждем пока тон отработает
        break;
      case 1:
        tone(outputPin, frequency, oneDelay); // задаем на ножке тон заданной частоты и длительности
        delay(oneDelay); // Ждем пока тон отработает
        break;
    }

    delay(waitDelay); // Ждем время защитного интервала
  }

  Serial.println(""); // Переносим на следующую строку для последующих выводных пользователю данных
}
