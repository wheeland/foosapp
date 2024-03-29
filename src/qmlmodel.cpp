#include "qmlmodel.h"

#include <QDebug>

// -----------------------------------------
//        Category
// -----------------------------------------
Category::Primary Category::primary() const
{
    return m_primary;
}

Category::Technique Category::technique() const
{
    return m_technique;
}

Category::SubTechnique Category::subTechnique() const
{
    return m_subTechnique;
}

int Category::toInt() const
{
    int ret = 0;
    ret += ((int) m_primary) << 16;
    ret += ((int) m_technique) << 8;
    ret += ((int) m_subTechnique) << 0;
    return ret;
}

void Category::fromInt(int i)
{
    setPrimary((Primary) ((i >> 16) & 0xFF));
    setTechnique((Technique) ((i >> 8) & 0xFF));
    setSubTechnique((SubTechnique) ((i >> 0) & 0xFF));
}

QString Category::label() const
{
    if (m_primary == PrimNone)
        return "Uncategorized";

    QString ret = primaryName(m_primary);
    if (m_technique != TechNone) {
        ret += ": ";
        ret += techniqueName(m_technique);

        if (m_subTechnique != SubTechNone) {
            ret += ": ";
            ret += subTechniqueName(m_subTechnique);
        }
    }

    return ret;
}

bool Category::isTechniqueValid(Category::Primary primary, Category::Technique technique)
{
    if (primary != PrimFiveBarOffensive && primary != PrimTwoBarOffensive)
        return false;
    return true;
}

bool Category::isSubTechniqueValid(Primary primary, Category::Technique technique, Category::SubTechnique subTechnique)
{
    if (primary != PrimFiveBarOffensive && primary != PrimTwoBarOffensive)
        return false;

    if (technique == TechNone || technique == TechPull || technique == TechPush || technique >= TechCount)
        return false;

    return true;
}

QVector<int> Category::validPrimaries()
{
    return QVector<int> {
        PrimNone,
        PrimThreeBarOffensive,
        PrimFiveBarOffensive,
        PrimFiveBarDefensive,
        PrimTwoBarOffensive,
        PrimTwoToFive,
        PrimMindeset,
    };
}

QVector<int> Category::validTechniques(Category::Primary primary)
{
    if (primary == PrimThreeBarOffensive || primary == PrimTwoBarOffensive) {
        return QVector<int> {
            TechNone,
            TechPinShot,
            TechJet,
            TechPull,
            TechPush,
            TechBackpin,
            TechTicTac
        };
    }

    return QVector<int>{ TechNone };
}

QVector<int> Category::validSubTechniques(Category::Primary primary, Category::Technique technique)
{
    if (technique == TechNone)
        return QVector<int>{ SubTechNone };

    if (primary == PrimThreeBarOffensive || primary == PrimTwoBarOffensive) {
        return QVector<int> {
            SubTechNone,
            SubTechLeft,
            SubTechHalfLeft,
            SubTechMidle,
            SubTechHalfRight,
            SubTechRight,
            SubTechWandering
        };
    }

    return QVector<int>{ SubTechNone };
}

QString Category::primaryName(Category::Primary primary)
{
    const QStringList primaries {
        "None", "3 Offensive", "5 Offensive", "5 Defensive", "2 Offensive", "2-to-5", "Mindset"
    };
    return primaries.value((int) primary);
}

QString Category::techniqueName(Category::Technique technique)
{
    const QStringList techniques {
        "None", "Pin-Shot", "Jet", "Pull", "Push", "Back-Pin", "Tic-Tac"
    };
    return techniques.value((int) technique);
}

QString Category::subTechniqueName(Category::SubTechnique subTechnique)
{
    const QStringList subtechniques {
        "None", "Left", "Half-Left", "Middle", "Half-Right", "Right", "Wandering"
    };
    return subtechniques.value((int) subTechnique);
}

