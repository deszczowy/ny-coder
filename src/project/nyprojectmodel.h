#ifndef NYPROJECTMODEL_H
#define NYPROJECTMODEL_H

#include "nyprojectitem.h"

#include <QAbstractItemModel>
#include <QObject>

class NyProjectModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NyProjectModel(const QString projectBasePath, QObject *parent = 0);
    ~NyProjectModel();

public:
    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public:
    void LoadProject(QString path);
    bool SaveAll();
    bool SaveEdited(NyEditor *editor);
    bool SaveEditedAs(QString newPath, NyEditor *editor);
    bool CloseEdited(NyEditor *editor);

    QString RootPath();
    QString ScriptToRun(NyEditor *editor);

    QModelIndex FindEdited(NyProjectItem *root, NyEditor *editor);

private:
    void Load(QString localisation, NyProjectItem *parent);
    void BuildFilter(QStringList &ext);
    void SaveRecursive(NyProjectItem *root);

    QModelIndex FindNode(QString nodeName, NyProjectItem *root);
    QStringList GetPathElements(QString path);

    void AddNewNode(QString path, NyEditor *editor);

private:
    NyProjectItem *_mainRoot;
    NyProjectItem *_projectRoot;
    NyProjectItem *_sideRoot;

private:
    QString _rootPath;
    QString _filter;
};

#endif // NYPROJECTMODEL_H
