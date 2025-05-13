#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), customPlot(new QCustomPlot(this)) {
    setCentralWidget(customPlot);
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->xAxis->setLabel("Distance");
    customPlot->yAxis->setLabel("Height");
}

void MainWindow::updatePlot(const QVector<TrajectoryPoint>& points) {
    QVector<double> x, y;
    for (const auto& point : points) {
        x.append(point.distance);
        y.append(point.height);
    }

    customPlot->graph(0)->setData(x, y);
    customPlot->xAxis->rescale();
    customPlot->yAxis->rescale();
    customPlot->replot();
}
