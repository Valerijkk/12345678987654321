#include <stdio.h>

// Объявление функции отображения игрового поля
void print(int ball_x, int ball_y, int play1_y, int play2_y);

int main() {
    // Инициализация начальных позиций мяча и ракеток
    int ball_x = 80 / 2;    // Начальная позиция мяча по оси X (центр поля)
    int ball_y = 25 / 2;    // Начальная позиция мяча по оси Y (центр поля)
    int play1_y = 25 / 2;   // Начальная позиция левой ракетки по оси Y
    int play2_y = 25 / 2;   // Начальная позиция правой ракетки по оси Y
    
    // Вызов функции отображения игрового поля
    print(ball_x, ball_y, play1_y, play2_y);
    
    // Команды для очистки экрана и перемещения курсора в начало (закомментированы)
    // printf("\x1b[2J"); // Очистка экрана
    // printf("\x1b[d");   // Перемещение курсора в начало
    
    return 0;
}

// Функция отображения игрового поля
void print(int ball_x, int ball_y, int play1_y, int play2_y){
    for (int y = 0; y <= 25; y++) {           // Проход по строкам игрового поля
        for (int x = 0; x <= 80; x++) {       // Проход по столбцам игрового поля
            if (y == 0 || y == 25)
                printf("#");                   // Рисование верхней и нижней границ поля
            else if (x == 0 || x == 80)
                printf("#");                   // Рисование левой и правой границ поля
            else if (((y >= (play1_y - 1) && y <= (play1_y + 1)) && x == 15) ||
                     ((y >= (play2_y - 1) && y <= (play2_y + 1)) && x == 65))
                printf("|");                   // Рисование ракеток игроков
            else if (x == ball_x && y == ball_y)
                printf("o");                   // Рисование мяча
            else
                printf(" ");                   // Пустые клетки игрового поля
        }
        printf("\n");                           // Переход на новую строку после завершения строки игрового поля
    }
}
