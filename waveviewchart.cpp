#include "waveviewchart.h"

WaveViewChart::WaveViewChart(QGraphicsItem *parent, Qt::WindowFlags wFlags) :
    QChart(parent, wFlags),
    m_axisX(new QValueAxis),
    m_axisY(new QValueAxis),
    m_currX(0.0),
    m_timer(QTimer()),
    m_seriesList({}),
    m_phaseList({})
{
    /// Set up axes
    m_axisX->setTickCount(10);
    m_axisX->setRange(0, 10);
    this->addAxis(m_axisX, Qt::AlignBottom);
    m_axisY->setRange(-5, 5);
    this->addAxis(m_axisY, Qt::AlignLeft);

    /// Set up series
    auto seriesColorPhasePairs = QList<std::tuple<QString, Qt::GlobalColor, qreal>>{
        {"IC", Qt::red,     QRandomGenerator::global()->bounded(5.0)},
        {"IB", Qt::green,   QRandomGenerator::global()->bounded(5.0)},
        {"IA", Qt::blue,    QRandomGenerator::global()->bounded(5.0)},
        {"VC", Qt::magenta, QRandomGenerator::global()->bounded(5.0)},
        {"VB", Qt::yellow,  QRandomGenerator::global()->bounded(5.0)},
        {"VA", Qt::cyan,    QRandomGenerator::global()->bounded(5.0)}
    };
    for (qsizetype i = 0; i < seriesColorPhasePairs.size(); ++i)
    {
        const auto &[name, color, phase] = seriesColorPhasePairs[i];

        /// 1. Create series
        auto series = new QSplineSeries(this);
        m_seriesList.append(series);
        m_phaseList.append(phase);
        series->setPen(QPen(color, 1));
        series->append(m_currX, getY(i));
        series->setName(name);

        /// 2. Add series to the chart
        this->addSeries(series);

        /// 3. Attach series to axes
        series->attachAxis(m_axisX);
        series->attachAxis(m_axisY);
    }

    /// Set up timer
    QObject::connect(&m_timer, &QTimer::timeout, this, &WaveViewChart::handleTimeout);
    m_timer.setInterval(500); // Signal every 0.5s
    m_timer.start();

    this->setAnimationOptions(QChart::AllAnimations);
}

WaveViewChart::~WaveViewChart()
{
    delete m_axisX;
    delete m_axisY;
    for (auto &series : m_seriesList)
    {
        delete series;
    }
}

void WaveViewChart::handleTimeout()
{
    m_currX += stepFactor;
    for (qsizetype i = 0; i < m_seriesList.size(); ++i)
    {
        m_seriesList[i]->append(m_currX, getY(i));
    }

    if (m_seriesList.first()->count() + 1e-9 < m_axisX->tickCount() / stepFactor) return;

    auto scrollX = (this->plotArea().width() / m_axisX->tickCount()) * stepFactor;
    this->scroll(scrollX, 0.0);
}

qreal WaveViewChart::getY(qsizetype seriesIndex)
{
//    return QRandomGenerator::global()->bounded(5.0) - 2.5;
    return qSin(m_currX + m_phaseList[seriesIndex]);
}
