#include "jsonformatter.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <algorithm>
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlaylist>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMediaPlaylist *playlist = new QMediaPlaylist;
    playlist->addMedia(QUrl("qrc:/sounds/plucky.mp3"));
    m_player.setPlaylist(playlist);

    m_stopTimer.setInterval(1000);
    connect(&m_stopTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    ui->sliderTimer->setMaximum(0);
    ui->sliderTimer->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimeout()
{
    m_time = m_time.addSecs(1);
    checkTimerLap();
    updateUi();
}

void MainWindow::on_pbStart_clicked()
{
    if (m_stopTimer.isActive()) {
        // pause
        m_stopTimer.stop();
    } else {
        m_stopTimer.start();
        if (!m_curSession) {
            m_curSession = new Stoptimer;
        }
    }
}

void MainWindow::on_pbStop_clicked()
{
    // reset
    m_stopTimer.stop();
    m_time.setHMS(0, 0, 0);
    updateUi();
    ui->tableWidget->setRowCount(0);
    ui->sliderTimer->setMaximum(0);

    m_time2Index.clear();
    if (m_curSession) {
        delete m_curSession;
        m_curSession = nullptr;
    }
}

void MainWindow::on_pbLap_clicked()
{
    int secs = QTime(0, 0, 0).secsTo(m_time);
    if (m_curSession) {
        TimerLap *lap = new TimerLap(secs, m_time.toString("hh_mm_ss"));
        m_curSession->addLap(lap);
        refreshTable();
    }
}

void MainWindow::updateUi()
{
    int secs = QTime(0, 0, 0).secsTo(m_time);
    if (ui->sliderTimer->maximum() < secs) {
        ui->sliderTimer->setMaximum(secs);
        ui->lblTotalTime->setText(m_time.toString("hh:mm:ss"));
    }
    ui->sliderTimer->setValue(secs);
    ui->lcdHours->display(m_time.toString("hh"));
    ui->lcdMins->display(m_time.toString("mm"));
    ui->lcdSecs->display(m_time.toString("ss"));
}

void MainWindow::refreshTable()
{
    m_time2Index.clear();
    ui->tableWidget->setRowCount(0);
    if (m_curSession) {
        QList<int> times = m_curSession->timerLaps().keys();
        std::sort(times.begin(), times.end());

        for (int i = 0; i < times.size(); ++i) {
            TimerLap * lap = m_curSession->timerLaps().value(times.at(i));
            ui->tableWidget->insertRow(i);
            m_time2Index.insert(times.at(i), i);

            QTableWidgetItem *itTime = new QTableWidgetItem(QTime(0, 0, 0).addSecs(lap->time()).toString("hh:mm:ss"));
            itTime->setFlags(itTime->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 0, itTime);

            QTableWidgetItem *itSecs = new QTableWidgetItem(QString::number(lap->time()));
            itSecs->setFlags(itSecs->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(i, 1, itSecs);

            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(lap->label()));
        }
    }
}

void MainWindow::checkTimerLap()
{
    int secs = QTime(0, 0, 0).secsTo(m_time) + 1;
    if (m_curSession->timerLaps().contains(secs)) {
        // warning user
        displayRowAsPassed(m_time2Index.value(secs));
        playSound();
    }

}

void MainWindow::displayRowAsPassed(int row)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        for (int i = 0; i < ui->tableWidget->columnCount(); ++i) {
            ui->tableWidget->item(row, i)->setTextColor(Qt::gray);
        }
    }
}

void MainWindow::playSound()
{
    m_player.setVolume(100);
    m_player.play();
}

bool MainWindow::saveToFile(const QString &path, const QByteArray &bytes)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << file.errorString();
        return false;
    }
    file.write(bytes);
    file.close();
    return true;
}

QByteArray MainWindow::readFileBytes(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return QByteArray();
    }
    return file.readAll();
}

void MainWindow::on_actionExport_triggered()
{
    if (m_curSession) {
        QJsonObject obj = JsonFormatter::makeJsonStoptimer(m_curSession);
        QJsonDocument doc(obj);
        QByteArray bytes = doc.toJson();

        QString file = QFileDialog::getSaveFileName(this, tr("Export file"), "D:/stoptimer.json", tr("JSON Files (*.json)"),
                                                    nullptr, QFileDialog::DontUseNativeDialog);
        if (saveToFile(file, bytes)) {
            QMessageBox::information(this, tr("Info"), tr("Export file done!"));
        }
    }
}

void MainWindow::on_actionImport_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open JSON"), "D:/", tr("JSON Files (*.json *.txt)"),
                                                nullptr, QFileDialog::DontUseNativeDialog);
    if (file.isEmpty()) {
        return;
    }
    qDebug() << "import file:" << file;

    on_pbStop_clicked();    // reset data
    QByteArray bytes = readFileBytes(file);
    if (!bytes.isEmpty()) {
        QJsonObject obj = QJsonDocument::fromJson(bytes).object();
        m_curSession = JsonFormatter::getStopTimerFromJson(obj);
        refreshTable();

        int maxTime = 0;
        if (ui->tableWidget->rowCount() > 0) {
            maxTime = ui->tableWidget->item(ui->tableWidget->rowCount()-1, 1)->text().toInt();
        }
        ui->lblTotalTime->setText(QTime(0,0,maxTime).toString("hh:mm:ss"));
        ui->sliderTimer->setMaximum(maxTime);
    }
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if (m_curSession && item->column() == 2) {
        int time = ui->tableWidget->item(item->row(), 1)->text().toInt();
        TimerLap * const lap = m_curSession->timerLaps().value(time);
        if (lap) {
            lap->setLabel(item->text());
            m_curSession->addLap(lap);
        }
    }
}

void MainWindow::on_pbRemove_clicked()
{
    if (m_curSession) {
        int r = ui->tableWidget->currentRow();
        int time = ui->tableWidget->item(r, 1)->text().toInt();
        m_curSession->removeLap(time);
        refreshTable();
    }
}

void MainWindow::on_pbClear_clicked()
{
    if (m_curSession) {
        m_curSession->clearLaps();
        refreshTable();
    }
}
