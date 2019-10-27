#include "controller.h"

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

void Controller::goToNotesList(Player *player, bool playerIsNew)
{
    m_currentPage = NotesList;
    m_viewedPlayer = player;
    m_viewedPlayerIsNew = playerIsNew;
    m_editedNote = nullptr;
    m_menuModel->setButtons({{"Back", 50}, {"Name", 120}, {"Add", 200}});
    emit currentPageChanged();
}

void Controller::goToNoteEdit(Note *note)
{
    m_currentPage = NoteEdit;
    m_editedNote = note;
    m_editedNoteOriginalText = note->text();
    m_editedNoteOriginalCategory = note->category()->toInt();
    m_menuModel->setButtons({{"Cancel", 50}, {"Category", 120}, {"Save", 200}});
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
            goToNotesList(m_database->addNewPlayer(), true);
            emit showEditPlayerName();
        }
        break;
    }
    case NotesList: {
        if (index == 0)
            goToPlayersList();
        else if (index == 1) {
            emit showEditPlayerName();
        } else if (index == 2) {
            goToNoteEdit(m_viewedPlayer->newNote());
            emit showCategorySelector(m_editedNote->category());
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
        // save
        else if (index == 2) {
            // remove, if empty
            if (m_editedNote->text().isEmpty())
                m_viewedPlayer->removeNote(m_editedNote);
            goToNotesList(m_viewedPlayer);
        }
        break;
    }
    }
}

void Controller::playerNameEntered(bool accept)
{
    // only care about this, if we are adding a new player right now
    if (!m_currentPage == NotesList || !m_viewedPlayerIsNew)
        return;

    const bool nameEmpty = m_viewedPlayer->firstName().isEmpty() && m_viewedPlayer->lastName().isEmpty();
    if (nameEmpty || !accept) {
        m_database->removePlayer(m_viewedPlayer);
        goToPlayersList();
    }
}
