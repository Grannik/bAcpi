#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <locale.h>
#include <wchar.h>

void draw_menu();
void print_menu_items(const wchar_t *items[]);
void cleanup(int sig);

int selected = 0;
const int MAX_ITEMS = 19;

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (!setlocale(LC_ALL, "")) {
        printf("Ошибка установки локали\n");
        return 1;
    }

    initscr();
    if (stdscr == NULL) { printf("Ошибка инициализации ncurses\n"); return 1; }
    if (has_colors() == FALSE) {
        endwin();
        printf("Терминал не поддерживает цвета\n");
        return 1;
    }
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    signal(SIGINT, cleanup);

    init_pair(1, COLOR_BLACK, COLOR_YELLOW);  // Чёрный на жёлтом (рамки и выделение)
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // Жёлтый текст (заголовки, линии)
    init_pair(3, COLOR_CYAN, COLOR_BLACK);    // "Git"
    init_pair(4, COLOR_GREEN, COLOR_BLACK);   // Пункты меню
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_WHITE - 1, COLOR_BLACK);

    const wchar_t *items[] = {
        L" Установка                                                          INSTALL ",
        L" Краткий обзор                                                     SYNOPSIS ",
        L" Описание                                                       DESCRIPTION ",
        L" Автор                                                               AUTHOR ",
        L" Показать нформацию об батарее                                 -b --battery ",
        L" Показать нформацию об адаптере питания                     -a --ac-adapter ",
        L" Показать нформацию о температуре                              -t --thermal ",
        L" Показать нформацию об охлаждении                              -c --cooling ",
        L" Показать все устройства, переопределяет другие опции       -V --everything ",
        L" Показать неработающие/неактивные устройства                -s --show-empty ",
        L" Показать дополнительные детали, если они доступны             -i --details ",
        L" Показать температуру в Фаренгейтах                         -f --fahrenheit ",
        L" Использовать Кельвины вместо Цельсия                           -k --kelvin ",
        L" Использовать старый /proc вместо нового /sys                     -p --proc ",
        L" Путь к ACPI (/proc/acpi или /sys/class)                     -d --directory ",
        L" Показать справку                                                 -h --help ",
        L" Показать версию                                             -v | --version ",
        L"                                                                        Git ",
        L"                                                                       Exit "
    };

    draw_menu();
    print_menu_items(items);

    while (1) {
        int ch = getch();

        switch (ch) {
            case KEY_UP:
                selected--;
                if (selected < 0) selected = MAX_ITEMS - 1;
                print_menu_items(items);
                break;

            case KEY_DOWN:
                selected++;
                if (selected >= MAX_ITEMS) selected = 0;
                print_menu_items(items);
                break;

            case 10:  // Enter
                int last_line = 0;

                clear();
                refresh();

                if (selected == 18) {  // Exit
                    cleanup(0);
                }

                // Специальная обработка для "Установка"
                if (selected == 0) {
                    mvprintw(1, 0, " Утилита предустановлена.");
                    last_line = 2;
                }

                // SYNOPSIS
                if (selected == 1) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi [options]");attroff(COLOR_PAIR(4)); // Выключаем и выключаем зеленый цвет
                    last_line = 2;
                }

                // DESCRIPTION
                if (selected == 2) {
                    mvprintw(1, 0, " ACPI показывает информацию из файловой системы");
                    mvprintw(2, 0, " или /sys, такой как состояние батареи или тепловая информация.");
                    last_line = 3;
                }

                // AUTHOR
                if (selected == 3) {
                    mvprintw(1, 0, " Оригинальная версия этой страницы руководства была написана Полом Телфордом");
                    mvprintw(2, 0, " pxt@debian.org для системы Debian.");
                    mvprintw(3, 0, " Новые дополнения были сделаны Майклом Месксом meskes@debian.org");
                    mvprintw(4, 0, " Разрешение предоставляется копировать, распространять");
                    mvprintw(5, 0, " и изменять этот документ в соответствии с условиями GNU GPL.");
                    last_line = 6;
                }

                // acpi -b --battery
                if (selected == 4) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -b");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --battery");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -a --ac-adapter
                if (selected == 5) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -a");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --ac-adapter");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -t acpi --thermal
                if (selected == 6) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -t");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --thermal");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -c --cooling
                if (selected == 7) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -c");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --cooling");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -V --everything
                if (selected == 8) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -V");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --everything");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -s --show-empty
                if (selected == 9) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -s");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --show-empty");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -i --details
                if (selected == 10) {
                    mvprintw(1, 0, " Информация о ёмкости батареи.");
                    mvprintw(2, 0, " Температурные пороги срабатывания.");
                    attron(COLOR_PAIR(4));mvprintw(3, 0, " acpi -i");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(4, 0, " acpi --details");attroff(COLOR_PAIR(4));
                    last_line = 5;
                }

                // acpi -f --fahrenheit
                if (selected == 11) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -f");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --fahrenheit");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -k | --kelvin
                if (selected == 12) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -k");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --kelvin");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -p | --proc
                if (selected == 13) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -p");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --proc");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -d --directory
                if (selected == 14) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -d");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --directory");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi acpi -h --help
                if (selected == 15) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -h");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --help");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // acpi -v --version
                if (selected == 16) {
                    attron(COLOR_PAIR(4));mvprintw(1, 0, " acpi -v");attroff(COLOR_PAIR(4));
                    attron(COLOR_PAIR(4));mvprintw(2, 0, " acpi --version");attroff(COLOR_PAIR(4));
                    last_line = 3;
                }

                // Git
                if (selected == 17) {
                    mvprintw(1, 0, " https://github.com/acpica/acpica ");
                    mvprintw(2, 0, " https://www.intel.com/content/www/us/en/developer/topic-technology/open/acpica/overview.html ");
                    last_line = 3;
                }

                attron(COLOR_PAIR(1));
                mvprintw(last_line + 2, 0, " ENTER = Main Menu ");
                attroff(COLOR_PAIR(1));
                refresh();
                getch();
                draw_menu();
                print_menu_items(items);
                break;
        }
    }

    cleanup(0);
    return 0;
}

