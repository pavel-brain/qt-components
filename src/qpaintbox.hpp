#pragma once

#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QWidget>

//-----------------------------------------------------------------------------
class QPaintBox : public QWidget
{
    Q_OBJECT

public:
    QPaintBox(QWidget* parent = 0) : QWidget(parent)
    {
        setMouseTracking(true);

        m_option_frame.init(this);
        m_option_frame.state |= QStyle::State_Sunken;
        m_option_frame.frameShape = QFrame::StyledPanel;
        m_option_frame.lineWidth =
            style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &m_option_frame, this);
        m_option_frame.midLineWidth = 0;
    }

    void drawFrame(QPainter* painter)
    {
        painter->save();
        m_option_frame.rect = rect();
        style()->drawPrimitive(QStyle::PE_Frame, &m_option_frame, painter, this);
        painter->restore();

        // Заполняем рабочую область
        painter->fillRect(m_option_frame.lineWidth,
                          m_option_frame.lineWidth,
                          width() - 2 * m_option_frame.lineWidth,
                          height() - 2 * m_option_frame.lineWidth,
                          QBrush(palette().color(QPalette::Base)));
    }

    int frameWidth() const
    {
        return m_option_frame.lineWidth;
    }

signals:
    void painting(QPainter* painter);
    void mousePressed(QMouseEvent* event);
    void mouseMoving(QMouseEvent* event);
    void mouseReleased(QMouseEvent* event);

protected:
    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, false);
        emit painting(&painter);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        emit mousePressed(event);
        if (!event->isAccepted())
            QWidget::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        emit mouseMoving(event);
        if (!event->isAccepted())
            QWidget::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        emit mouseReleased(event);
        if (!event->isAccepted())
            QWidget::mouseReleaseEvent(event);
    }

private:
    QStyleOptionFrame m_option_frame;
};
