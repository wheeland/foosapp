#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QDateTime>

#include "controller.h"
#include "datamodel.h"

class Category : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Primary primary READ primary WRITE setPrimary NOTIFY primaryChanged)
    Q_PROPERTY(Technique technique READ technique WRITE setTechnique NOTIFY techniqueChanged)
    Q_PROPERTY(SubTechnique subTechnique READ subTechnique WRITE setSubTechnique NOTIFY subTechniqueChanged)
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)

public:
    enum Primary
    {
        PrimNone,
        PrimThreeBarOffensive,
        PrimFiveBarOffensive,
        PrimFiveBarDefensive,
        PrimTwoBarOffensive,
        PrimTwoToFive,
        PrimMindeset,
        PrimCount,
    };
    Q_ENUM(Primary)

    enum Technique
    {
        TechNone,
        TechPinShot,
        TechJet,
        TechPull,
        TechPush,
        TechBackpin,
        TechTicTac,
        TechCount
    };
    Q_ENUM(Technique)

    enum SubTechnique
    {
        SubTechNone,
        SubTechLeft,
        SubTechHalfLeft,
        SubTechMidle,
        SubTechHalfRight,
        SubTechRight,
        SubTechWandering,
        SubTechCount
    };
    Q_ENUM(SubTechnique)

    using QObject::QObject;
    ~Category() = default;

    Primary primary() const;
    Technique technique() const;
    SubTechnique subTechnique() const;

    int toInt() const;
    void fromInt(int i);

    QString label() const;

    Q_INVOKABLE static bool isTechniqueValid(Primary primary, Technique technique);
    Q_INVOKABLE static bool isSubTechniqueValid(Primary primary, Technique technique, SubTechnique subTechnique);

    Q_INVOKABLE static QVector<int> validPrimaries();
    Q_INVOKABLE static QVector<int> validTechniques(Primary primary);
    Q_INVOKABLE static QVector<int> validSubTechniques(Primary primary, Technique technique);

    Q_INVOKABLE static QString primaryName(Primary primary);
    Q_INVOKABLE static QString techniqueName(Technique technique);
    Q_INVOKABLE static QString subTechniqueName(SubTechnique subTechnique);

public slots:
    void setPrimary(Primary primary);
    void setTechnique(Technique technique);
    void setSubTechnique(SubTechnique subTechnique);

signals:
    void primaryChanged(Primary primary);
    void techniqueChanged(Technique technique);
    void subTechniqueChanged(SubTechnique subTechnique);
    void labelChanged();

private:
    Primary m_primary;
    Technique m_technique;
    SubTechnique m_subTechnique;
};

class Note : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Category *category READ category CONSTANT)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    Note(QString text, QObject *parent = nullptr);
    Note(QObject *parent = nullptr);
    ~Note() = default;

//    QString title() const { return m_title; }
    QString text() const;
    Category *category() const;

public slots:
    void setText(QString text);

signals:
    void textChanged(QString text);
//    void titleChanged(QString title);

private:
    QString m_text;
//    QString m_title;
    Category *m_category;
};



class Player : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString secondName READ secondName WRITE setSecondName NOTIFY secondNameChanged)
    Q_PROPERTY(QDateTime lastUpdate READ lastUpdate NOTIFY lastUpdateChanged)

    enum Role {
        NoteRole = Qt::UserRole + 1
    };

public:
    Player(QObject *parent = nullptr);
    ~Player() = default;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString firstName() const;
    QString secondName() const;
    QDateTime lastUpdate() const;

    void setLastUpdate(const QDateTime &time);
    Q_INVOKABLE void touchLastUpdate();

    Note *newNote();
    void removeNote(Note *note);
    QVector<Note*> notes() const;

public slots:
    void setFirstName(QString firstName);
    void setSecondName(QString secondName);

signals:
    void firstNameChanged(QString firstName);
    void secondNameChanged(QString secondName);
    void lastUpdateChanged(QDateTime lastUpdate);

private:
    QString m_firstName;
    QString m_secondName;
    QVector<Note*> m_notes;
    QDateTime m_lastUpdate;
};

class FoosController : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Player *training READ training CONSTANT)
    Q_PROPERTY(Player *myself READ myself CONSTANT)

    enum Role {
        PlayerRole = Qt::UserRole + 1
    };

public:
    FoosController(const DataModel::Model_V0 &model_v0, QObject *parent = nullptr);
    ~FoosController() = default;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    DataModel::Model_V0 toModel_V0() const;

    Player *training() const { return m_training; }
    Player *myself() const { return m_myself; }

private:
    QVector<Player*> m_players;
    Player *m_myself;
    Player *m_training;
};
