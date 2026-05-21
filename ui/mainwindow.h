#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QTimer>
#include <QPainter>
#include <QApplication>
#include "lazy_sequence.hpp"
#include "stream.hpp"
#include "statistics.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onSeqTypeChanged(int index);
    void onRun();
    void onTimerTick();
    void onStop();
    void onCheckTest();

private:
    QTabWidget  *tabs;

    QWidget *tabStats;
    QLabel *labelSeqType;
    QComboBox *comboSeqType;

    QGroupBox *groupFinite;
    QLabel *labelInput;
    QLineEdit *lineInput;

    QGroupBox *groupInfinite;
    QLabel *labelGen;
    QComboBox *comboGen;
    QLabel *labelInfo;

    QPushButton *btnRun;
    QPushButton *btnStop;

    QGroupBox *groupResult;
    QLabel *labelCount;
    QLabel *labelSum;
    QLabel *labelMin;
    QLabel *labelMax;
    QLabel *labelAvg;
    QLabel *labelElemsTitle;
    QTextEdit *textElements;

    QTimer *timer;
    size_t processedCount = 0;
    Statistics<int> liveStats;


    QWidget *tabTests;

    QGroupBox *groupTestInput;
    QLabel *labelTestData;
    QLineEdit *lineTestData;

    QLabel *labelExpCount; 
    QLineEdit *lineExpCount;
    QLabel *labelExpSum;
    QLineEdit *lineExpSum;
    QLabel *labelExpMin;
    QLineEdit *lineExpMin;
    QLabel *labelExpMax;
    QLineEdit *lineExpMax;
    QLabel *labelExpAvg;
    QLineEdit *lineExpAvg;
    QPushButton *btnCheck;
    QTextEdit *textTestResult;

    LazySequence<int>* liveSequence = nullptr;
    ReadOnlyStream<int>* liveStream = nullptr;
   
    bool hasError = false;

    void setupStatsTab();
    void setupTestsTab();
    void updateResult(const Statistics<int>& stats, int newVal = -1);
    QString mainStyle();
};