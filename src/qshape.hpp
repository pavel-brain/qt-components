#pragma once

#include <QBrush>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPushButton>
#include <QWidget>

class QShape : public QWidget
{
    Q_OBJECT

public:
    enum QShapeType
    {
        Rectangle,
        Ellipse,
        RoundRect,
        Square,
        Circle,
        RoundSquare,
        HorizontalHexagon,
        VerticalHexagon,
        Octagon,
    };

    QShape(const QShapeType& shape_type, QWidget* parent = 0)
            : QWidget(parent), m_alignment(Qt::AlignCenter), m_shape_type(shape_type)
    {
        if (!m_standard_height)
        {
            m_standard_height = QPushButton().sizeHint().height();
        }
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_brush.setColor(Qt::white);
        m_brush.setStyle(Qt::SolidPattern);
        m_pen.setColor(Qt::black);
        m_pen.setStyle(Qt::SolidLine);
    }

    QShape(QWidget* parent = 0) : QShape(QShape::Circle, parent) {}

    QSize sizeHint() const override
    {
        return QSize(m_standard_height, m_standard_height);
    }

    void setShapeBrush(const QBrush& brush)
    {
        m_brush = brush;
        update();
    }
    QBrush shapeBrush() const
    {
        return m_brush;
    }
    void setShapePen(const QPen& pen)
    {
        m_pen = pen;
        update();
    }
    QPen shapePen() const
    {
        return m_pen;
    }
    void setShapeType(const QShapeType& type)
    {
        m_shape_type = type;
        update();
    }
    QShapeType shapeType() const
    {
        return m_shape_type;
    }
    void setText(const QString& text)
    {
        m_text = text;
        update();
    }
    QString text() const
    {
        return m_text;
    }
    void setFont(const QFont& font)
    {
        m_font = font;
        update();
    }
    QFont font() const
    {
        return m_font;
    }
    void setAlignment(Qt::Alignment flag)
    {
        m_alignment = flag;
        update();
    }
    Qt::Alignment alignment() const
    {
        return m_alignment;
    }

protected:
    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.setPen(isEnabled() ? m_pen : QPen(Qt::darkGray));
        painter.setBrush(isEnabled() ? m_brush : QBrush(Qt::lightGray));

        qreal half_pen = painter.pen().width() / 2.;
        QRectF r = QRectF(rect()).adjusted(half_pen, half_pen, -half_pen, -half_pen);
        QPointF center = r.center();
        switch (m_shape_type)
        {
        default:
        case QShape::Rectangle:
            painter.drawRect(r);
            break;
        case QShape::Ellipse:
            painter.drawEllipse(r);
            break;
        case QShape::RoundRect:
        {
            qreal radius = std::min(r.width(), r.height()) / 4;
            painter.drawRoundedRect(r, radius, radius);
            break;
        }
        case QShape::Square:
            (r.height() > r.width()) ? r.setHeight(r.width()) : r.setWidth(r.height());
            r.moveCenter(center);
            painter.drawRect(r);
            break;
        case QShape::Circle:
            (r.height() > r.width()) ? r.setHeight(r.width()) : r.setWidth(r.height());
            r.moveCenter(center);
            painter.drawEllipse(r);
            break;
        case QShape::RoundSquare:
        {
            (r.height() > r.width()) ? r.setHeight(r.width()) : r.setWidth(r.height());
            r.moveCenter(center);
            qreal radius = std::min(r.width(), r.height()) / 4;
            painter.drawRoundedRect(r, radius, radius);
            break;
        }
        case QShape::Octagon:
        {
            (r.height() > r.width()) ? r.setHeight(r.width()) : r.setWidth(r.height());
            r.moveCenter(center);
            QPainterPath path;
            qreal offset = r.width() / (m_sqrt2 + 2);
            path.moveTo(r.left(), r.top() + offset);
            path.lineTo(r.left() + offset, r.top());
            path.lineTo(r.right() - offset, r.top());
            path.lineTo(r.right(), r.top() + offset);
            path.lineTo(r.right(), r.bottom() - offset);
            path.lineTo(r.right() - offset, r.bottom());
            path.lineTo(r.left() + offset, r.bottom());
            path.lineTo(r.left(), r.bottom() - offset);
            path.lineTo(r.left(), r.top() + offset);
            painter.drawPath(path);
            break;
        }
        case QShape::HorizontalHexagon:
        {
            qreal a = std::min(r.width() / 2, r.height() / m_sqrt3);
            r.setSize(QSizeF(a * 2, a * m_sqrt3));
            r.moveCenter(center);
            QPainterPath path;
            path.moveTo(r.left(), r.top() + r.height() / 2);
            path.lineTo(r.left() + a / 2, r.top());
            path.lineTo(r.right() - a / 2, r.top());
            path.lineTo(r.right(), r.top() + r.height() / 2);
            path.lineTo(r.right() - a / 2, r.bottom());
            path.lineTo(r.left() + a / 2, r.bottom());
            path.lineTo(r.left(), r.top() + r.height() / 2);
            painter.drawPath(path);
            break;
        }
        case QShape::VerticalHexagon:
        {
            qreal a = std::min(r.width() / m_sqrt3, r.height() / 2);
            r.setSize(QSizeF(a * m_sqrt3, a * 2));
            r.moveCenter(center);
            QPainterPath path;
            path.moveTo(r.left(), r.top() + a / 2);
            path.lineTo(r.left() + r.width() / 2, r.top());
            path.lineTo(r.right(), r.top() + a / 2);
            path.lineTo(r.right(), r.bottom() - a / 2);
            path.lineTo(r.left() + r.width() / 2, r.bottom());
            path.lineTo(r.left(), r.bottom() - a / 2);
            path.lineTo(r.left(), r.top() + a / 2);
            painter.drawPath(path);
            break;
        }
        }

        if (!m_text.isEmpty())
        {
            painter.setFont(m_font);
            painter.setPen(Qt::black);
            painter.drawText(rect(), m_alignment, m_text);
        }
    }

    virtual void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            emit clicked();
        }
    }
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            emit doubleClicked();
        }
    }

signals:
    void clicked();
    void doubleClicked();

public slots:
    void clear()
    {
        m_text.clear();
        m_brush.setColor(Qt::white);
        m_pen.setColor(Qt::black);
        update();
    }

    void success()
    {
        m_brush.setColor(Qt::green);
        update();
    }
    void fail()
    {
        m_brush.setColor(Qt::red);
        update();
    }
    void warning()
    {
        m_brush.setColor(Qt::yellow);
        update();
    }

private:
    static inline int m_standard_height = 0;
    static inline const qreal m_sqrt2 = sqrt(2);
    static inline const qreal m_sqrt3 = sqrt(3);

    QBrush m_brush;
    QPen m_pen;
    QShapeType m_shape_type;
    QString m_text;
    QFont m_font;
    Qt::Alignment m_alignment;
};
