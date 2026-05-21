#include "mainwindow.h"
#include "resources.hpp"
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
    setWindowTitle(APP_TITLE);
    resize(640, 740);
    setStyleSheet(mainStyle());

    timer = new QTimer(this);
    timer->setInterval(400);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTick);

    tabs = new QTabWidget(this);
    tabs->setGeometry(0, 0, 640, 740);

    tabStats = new QWidget();
    tabTests = new QWidget();
    tabs->addTab(tabStats, TAB_STATS);
    tabs->addTab(tabTests, TAB_TESTS);

    setupStatsTab();
    setupTestsTab();
}


QString MainWindow::mainStyle() {
    return STYLE_MAIN;
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
    labelSeqType = new QLabel(TEXT_SEQ_TYPE);
    comboSeqType = new QComboBox();
    comboSeqType->addItem(TEXT_FINITE);
    comboSeqType->addItem(TEXT_INFINITE);
    comboSeqType->setMinimumWidth(280);
    comboSeqType->setStyleSheet(STYLE_COMBO);
    seqRow->addWidget(labelSeqType);
    seqRow->addWidget(comboSeqType);
    seqRow->addStretch();
    lay->addLayout(seqRow);

    groupFinite = new QGroupBox(TEXT_GROUP_FINITE);
    groupFinite->setStyleSheet(STYLE_GROUP_WHITE);
    QVBoxLayout *finLay = new QVBoxLayout(groupFinite);
    labelInput = new QLabel(TEXT_INPUT);
    lineInput = new QLineEdit();
    lineInput->setPlaceholderText(TEXT_PLACEHOLDER);
    finLay->addWidget(labelInput);
    finLay->addWidget(lineInput);
    lay->addWidget(groupFinite);

    groupInfinite = new QGroupBox(TEXT_GROUP_INFINITE);
    groupInfinite->setStyleSheet(STYLE_GROUP_WHITE);
    QVBoxLayout *infLay = new QVBoxLayout(groupInfinite);
    QHBoxLayout *genRow = new QHBoxLayout();
    labelGen = new QLabel(TEXT_GENERATOR);
    comboGen = new QComboBox();
    comboGen->setStyleSheet(STYLE_COMBO);
    comboGen->addItem(TEXT_FIBONACCI);
    comboGen->addItem(TEXT_RANDOM);
    comboGen->addItem(TEXT_ARITHMETIC);
    genRow->addWidget(labelGen);
    genRow->addWidget(comboGen);
    infLay->addLayout(genRow);

    labelInfo = new QLabel(TEXT_INFINITE_INFO);
    labelInfo->setStyleSheet(STYLE_LABEL_INFO);
    infLay->addWidget(labelInfo);
    groupInfinite->setVisible(false);
    lay->addWidget(groupInfinite);

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRun = new QPushButton(TEXT_RUN);
    btnStop = new QPushButton(TEXT_STOP);
    btnStop->setEnabled(false);
    btnRow->addWidget(btnRun);
    btnRow->addWidget(btnStop);
    lay->addLayout(btnRow);

    groupResult = new QGroupBox(TEXT_GROUP_RESULT);
    QVBoxLayout *resLay = new QVBoxLayout(groupResult);

    labelCount = new QLabel(TEXT_COUNT);
    labelSum = new QLabel(TEXT_SUM);
    labelMin = new QLabel(TEXT_MIN);
    labelMax = new QLabel(TEXT_MAX);
    labelAvg = new QLabel(TEXT_AVG);

    for(QLabel *l : {labelCount, labelSum, labelMin, labelMax, labelAvg})
    {
        l->setStyleSheet(STYLE_LABEL_RESULT);
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(6);
        shadow->setColor(Qt::black);
        shadow->setOffset(1, 1);
        l->setGraphicsEffect(shadow);

        resLay->addWidget(l);
    }

    labelElemsTitle = new QLabel(TEXT_ELEMENTS);
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

    groupTestInput = new QGroupBox(TEXT_GROUP_TEST);
    QFormLayout *form = new QFormLayout(groupTestInput);
    form->setSpacing(8);

    labelTestData = new QLabel(TEXT_TEST_DATA);
    lineTestData = new QLineEdit();
    lineTestData->setPlaceholderText(TEXT_PLACEHOLDER_TEST);
    form->addRow(labelTestData, lineTestData);

    labelExpCount = new QLabel(TEXT_EXP_COUNT);
    lineExpCount = new QLineEdit();
    lineExpCount->setPlaceholderText("5");
    form->addRow(labelExpCount, lineExpCount);

    labelExpSum = new QLabel(TEXT_EXP_SUM);
    lineExpSum = new QLineEdit();
    lineExpSum->setPlaceholderText("15");
    form->addRow(labelExpSum, lineExpSum);

    labelExpMin = new QLabel(TEXT_EXP_MIN);
    lineExpMin = new QLineEdit();
    lineExpMin->setPlaceholderText("1");
    form->addRow(labelExpMin, lineExpMin);

    labelExpMax = new QLabel(TEXT_EXP_MAX);
    lineExpMax = new QLineEdit();
    lineExpMax->setPlaceholderText("5");
    form->addRow(labelExpMax, lineExpMax);

    labelExpAvg = new QLabel(TEXT_EXP_AVG);
    lineExpAvg = new QLineEdit();
    lineExpAvg->setPlaceholderText("3.0");
    form->addRow(labelExpAvg, lineExpAvg);

    lay->addWidget(groupTestInput);

    btnCheck = new QPushButton(TEXT_CHECK);
    lay->addWidget(btnCheck);

    textTestResult = new QTextEdit();
    textTestResult->setReadOnly(true);
    textTestResult->setStyleSheet(STYLE_TEXT_RESULT);
    lay->addWidget(textTestResult);

    connect(btnCheck, &QPushButton::clicked, this, &MainWindow::onCheckTest);
}


