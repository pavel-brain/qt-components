#include <QApplication>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>

#include <qusbnotifier.hpp>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.setWindowTitle(APP_NAME " v" APP_VERSION);

    QVBoxLayout* main_layout = new QVBoxLayout(&w);
    QPlainTextEdit* text_log = new QPlainTextEdit(&w);
    main_layout->addWidget(text_log);

    QHBoxLayout* button_layout = new QHBoxLayout(&w);
    QPushButton* close_button = new QPushButton("Close", &w);
    QWidget::connect(close_button, &QPushButton::clicked, &w, &QWidget::close);
    button_layout->addStretch();
    button_layout->addWidget(close_button);
    button_layout->addStretch();
    main_layout->addLayout(button_layout);
    w.setLayout(main_layout);

    QUsbNotifier notifier(a);
    QObject::connect(&notifier,
                     &QUsbNotifier::changed,
                     &w,
                     [text_log] { text_log->appendPlainText("USB list changed"); });
    QObject::connect(&notifier,
                     &QUsbNotifier::attached,
                     &w,
                     [text_log] { text_log->appendPlainText("USB attached"); });
    QObject::connect(&notifier,
                     &QUsbNotifier::detached,
                     &w,
                     [text_log] { text_log->appendPlainText("USB detached"); });

    w.show();
    return a.exec();
}
