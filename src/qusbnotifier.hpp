#pragma once

/* Определитель подключенного USB устройства
* Правда работает только под Windows
*/

#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QObject>

class QUsbNotifier
        : public QObject
        , public QAbstractNativeEventFilter
{
    Q_OBJECT

signals:
    void attached();
    void detached();
    void changed();

public:
    QUsbNotifier(QApplication& app) : QObject(), QAbstractNativeEventFilter()
    {
        app.installNativeEventFilter(this);
    }

    virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) final
    {
        (void)result;
        (void)eventType;
#ifdef Q_OS_WINDOWS
        Message* msg = reinterpret_cast<Message*>(message);
        if (msg->message == wm_devicechange)
        {
            switch (msg->wParam)
            {
            case dbt_devnodes_changed:
                emit changed();
                break;
            case dbt_devicearrival:
                emit attached();
                break;
            case dbt_deviceremovecomplete:
                emit detached();
                break;
            }
        }
#endif  // Q_OS_WINDOWS
        return false;
    }

#if (Q_PROCESSOR_WORDSIZE == 8)
    typedef uint64_t _WPARAM;
    typedef int64_t _LPARAM;
    typedef unsigned long _DWORD;
    struct Message
    {
        void* hwnd;
        unsigned int message;
        uint64_t wParam;
        int64_t lParam;
        unsigned long time;
    };
#else
    struct Message
    {
        void* hwnd;        // HWND
        uint32_t message;  // UINT
        uint32_t wParam;   // WPARAM
        uint32_t lParam;   // LPARAM
        uint32_t time;     // DWORD
    };
#endif  // Q_PROCESSOR_WORDSIZE

#ifdef Q_OS_WINDOWS

#ifdef WM_DEVICECHANGE
    static constexpr unsigned int wm_devicechange = WM_DEVICECHANGE;
#else
    static constexpr unsigned int wm_devicechange = 0x0219;
#endif

#ifdef DBT_DEVNODES_CHANGED
    static constexpr unsigned int dbt_devnodes_changed = DBT_DEVNODES_CHANGED;
#else
    static constexpr unsigned int dbt_devnodes_changed = 0x0007;
#endif

#ifdef DBT_DEVICEARRIVAL
    static constexpr unsigned int dbt_devicearrival = DBT_DEVICEARRIVAL;
#else
    static constexpr unsigned int dbt_devicearrival = 0x8000;
#endif

#ifdef DBT_DEVICEREMOVECOMPLETE
    static constexpr unsigned int dbt_deviceremovecomplete = DBT_DEVICEREMOVECOMPLETE;
#else
    static constexpr unsigned int dbt_deviceremovecomplete = 0x8004;
#endif

#endif  // Q_OS_WINDOWS
};
