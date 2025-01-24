Ниже представлены два варианта реализации игры Pong на языке Си.

Первый — пошаговый режим (исключительно стандартная библиотека, без динамического выделения памяти).
Второй — бонусный вариант, интерактивный (используется библиотека ncurses).
Оба кода пишутся в структурном стиле, с комментариями и пояснениями.
При этом они соответствуют описанию из задания (поле 80x25, ракетки по 3 символа, мяч 1 символ, управление A/Z и K/M, игра до 21 очка).

1. Пошаговый режим (файл src/pong.c)
c
Копировать
/*
 * pong.c — пошаговый вариант классической игры Pong
 * Управление:
 *   A/Z — передвижение левой ракетки вверх/вниз
 *   K/M — передвижение правой ракетки вверх/вниз
 *   SPACE (пробел) — пропуск хода (шаг без изменения позиций ракеток)
 * Игра идёт до 21 очка. При достижении 21 очка одним из игроков —
 * выводится сообщение о победе и игра завершается.
 *
 * Компиляция (пример для Linux/Mac):
 *   gcc pong.c -o pong
 *
 * Запуск:
 *   ./pong
 *
 * При желании можно перенастроить очистку экрана 
 * (system("clear") для Unix-подобных систем, system("cls") для Windows).
 */

#include <stdio.h>   // Для printf, scanf, getchar и т.д.
#include <stdlib.h>  // Для system()
#include <ctype.h>   // Для tolower()

// Размер поля
#define WIDTH 80
#define HEIGHT 25

// Размер ракетки
#define PADDLE_SIZE 3

// Очко, до которого играем
#define WIN_SCORE 21

// Структура, описывающая состояние игры
typedef struct {
    int ballX;       // x-координата мяча
    int ballY;       // y-координата мяча
    int ballDX;      // горизонтальное направление движения мяча (-1 или +1)
    int ballDY;      // вертикальное направление движения мяча (-1 или +1)

    int leftPaddleY;  // позиция верхней части левой ракетки
    int rightPaddleY; // позиция верхней части правой ракетки

    int scoreLeft;   // счёт левого игрока
    int scoreRight;  // счёт правого игрока
} GameState;


// Инициализация начальных значений
void initGame(GameState* game) {
    // Мяч в центре
    game->ballX = WIDTH / 2;
    game->ballY = HEIGHT / 2;
    // Начальное направление (вправо-вниз, например)
    game->ballDX = 1;
    game->ballDY = 1;

    // Левая ракетка по вертикали
    // Выставим так, чтобы она была примерно посередине
    game->leftPaddleY = (HEIGHT - PADDLE_SIZE) / 2;
    // Правая ракетка
    game->rightPaddleY = (HEIGHT - PADDLE_SIZE) / 2;

    // Счёт
    game->scoreLeft = 0;
    game->scoreRight = 0;
}


// Функция отрисовки кадра (ASCII-графика в терминале)
void drawFrame(const GameState* game) {
    // Очистка экрана (для Windows используйте "cls")
    system("clear");

    // Выводим счёт
    printf("Left Player (A/Z): %d    |    Right Player (K/M): %d\n",
           game->scoreLeft, game->scoreRight);

    // Выводим верхнюю границу
    for (int i = 0; i < WIDTH; i++) {
        putchar('-');
    }
    putchar('\n');

    // Перебираем каждую строку игрового поля
    for (int y = 0; y < HEIGHT; y++) {
        // Левая граница поля
        putchar('|');

        // Перебираем каждый столбец поля
        for (int x = 0; x < WIDTH - 2; x++) {
            // Проверяем, не находится ли здесь мяч
            if (x == game->ballX - 1 && y == game->ballY) {
                // Выводим мяч (один символ, допустим 'O')
                putchar('O');
            }
            // Проверяем левую ракетку
            else if (x == 0 &&
                     y >= game->leftPaddleY &&
                     y < game->leftPaddleY + PADDLE_SIZE) {
                putchar('#');
            }
            // Проверяем правую ракетку
            else if (x == WIDTH - 3 &&
                     y >= game->rightPaddleY &&
                     y < game->rightPaddleY + PADDLE_SIZE) {
                putchar('#');
            }
            else {
                // Пустое пространство
                putchar(' ');
            }
        }

        // Правая граница поля
        putchar('|');
        putchar('\n');
    }

    // Выводим нижнюю границу
    for (int i = 0; i < WIDTH; i++) {
        putchar('-');
    }
    putchar('\n');
}


