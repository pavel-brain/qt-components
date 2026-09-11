#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScreen>

#include <qshape.hpp>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.setWindowTitle(APP_NAME " v" APP_VERSION);

    QVBoxLayout* main_layout = new QVBoxLayout(&w);
    w.setLayout(main_layout);

    //
    main_layout->addWidget(new QLabel("Different shapes:", &w));
    QHBoxLayout* shapes_layout = new QHBoxLayout(&w);
    main_layout->addLayout(shapes_layout);
    shapes_layout->addWidget(new QShape(QShape::Rectangle, &w));
    shapes_layout->addWidget(new QShape(QShape::Ellipse, &w));
    shapes_layout->addWidget(new QShape(QShape::RoundRect, &w));
    shapes_layout->addWidget(new QShape(QShape::Square, &w));
    shapes_layout->addWidget(new QShape(QShape::Circle, &w));
    shapes_layout->addWidget(new QShape(QShape::RoundSquare, &w));
    shapes_layout->addWidget(new QShape(QShape::Octagon, &w));
    shapes_layout->addWidget(new QShape(QShape::HorizontalHexagon, &w));
    shapes_layout->addWidget(new QShape(QShape::VerticalHexagon, &w));

    //
    main_layout->addWidget(new QLabel("Different states:", &w));
    QHBoxLayout* states_layout = new QHBoxLayout(&w);
    main_layout->addLayout(states_layout);

    states_layout->addWidget(new QLabel("Clear", &w));
    QShape* sh_clear = new QShape(&w);
    sh_clear->clear();
    states_layout->addWidget(sh_clear);

    states_layout->addWidget(new QLabel("Success", &w));
    QShape* sh_success = new QShape(&w);
    sh_success->success();
    states_layout->addWidget(sh_success);

    states_layout->addWidget(new QLabel("Fail", &w));
    QShape* sh_fail = new QShape(&w);
    sh_fail->fail();
    states_layout->addWidget(sh_fail);

    states_layout->addWidget(new QLabel("Warning", &w));
    QShape* sh_warning = new QShape(&w);
    sh_warning->warning();
    states_layout->addWidget(sh_warning);

    states_layout->addWidget(new QLabel("Disabled", &w));
    QShape* sh_disabled = new QShape(&w);
    sh_disabled->setEnabled(false);
    states_layout->addWidget(sh_disabled);

    //
    main_layout->addWidget(new QLabel("Support text, click, doubleClick:", &w));
    QHBoxLayout* text_layout = new QHBoxLayout(&w);
    main_layout->addLayout(text_layout);

    const QList<QColor> colors{
        Qt::red, Qt::darkYellow, Qt::yellow, Qt::green, Qt::blue, Qt::darkBlue, Qt::magenta};
    int click_color_index = 0;
    int dblclick_color_index = 0;

    QShape* sh_click = new QShape(&w);
    sh_click->setShapeType(QShape::RoundRect);
    sh_click->setText("Click");
    QObject::connect(sh_click,
                     &QShape::clicked,
                     &w,
                     [&click_color_index, colors, sh_click]
                     {
                         sh_click->setShapeBrush(colors[click_color_index++]);
                         click_color_index %= colors.count();
                     });
    text_layout->addWidget(sh_click);

    QShape* sh_dblclick = new QShape(&w);
    sh_dblclick->setShapeType(QShape::RoundRect);
    sh_dblclick->setText("Double Click");
    QObject::connect(sh_dblclick,
                     &QShape::doubleClicked,
                     &w,
                     [&dblclick_color_index, colors, sh_dblclick]
                     {
                         sh_dblclick->setShapeBrush(colors[dblclick_color_index++]);
                         dblclick_color_index %= colors.count();
                     });
    text_layout->addWidget(sh_dblclick);

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
