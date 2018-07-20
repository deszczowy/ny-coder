#include "nyprojectmodel.h"

#include <QDir>

NyProjectModel::NyProjectModel(const QString projectBasePath, QObject *parent)
    : QAbstractItemModel(parent)
{
    QDir directory(projectBasePath);
    _mainRoot = new NyProjectItem("Root", "", (NyProjectItem*)parent);
    _projectRoot = new NyProjectItem(projectBasePath, "", _mainRoot);
    _sideRoot = new NyProjectItem("$", "Other", _mainRoot);

    _mainRoot->AppendChild(_projectRoot);
    _mainRoot->AppendChild(_sideRoot);

    QStringList extensions;
    extensions << ".lsp" << ".lisp";

    BuildFilter(extensions);
    LoadProject(projectBasePath);
}

NyProjectModel::~NyProjectModel()
{
    delete _mainRoot;
}

QModelIndex NyProjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    NyProjectItem *parentItem;

    if (!parent.isValid())
        parentItem = _mainRoot;
    else
        parentItem = static_cast<NyProjectItem*>(parent.internalPointer());

    NyProjectItem *childItem = parentItem->Child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex NyProjectModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    NyProjectItem *childItem = static_cast<NyProjectItem*>(child.internalPointer());
    NyProjectItem *parentItem = childItem->Parent();

    if (parentItem == _mainRoot)
        return QModelIndex();

    return createIndex(parentItem->Row(), 0, parentItem);
}

int NyProjectModel::rowCount(const QModelIndex &parent) const
{
    NyProjectItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = _mainRoot;
    else
        parentItem = static_cast<NyProjectItem*>(parent.internalPointer());

    return parentItem->ChildCount();
}

int NyProjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant NyProjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());

    switch (role){
    case Qt::DisplayRole:
    { return item->FileName(); break; }

    case Qt::ToolTipRole:
    { return item->AbsolutePath(); break; }

    case Qt::DecorationRole:
    { return item->Icon(); break; }

    // user roles
    default:
        return QVariant();
    }
}

bool NyProjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());

    item->SetFile(value.toString(), _rootPath);

    emit dataChanged(index, index);
}

void NyProjectModel::LoadProject(QString path)
{
    _rootPath = QDir::cleanPath(path);
    Load(path, _projectRoot);
}

bool NyProjectModel::SaveAll()
{
    SaveRecursive(_mainRoot);
}

bool NyProjectModel::SaveEdited(NyEditor *editor)
{
    QModelIndex index = FindEdited(_mainRoot, editor);
    if (index.isValid()){
        NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());
        item->Save();
    }
}

bool NyProjectModel::SaveEditedAs(QString newPath, NyEditor *editor)
{
    QModelIndex index = FindEdited(_mainRoot, editor);
    if (index.isValid()){

        NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());
        item->Close();

        if (newPath.startsWith(_rootPath)){
            AddNewNode(newPath, editor);
        }
        else
        {
            NyProjectItem *item = new NyProjectItem(newPath, "", _sideRoot);
            item->Open(editor);
            item->Save();

            beginInsertRows(index, 0, 0);
            _sideRoot->AppendChild(item);
            endInsertRows();
        }
    }
}

bool NyProjectModel::RenameIndex(QModelIndex index, QString newName)
{
    if (index.isValid()){
        NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());
        item->Rename(newName);
    }

    return true;
}

bool NyProjectModel::CloseEdited(NyEditor *editor)
{
    QModelIndex index = FindEdited(_mainRoot, editor);
    if (index.isValid()){
        NyProjectItem *item = static_cast<NyProjectItem*>(index.internalPointer());
        item->Close();
    }
}

QString NyProjectModel::RootPath()
{
    return _rootPath;
}

QString NyProjectModel::ScriptToRun(NyEditor *editor)
{
    QModelIndex index = FindEdited(_mainRoot, editor);
    if (index.isValid()){
        return (static_cast<NyProjectItem*>(index.internalPointer()))->ScriptPath();
    }

    return "";
}

QString NyProjectModel::FileNameOfIndex(QModelIndex index)
{
    return (static_cast<NyProjectItem*>(index.internalPointer()))->FileName();
}

QModelIndex NyProjectModel::FindEdited(NyProjectItem *root, NyEditor *editor)
{
    QModelIndex index;

    for (int i = 0; i < root->ChildCount(); ++i){
        NyProjectItem *item = root->Child(i);

        if (item->ChildCount() > 0){
            index = FindEdited(item, editor);
        }

        if(!index.isValid()){
            if (item->HasEditor(editor)){
                index = createIndex(item->Row(), 0, item);
            }
        }

        if(index.isValid()) break;
    }

    return index;
}

void NyProjectModel::Load(QString localisation, NyProjectItem *parent)
{
    int offset = localisation.length() +1;

    QFileInfoList infos = QDir(localisation).entryInfoList(
        QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst
    );

    foreach (QFileInfo info, infos)
    {
        if (info.isDir())
        {
            NyProjectItem *child = new NyProjectItem(info.absoluteFilePath(), _rootPath, parent);
            parent->AppendChild(child);
            Load(info.absoluteFilePath(), child);
        }
        else
        {
            if(info.absoluteFilePath().contains(QRegExp(_filter)))
            {
                parent->AppendChild(new NyProjectItem(info.absoluteFilePath(), _rootPath, parent));
            }
        }
    }
}

void NyProjectModel::BuildFilter(QStringList &ext)
{
    _filter = QString(
        ext
            .replaceInStrings(".", "\\.")
            .join("|")
            .prepend(".*(")
            .append(")")
                );
}

void NyProjectModel::SaveRecursive(NyProjectItem *root)
{
    for (int i = 0; i < root->ChildCount(); ++i){
        NyProjectItem *item = root->Child(i);

        if (item->ChildCount() > 0){
            SaveRecursive(item);
        }

        item->Save();
    }
}

QModelIndex NyProjectModel::FindNode(QString nodeName, NyProjectItem *root)
{
    QModelIndex index;

    for (int i = 0; i < root->ChildCount(); ++i){
        NyProjectItem *item = root->Child(i);

        if(!index.isValid()){
            if (
                    item->IsDirectory() &&
                    item->FileName() == nodeName
            ) {
                index = createIndex(item->Row(), 0, item);
            }
        }

        if(index.isValid()) break;
    }

    return index;
}

QStringList NyProjectModel::GetPathElements(QString path)
{
    int offset = _rootPath.length() +1;
    QString newPath = path.mid(offset);

    return newPath.split("/");
}

void NyProjectModel::AddNewNode(QString path, NyEditor *editor)
{
    QString basePath = _rootPath;

    QStringList splitted = GetPathElements(path);
    NyProjectItem *item = _projectRoot;
    QModelIndex index;

    for (int i = 0; i < splitted.length(); ++i){
        basePath.append("/");
        basePath.append(splitted[i]);

        index = FindNode(splitted[i], item);

        if (index.isValid()){
            item = static_cast<NyProjectItem*>(index.internalPointer());
        } else {
            NyProjectItem *item2 = new NyProjectItem(basePath, _rootPath, item);
            beginInsertRows(index, 0, 1);
            item->AppendChild(item2);
            endInsertRows();
            item = item2;
        }
    }

    if (!item->IsDirectory()){
        item->Open(editor);
        item->Save();
    }
}
