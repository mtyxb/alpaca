/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#ifndef BUTIANYUNFILESYSTEMMODEL_H
#define BUTIANYUNFILESYSTEMMODEL_H

#include <QAbstractItemModel>
#include "butianyun_alpaca_common_api.h"

struct BUTIANYUN_ALPACA_COMMON_API ButianyunFileInfo
{
    ButianyunFileInfo();
    ButianyunFileInfo(const QString& vFilePath, int vLevel, quintptr vId, quintptr vPid, int vRow);
    bool isValid(quintptr vId) const;
    QString filetype() const;
    QIcon fileicon() const;
    QString filesize() const;
    static QString filetype(const QString& filename);
    static QIcon fileicon(const QString& filename);
    QString filePath;
    int level;
    quintptr id;
    quintptr pid;
    int row;
};

enum ButianyunFileInfoRole
{
    Role_ButianyunFileName = Qt::UserRole + 1,
    Role_ButianyunFileSize,
    Role_ButianyunFileType,
    Role_ButianyunFileLastModified,

    Role_ButianyunLastPlaceHolder,
    ButianyunFileInfoRoleCount = Role_ButianyunLastPlaceHolder - Role_ButianyunFileName
};

struct ButianyunFileSystemModelPrivate;

class BUTIANYUN_ALPACA_COMMON_API ButianyunFileSystemModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ButianyunFileSystemModel)
    Q_PROPERTY(QString rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)
    Q_PROPERTY(QModelIndex rootIndex READ rootIndex CONSTANT)

public:
    Q_INVOKABLE explicit ButianyunFileSystemModel(QObject* p = nullptr);
    ~ButianyunFileSystemModel();

    void setRootPath(const QString& value);
    QString rootPath() const;
    QModelIndex rootIndex() const;
    ButianyunFileInfo fileInfo(const QModelIndex& index) const;
    bool isValid(const QModelIndex& index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(const QString& filePath, const QModelIndex &parent = QModelIndex()) const;


signals:
    void rootPathChanged();

public slots:
    void clearCache();

private:
    QSharedPointer<ButianyunFileSystemModelPrivate> d_ptr;
    Q_DISABLE_COPY(ButianyunFileSystemModel)
};

#endif // BUTIANYUNFILESYSTEMMODEL_H
