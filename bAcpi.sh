#!/bin/bash

clear_box() {
    echo -en "\e[?25l"
    clear
}

double_line_box() {
# Получаем уровень заряда батареи
    battery_level=$(acpi -b | grep -oP '\d+%' | tr -d '%')

    # Задаём цвет фона в зависимости от процента
    if [ "$battery_level" -eq 100 ]; then
        bf=43  # Жёлтый фон для 100%
    elif [ "$battery_level" -ge 96 ]; then
        bf=44  # Синий фон для 96% и выше (но меньше 100%)
    elif [ "$battery_level" -ge 92 ]; then
        bf=45  # Фиолетовый фон для 92% и выше (но меньше 96%)
    elif [ "$battery_level" -ge 88 ]; then
        bf=41  # Красный фон для 88% и выше (но меньше 92%)
    else
        bf=43  # По умолчанию жёлтый фон для всех значений ниже 88%
    fi
#  bf=44
    echo -en "\e[1;1H\e[${bf};33m╔═════════════════════════════════════════════════════════════════════════════════╗\e[0m"
  for (( a=2; a<=41; a++ )); do
    echo -en "\e[${a};1H\e[${bf};33m║\e[0m                                                                                 \e[${bf};33m║\e[0m"
  done
    echo -en "\e[42;1H\e[${bf};33m╚═════════════════════════════════════════════════════════════════════════════════╝\e[0m"
    echo -en "\e[5;0H\e[${bf};33m╠\e[0m\e[33m═════════════════════════════════════════════════════════════════════════════════\e[${bf};33m╣\e[0m"
    echo -en "\e[7;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[9;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[11;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[13;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[15;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[17;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[29;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[35;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
    echo -en "\e[39;0H\e[${bf};33m╟\e[0m\e[33m─────────────────────────────────────────────────────────────────────────────────\e[${bf};33m╢\e[0m"
}

ifo_box() {
    echo -en "\e[3;5H\e[1;33m Linux OS acpi The state of the battery\e[0m"
    echo -en "\e[40;6H\e[33m Нажмите 'ctrl+c' для выхода\e[0m"
}

acpi_box() {
    if command -v acpi &> /dev/null; then
        ACPI_VERSION=$(acpi -v | grep "acpi" | awk '{print $2}')  # Берем версию из строки с "acpi"
        echo -en "\e[6;5H \e[32macpi         \e[0m  \e[90mACPI version: \e[97m${ACPI_VERSION}\e[0m    \e[0m"
    else
        echo -en "\e[6;5H \e[31macpi         \e[0m  \e[90mUtility not found, install: \e[97msudo apt install acpi\e[0m    \e[0m"
    fi
}

bat_box() {
    if acpi -b &> /dev/null && [ -n "$(acpi -b)" ]; then
        echo -en "\e[8;5H \e[32mBattery:\e[0m\e[90m\e[97m Detected\e[0m"
    else
        echo -en "\e[8;5H \e[31mBattery:\e[0m\e[90m\e[97m Undetected\e[0m"
    fi
}

line_box() {
    BATTERY_STATE=$(acpi -b | awk '{print $3}' | tr -d ',')
    case "$BATTERY_STATE" in
        "Charging")
            echo -en "\e[8;28H\e[32m Battery State \e[0m Charging\e[0m"
            ;;
        "Discharging")
            echo -en "\e[8;20H\e[32m Battery State \e[31m Discharging\e[0m"
            ;;
        "Full")
            echo -en "\e[8;20H\e[32m Battery State \e[0m Running on AC power / Plugged in\e[0m"
            ;;
        *)
            echo -en "\e[8;20H\e[31m Battery State \e[0m Unknown state\e[0m"
            ;;
    esac
}

time_box() {
    BATTERY_TIME=$(acpi -b | grep -o '[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}' || echo "N/A")
    echo -en "\e[10;5H \e[32mBattery Time \e[0m  \e[90mОставшееся время: \e[97m${BATTERY_TIME}\e[0m     \e[0m"
}

battery_progress_bar() {
        battery_level=$(acpi -b | grep -oP '\d+%' | tr -d '%')  # Получаем уровень заряда
        # Округляем вверх до ближайшего целого
        rounded_level=$(( (battery_level + 1) ))
        if [ $rounded_level -gt 100 ]; then
            rounded_level=100
        fi
        # Вычисляем количество заполненных символов (полоса из 50 символов)
        filled=$(( (rounded_level * 50) / 100 ))
        empty=$(( 50 - filled ))
        # Создаём строку прогресс-бара
        progress=""
        for ((i=0; i<filled; i++)); do
            progress="${progress}▓"
        done
        for ((i=0; i<empty; i++)); do
            progress="${progress}░"
        done
        # Выводим прогресс-бар с цветами и процентом
        echo -en "\e[12;5H\e[32m Battery level: \e[33m[\e[0m${progress}\e[33m]\e[0m $battery_level%\e[0m"
}

check_battery_rate() {
    BATTERY_RATE=$(acpi -b | grep -o 'rate information unavailable')

    if [ -n "$BATTERY_RATE" ]; then
        echo -e "\e[14;5H\e[32m Battery rate: \e[0m \e[90mRate information unavailable\e[0m"
    fi
}