void Category::setPrimary(Category::Primary primary)
{
    if (m_primary == primary)
        return;

    m_primary = primary;
    emit primaryChanged(m_primary);
    emit labelChanged();
}

void Category::setTechnique(Category::Technique technique)
{
    if (m_technique == technique)
        return;

    m_technique = technique;
    emit techniqueChanged(m_technique);
    emit labelChanged();
}

void Category::setSubTechnique(Category::SubTechnique subTechnique)
{
    if (m_subTechnique == subTechnique)
        return;

    m_subTechnique = subTechnique;
    emit subTechniqueChanged(m_subTechnique);
    emit labelChanged();
}

// -----------------------------------------
//        Note
// -----------------------------------------
Note::Note(QString text, QObject *parent)
    : QObject(parent)
    , m_text(text)
    , m_category(new Category(this))
{
}

Note::Note(QObject *parent)
    : QObject(parent)
    , m_category(new Category(this))
{
}

QString Note::text() const
{
    return m_text;
}

Category *Note::category() const
{
    return m_category;
}

// -----------------------------------------
//        Player
// -----------------------------------------
Player::Player(QObject *parent)
    : QAbstractListModel(parent)
{
}

int Player::rowCount(const QModelIndex &parent) const
{
    return m_notes.size();
}

QVariant Player::data(const QModelIndex &index, int role) const
{
    const int idx = index.row();
    if (idx < 0 || idx >= m_notes.size()) {
        qWarning() << "Player: invalid note index" << idx;
        return QVariant();
    }

    switch (role) {
    case NoteRole:
        return QVariant::fromValue(m_notes[idx]);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> Player::roleNames() const
{
    QHash<int, QByteArray> ret;
    ret[NoteRole] = "note";
    return ret;
}

QString Player::firstName() const
{
    return m_firstName;
}

QString Player::lastName() const
{
    return m_lastName;
}

void Player::setFirstName(QString firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged(m_firstName);
}

void Player::setLastName(QString lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged(m_lastName);
}

QDateTime Player::lastUpdate() const
{
    return m_lastUpdate;
}

void Player::setLastUpdate(const QDateTime &time)
{
    if (m_lastUpdate != time) {
        m_lastUpdate = time;
        Q_EMIT lastUpdateChanged(time);
    }
}

void Player::touchLastUpdate()
{
    setLastUpdate(QDateTime::currentDateTime());
}

Note *Player::newNote()
{
    Note *note = new Note(this);

    // make sure to inform sorting models
    connect(note->category(), &Category::primaryChanged, this, &Player::categoriesChanged);
    connect(note->category(), &Category::techniqueChanged, this, &Player::categoriesChanged);
    connect(note->category(), &Category::subTechniqueChanged, this, &Player::categoriesChanged);

    beginInsertRows(QModelIndex(), m_notes.size(), m_notes.size());
    m_notes << note;
    endInsertRows();
    return note;
}

void Player::removeNote(Note *note)
{
    const int idx = m_notes.indexOf(note);
    if (idx < 0) {
        qWarning() << "No Such note in Player";
        return;
    }

    beginRemoveRows(QModelIndex(), idx, idx);
    m_notes.remove(idx);
    endRemoveRows();

    note->deleteLater();
}

QVector<Note *> Player::notes() const
{
    return m_notes;
}

NotesSortModel::NotesSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(this, &QSortFilterProxyModel::sourceModelChanged, this, [=]() {
        sort(0);

        Player *player = qobject_cast<Player*>(sourceModel());
        if (player) {
            connect(player, &Player::categoriesChanged, this, [=]() {
                invalidate();
                sort(0);
            });
        }
    });
}

bool NotesSortModel::lessThan(const QModelIndex &lhs, const QModelIndex &rhs) const
{
    Note *left = sourceModel()->data(lhs, Player::NoteRole).value<Note*>();
    Note *right = sourceModel()->data(rhs, Player::NoteRole).value<Note*>();
    if (!left || !right)
        return false;
    return left->category()->toInt() <= right->category()->toInt();
}

void NotesSortModel::doSort()
{
    sort(0);
}

// -----------------------------------------
//        FoosController
// -----------------------------------------
Database::Database(const DataModel::Model_V0 &model_v0, QObject *parent)
    : QAbstractListModel(parent)
{
    const auto initPlayer = [&](Player *player, const DataModel::Notes &notes) {
        for (const QPair<int, QString> &nt : notes) {
            Note *note = player->newNote();
            note->category()->fromInt(nt.first);
            note->setText(nt.second);
        }
    };

    m_myself = new Player();
    initPlayer(m_myself, model_v0.myself);
    m_training = new Player();
    initPlayer(m_training, model_v0.training);

    for (auto it = model_v0.players.begin(); it != model_v0.players.end(); ++it) {
        Player *player = addNewPlayer();
        initPlayer(player, it.value());
        player->setFirstName(it.key().firstName);
        player->setLastName(it.key().lastName);
        player->setLastUpdate(it.key().lastUpdate);
    }
}

int Database::rowCount(const QModelIndex &parent) const
{
    return m_players.size();
}

QVariant Database::data(const QModelIndex &index, int role) const
{
    const int idx = index.row();
    if (idx < 0 || idx >= m_players.size()) {
        qWarning() << "FoosController: invalid player index" << idx;
        return QVariant();
    }

    switch (role) {
    case PlayerRole:
        return QVariant::fromValue(m_players[idx]);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> Database::roleNames() const
{
    QHash<int, QByteArray> ret;
    ret[PlayerRole] = "player";
    return ret;
}

DataModel::Model_V0 Database::toModel_V0() const
{
    DataModel::Model_V0 ret;

    const auto addNotes = [&](Player *player) -> DataModel::Notes {
        DataModel::Notes ret;
        const auto notes = player->notes();
        for (Note *note : notes)
            ret << qMakePair(note->category()->toInt(), note->text());
        return ret;
    };

    ret.myself = addNotes(m_myself);
    ret.training = addNotes(m_training);
    for (Player *player : m_players) {
        const DataModel::Player p {
            player->firstName(),
            player->lastName(),
            player->lastUpdate()
        };
        ret.players[p] = addNotes(player);
    }

    return ret;
}

Player *Database::addNewPlayer()
{
    Player *player = new Player(this);
    beginInsertRows(QModelIndex(), m_players.size(), m_players.size());
    m_players << player;
    endInsertRows();
    return player;
}

void Database::removePlayer(Player *player)
{
    const int idx = m_players.indexOf(player);
    if (idx < 0) {
        qWarning() << "No such Player in Database";
        return;
    }

    beginRemoveRows(QModelIndex(), idx, idx);
    m_players.remove(idx);
    endRemoveRows();

    player->deleteLater();
}

void Note::setText(QString text)
{
    if (m_text == text)
        return;

    m_text = text;
    emit textChanged(m_text);
}

PlayersSortModel::PlayersSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(this, &QSortFilterProxyModel::sourceModelChanged, this, [=]() {
        Database *db = qobject_cast<Database*>(sourceModel());
        if (db)
            connect(db, &Database::playerNamesChanged, this, &PlayersSortModel::doSort);
        doSort();
    });
}

bool PlayersSortModel::lessThan(const QModelIndex &lhs, const QModelIndex &rhs) const
{
    Player *left = sourceModel()->data(lhs, Database::PlayerRole).value<Player*>();
    Player *right = sourceModel()->data(rhs, Database::PlayerRole).value<Player*>();
    if (!left || !right)
        return false;
    return (left->firstName() + left->lastName()) <= (right->firstName() + right->lastName());
}

void PlayersSortModel::doSort()
{
    invalidate();
    sort(0);
}
