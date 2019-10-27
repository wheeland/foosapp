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

public:
    Controller(Database *database, QObject *parent = nullptr);
    ~Controller();

    enum Page
    {
        StartPage,
        PlayersList,
        NotesList,
        NoteEdit,
        CategorySelector,
    };
    Q_ENUM(Page)

    MenuModel *menuModel() const { return m_menuModel.data(); }
    Database *database() const { return m_database; }
    Player *viewedPlayer() const { return m_viewedPlayer; }
    Note *editedNote() const { return m_editedNote; }
    Page currentPage() const{ return m_currentPage; }

    Q_INVOKABLE void menuClicked(int index);
    Q_INVOKABLE void goToStartPage();
    Q_INVOKABLE void goToPlayersList();
    Q_INVOKABLE void goToNotesList(Player *player);
    Q_INVOKABLE void goToNoteEdit(Note *note);

signals:
    void currentPageChanged();
    void showCategorySelector(Category *category);
    void showEditPlayerName();

private:
    Database *m_database;
    QScopedPointer<MenuModel> m_menuModel;

    Page m_currentPage = StartPage;
    Player *m_viewedPlayer = nullptr;

    Note *m_editedNote = nullptr;
    QString m_editedNoteOriginalText;
    int m_editedNoteOriginalCategory;
};
