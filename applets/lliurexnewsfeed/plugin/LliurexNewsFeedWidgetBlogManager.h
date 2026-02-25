#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QVariantList>
#include <QLocale>

class LliurexNewsFeedWidgetBlogManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList feedData READ feedData NOTIFY feedDataChanged)
    Q_PROPERTY(QString currentLanguage READ currentLanguage CONSTANT)

public:
    explicit LliurexNewsFeedWidgetBlogManager(QObject *parent = nullptr) : QObject(parent) {
        // Detectar idioma y asignar URL
        QLocale locale;
        if (locale.name().startsWith("es")) {
            rssUrl = "https://portal.edu.gva.es/blogs/s1/lliurex/es/feed/";
        } else if (locale.name().startsWith("ca")) {
            rssUrl = "https://portal.edu.gva.es/blogs/s1/lliurex/feed/";
        }
    }

    Q_INVOKABLE void fetch() {
        QLocale locale;
        QUrl m_url;
        if (locale.name().startsWith("es")) {
            QUrl m_url("https://portal.edu.gva.es/blogs/s1/lliurex/es/feed/");
        } else if (locale.name().startsWith("ca")) {
            QUrl m_url("https://portal.edu.gva.es/blogs/s1/lliurex/feed/");
        }
        QNetworkRequest request(m_url);
        QNetworkReply *reply = m_networkManager.get(request);
        connect(reply, &QNetworkReply::finished, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                parse(reply->readAll());
            }
            reply->deleteLater();
        });
    }

    QVariantList feedData() const { return m_feedData; }
    QString currentLanguage() const { return QLocale().nativeLanguageName(); }

signals:
    void feedDataChanged();

private:
    QNetworkAccessManager m_networkManager;
    QVariantList m_feedData;
    QString rssUrl;

    void parse(const QByteArray &data) {
        m_feedData.clear();
        QXmlStreamReader xml(data);
        while (!xml.atEnd()) {
            if (xml.readNextStartElement() && xml.name() == "item") {
                QVariantMap item;
                while (xml.readNextStartElement()) {
                    if (xml.name() == "title") item["title"] = xml.readElementText();
                    else if (xml.name() == "link") item["link"] = xml.readElementText();
                    else xml.skipCurrentElement();
                }
                m_feedData.append(item);
            }
        }
        emit feedDataChanged();
    }
};