void draw_menu() {
    clear();

    // Верхняя линия с жёлтым фоном
    attron(COLOR_PAIR(1));mvprintw(0, 0, "┌──────────────────────────────────────────────────────────────────────────────┐");attroff(COLOR_PAIR(1));

    // Вертикальные линии рамки (жёлтый фон только для символов)
    attron(COLOR_PAIR(1));mvvline(1, 0, ACS_VLINE, 29);mvvline(1, 79, ACS_VLINE, 29);attroff(COLOR_PAIR(1));

    // Нижняя линия с жёлтым фоном
    attron(COLOR_PAIR(1));mvprintw(30, 0, "└──────────────────────────────────────────────────────────────────────────────┘");attroff(COLOR_PAIR(1));

    // Разделители (жёлтый фон только для соединителей)
    attron(COLOR_PAIR(1));
    mvaddch(3, 0, ACS_LTEE);
    mvaddch(3, 79, ACS_RTEE);
    mvaddch(5, 0, ACS_LTEE);
    mvaddch(5, 79, ACS_RTEE);
//    mvaddch(30, 0, ACS_LTEE);
//    mvaddch(31, 79, ACS_RTEE);
    attroff(COLOR_PAIR(1));

    // Горизонтальные линии разделителей (жёлтый текст без фона)
    attron(COLOR_PAIR(2));
    mvhline(3, 1, ACS_HLINE, 78);
    mvhline(5, 1, ACS_HLINE, 78);
    mvhline(10, 1, ACS_HLINE, 78);
    mvhline(25, 1, ACS_HLINE, 78);
    attroff(COLOR_PAIR(2));

    // Текст
//    attron(A_BOLD | hunting(COLOR_PAIR(5));mvprintw(2, 3, "*** acpi ***");attroff(A_BOLD | COLOR_PAIR(6));
    attron(A_BOLD | COLOR_PAIR(5)); mvprintw(2, 3, "*** acpi ***"); attroff(A_BOLD | COLOR_PAIR(6));
    attron(A_DIM); mvprintw(4, 3, "Shows battery status and other ACPI information"); attroff(A_DIM);
    attron(A_DIM); mvprintw(11, 3, "ОПЦИИ                                                              OPTIONS"); attroff(A_DIM);
    attron(COLOR_PAIR(2));mvprintw(27, 1, "── ↑ Up ──── ↓ Down ──── ↵ Select Enter ──────────────────────────────────────");attroff(COLOR_PAIR(2));
    refresh();
}

