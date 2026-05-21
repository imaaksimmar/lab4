 #pragma once
#include <QString>


const QString APP_TITLE = "Ленивые последовательности";

const QString TAB_STATS = "Статистика";
const QString TAB_TESTS = "Тестирование";

const QString TEXT_SEQ_TYPE = "Тип последовательности:";
const QString TEXT_FINITE = "Конечная последовательность";
const QString TEXT_INFINITE = "Бесконечная последовательность";
const QString TEXT_GROUP_FINITE = "Данные (конечная последовательность)";
const QString TEXT_GROUP_INFINITE= "Генератор (бесконечная последовательность)";
const QString TEXT_INPUT = "Введите числа через запятую:";
const QString TEXT_PLACEHOLDER = "Например: 1, 2, 3";
const QString TEXT_GENERATOR = "Генератор:";
const QString TEXT_FIBONACCI = "Фибоначчи";
const QString TEXT_RANDOM = "Случайные числа";
const QString TEXT_ARITHMETIC = "Арифметическая прогрессия";
const QString TEXT_INFINITE_INFO = "Бесконечная последовательность — нажмите Стоп, чтобы остановить";
const QString TEXT_RUN  = "Запустить";
const QString TEXT_STOP = "Стоп";
const QString TEXT_GROUP_RESULT = "Результат";
const QString TEXT_COUNT = "Количество: ";
const QString TEXT_SUM = "Сумма: ";
const QString TEXT_MIN = "Минимум: ";
const QString TEXT_MAX = "Максимум: ";
const QString TEXT_AVG = "Среднее: ";
const QString TEXT_ELEMENTS = "Элементы:";


const QString TEXT_GROUP_TEST = "Входные данные и ожидаемые значения";
const QString TEXT_TEST_DATA = "Числа:";
const QString TEXT_PLACEHOLDER_TEST = "1, 2, 3, 4, 5";
const QString TEXT_EXP_COUNT = "Ожидаемое количество:";
const QString TEXT_EXP_SUM = "Ожидаемая сумма:";
const QString TEXT_EXP_MIN = "Ожидаемый минимум:";
const QString TEXT_EXP_MAX = "Ожидаемый максимум:";
const QString TEXT_EXP_AVG = "Ожидаемое среднее:";
const QString TEXT_CHECK  = "Проверить";


const QString ERR_TITLE  = "Ошибка";
const QString ERR_EMPTY_INPUT = "Введите числа!";
const QString ERR_INVALID_VALUE  = "Некорректное значение: ";
const QString ERR_EMPTY_TEST = "Введите числа для теста!";

const QString TEST_PASSED = "Все проверки пройдены!";
const QString TEST_FAILED = "Есть несовпадения.";


const QString STYLE_COMBO =
    "QComboBox { background: rgba(0,0,0,200); color: white; "
    "border: 1px solid rgba(255,255,255,80); border-radius: 5px; padding: 5px; }"
    "QComboBox QAbstractItemView { background: rgba(20,20,20,240); color: white; "
    "selection-background-color: rgba(83,52,131,200); }";

const QString STYLE_GROUP_WHITE =
    "QGroupBox { color: white; } QGroupBox::title { color: white; }";

const QString STYLE_LABEL_RESULT =
    "color: #a0c4ff; font-size: 13px; padding: 2px;";

const QString STYLE_LABEL_INFO =
    "color: #ffffff; font-style: italic; font-size: 13px;";

const QString STYLE_TEXT_RESULT =
    "QTextEdit { background: rgba(0,0,0,150); color: white; "
    "border: 1px solid rgba(255,255,255,60); border-radius: 5px; font-size: 13px; }";

const QString STYLE_MAIN = R"(
    QTabWidget::pane {
        border: 1px solid rgba(255,255,255,60);
        background: transparent;
    }

    QTabBar::tab {
        background: rgba(0,0,0,120);
        color: white;
        padding: 8px 20px;
        border: 1px solid rgba(255,255,255,60);
        border-radius: 4px 4px 0 0;
    }

    QTabBar::tab:selected {
        background: rgba(83,52,131,180);
    }

    QGroupBox {
        color: white;
        font-weight: bold;
        border: 1px solid rgba(255,255,255,70);
        border-radius: 8px;
        margin-top: 10px;
        padding-top: 10px;
        background: rgba(0,0,0,80);
    }

    QGroupBox::title {
        subcontrol-origin: margin;
        left: 12px;
        color: white;
    }

    QLabel { color: white; }

    QComboBox {
        background: rgba(0,0,0,200);
        color: white;
        border: 1px solid rgba(255,255,255,80);
        border-radius: 5px;
        padding: 5px;
    }

    QComboBox QAbstractItemView {
        background: rgba(20,20,20,240);
        color: white;
        selection-background-color: rgba(83,52,131,200);
    }

    QSpinBox {
        background: rgba(0,0,0,140);
        color: white;
        border: 1px solid rgba(255,255,255,80);
        border-radius: 5px;
        padding: 5px;
    }

    QLineEdit {
        background: rgba(0,0,0,140);
        color: white;
        border: 1px solid rgba(255,255,255,80);
        border-radius: 5px;
        padding: 5px;
    }

    QTextEdit {
        background: rgba(0,0,0,120);
        color: #a0c4ff;
        border: 1px solid rgba(255,255,255,60);
        border-radius: 5px;
    }

    QPushButton {
        background: rgba(60,60,60,180);
        color: white;
        border: 1px solid rgba(255,255,255,100);
        border-radius: 6px;
        padding: 8px 16px;
        font-size: 13px;
    }

    QPushButton:hover { background: rgba(100,100,100,200); }
    QPushButton:pressed { background: rgba(40,40,40,200); }
    QPushButton:disabled {
        background: rgba(40,40,40,120);
        color: rgba(255,255,255,60);
    }
)";