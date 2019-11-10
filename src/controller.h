#pragma once

#include <QObject>
#include <QAbstractListModel>

#include "qmlmodel.h"
#include "menumodel.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MenuModel *menuModel READ menuModel CONSTANT)
    Q_PROPERTY(Page currentPage READ currentPage NOTIFY currentPageChanged)
    Q_PROPERTY(Database *database READ database CONSTANT)
    Q_PROPERTY(Player *viewedPlayer READ viewedPlayer NOTIFY currentPageChanged)
    Q_PROPERTY(Note *editedNote READ editedNote NOTIFY currentPageChanged)

    Q_PROPERTY(bool verificationOpen READ isVerificationOpen NOTIFY currentPageChanged)
    Q_PROPERTY(QString verificationText READ verificationText NOTIFY currentPageChanged)

public:
    Controller(Database *database, QObject *parent = nullptr);
    ~Controller();

    enum Page
    {
        PlayersList,
        PlayerView,
        PlayerNameEdit,
        NoteEdit,
    };
    Q_ENUM(Page)

    MenuModel *menuModel() const { return m_menuModel.data(); }
    Database *database() const { return m_database; }
    Player *viewedPlayer() const { return m_viewedPlayer; }
    Note *editedNote() const { return m_editedNote; }
    Page currentPage() const{ return m_currentPage; }

    Q_INVOKABLE void menuClicked(int index);
    Q_INVOKABLE void noteEdited(const QString &text);

    QString verificationText() const;
    bool isVerificationOpen() const;
    Q_INVOKABLE void verificationClicked(bool ok);

    void goToPlayersList();
    void goToPlayerNameEdit(Player *player, bool newPlayer);

    Q_INVOKABLE void goToPlayerView(Player *player);
    Q_INVOKABLE void goToNoteEdit(Note *note);

    bool onBackButton();

signals:
    void currentPageChanged();
    void endNoteEditing();
    void showCategorySelector(Category *category);
    void saveData();

private:
    void cancelPlayerEditName();

    Database *m_database;
    QScopedPointer<MenuModel> m_menuModel;

    Page m_currentPage = PlayersList;

    enum VerificationPopup {
        None,
        DeleteNote,
        DeletePlayer,
    } m_verification = None;

    QPair<QString, QString> m_playerNameBeforeEdit;
    Player *m_viewedPlayer = nullptr;
    bool m_viewedPlayerIsNew = false;

    Note *m_editedNote = nullptr;
    QString m_editedNoteOriginalText;
    int m_editedNoteOriginalCategory = 0;
};