void MainWindow::onSeqTypeChanged(int index) {
    groupFinite->setVisible(index==0);
    groupInfinite->setVisible(index==1);
    onStop();
    hasError = false;
    update();

    labelCount->setText(TEXT_COUNT);
    labelSum->setText(TEXT_SUM);
    labelMin->setText(TEXT_MIN);
    labelMax->setText(TEXT_MAX);
    labelAvg->setText(TEXT_AVG);
    textElements->clear();
}

////// поменяла теперь принимает Statistics<int> а не 5 отдельных переменных
void MainWindow::updateResult(const Statistics<int>& stats, int newVal) {
    labelCount->setText(TEXT_COUNT + QString::number(stats.GetCount()));
    labelSum->setText(TEXT_SUM + QString::number(stats.GetSum()));
    labelMin->setText(TEXT_MIN + QString::number(stats.GetMin()));
    labelMax->setText(TEXT_MAX + QString::number(stats.GetMax()));
    labelAvg->setText(TEXT_AVG + QString::number(stats.GetAverage(), 'f', 4));

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
    if(comboSeqType->currentIndex()==0) {
        onStop();

        QString input = lineInput->text().trimmed();
        if(input.isEmpty()) {
            hasError = true;
            update();
            QMessageBox::warning(this, ERR_TITLE, ERR_EMPTY_INPUT);
            return;
        }

        QStringList parts = input.split(',');
        ArraySequence<int> items; 
        for(QString &p : parts) {
            bool ok;
            int val = p.trimmed().toInt(&ok);
            if(!ok) {
                hasError = true;
                update();
                QMessageBox::warning(this, ERR_TITLE,
                    ERR_INVALID_VALUE + p.trimmed());
                return;
            }
            items.Append(val);
        }

        hasError = false;
        update();

        textElements->clear();
        LazySequence<int> seq(&items); 
        Statistics<int> stats(seq);

        QString elems;
        for(size_t i=0; i<items.GetLength(); i++) {
            elems += QString::number(items.Get(i));
            if(i<items.GetLength()-1) elems += ", ";
        }
        textElements->setPlainText(elems);
        updateResult(stats); 
    }

    else {
        onStop();

        delete liveSequence; liveSequence = nullptr;
        delete liveStream; liveStream   = nullptr;

        liveStats.Reset(); 
        processedCount = 0;

        textElements->clear();
        hasError = false;
        update();

        size_t genIndex = comboGen->currentIndex();

        if(genIndex==0) 
        {
            std::function<int(const ArraySequence<int>&)> fibonacciGenerator = [](const ArraySequence<int>& c) {
                size_t k = c.GetLength();
                return c.Get(k-1) + c.Get(k- 2);
            };
            int seed[] = {0, 1};
            size_t seedCount = 2;
            liveSequence = new LazySequence<int>(fibonacciGenerator, seed, seedCount);
        }

        else if(genIndex==1) 
        {
            std::function<int(const ArraySequence<int>&)> randomGenerator = [](const ArraySequence<int>& c) {
                return rand()%100;
            };
            int seed[] = {rand() % 100};
            size_t seedCount = 1;
            liveSequence = new LazySequence<int>(randomGenerator, seed, seedCount);
        }

        else 
        {
            std::function<int(const ArraySequence<int>&)> arithmeticGenerator = [](const ArraySequence<int>& c) {
                return (int)c.GetLength()+ 1;
            };
            int seed[] = {1};
            size_t seedCount = 1;
            liveSequence = new LazySequence<int>(arithmeticGenerator, seed, seedCount);
        }

        liveStream = new ReadOnlyStream<int>(*liveSequence);
        btnRun->setEnabled(false);
        btnStop->setEnabled(true);
        timer->start();
    }
}


void MainWindow::onTimerTick() {
    if(!liveStream) { onStop(); return; }

    int val = liveStream->Read();
    processedCount++;
    liveStats.Update(val);        
    updateResult(liveStats, val); 
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
        QMessageBox::warning(this, ERR_TITLE, ERR_EMPTY_TEST);
        return;
    }

    QStringList parts = input.split(',');
    ArraySequence<int> items;
    for(QString &p : parts) {
        bool ok;
        int val = p.trimmed().toInt(&ok);
        if(!ok) {
            hasError = true;
            update();
            QMessageBox::warning(this, ERR_TITLE,
                ERR_INVALID_VALUE + p.trimmed());
            return;
        }
        items.Append(val);
    }

    LazySequence<int> seq(&items);
    Statistics<int> stats(seq, items.GetLength());

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
        double diff = actual-exp;
        if(diff<0) diff = -diff;
        if(diff<0.001) {
            result += "✅  " + name + ": ожидалось " + QString::number(exp, 'f', 2) +
                      ", получилось " + QString::number(actual, 'f', 2) + "\n";
        }
        else {
            result += "❌  " + name + ": ожидалось " + QString::number(exp, 'f', 2) +
                      ", получилось " + QString::number(actual, 'f', 2) + "\n";
            allPassed = false;
        }
    };

    check("Количество", lineExpCount, (double)stats.GetCount());
    check("Сумма", lineExpSum, (double)stats.GetSum());
    check("Минимум", lineExpMin, (double)stats.GetMin());
    check("Максимум", lineExpMax, (double)stats.GetMax());
    check("Среднее", lineExpAvg, stats.GetAverage());

    result += "\n";
    if(allPassed) {
        result += TEST_PASSED;
        hasError = false;
    }
    else {
        result += TEST_FAILED;
        hasError = true;
    }
    update();
    textTestResult->setPlainText(result);
}


MainWindow::~MainWindow() {
    delete liveSequence;
    delete liveStream;
}
