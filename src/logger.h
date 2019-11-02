#pragma once

#include <QStringList>
#include <QAbstractListModel>

class Logger : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int lineCount READ lineCount WRITE setLineCount NOTIFY lineCountChanged)

    enum Role {
        TextRole = Qt::UserRole + 1
    };

public:
    using QAbstractListModel::QAbstractListModel;
    ~Logger() override = default;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void install();
    void uninstall();

    void add(const QString &line);

    void setLineCount(int lineCount);
    int lineCount() const;

signals:
    void lineCountChanged(int lineCount);

private:
    QStringList m_lines;
    int m_lineCount = 256;
};
