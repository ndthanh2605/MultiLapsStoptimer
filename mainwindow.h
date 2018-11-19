#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QMediaPlayer>
#include <QTableWidget>
#include "Entity/stoptimer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTimeout();

    void on_pbStart_clicked();

    void on_pbStop_clicked();

    void on_pbLap_clicked();

    void on_actionExport_triggered();

    void on_actionImport_triggered();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pbRemove_clicked();

    void on_pbClear_clicked();

private:
    Ui::MainWindow *ui;

    QTime m_time = QTime(0, 0, 0);
    QTimer m_stopTimer;

    Stoptimer *m_curSession = nullptr;
    QMediaPlayer m_player;
    QHash<int, int> m_time2Index;       // lap's time - table index

    void updateUi();
    void refreshTable();
    void checkTimerLap();
    void displayRowAsPassed(int row);
    void playSound();

    bool saveToFile(const QString &path, const QByteArray &bytes);
    QByteArray readFileBytes(const QString &path);

};

#endif // MAINWINDOW_H
