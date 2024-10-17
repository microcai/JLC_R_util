
#pragma once

#include <QTimer>
#include "awaitable.hpp"

namespace mocutil
{
// The maximum Size of a string literal is 2 GB on 32-bit and 4 GB on 64-bit
// (but the compiler is likely to give up before you get anywhere near that much)
static constexpr size_t MaxStringSize =
        (std::min)(size_t((std::numeric_limits<uint>::max)()),
                   size_t((std::numeric_limits<qsizetype>::max)()));

template <uint... Nx> constexpr size_t stringDataSizeHelper(std::integer_sequence<uint, Nx...>)
{
    // same as:
    //   return (0 + ... + Nx);
    // but not using the fold expression to avoid exceeding compiler limits
    size_t total = 0;
    uint sizes[] = { Nx... };
    for (uint n : sizes)
        total += n;
    return total;
}

template <int Count, size_t StringSize> struct StringData
{
    static_assert(StringSize <= MaxStringSize, "Meta Object data is too big");
    uint offsetsAndSizes[Count] = {};
    char stringdata0[StringSize] = {};
    constexpr StringData() = default;
};

template <uint... Nx> constexpr auto stringData(const char (&...strings)[Nx])
{
    constexpr size_t StringSize = stringDataSizeHelper<Nx...>({});
    constexpr size_t Count = 2 * sizeof...(Nx);

    StringData<Count, StringSize> result;
    const char *inputs[] = { strings... };
    uint sizes[] = { Nx... };

    uint offset = 0;
    char *output = result.stringdata0;
    for (size_t i = 0; i < sizeof...(Nx); ++i) {
        // copy the input string, including the terminating null
        uint len = sizes[i];
        for (uint j = 0; j < len; ++j)
            output[offset + j] = inputs[i][j];
        result.offsetsAndSizes[2 * i] = offset + sizeof(result.offsetsAndSizes);
        result.offsetsAndSizes[2 * i + 1] = len - 1;
        offset += len;
    }

    return result;
}

}

namespace qtcoro
{
    using namespace ucoro;

    using ::coro_start;

    template<typename Func>
    class MyCallableQObject : public QObject
    {
        struct qt_meta_stringdata_CLASSqtcoroSCOPEMyCallableQObjectENDCLASS_t {};
        static constexpr auto qt_meta_stringdata_CLASSqtcoroSCOPEMyCallableQObjectENDCLASS = mocutil::stringData(
            "qtcoro::MyCallableQObject",
            "call",
            ""
        );

        static constexpr uint qt_meta_data_CLASSqtcoroSCOPEMyCallableQObjectENDCLASS[] = {
        // content:
            12,       // revision
            0,       // classname
            0,    0, // classinfo
            1,   14, // methods
            0,    0, // properties
            0,    0, // enums/sets
            0,    0, // constructors
            0,       // flags
            0,       // signalCount

        // slots: name, argc, parameters, tag, flags, initial metatype offsets
            1,    0,   20,    2, 0x0a,    1 /* Public */,

        // slots: parameters
            QMetaType::Void,

            0        // eod
        };

    public:
        static void qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
        {
            if (_c == QMetaObject::InvokeMetaMethod) {
                auto *_t = reinterpret_cast<MyCallableQObject *>(_o);
                (void)_t;
                switch (_id) {
                case 0: _t->call(); break;
                default: ;
                }
            }
            (void)_a;
        }

        virtual const QMetaObject *metaObject() const
        {
            static QMetaObject staticMetaObject = { {
                QMetaObject::SuperData::link<QObject::staticMetaObject>(),
                qt_meta_stringdata_CLASSqtcoroSCOPEMyCallableQObjectENDCLASS.offsetsAndSizes,
                qt_meta_data_CLASSqtcoroSCOPEMyCallableQObjectENDCLASS,
                qt_static_metacall,
                nullptr,
                qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSqtcoroSCOPEMyCallableQObjectENDCLASS_t,
                    // Q_OBJECT / Q_GADGET
                    QtPrivate::TypeAndForceComplete<MyCallableQObject, std::true_type>,
                    // method 'call_slot'
                    QtPrivate::TypeAndForceComplete<void, std::false_type>
                >,
                nullptr
            } };

            return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
        }

        virtual void* qt_metacast(const char *_clname)
        {
            if (!_clname) return nullptr;
            if (!strcmp(_clname, qt_meta_stringdata_CLASSqtcoroSCOPEMyCallableQObjectENDCLASS.stringdata0))
                return static_cast<void*>(this);
            return QObject::qt_metacast(_clname);
        }

        virtual int qt_metacall(QMetaObject::Call _c, int _id, void **_a)
        {
            _id = QObject::qt_metacall(_c, _id, _a);
            if (_id < 0)
                return _id;
            if (_c == QMetaObject::InvokeMetaMethod) {
                if (_id < 1)
                    qt_static_metacall(this, _c, _id, _a);
                _id -= 1;
            } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
                if (_id < 1)
                    *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
                _id -= 1;
            }
            return _id;
        }

    private:

        struct QPrivateSignal {
            explicit QPrivateSignal() = default;
        };

    public slots:
        void call()
        {
            deleteLater();
            func();
        }
    public:

        MyCallableQObject(Func&& f) : func(std::forward<Func>(f)){}

        Func func;
    };

    template <typename INT>
    awaitable<void> coro_delay_ms(INT ms)
    {
        co_await callback_awaitable<void>([ms](auto continuation)
        {
            auto reciver = new MyCallableQObject<decltype(continuation)>(std::move(continuation));
            QTimer::singleShot(std::chrono::milliseconds(ms), reciver, SLOT(call()));
        });
    }

} // namespace qtcoro

