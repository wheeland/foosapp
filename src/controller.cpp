#include "controller.h"

#include <QDebug>

Controller::Controller(Database *database, QObject *parent)
    : QObject(parent)
    , m_database(database)
    , m_menuModel(new MenuModel(this))
{
    goToStartPage();
}

Controller::~Controller()
{
}

void Controller::goToStartPage()
{
    m_currentPage = StartPage;
    m_viewedPlayer = nullptr;
    m_editedNote = nullptr;
    m_menuModel->setButtons({{"Player List", 50}});
    emit currentPageChanged();
}

void Controller::goToPlayersList()
{
    m_currentPage = PlayersList;
    m_viewedPlayer = nullptr;
    m_editedNote = nullptr;
    m_menuModel->setButtons({{"Back", 50}, {"Add", 200}});
    emit currentPageChanged();
}

void Controller::goToNotesList(Player *player)
{
    m_currentPage = PlayerView;
    m_viewedPlayer = player;
    m_editedNote = nullptr;
    m_menuModel->setButtons({{"Back", 50}, {"Name", 110}, {"Add", 210}});
    emit currentPageChanged();
}

void Controller::goToPlayerNameEdit(Player *player, bool newPlayer)
{
    m_currentPage = PlayerNameEdit;
    m_viewedPlayer = player;
    m_viewedPlayerIsNew = newPlayer;
    m_playerNameBeforeEdit = qMakePair(player->firstName(), player->lastName());
    m_editedNote = nullptr;
    m_menuModel->setButtons({{"Back", 50}, {"OK", 210}});
    emit currentPageChanged();
}

void Controller::goToNoteEdit(Note *note)
{
    m_currentPage = NoteEdit;
    m_editedNote = note;
    m_editedNoteOriginalText = note->text();
    m_editedNoteOriginalCategory = note->category()->toInt();
    m_menuModel->setButtons({{"Back", 25}, {"Cat.", 90}, {"Del.", 150}, {"Save", 210}});
    emit currentPageChanged();
}

void Controller::menuClicked(int index)
{
    switch (m_currentPage) {
    case StartPage: {
        if (index == 0)
            goToPlayersList();
        break;
    }
    case PlayersList: {
        if (index == 0)
            goToStartPage();
        else if (index == 1) {
            goToPlayerNameEdit(m_database->addNewPlayer(), true);
        }
        break;
    }
    case PlayerView: {
        if (index == 0)
            goToPlayersList();
        else if (index == 1) {
            goToPlayerNameEdit(m_viewedPlayer, false);
        } else if (index == 2) {
            goToNoteEdit(m_viewedPlayer->newNote());
            emit showCategorySelector(m_editedNote->category());
        }
        break;
    }
    case PlayerNameEdit: {
        // back
        if (index == 0) {
            m_viewedPlayer->setFirstName(m_playerNameBeforeEdit.first);
            m_viewedPlayer->setLastName(m_playerNameBeforeEdit.second);
            if (m_viewedPlayerIsNew) {
                m_database->removePlayer(m_viewedPlayer);
                goToPlayersList();
            } else {
                goToNotesList(m_viewedPlayer);
            }
        }
        // OK
        else if (index == 1) {
            goToNotesList(m_viewedPlayer);
        }
        break;
    }
    case NoteEdit: {
        // cancel
        if (index == 0) {
            // reset note
            m_editedNote->setText(m_editedNoteOriginalText);
            m_editedNote->category()->fromInt(m_editedNoteOriginalCategory);
            // remove, if empty
            if (m_editedNote->text().isEmpty())
                m_viewedPlayer->removeNote(m_editedNote);
            goToNotesList(m_viewedPlayer);
        }
        // category selectah
        else if (index == 1) {
            emit showCategorySelector(m_editedNote->category());
        }
        // delete
        else if (index == 2) {
            m_viewedPlayer->removeNote(m_editedNote);
            goToNotesList(m_viewedPlayer);
        }
        // save
        else if (index == 3) {
            // we will have to get control back to QML, so that it can
            // un-set the focus from the TextEdit, so that the inputMethod stops
            // composing and we can use the 'text' property. then we'll get called
            // in noteEdited()
            emit endNoteEditing();
        }
        break;
    }
    }
}

void Controller::noteEdited(const QString &text)
{
    // we get called after the save button has been hit
    if (m_currentPage == NoteEdit && m_editedNote) {
        m_editedNote->setText(text);
        if (m_editedNote->text().isEmpty())
            m_viewedPlayer->removeNote(m_editedNote);
        goToNotesList(m_viewedPlayer);
    }
}
