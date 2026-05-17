#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <cstdlib>
#include <ctime>
#include "lazy_sequence.hpp"
#include "stream.hpp"
#include "statistics.hpp"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    srand((unsigned)time(nullptr));
    setWindowTitle("Ленивые последовательности");
    resize(640, 740);
    setStyleSheet(mainStyle());

    timer = new QTimer(this);
    timer->setInterval(400);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTick);

    tabs = new QTabWidget(this);
    tabs->setGeometry(0, 0, 640, 740);

    tabStats = new QWidget();
    tabTests = new QWidget();
    tabs->addTab(tabStats, "Статистика");
    tabs->addTab(tabTests, "Тестирование");

    setupStatsTab();
    setupTestsTab();
}


MainWindow::~MainWindow() {
    delete liveSequence;
    delete liveStream;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(hasError) {
        QPixmap bg("/Users/yana/Desktop/p2.jpeg");
        painter.setOpacity(0.7);
        painter.drawPixmap(0, 0, width(), height(), bg);
    }
    else {
        QPixmap bg("/Users/yana/Desktop/p1.jpeg");
        painter.setOpacity(0.8);
        painter.drawPixmap(0, 0, width(), height(), bg);
    }
    QMainWindow::paintEvent(event);
}


void MainWindow::setupStatsTab() {
    QVBoxLayout *lay = new QVBoxLayout(tabStats);
    lay->setSpacing(12);
    lay->setContentsMargins(16, 16, 16, 16);

    QHBoxLayout *seqRow = new QHBoxLayout();
    labelSeqType = new QLabel("Тип последовательности:");
    comboSeqType = new QComboBox();
    comboSeqType->addItem("Конечная последовательность");
    comboSeqType->addItem("Бесконечная последовательность");
    comboSeqType->setMinimumWidth(280); 

    comboSeqType->setStyleSheet(
        "QComboBox { background: rgba(0,0,0,200); color: white; "
        "border: 1px solid rgba(255,255,255,80); border-radius: 5px; padding: 5px; }"
        "QComboBox QAbstractItemView { background: rgba(20,20,20,240); color: white; "
        "selection-background-color: rgba(83,52,131,200); }"
    );
    seqRow->addWidget(labelSeqType);
    seqRow->addWidget(comboSeqType);
    seqRow->addStretch();
    lay->addLayout(seqRow);

    groupFinite = new QGroupBox("Данные (конечная последовательность)");
    groupFinite->setStyleSheet(
        "QGroupBox { color: white; } QGroupBox::title { color: white; }"
    );
    QVBoxLayout *finLay = new QVBoxLayout(groupFinite);
    labelInput = new QLabel("Введите числа через запятую:");
    lineInput = new QLineEdit();
    lineInput->setPlaceholderText("Например: 1, 2, 3");
    finLay->addWidget(labelInput);
    finLay->addWidget(lineInput);
    lay->addWidget(groupFinite);

    groupInfinite = new QGroupBox("Генератор (бесконечная последовательность)");
    groupInfinite->setStyleSheet(
        "QGroupBox { color: white; } QGroupBox::title { color: white; }"
    );
    QVBoxLayout *infLay = new QVBoxLayout(groupInfinite);
    QHBoxLayout *genRow = new QHBoxLayout();
    labelGen = new QLabel("Генератор:");
    comboGen = new QComboBox();
    comboGen->setStyleSheet(
        "QComboBox { background: rgba(0,0,0,200); color: white; "
        "border: 1px solid rgba(255,255,255,80); border-radius: 5px; padding: 5px; }"
        "QComboBox QAbstractItemView { background: rgba(20,20,20,240); color: white; "
        "selection-background-color: rgba(83,52,131,200); }"
    );
    comboGen->addItem("Фибоначчи");
    comboGen->addItem("Случайные числа");
    comboGen->addItem("Арифметическая прогрессия");
    genRow->addWidget(labelGen);
    genRow->addWidget(comboGen);
    infLay->addLayout(genRow);

    labelInfo = new QLabel("Бесконечная последовательность — нажми Стоп, чтобы остановить");
    labelInfo->setStyleSheet("color: #ffffff; font-style: italic; font-size: 13px;");
    infLay->addWidget(labelInfo);
    groupInfinite->setVisible(false);
    lay->addWidget(groupInfinite);

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRun = new QPushButton("Запустить");
    btnStop = new QPushButton("Стоп");
    btnStop->setEnabled(false);
    btnRow->addWidget(btnRun);
    btnRow->addWidget(btnStop);
    lay->addLayout(btnRow);

    groupResult = new QGroupBox("Результат");
    QVBoxLayout *resLay = new QVBoxLayout(groupResult);

    labelCount2 = new QLabel("Количество: ");
    labelSum = new QLabel("Сумма: ");
    labelMin = new QLabel("Минимум: ");
    labelMax = new QLabel("Максимум: ");
    labelAvg = new QLabel("Среднее: ");

    for(QLabel *l : {labelCount2, labelSum, labelMin, labelMax, labelAvg}) 
    {
        l->setStyleSheet("color: #a0c4ff; font-size: 13px; padding: 2px;");
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(6);
        shadow->setColor(Qt::black);
        shadow->setOffset(1, 1);
        l->setGraphicsEffect(shadow);
        resLay->addWidget(l);
    }

    labelElemsTitle = new QLabel("Элементы:");
    resLay->addWidget(labelElemsTitle);
    textElements = new QTextEdit();
    textElements->setReadOnly(true);
    textElements->setLineWrapMode(QTextEdit::WidgetWidth);
    textElements->setMaximumHeight(130);
    resLay->addWidget(textElements);
    lay->addWidget(groupResult);
    lay->addStretch();

    connect(comboSeqType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSeqTypeChanged);
    connect(btnRun,  &QPushButton::clicked, this, &MainWindow::onRun);
    connect(btnStop, &QPushButton::clicked, this, &MainWindow::onStop);
}