get_adapter_status() {
  local adapter_status=$(acpi -a)
  echo -en "\e[16;5H\e[32m Battery \e[0m $adapter_status"
}

get_thermal1_status() {
  local thermal1=$(acpi -t | grep "Thermal 0:" | awk '{print $4, $5, $6}')
  echo -en "\e[18;5H\e[32m Battery\e[90m thermal 1:\e[0m $thermal1"
}

get_thermal2_status() {
  local thermal2=$(acpi -t | grep "Thermal 1:" | awk '{print $4, $5, $6}')
  echo -en "\e[18;40H \e[90m Thermal 2:\e[0m $thermal2"
}

get_cooling() {
  # Получаем вывод acpi -c
  local cooling_lines=$(acpi -c)
  
  # Принимаем начальную позицию от пользователя (по умолчанию 22 строка, 5 колонка)
  local start_line=${1:-19}
  local start_col=${2:-5}
  
  # Флаг для первой строки
  local first_line=true
  
  # Читаем вывод построчно
  while IFS= read -r line; do
    # Проверяем, что строка начинается с "Cooling"
    if [[ "$line" =~ ^Cooling\ [0-9]+: ]]; then
      if [ "$first_line" = true ]; then
        # Первая строка: "Battery" слева, "Cooling" с нужным отступом
        echo -en "\e[${start_line};$((start_col + 1))H\e[32mBattery \e[0m\e[${start_line};$((start_col + 9))H$line"
        first_line=false
      else
        # Последующие строки с "Cooling" выровнены по той же позиции
        echo -en "\e[${start_line};$((start_col + 9))H$line"
      fi
      ((start_line++))  # Увеличиваем номер строки для следующей записи
    fi
  done <<< "$cooling_lines"
}

get_battery_health() {
  local line=$(acpi --details | grep -i "design capacity")
  local design=$(echo "$line" | awk '{print $5}')
  local full=$(echo "$line" | awk '{print $10}')
  local percent=$(echo "$line" | awk -F '=' '{print $2}' | tr -d ' %')
  local wear=$((100 - percent))
  echo -en "\e[30;5H Design capacity:    ${design} mAh — original factory battery capacity"
  echo -en "\e[31;5H Last full capacity: ${full} mAh — current maximum capacity after wear"
  echo -en "\e[32;5H Current capacity:   ${percent}% — remaining capacity relative to original"
  echo -en "\e[33;5H Battery wear:       ${wear}% — battery has lost ${wear}% of original capacity"
}

progress_bar() {
    # Получаем процент здоровья из вывода acpi --details
    local line=$(acpi --details | grep -i "design capacity")
    local percent=$(echo "$line" | awk -F '=' '{print $2}' | tr -d ' %')
    # Округляем здоровье вверх (если есть десятые, хотя здесь всегда целое — добавим +1, если остаток)
    rounded_health=$((percent))
    if [ $((percent % 1)) -ne 0 ]; then
        rounded_health=$((rounded_health + 1))
    fi
    # Принудительное округление вверх, если не 100
    if [ "$percent" -lt 100 ]; then
        rounded_health=$((rounded_health + 1))
    fi
    if [ "$rounded_health" -gt 100 ]; then
        rounded_health=100
    fi
    # Вычисляем заполненные и пустые блоки
    filled=$(( (rounded_health * 50) / 100 ))
    empty=$(( 50 - filled ))
    # Создаём прогресс-бар
    progress=""
    for ((i=0; i<filled; i++)); do
        progress+="▓"
    done
    for ((i=0; i<empty; i++)); do
        progress+="░"
    done
    # Выводим прогресс-бар с процентом здоровья
    echo -en "\e[34;5H\e[32m Battery health: \e[33m[\e[0m${progress}\e[33m] \e[0m${rounded_health}%\e[0m"
}


alert_box() {
# Получаем уровень заряда батареи
battery_level=$(acpi -b | grep -oP '\d+%' | tr -d '%')
# Цвета: чёрный текст всегда, фон зависит от уровня заряда
if [ "$battery_level" -le 91 ]; then
    bf=41  # красный фон
else
    bf=40  # чёрный фон
fi
       echo -en "\e[36;6H\e[30;${bf}m╔════════════════════════════════════════════════════════════════════════╗"
       echo -en "\e[37;6H║          The computer is about to shut down!                           ║"
       echo -en "\e[38;6H╚════════════════════════════════════════════════════════════════════════╝\e[0m"
}

THRESHOLD=84

trap 'echo -en "\e[?25h"; clear; exit' SIGINT

# Начальная отрисовка
clear_box

# Основной цикл отрисовки каждые 5 минут
while true; do
   double_line_box
   ifo_box
   acpi_box
   bat_box
   line_box
   time_box
   battery_progress_bar
   check_battery_rate
   get_adapter_status
   get_thermal1_status
   get_thermal2_status
   get_cooling
   get_battery_health
   progress_bar
   alert_box

    BATTERY_LEVEL=$(acpi -b | grep -P -o '[0-9]+(?=%)')
    if [ -n "$BATTERY_LEVEL" ] && [ "$BATTERY_LEVEL" -le "$THRESHOLD" ]; then
        systemctl poweroff
    fi
    sleep 300
done