// Сброс мяча в центр после пропущенного гола
void resetBall(GameState* game, int direction) {
    game->ballX = WIDTH / 2;
    game->ballY = HEIGHT / 2;
    game->ballDX = direction; // Если забил левый, то мяч летит вправо, и наоборот
    game->ballDY = 1;
}


// Обработка пользовательского ввода (пошагово)
void processInput(GameState* game, char input) {
    // Приведём к нижнему регистру
    input = (char)tolower((unsigned char)input);

    switch (input) {
        case 'a':
            // Левую ракетку вверх
            if (game->leftPaddleY > 0) {
                game->leftPaddleY--;
            }
            break;
        case 'z':
            // Левую ракетку вниз
            if (game->leftPaddleY + PADDLE_SIZE < HEIGHT) {
                game->leftPaddleY++;
            }
            break;
        case 'k':
            // Правую ракетку вверх
            if (game->rightPaddleY > 0) {
                game->rightPaddleY--;
            }
            break;
        case 'm':
            // Правую ракетку вниз
            if (game->rightPaddleY + PADDLE_SIZE < HEIGHT) {
                game->rightPaddleY++;
            }
            break;
        case ' ':
            // Пропуск действия
            // Ничего не делаем с ракетками
            break;
        default:
            // Любая другая клавиша — игнор
            break;
    }
}


// Обновление позиции мяча, проверка на столкновения
void updateBall(GameState* game) {
    // Двигаем мяч
    game->ballX += game->ballDX;
    game->ballY += game->ballDY;

    // Проверка столкновений с верхом/низом
    if (game->ballY < 0) {
        game->ballY = 0;
        game->ballDY = 1; // Меняем направление на вниз
    } else if (game->ballY >= HEIGHT) {
        game->ballY = HEIGHT - 1;
        game->ballDY = -1; // Меняем направление на вверх
    }

    // Проверка на вылет мяча за левый край
    if (game->ballX < 0) {
        // Правый игрок получает очко
        game->scoreRight++;
        // Сбрасываем мяч в центр, направляем вправо
        resetBall(game, 1);
        return;
    }

    // Проверка на вылет мяча за правый край
    if (game->ballX >= WIDTH - 1) {
        // Левый игрок получает очко
        game->scoreLeft++;
        // Сбрасываем мяч в центр, направляем влево
        resetBall(game, -1);
        return;
    }

    // Координаты для проверки столкновения с ракетками
    // Левая ракетка расположена в x=0, но у нас мяч рисуется в (ballX - 1)
    // поэтому проверим примерно ballX == 1
    if (game->ballX == 1) {
        // Если по вертикали мяч совпадает с областью ракетки
        if (game->ballY >= game->leftPaddleY &&
            game->ballY < game->leftPaddleY + PADDLE_SIZE) {
            // Отскакиваем мяч
            game->ballDX = 1;
        }
    }

    // Правая ракетка расположена в x=WIDTH-3, 
    // мяч находится в x=ballX (если символ мяча в (ballX-1)),
    // значит когда game->ballX == WIDTH-3, может произойти столкновение.
    if (game->ballX == WIDTH - 3) {
        if (game->ballY >= game->rightPaddleY &&
            game->ballY < game->rightPaddleY + PADDLE_SIZE) {
            game->ballDX = -1;
        }
    }
}


int main(void) {
    GameState game;
    initGame(&game);

    // Основной цикл игры
    while (1) {
        // Отрисовываем текущее состояние
        drawFrame(&game);

        // Проверяем, не достиг ли кто-то 21 очка
        if (game.scoreLeft >= WIN_SCORE) {
            printf("Поздравляем! Левый игрок набрал 21 очко и побеждает!\n");
            break;
        }
        if (game.scoreRight >= WIN_SCORE) {
            printf("Поздравляем! Правый игрок набрал 21 очко и побеждает!\n");
            break;
        }

        // Ждём ввода пользователя (пошаговый режим — при каждом шаге)
        printf("Введите команду (A/Z - левая ракетка, K/M - правая ракетка, Space - пропуск): ");
        char input = (char)getchar();

        // Если пользователь ввёл Enter перед буквами — пропустим
        while (input == '\n') {
            input = (char)getchar();
        }

        // Обрабатываем ввод
        processInput(&game, input);

        // Обновляем позицию мяча
        updateBall(&game);
    }

    return 0;
}
Краткие замечания к пошаговой реализации
Весь вывод идёт в консоль через стандартные функции printf() и putchar().
Чтобы улучшить внешний вид, каждое обновление экрана сопровождается system("clear"). В Windows может потребоваться заменить на system("cls").
При каждом ходе игроки могут нажать A/Z и K/M для изменения положения ракеток (по одной «ступеньке»), либо пробел, чтобы пропустить действие. Затем мяч сдвигается на 1 клетку согласно своему вектору движения.
Если мяч «вышел» за край, соответствующий игрок получает очко, и мяч сбрасывается в центр.
Игра продолжается, пока счёт одного из игроков не достигнет 21.
2. Интерактивный режим (бонус) (файл src/pong_interactive.c)
Ниже — упрощённая версия с использованием ncurses (или curses) для организации «живого» движения без пошагового ввода.
Суть:

