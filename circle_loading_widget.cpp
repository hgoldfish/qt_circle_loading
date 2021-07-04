#include <QtGui/qpainter.h>
#include <QtWidgets/qapplication.h>
#include "circle_loading_widget.h"


CircleLoadingWidget::CircleLoadingWidget(QWidget *parent)
    : QWidget(parent)
    , background(Qt::white)
    , timerId(0)
{
}


CircleLoadingWidget::~CircleLoadingWidget()
{
}


void CircleLoadingWidget::setColors(const QList<QColor> &colors)
{
    this->colors = colors;
    if (timerId) {
        update();
    }
}


QColor CircleLoadingWidget::getColor(int i)
{
    if (colors.size() > i) {
        return colors.at(i);
    } else {
        switch (i) {
        case 0:
            return QColor("#dc322f");
        case 1:
            return QColor("#268bd2");
        default:
            return QColor("#2aa198");
        }
    }
}


void CircleLoadingWidget::setBackground(const QBrush &background)
{
    this->background = background;
    if (timerId) {
        update();
    }
}


void CircleLoadingWidget::start()
{
    if (!timerId) {
        timerId = startTimer(1000 / 60);
        timer.restart();
    }
}


void CircleLoadingWidget::stop()
{
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}


void CircleLoadingWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    qint64 msecs = timer.elapsed();
    msecs %= 5000;
    bool inverted;
    if (msecs > 2500) {
        msecs = msecs - 2500;
        inverted = true;
    } else {
        inverted = false;
    }
    QEasingCurve headCurve(QEasingCurve::OutQuart);
    QEasingCurve tailCurve(QEasingCurve::InQuad);

    QRect r0 = rect();
    if (r0.width() > r0.height()) {
        r0.setWidth(r0.height());
    } else {
        r0.setHeight(r0.width());
    }
    r0.moveCenter(rect().center());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(background);
    painter.setPen(background.color());
    painter.drawRect(rect());
    for (int i = 0; i < 10; ++i) {
        qreal k1 = qMin(msecs / (1000.0 + 1000.0 / 10 * i), 1.0);
        qreal k2 = qMin(msecs / (1500.0 + 1000.0 / 10 * i), 1.0);
        if (qFuzzyCompare(k1, k2)) {
            continue;
        }

        qreal headAngle, tailAngle, spanAngle;
        if (inverted) {
            headAngle = 90 - headCurve.valueForProgress(k1) * 360;
            tailAngle = 90 - tailCurve.valueForProgress(k2) * 360;
        } else {
            headAngle = headCurve.valueForProgress(k1) * 360 + 90;
            tailAngle = tailCurve.valueForProgress(k2) * 360 + 90;
        }
        spanAngle =  tailAngle - headAngle;

        QRect r1 = r0 ;
        r1.setSize(r0.size() * (0.1 * (10 - i)));
        r1.moveCenter(r0.center());
        QRect r2 = r0;
        r2.setSize(r0.size() * (0.1 * (9 - i) + 0.01));
        r2.moveCenter(r0.center());

        qreal l1 = 1.0 - headCurve.valueForProgress(k1);
        qreal l3 = tailCurve.valueForProgress(k1);
        qreal l2 = 1.0 - l1 - l3;

        painter.setBrush(getColor(0));
        painter.drawPie(r1, headAngle * 16, spanAngle * 16 * l1 * 0.9);
        painter.setBrush(background);
        painter.drawPie(r2, headAngle * 16, spanAngle * 16 * l1 * 0.9);

        painter.setBrush(getColor(1));
        painter.drawPie(r1, headAngle * 16 + spanAngle * 16 * l1, spanAngle * 16 * l2 * 0.9);
        painter.setBrush(background);
        painter.drawPie(r2, headAngle * 16 + spanAngle * 16 * l1, spanAngle * 16 * l2 * 0.9);

        painter.setBrush(getColor(2));
        painter.drawPie(r1, headAngle * 16 + spanAngle * 16 * (l1 + l2), spanAngle * 16 * l3 * 0.9);
        painter.setBrush(background);
        painter.drawPie(r2, headAngle * 16 + spanAngle * 16 * (l1 + l2), spanAngle * 16 * l3 * 0.9);
    }
}



void CircleLoadingWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != timerId) {
        QWidget::timerEvent(event);
        return;
    }
    update();
}
