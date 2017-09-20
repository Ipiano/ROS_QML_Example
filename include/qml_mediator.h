#include <QObject>
#include <QVariant>
#include <QDebug>

class QMLMediator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList strings READ getStrings NOTIFY stringsChanged);
    QVariantList m_strings;

public:
    QMLMediator(QObject* parent = nullptr){}

    QVariantList getStrings()
    {
        return m_strings;
    }

    void addString(QString s)
    {
        m_strings.append(s);
        stringsChanged();
    }

signals:
    void stringsChanged();
};