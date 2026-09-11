#include <QApplication>
#include <QBoxLayout>
#include <QPainterPath>
#include <QPushButton>

#include <qpaintbox.hpp>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.setWindowTitle(APP_NAME " v" APP_VERSION);

    QVBoxLayout* main_layout = new QVBoxLayout(&w);
    w.setLayout(main_layout);
    QPaintBox* paintbox = new QPaintBox(&w);
    main_layout->addWidget(paintbox);
    paintbox->setMinimumWidth(640);
    paintbox->setMinimumHeight(480);

    QList<QPoint> path_points;
    bool mouse_pressed = false;

    QWidget::connect(paintbox,
                     &QPaintBox::mousePressed,
                     &w,
                     [&path_points, &mouse_pressed, paintbox](QMouseEvent* event)
                     {
                         path_points.clear();
                         if (event->button() == Qt::LeftButton)
                         {
                             mouse_pressed = true;
                             path_points.append(event->pos());
                         }
                     });

    QWidget::connect(paintbox,
                     &QPaintBox::mouseMoving,
                     &w,
                     [&path_points, &mouse_pressed, paintbox](QMouseEvent* event)
                     {
                         if (mouse_pressed)
                         {
                             path_points.append(event->pos());
                             paintbox->update();
                         }
                     });

    QWidget::connect(paintbox,
                     &QPaintBox::mouseReleased,
                     &w,
                     [&path_points, &mouse_pressed, paintbox](QMouseEvent* event)
                     {
                         mouse_pressed = false;
                         paintbox->update();
                     });

    QWidget::connect(paintbox,
                     &QPaintBox::painting,
                     &w,
                     [&path_points, paintbox](QPainter* painter)
                     {
                         paintbox->drawFrame(painter);
                         if (!path_points.empty())
                         {
                             QPainterPath path;
                             path.moveTo(path_points.first());
                             foreach (const auto& point, path_points)
                             {
                                 path.lineTo(point);
                             }
                             painter->drawPath(path);
                         }
                     });

    //
    QHBoxLayout* button_layout = new QHBoxLayout(&w);
    main_layout->addLayout(button_layout);

    QPushButton* close_button = new QPushButton("Close", &w);
    QWidget::connect(close_button, &QPushButton::clicked, &w, &QWidget::close);
    button_layout->addStretch();
    button_layout->addWidget(close_button);
    button_layout->addStretch();

    w.show();
    return a.exec();
}
