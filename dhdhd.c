#include <stdio.h>  // Подключаем стандартную библиотеку ввода-вывода

// Прототипы функций
int detectCollision();                   // Определяем столкновения
int shiftSphere();                       // Двигаем сферу
int verifyWinner(int left, int right);   // Проверяем, не победил ли кто-нибудь
int paintPlayground(int leftScr, int rightScr, int leftPadY, int rightPadY,
                    int sX, int sY);     // Рисуем игровое поле

// Глобальные переменные
int sPosX = 40;    // Положение сферы по оси X
int sPosY = 12;    // Положение сферы по оси Y
int sDirX = 1;     // Скорость сферы по оси X
int sDirY = 0;     // Скорость сферы по оси Y
int lPaddleY = 12; // Положение левой ракетки по оси Y
int rPaddleY = 12; // Положение правой ракетки по оси Y
const int lPaddleX = 10; // Положение левой ракетки по оси X (не меняется)
const int rPaddleX = 70; // Положение правой ракетки по оси X (не меняется)
int leftScore = 0;       // Очки левого игрока
int rightScore = 0;      // Очки правого игрока
int gameOver = 0;        // Флаг окончания игры (0 - игра идет, 1 или 2 - конец)
char prevKey = ' ';      // Последняя нажимавшаяся клавиша

int main() {
    paintPlayground(leftScore, rightScore, lPaddleY, rPaddleY, sPosX, sPosY);  // Отрисовываем начальное поле
    
    // Пока никто не победил, ждем действий игроков
    while (gameOver == 0) {
        switch (getchar()) {  
            case ('z'):         // Нажатие на 'z' (сдвиг левой ракетки вниз)
                prevKey = 'z';  // Запоминаем, что нажали 'z'
                lPaddleY++;     // Двигаем ракетку
                shiftSphere();  // Смещаем сферу
                detectCollision(); // Проверяем столкновения
                paintPlayground(leftScore, rightScore, lPaddleY, rPaddleY, sPosX, sPosY);
                break;
                
            case ('a'):         // Нажатие на 'a' (сдвиг левой ракетки вверх)
                prevKey = 'a';  // Запоминаем, что нажали 'a'
                lPaddleY--;     // Двигаем ракетку
                shiftSphere();  // Смещаем сферу
                detectCollision(); // Проверяем столкновения
                paintPlayground(leftScore, rightScore, lPaddleY, rPaddleY, sPosX, sPosY);
                break;
                
            case ('m'):         // Нажатие на 'm' (сдвиг правой ракетки вниз)
                prevKey = 'm';  // Запоминаем, что нажали 'm'
                rPaddleY++;     // Двигаем ракетку
                shiftSphere();  // Смещаем сферу
                detectCollision(); // Проверяем столкновения
                paintPlayground(leftScore, rightScore, lPaddleY, rPaddleY, sPosX, sPosY);
                break;
                
            case ('k'):         // Нажатие на 'k' (сдвиг правой ракетки вверх)
                prevKey = 'k';  // Запоминаем, что нажали 'k'
                rPaddleY--;     // Двигаем ракетку
                shiftSphere();  // Смещаем сферу
                detectCollision(); // Проверяем столкновения
                paintPlayground(leftScore, rightScore, lPaddleY, rPaddleY, sPosX, sPosY);
                break;
                
            case (' '):         // Нажатие на пробел (просто удар по мячу)
                prevKey = ' ';  // Запоминаем, что нажали ' '
                shiftSphere();  // Смещаем сферу
                detectCollision(); // Проверяем столкновения
                paintPlayground(leftScore, rightScore, lPaddleY, rPaddleY, sPosX, sPosY);
                break;
        }
    }
    return 0;  // Завершаем программу
}

// Функция, которая двигает сферу в соответствии с направлением
int shiftSphere() {
    sPosX += sDirX;  // Изменяем координату X
    sPosY += sDirY;  // Изменяем координату Y
    return 0;        // Возвращаем 0 для совместимости
}