void MainWindow::setupTestsTab() {
    QVBoxLayout *lay = new QVBoxLayout(tabTests);
    lay->setSpacing(12);
    lay->setContentsMargins(16, 16, 16, 16);

    groupTestInput = new QGroupBox("Входные данные и ожидаемые значения");
    QFormLayout *form = new QFormLayout(groupTestInput);
    form->setSpacing(8);

    labelTestData = new QLabel("Числа:");
    lineTestData = new QLineEdit();
    lineTestData->setPlaceholderText("1, 2, 3, 4, 5");
    form->addRow(labelTestData, lineTestData);

    labelExpCount = new QLabel("Ожидаемое количество:");
    lineExpCount = new QLineEdit();
    lineExpCount->setPlaceholderText("5");
    form->addRow(labelExpCount, lineExpCount);

    labelExpSum = new QLabel("Ожидаемая сумма:");
    lineExpSum = new QLineEdit();
    lineExpSum->setPlaceholderText("15");
    form->addRow(labelExpSum, lineExpSum);

    labelExpMin = new QLabel("Ожидаемый минимум:");
    lineExpMin = new QLineEdit();
    lineExpMin->setPlaceholderText("1");
    form->addRow(labelExpMin, lineExpMin);

    labelExpMax = new QLabel("Ожидаемый максимум:");
    lineExpMax = new QLineEdit();
    lineExpMax->setPlaceholderText("5");
    form->addRow(labelExpMax, lineExpMax);

    labelExpAvg = new QLabel("Ожидаемое среднее:");
    lineExpAvg = new QLineEdit();
    lineExpAvg->setPlaceholderText("3.0");
    form->addRow(labelExpAvg, lineExpAvg);

    lay->addWidget(groupTestInput);

    btnCheck = new QPushButton("Проверить");
    lay->addWidget(btnCheck);

    textTestResult = new QTextEdit();
    textTestResult->setReadOnly(true);
    textTestResult->setStyleSheet(
        "QTextEdit { background: rgba(0,0,0,150); color: white; "
        "border: 1px solid rgba(255,255,255,60); border-radius: 5px; font-size: 13px; }"
    );
    lay->addWidget(textTestResult);

    connect(btnCheck, &QPushButton::clicked, this, &MainWindow::onCheckTest);
}