Игра идёт в реальном времени (до 21 очка).
Считываем нажатые клавиши с клавиатуры в неблокирующем режиме, чтобы одновременно управлять ракетками и наблюдать за движением мяча.
Для упрощения, задержка между кадрами (sleep) может быть реализована через napms(...) (из ncurses) или usleep().
Управление то же самое: A/Z и K/M.
c
Копировать
/*
 * pong_interactive.c — интерактивная версия игры Pong с использованием ncurses
 *
 * Требует установки библиотеки ncurses (пример в Linux: sudo apt-get install libncurses5-dev).
 *
 * Компиляция:
 *   gcc pong_interactive.c -o pong_interactive -lncurses
 *
 * Запуск:
 *   ./pong_interactive
 *
 * Управление:
 *   A/Z — передвижение левой ракетки вверх/вниз
 *   K/M — передвижение правой ракетки вверх/вниз
 *   Q или Esc — выход из игры (досрочно)
 *   Игра ведётся до 21 очка.
 */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> // для usleep, если нужно

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_SIZE 3
#define WIN_SCORE 21

typedef struct {
    int ballX;
    int ballY;
    int ballDX;
    int ballDY;

    int leftPaddleY;
    int rightPaddleY;

    int scoreLeft;
    int scoreRight;
} GameState;

void initGame(GameState* game) {
    game->ballX = WIDTH / 2;
    game->ballY = HEIGHT / 2;
    game->ballDX = 1;
    game->ballDY = 1;

    game->leftPaddleY = (HEIGHT - PADDLE_SIZE) / 2;
    game->rightPaddleY = (HEIGHT - PADDLE_SIZE) / 2;

    game->scoreLeft = 0;
    game->scoreRight = 0;
}

void resetBall(GameState* game, int direction) {
    game->ballX = WIDTH / 2;
    game->ballY = HEIGHT / 2;
    game->ballDX = direction;
    game->ballDY = 1;
}

// Рисуем текущее состояние
void drawGame(const GameState* game) {
    // Очищаем окно ncurses
    clear();

    // Вывод счёта сверху
    mvprintw(0, 0, "Left (A/Z): %d   Right (K/M): %d", game->scoreLeft, game->scoreRight);

    // Верхняя граница (нарисуем просто символами '-')
    for (int i = 0; i < WIDTH; i++) {
        mvaddch(1, i, '-');
    }

    // Основная часть поля
    for (int y = 0; y < HEIGHT; y++) {
        // Левая граница
        mvaddch(y + 2, 0, '|');
        // Пустое пространство посередине
        for (int x = 1; x < WIDTH - 1; x++) {
            // Определяем, нужно ли нарисовать мяч
            if (x == game->ballX && (y == game->ballY)) {
                mvaddch(y + 2, x, 'O');
            }
            // Левая ракетка
            else if (x == 1 &&
                     y >= game->leftPaddleY &&
                     y < game->leftPaddleY + PADDLE_SIZE) {
                mvaddch(y + 2, x, '#');
            }
            // Правая ракетка
            else if (x == WIDTH - 2 &&
                     y >= game->rightPaddleY &&
                     y < game->rightPaddleY + PADDLE_SIZE) {
                mvaddch(y + 2, x, '#');
            }
            else {
                // Оставляем пусто: ' '
            }
        }
        // Правая граница
        mvaddch(y + 2, WIDTH - 1, '|');
    }

    // Нижняя граница
    for (int i = 0; i < WIDTH; i++) {
        mvaddch(HEIGHT + 2, i, '-');
    }

    // Обновляем экран
    refresh();
}

