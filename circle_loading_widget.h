#ifndef CIRCLELOADINGWIDGET_H
#define CIRCLELOADINGWIDGET_H

#include <QtCore/qtimer.h>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qeasingcurve.h>
#include <QtWidgets/qwidget.h>


class CircleLoadingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CircleLoadingWidget(QWidget *parent = nullptr);
    virtual ~CircleLoadingWidget() override;
public:
    void setColors(const QList<QColor> &colors);
    void setBackground(const QBrush &background);
    void start();
    void stop();
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void timerEvent(QTimerEvent *event) override;
private:
    QColor getColor(int i);
private:
    QList<QColor> colors;
    QBrush background;
    QElapsedTimer timer;
    int timerId;
};


#endif // CIRCLELOADINGWIDGET_H
