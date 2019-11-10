#include "controller.h"
#include "androidutil.h"

#include <QDebug>

Controller::Controller(Database *database, QObject *parent)
    : QObject(parent)
    , m_database(database)
    , m_menuModel(new MenuModel(this))
{
    goToPlayersList();
}

Controller::~Controller()
{
}

void Controller::goToPlayersList()
{
    m_currentPage = PlayersList;
    m_viewedPlayer = nullptr;
    m_editedNote = nullptr;
    m_menuModel->setButtons({{"Add", 210}});
    emit currentPageChanged();
}

void Controller::goToPlayerView(Player *player)
{
    m_currentPage = PlayerView;
    m_viewedPlayer = player;
    m_editedNote = nullptr;
    m_menuModel->setButtons({{"Back", 25}, {"Name", 90}, {"Del.", 150}, {"Add", 210}});
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

bool Controller::onBackButton()
{
    switch (m_currentPage) {
    case PlayersList: {
        return false;
    }
    case PlayerView: {
        goToPlayersList();
        return true;
    }
    case PlayerNameEdit: {
        cancelPlayerEditName();
        return true;
    }
    case NoteEdit: {
        emit endNoteEditing();
        return true;
    }
    default:
        return true;
    }
}

void Controller::cancelPlayerEditName()
{
    m_viewedPlayer->setFirstName(m_playerNameBeforeEdit.first);
    m_viewedPlayer->setLastName(m_playerNameBeforeEdit.second);
    if (m_viewedPlayerIsNew) {
        m_database->removePlayer(m_viewedPlayer);
        goToPlayersList();
    } else {
        goToPlayerView(m_viewedPlayer);
    }
}

void Controller::menuClicked(int index)
{
    if (isVerificationOpen()) {
        // ignore menu click while verification overlay is open
        return;
    }

    switch (m_currentPage) {
    case PlayersList: {
        if (index == 0) {
            goToPlayerNameEdit(m_database->addNewPlayer(), true);
        }
        break;
    }
    case PlayerView: {
        // Back
        if (index == 0) {
            goToPlayersList();
        }
        // Player Name
        else if (index == 1) {
            goToPlayerNameEdit(m_viewedPlayer, false);
        }
        // Delete Player
        else if (index == 2) {
            m_verification = DeletePlayer;
            emit currentPageChanged();
        }
        // Add Note
        else if (index == 3) {
            goToNoteEdit(m_viewedPlayer->newNote());
            emit showCategorySelector(m_editedNote->category());
        }
        break;
    }
    case PlayerNameEdit: {
        // back
        if (index == 0) {
            cancelPlayerEditName();
        }
        // OK
        else if (index == 1) {
            goToPlayerView(m_viewedPlayer);
            emit m_database->playerNamesChanged();
            emit saveData();
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
            goToPlayerView(m_viewedPlayer);
        }
        // category selectah
        else if (index == 1) {
            emit showCategorySelector(m_editedNote->category());
        }
        // delete
        else if (index == 2) {
            m_verification = DeleteNote;
            emit currentPageChanged();
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
        goToPlayerView(m_viewedPlayer);

        emit saveData();
    }
}

QString Controller::verificationText() const
{
    switch (m_verification) {
    case None: return "";
    case DeleteNote: return "Delete Note?";
    case DeletePlayer: return "Delete Player?";
    default: return "";
    }
}

bool Controller::isVerificationOpen() const
{
    return (m_verification != None);
}

void Controller::verificationClicked(bool ok)
{
    switch (m_verification) {
    case DeleteNote: {
        if (ok) {
            m_viewedPlayer->removeNote(m_editedNote);
            goToPlayerView(m_viewedPlayer);
            emit saveData();
        }
        break;
    }
    case DeletePlayer: {
        if (ok) {
            m_database->removePlayer(m_viewedPlayer);
            goToPlayersList();
            emit saveData();
        }
        break;
    }
    default: break;
    }

    m_verification = None;
    emit currentPageChanged();
}
