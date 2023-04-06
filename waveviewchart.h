#ifndef WAVEVIEWCHART_H
#define WAVEVIEWCHART_H

#include <QtCharts>
#include <QtMath>

class WaveViewChart : public QChart
{
public:
    explicit WaveViewChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    ~WaveViewChart();

    void handleTimeout();

private:
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_currX;
    QTimer m_timer;
    QList<QSplineSeries *> m_seriesList;
    QList<qreal> m_phaseList;

    qreal getY(qsizetype seriesIndex);

    static constexpr qreal stepFactor = 0.5;
};

#endif // WAVEVIEWCHART_H