void print_menu_items(const wchar_t *items[]) {
    int y_positions[] = { 6, 7, 8, 9, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 26, 28 };

    for (int i = 0; i < MAX_ITEMS; i++) {
        attron(COLOR_PAIR(4)); mvprintw(y_positions[i], 2, "%-77ls", L" "); attroff(COLOR_PAIR(4));

        if (i == selected) {
            attron(COLOR_PAIR(1));
            if (items[i][0] == L'\0') {
                mvprintw(y_positions[i], 2, "%-77ls", L" ");
            } else {
                mvprintw(y_positions[i], 2, "%ls", items[i]);
            }
            attroff(COLOR_PAIR(1));
        } else if (i == 0) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L"");
            attron(COLOR_PAIR(4)); printw("                                                                    INSTALL "); attroff(COLOR_PAIR(4));
        } else if (i == 1) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L"");
            attron(COLOR_PAIR(4)); printw("                                                                   SYNOPSIS "); attroff(COLOR_PAIR(4));
        } else if (i == 2) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L"");
            attron(COLOR_PAIR(4)); printw("                                                                DESCRIPTION "); attroff(COLOR_PAIR(4));
        } else if (i == 3) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L"");
            attron(COLOR_PAIR(4)); printw("                                                                     AUTHOR "); attroff(COLOR_PAIR(4));
        } else if (i == 4) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show battery information ");
            attron(COLOR_PAIR(4)); printw("                                     -b --battery "); attroff(COLOR_PAIR(4));
        } else if (i == 5) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show ac adapter information ");
            attron(COLOR_PAIR(4)); printw("                               -a --ac-adapter "); attroff(COLOR_PAIR(4));
        } else if (i == 6) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show thermal information ");
            attron(COLOR_PAIR(4)); printw("                                     -t --thermal "); attroff(COLOR_PAIR(4));
        } else if (i == 7) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show cooling device information ");
            attron(COLOR_PAIR(4)); printw("                              -c --cooling "); attroff(COLOR_PAIR(4));
        } else if (i == 8) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show every device, overrides above options ");
            attron(COLOR_PAIR(4)); printw("                -V --everything "); attroff(COLOR_PAIR(4));
        } else if (i == 9) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show non-operational devices");
            attron(COLOR_PAIR(4)); printw("                               -s --show-empty "); attroff(COLOR_PAIR(4));
        } else if (i == 10) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show additional details if available:");
            attron(COLOR_PAIR(4)); printw("                         -i --details "); attroff(COLOR_PAIR(4));
        } else if (i == 11) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Show temperature in Fahrenheit");
            attron(COLOR_PAIR(4)); printw("                             -f --fahrenheit "); attroff(COLOR_PAIR(4));
        } else if (i == 12) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Use Kelvin instead of Celsius");
            attron(COLOR_PAIR(4)); printw("                                  -k --kelvin "); attroff(COLOR_PAIR(4));
        } else if (i == 13) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Use old /proc instead of new /sys");
            attron(COLOR_PAIR(4)); printw("                                -p --proc "); attroff(COLOR_PAIR(4));
        } else if (i == 14) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Path to ACPI info (/proc/acpi or /sys/class)");
            attron(COLOR_PAIR(4)); printw("                -d --directory "); attroff(COLOR_PAIR(4));
        } else if (i == 15) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Display help and exit");
            attron(COLOR_PAIR(4)); printw("                                            -h --help "); attroff(COLOR_PAIR(4));
        } else if (i == 16) {
            attron(COLOR_PAIR(5)); mvprintw(y_positions[i], 2, "%ls", L" Output version information and exit");
            attron(COLOR_PAIR(4)); printw("                           -v --version "); attroff(COLOR_PAIR(4));
        } else {
            attron(COLOR_PAIR(4)); mvprintw(y_positions[i], 2, "%ls", items[i]); attroff(COLOR_PAIR(4));
        }
    }
    refresh();
}

void cleanup(int sig) {
    endwin();
    exit(0);
}