// Обновляем мяч
void updateBall(GameState* game) {
    game->ballX += game->ballDX;
    game->ballY += game->ballDY;

    // Столкновение с верхом/низом
    if (game->ballY < 0) {
        game->ballY = 0;
        game->ballDY = 1;
    } else if (game->ballY >= HEIGHT) {
        game->ballY = HEIGHT - 1;
        game->ballDY = -1;
    }

    // Вылет за левый край
    if (game->ballX < 0) {
        game->scoreRight++;
        resetBall(game, 1);
        return;
    }
    // Вылет за правый край
    if (game->ballX >= WIDTH) {
        game->scoreLeft++;
        resetBall(game, -1);
        return;
    }

    // Проверка столкновения с левой ракеткой
    if (game->ballX == 1) {
        if (game->ballY >= game->leftPaddleY &&
            game->ballY < game->leftPaddleY + PADDLE_SIZE) {
            game->ballDX = 1;
        }
    }

    // Проверка столкновения с правой ракеткой
    if (game->ballX == WIDTH - 2) {
        if (game->ballY >= game->rightPaddleY &&
            game->ballY < game->rightPaddleY + PADDLE_SIZE) {
            game->ballDX = -1;
        }
    }
}

// Обработка ввода
void handleInput(GameState* game, int ch) {
    // Преобразуем к нижнему регистру
    ch = tolower(ch);

    switch (ch) {
        case 'a':
            // Вверх левая ракетка
            if (game->leftPaddleY > 0) {
                game->leftPaddleY--;
            }
            break;
        case 'z':
            // Вниз левая ракетка
            if (game->leftPaddleY + PADDLE_SIZE < HEIGHT) {
                game->leftPaddleY++;
            }
            break;
        case 'k':
            // Вверх правая ракетка
            if (game->rightPaddleY > 0) {
                game->rightPaddleY--;
            }
            break;
        case 'm':
            // Вниз правая ракетка
            if (game->rightPaddleY + PADDLE_SIZE < HEIGHT) {
                game->rightPaddleY++;
            }
            break;
        default:
            break;
    }
}

int main(void) {
    // Инициализация ncurses
    initscr();            // Инициализировать экран
    cbreak();             // Режим посимвольного ввода
    noecho();             // Не выводить нажатые клавиши
    nodelay(stdscr, TRUE); // Неблокирующий ввод (getch() вернёт ERR если нет нажатий)
    keypad(stdscr, TRUE); // Разрешить спец. клавиши
    curs_set(0);          // Скрыть курсор

    GameState game;
    initGame(&game);

    // Основной цикл
    while (1) {
        // Считываем нажатую клавишу (или ERR, если нет)
        int ch = getch();
        if (ch != ERR) {
            // Проверяем на выход
            if (ch == 'q' || ch == 'Q' || ch == 27) { // 27 — Esc
                break;
            }
            // Обрабатываем управление ракетками
            handleInput(&game, ch);
        }

        // Обновляем мяч
        updateBall(&game);

        // Рисуем текущее состояние
        drawGame(&game);

        // Проверяем счёт
        if (game.scoreLeft >= WIN_SCORE) {
            // Выводим сообщение о победе левого игрока
            clear();
            mvprintw(HEIGHT / 2, (WIDTH / 2) - 10, "Левый игрок победил!");
            refresh();
            napms(2000);
            break;
        } else if (game.scoreRight >= WIN_SCORE) {
            // Выводим сообщение о победе правого игрока
            clear();
            mvprintw(HEIGHT / 2, (WIDTH / 2) - 10, "Правый игрок победил!");
            refresh();
            napms(2000);
            break;
        }

        // Небольшая задержка, чтобы игра шла не слишком быстро
        // 60 кадров в секунду ~ 16-17 мс на кадр
        napms(50); // задержка ~50 мс (около 20 кадров в секунду)
    }

    // Завершение ncurses
    endwin();
    return 0;
}
Краткие замечания к интерактивной версии
Используется библиотека ncurses для «живой» отрисовки в терминале.
Ввод с клавиатуры «неблокирующий», что позволяет мячику двигаться, даже если пользователь не нажимает клавиши.
Для плавности работы рекомендуется ставить небольшую задержку (napms(50)), иначе мяч может двигаться слишком быстро.
Управление теми же клавишами (A/Z и K/M). Для быстрого выхода: Q или Esc.
