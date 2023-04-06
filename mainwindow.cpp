#include "mainwindow.h"
#include "waveviewchart.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto chart = new WaveViewChart;
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    this->setCentralWidget(m_chartView);
    this->resize(400, 300);
}

MainWindow::~MainWindow()
{
    delete m_chartView;
}