// Функция, которая проверяет столкновения со стенами, ракетками и отрабатывает логику счета
int detectCollision() {
    // Столкновения по вертикали (верх/низ)
    if ((sPosY <= 1 && sPosX >= 1 && sPosX <= 78) ||
        (sPosY >= 24 && sPosX >= 1 && sPosX <= 78)) {
        sDirY = -sDirY; // Меняем направление по Y
    }
    
    // Столкновение с левой границей (если мяч вылетел слева - очко правому)
    if (sPosX <= 1 && sPosY >= 1 && sPosY <= 24) {
        rightScore++;   // Увеличиваем счет правого игрока
        sPosX = 40;     // Сбрасываем координаты мяча
        sPosY = 12;
        lPaddleY = 12;  // Сбрасываем позиции ракеток
        rPaddleY = 12;
        sDirX = -1;     // Меняем направление мяча
        sDirY = 0;
        verifyWinner(leftScore, rightScore); // Проверяем, не выиграл ли кто
    }
    
    // Столкновение с правой границей (если мяч вылетел справа - очко левому)
    if (sPosX >= 78 && sPosY >= 1 && sPosY <= 24) {
        leftScore++;    // Увеличиваем счет левого игрока
        sPosX = 40;     // Сбрасываем координаты мяча
        sPosY = 12;
        rPaddleY = 12;  // Сбрасываем позиции ракеток
        lPaddleY = 12;
        sDirX = 1;      // Меняем направление мяча
        sDirY = 0;
        verifyWinner(leftScore, rightScore); // Проверяем, не выиграл ли кто
    }
    
    // Столкновение с левой ракеткой
    if (sPosX == lPaddleX + 1 &&
        (sPosY == lPaddleY || sPosY == lPaddleY + 1 || sPosY == lPaddleY - 1)) {
        sDirX = -sDirX;  // Меняем направление по X
        // Добавляем "скручивание" мяча в зависимости от движения ракетки
        switch (prevKey) {
            case ('a'):
                sDirY = 1;
                break;
            case ('z'):
                sDirY = -1;
                break;
            case ('k'):
                sDirY = 1;
                break;
            case ('m'):
                sDirY = -1;
                break;
        }
    }
    
    // Столкновение с правой ракеткой
    if (sPosX == rPaddleX - 1 &&
        (sPosY == rPaddleY || sPosY == rPaddleY + 1 || sPosY == rPaddleY - 1)) {
        sDirX = -sDirX; // Меняем направление по X
        // Добавляем "скручивание" мяча в зависимости от движения ракетки
        switch (prevKey) {
            case ('a'):
                sDirY = 1;
                break;
            case ('z'):
                sDirY = -1;
                break;
            case ('k'):
                sDirY = 1;
                break;
            case ('m'):
                sDirY = -1;
                break;
        }
    }
    
    // Следим, чтобы ракетки не выходили за границы поля
    if (lPaddleY < 2)   lPaddleY++;
    if (lPaddleY > 23)  lPaddleY--;
    if (rPaddleY < 2)   rPaddleY++;
    if (rPaddleY > 23)  rPaddleY--;
    
    return 0; // Возвращаем 0
}

// Функция, которая проверяет, достиг ли кто-то 21 очка
int verifyWinner(int left, int right) {
    if (left == 21) {     // Если у левого 21 - он победитель
        printf("Левый игрок выиграл!\n");
        gameOver = 1;     // Завершаем игру
    }
    if (right == 21) {    // Если у правого 21 - он победитель
        printf("Правый игрок выиграл!\n");
        gameOver = 2;     // Завершаем игру
    }
    return 0;  // Возвращаем 0
}

// Функция, которая очищает экран и рисует игровое поле с ракетками, мячом, счетом
int paintPlayground(int leftScr, int rightScr, int leftPadY, int rightPadY,
                    int sX, int sY) {
    printf("\033[0d\033[2J");  // Очистка консоли
    // Отображаем счет
    printf("                 *** ПИНГ-ПОНГ ***    |Левый: %d| |Правый: %d|\n", leftScr, rightScr);
    
    // Если игра уже закончена, просто прерываем отрисовку поля
    if (gameOver == 1) {
        printf("Левый игрок празднует победу!\n");
        return 0;  // Завершение функции
    }
    if (gameOver == 2) {
        printf("Правый игрок празднует победу!\n");
        return 0;  // Завершение функции
    }
    
    int printed = 0;   // Флаг, который показывает, печатали ли мы символ
    
    // Пробегаем по всей сетке (высота 25, ширина 80)
    for (int row = 0; row <= 25; row++) {
        for (int col = 0; col <= 80; col++) {
            // Верхняя и нижняя границы
            if (row == 0 || row == 25) {
                printf("-");  // Рисуем горизонтальную линию
                printed = 1;
            }
            
            // Левые и правые границы, если мы не на верхней/нижней строке
            if (((row > 0 && row < 25) && col == 0) ||
                ((row > 0 && row < 25) && col == 80)) {
                printf("|");  // Рисуем вертикальную линию
                printed = 1;
            }
            
            // Отрисовываем саму сферу (мяч)
            if (row == sY && col == sX) {
                printf("*");  // Сама сфера
                printed = 1;
            }
            
            // Линия в центре поля
            if (row >= 1 && row <= 24 && col == 40 &&
               !(row == sY && col == sX)) {
                printf("|");  // Вертикальная разделительная линия
                printed = 1;
            }
            
            // Левая ракетка (три клетки по вертикали)
            if ((row == leftPadY && col == lPaddleX) ||
                (row == leftPadY + 1 && col == lPaddleX) ||
                (row == leftPadY - 1 && col == lPaddleX)) {
                printf("H");  // Условно обозначим ракетку символом 'H'
                printed = 1;
            }
            
            // Правая ракетка (три клетки по вертикали)
            if ((row == rightPadY && col == rPaddleX) ||
                (row == rightPadY + 1 && col == rPaddleX) ||
                (row == rightPadY - 1 && col == rPaddleX)) {
                printf("H");  // Аналогично 'H'
                printed = 1;
            }
            
            // Дополнительный декоративный элемент (просто в середине поля)
            if (row == 12 && col == 20 && !(row == sY && col == sX)) {
                // Если не попадаем в позицию мяча, рисуем небольшую звёздочку
                printf("@");
                printed = 1;
            }
            
            // Если достигли конца строки - перенос
            if (col == 80) {
                printf("\n");
                printed = 1;
            }
            
            // Если мы ничего не нарисовали, ставим пробел
            if (!printed) {
                printf(" ");
            }
            
            printed = 0; // Сбрасываем флаг для следующего символа
        }
    }
    return 0; // Возвращаем 0 по окончании отрисовки
}