void MainWindow::onSeqTypeChanged(int index) {
    groupFinite->setVisible(index == 0);
    groupInfinite->setVisible(index == 1);
    onStop();
    hasError = false;
    update();

    labelCount2->setText("Количество: ");
    labelSum->setText("Сумма: ");
    labelMin->setText("Минимум: ");
    labelMax->setText("Максимум: ");
    labelAvg->setText("Среднее: ");
    textElements->clear();
}

void MainWindow::updateResult(int cnt, int sum, int mn, int mx, double avg, int newVal) {
    labelCount2->setText("Количество: " + QString::number(cnt));
    labelSum->setText("Сумма: " + QString::number(sum));
    labelMin->setText("Минимум: " + QString::number(mn));
    labelMax->setText("Максимум: "       + QString::number(mx));
    labelAvg->setText("Среднее: "        + QString::number(avg, 'f', 4));

    if(newVal != -1) {
        QString current = textElements->toPlainText();
        if(!current.isEmpty()) current += ", ";
        current += QString::number(newVal);
        textElements->setPlainText(current);
        QTextCursor cursor = textElements->textCursor();
        cursor.movePosition(QTextCursor::End);
        textElements->setTextCursor(cursor);
    }
}

void MainWindow::onRun() {
    if(comboSeqType->currentIndex() == 0) {
        onStop();

        QString input = lineInput->text().trimmed();
        if(input.isEmpty()) {
            hasError = true;
            update();
            QMessageBox::warning(this, "Ошибка", "Введите числа!");
            return;
        }

        QStringList parts = input.split(',');
        int items_arr[10000];
        int cnt = 0;

        for(QString &p : parts) {
            bool ok;
            int val = p.trimmed().toInt(&ok);
            if(!ok) {
                hasError = true;
                update();
                QMessageBox::warning(this, "Ошибка",
                    "Некорректное значение: " + p.trimmed());
                return;
            }
            items_arr[cnt++] = val;
        }

        hasError = false;
        update();

        textElements->clear();
        LazySequence<int> seq(items_arr, cnt);
        Statistics<int> stats(seq, cnt);

        QString elems;
        for(int i=0; i<cnt; i++) {
            elems += QString::number(items_arr[i]);
            if(i < cnt-1) elems += ", ";
        }
        textElements->setPlainText(elems);

        labelCount2->setText("Количество: " + QString::number(stats.GetCount()));
        labelSum->setText("Сумма: "         + QString::number(stats.GetSum()));
        labelMin->setText("Минимум: "        + QString::number(stats.GetMin()));
        labelMax->setText("Максимум: "       + QString::number(stats.GetMax()));
        labelAvg->setText("Среднее: "        + QString::number(stats.GetAverage(), 'f', 4));
    }
    else {
        onStop();

        delete liveSequence; liveSequence = nullptr;
        delete liveStream;   liveStream   = nullptr;

        liveCount = liveSum = liveMin = liveMax = 0;
        processedCount = 0;

        textElements->clear();
        hasError = false;
        update();

        int genIndex = comboGen->currentIndex();

        // Фибоначчи: 0, 1, 1, 2, 3, 5, 8...
        if(genIndex == 0) {
            int seed[] = {0, 1};
            liveSequence = new LazySequence<int>(
                [](const ArraySequence<int>& c) {
                    size_t k = c.GetLength();
                    return c.Get(k-1) + c.Get(k-2);
                },
                seed, 2
            );
        }
        // Случайные числа 0–100
        else if(genIndex == 1) {
            int seed[] = {rand() % 100};
            liveSequence = new LazySequence<int>(
                [](const ArraySequence<int>& c) {
                    return rand() % 100;
                },
                seed, 1
            );
        }
        // Арифметическая прогрессия: 1, 2, 3, 4...
        else {
            int seed[] = {1};
            liveSequence = new LazySequence<int>(
                [](const ArraySequence<int>& c) {
                    return (int)c.GetLength() + 1;
                },
                seed, 1
            );
        }

        liveStream = new ReadOnlyStream<int>(liveSequence);
        btnRun->setEnabled(false);
        btnStop->setEnabled(true);
        timer->start();
    }
}

