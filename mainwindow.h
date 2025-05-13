#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "trajectorypoint.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    void updatePlot(const QVector<TrajectoryPoint>& points);

private:
    QCustomPlot* customPlot;
};

#endif // MAINWINDOW_H
