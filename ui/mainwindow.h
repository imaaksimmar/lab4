#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QTimer>
#include <QPainter>
#include <QApplication>
#include <QCoreApplication>
#include <QProcess>
#include "lazy_sequence.hpp"
#include "stream.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
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
    QLabel *labelCount2;
    QLabel *labelSum;
    QLabel *labelMin;
    QLabel *labelMax;
    QLabel *labelAvg;
    QLabel *labelElemsTitle;
    QTextEdit *textElements;

    QTimer *timer;
    int processedCount = 0;
    int liveCount = 0;  
    int liveMin = 0;
    int liveSum = 0;
    int liveMax = 0;

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
    void updateResult(int cnt, int sum, int mn, int mx, double avg, int newVal = -1);
    QString mainStyle();
};