void MainWindow::onTimerTick() {
    if(!liveStream) { onStop(); return; }

    int val = liveStream->Read();
    processedCount++;

    if(liveCount == 0) { liveMin = val; liveMax = val; }
    liveCount++;
    liveSum += val;
    if(val < liveMin) liveMin = val;
    if(val > liveMax) liveMax = val;
    double avg = (double)liveSum / liveCount;

    updateResult(liveCount, liveSum, liveMin, liveMax, avg, val);
}

void MainWindow::onStop() {
    timer->stop();
    btnRun->setEnabled(true);
    btnStop->setEnabled(false);
}

void MainWindow::onCheckTest() {
    QString input = lineTestData->text().trimmed();
    if(input.isEmpty()) {
        hasError = true;
        update();
        QMessageBox::warning(this, "Ошибка", "Введите числа для теста!");
        return;
    }

    QStringList parts = input.split(',');
    int items_arr[10000];
    int cnt = 0;
    for(QString &p : parts) {
        bool ok;
        int val = p.trimmed().toInt(&ok);
        if(!ok) {
            hasError = true;
            update();
            QMessageBox::warning(this, "Ошибка",
                "Некорректное значение: " + p.trimmed());
            return;
        }
        items_arr[cnt++] = val;
    }

    LazySequence<int> seq(items_arr, cnt);
    Statistics<int> stats(seq, cnt);

    QString result = "";
    bool allPassed = true;

    auto check = [&](const QString& name, QLineEdit* line, double actual) {
        QString expStr = line->text().trimmed();
        if(expStr.isEmpty()) return;
        bool ok;
        double exp = expStr.toDouble(&ok);
        if(!ok) {
            result += "⚠️  " + name + ": некорректное ожидаемое значение\n";
            allPassed = false;
            return;
        }
        double diff = actual - exp;
        if(diff < 0) diff = -diff;
        if(diff < 0.001) {
            result += "✅  " + name + ": ожидалось " + QString::number(exp, 'f', 2) +
                      ", получилось " + QString::number(actual, 'f', 2) + "\n";
        } else {
            result += "❌  " + name + ": ожидалось " + QString::number(exp, 'f', 2) +
                      ", получилось " + QString::number(actual, 'f', 2) + "\n";
            allPassed = false;
        }
    };

    check("Количество", lineExpCount, (double)stats.GetCount());
    check("Сумма",      lineExpSum,   (double)stats.GetSum());
    check("Минимум",    lineExpMin,   (double)stats.GetMin());
    check("Максимум",   lineExpMax,   (double)stats.GetMax());
    check("Среднее",    lineExpAvg,   stats.GetAverage());

    result += "\n";
    if(allPassed) {
        result += "Все проверки пройдены!";
        hasError = false;
    }
    else {
        result += "Есть несовпадения.";
        hasError = true;
    }
    update();
    textTestResult->setPlainText(result);
}


QString MainWindow::mainStyle() {
    return R"(
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

        QLabel {
            color: white;
        }

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

        QPushButton:hover {
            background: rgba(100,100,100,200);
        }

        QPushButton:pressed {
            background: rgba(40,40,40,200);
        }

        QPushButton:disabled {
            background: rgba(40,40,40,120);
            color: rgba(255,255,255,60);
        }
    )";
}