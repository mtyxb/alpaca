/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunfilesystemmodel.h"
#include <QFileInfo>
#include <QDir>
#include <QFileIconProvider>

ButianyunFileInfo::ButianyunFileInfo()
    : level(-1)
    , id(0)
{

}

ButianyunFileInfo::ButianyunFileInfo(const QString& vFilePath, int vLevel, quintptr vId, quintptr vPid, int vRow)
    : filePath(vFilePath)
    , level(vLevel)
    , id(vId)
    , pid(vPid)
    , row(vRow)
{

}

bool ButianyunFileInfo::isValid(quintptr vId) const
{
    return id != 0 && vId == id && !filePath.isEmpty();
}

QString ButianyunFileInfo::filetype() const
{
    return filetype(filePath);
}

QIcon ButianyunFileInfo::fileicon() const
{
    return fileicon(filePath);
}

#define  KB  (1024LL)
#define  MB  (KB * 1024LL)
#define  GB  (MB * 1024LL)
#define  TB  (GB * 1024LL)

static QString butianyun_bytes_human_readable(long long bytes)
{
    long long tb = bytes / TB;
    long long gb = (bytes % TB) / GB;
    long long mb = (bytes % GB) / MB;
    long long kb = (bytes % MB) / KB;
    long long b = (bytes % KB);
    return QString("%1%2%3%4%5")
            .arg(tb > 0 ? QString::number(tb) + "TB" : "")
            .arg(gb > 0 ? QString::number(gb) + "GB" : "")
            .arg(mb > 0 ? QString::number(mb) + "MB" : "")
            .arg(kb > 0 ? QString::number(kb) + "KB" : "")
            .arg(b > 0 ? QString::number(b) : "");
}

QString ButianyunFileInfo::filesize() const
{
    QFileInfo fi(filePath);
    if (fi.isDir())
    {
        return QString();
    }
    return butianyun_bytes_human_readable(fi.size());
}

QString ButianyunFileInfo::filetype(const QString& filename)
{
    QFileInfo fi(filename);
    QFileIconProvider provider;
    return provider.type(fi);
}

QIcon ButianyunFileInfo::fileicon(const QString& filename)
{
    QFileInfo fi(filename);
    QFileIconProvider provider;
    return provider.icon(fi);
}

class ButianyunFileSystemModelPrivate
{
    Q_DECLARE_PUBLIC(ButianyunFileSystemModel)

    using PathCacheHash = QHash<quintptr, ButianyunFileInfo>;
    using PathCacheHashInverted = QHash<QString, quintptr>;

    QString _rootPath;
    QModelIndex _rootIndex;
    PathCacheHash _id_to_info_cache;
    PathCacheHashInverted _path_to_id_cache;
    ButianyunFileSystemModel* q_ptr;
};

ButianyunFileSystemModel::ButianyunFileSystemModel(QObject* p)
    : QAbstractItemModel(p)
    , d_ptr(new ButianyunFileSystemModelPrivate())
{
    setRootPath("");
}

ButianyunFileSystemModel::~ButianyunFileSystemModel()
{
    clearCache();
}

void ButianyunFileSystemModel::clearCache()
{
    Q_D(ButianyunFileSystemModel);
    d->_id_to_info_cache.clear();
    d->_path_to_id_cache.clear();
    QString rootPath = d->_rootPath;
    d->_rootPath.clear();
    setRootPath(rootPath);
}

void ButianyunFileSystemModel::setRootPath(const QString& value)
{
    Q_D(ButianyunFileSystemModel);
    QFileInfo fi(value);
    if (fi.absoluteFilePath() == d->_rootPath)
    {
        return;
    }

    beginResetModel();
    d->_id_to_info_cache.clear();
    d->_path_to_id_cache.clear();
    d->_rootPath = fi.absoluteFilePath();
    quintptr id = d->_id_to_info_cache.size() + 1;
    d->_id_to_info_cache[id] = ButianyunFileInfo(d->_rootPath, 0, id, -1, 0);
    d->_path_to_id_cache[d->_rootPath] = id;
    d->_rootIndex = createIndex(0, 0, reinterpret_cast<const void*>(id));
    emit rootPathChanged();
    endResetModel();
}

QString ButianyunFileSystemModel::rootPath() const
{
    const ButianyunFileSystemModelPrivate* d = d_func();
    return d->_rootPath;
}

QModelIndex ButianyunFileSystemModel::rootIndex() const
{
    const ButianyunFileSystemModelPrivate* d = d_func();
    return d->_rootIndex;
}

ButianyunFileInfo ButianyunFileSystemModel::fileInfo(const QModelIndex& index) const
{
    const ButianyunFileSystemModelPrivate* d = d_func();
    quintptr pid = index.internalId();
    if (0 == pid)
    {
        if (index.parent().isValid())
        {
            return ButianyunFileInfo();
        }
        pid = 1;
    }
    auto it = d->_id_to_info_cache.find(pid);
    if (it == d->_id_to_info_cache.end())
    {
        return ButianyunFileInfo();
    }
    return it.value();
}

bool ButianyunFileSystemModel::isValid(const QModelIndex& index) const
{
   const ButianyunFileSystemModelPrivate* d = d_func();
   if (index.column() >= ButianyunFileInfoRoleCount)
   {
       return false;
   }

    quintptr pid = index.internalId();
    if (0 == pid)
    {
        if (index.parent().isValid())
        {
            return false;
        }
        pid = 1;
    }
    auto it = d->_id_to_info_cache.find(pid);
    if (it == d->_id_to_info_cache.end())
    {
        return false;
    }
    return it.value().isValid(pid);
}

static QFileInfoList butianyun_get_file_info_list(const QString& filePath)
{
    QDir dir(filePath);
    return  dir.entryInfoList(QDir::Filter::NoDotAndDotDot |QDir::Drives | QDir::Dirs |QDir::Files,
                              QDir::DirsFirst |QDir::Name |QDir::Type |QDir::IgnoreCase);
}

QModelIndex ButianyunFileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
   ButianyunFileSystemModelPrivate* d = const_cast<ButianyunFileSystemModel*>(this)->d_func();
   if (row < 0 || column < 0 || column >= ButianyunFileInfoRoleCount)
   {
       return QModelIndex();
   }
   quintptr pid = parent.internalId();
   auto parent_fileinfo = fileInfo(parent);

   if (0 == pid && !parent.parent().isValid())
   {
       pid = 1;
   }
   if (!parent_fileinfo.isValid(pid))
   {
       return QModelIndex();
   }

   QFileInfoList list = butianyun_get_file_info_list(parent_fileinfo.filePath);
   if (row >= list.length())
   {
       return QModelIndex();
   }

   quintptr this_id = 0;
   int max_count = 100;
   int count = 0;
   for (int i = row; i < list.length() && count <= max_count; i++)
   {
       QFileInfo fi = list[i];
       quintptr id;
       auto it = d->_path_to_id_cache.find(fi.absoluteFilePath());
       if (it != d->_path_to_id_cache.end())
       {
           id = it.value();
       }
       else
       {
           id = d->_id_to_info_cache.size() + 1;
           d->_id_to_info_cache[id] = ButianyunFileInfo(fi.absoluteFilePath(), parent_fileinfo.level + 1, id,  pid, row);
           d->_path_to_id_cache[fi.absoluteFilePath()] = id;
           count++;
       }
       if (row == i)
       {
           this_id = id;
       }
   }
   return createIndex(row, column, reinterpret_cast<const void *>(this_id));
}

QModelIndex ButianyunFileSystemModel::parent(const QModelIndex &child) const
{
    const ButianyunFileSystemModelPrivate* d = d_func();
    if (!child.isValid())
    {
        return QModelIndex();
    }

    quintptr id = child.internalId();
    auto fileinfo = fileInfo(child);
    if (!fileinfo.isValid(id) || 0 == fileinfo.level)
    {
       return QModelIndex();
    }

    int index = fileinfo.filePath.lastIndexOf("/");
    if (index <= 0)
    {
        return QModelIndex();
    }

    QString filePath = fileinfo.filePath.left(index);
    if (filePath.isEmpty())
    {
        return QModelIndex();
    }

    auto it = d->_path_to_id_cache.find(filePath);
    if (it == d->_path_to_id_cache.end())
    {
        return QModelIndex();
    }

    if (it.value() != fileinfo.pid)
    {
        return QModelIndex();
    }

    quintptr pid = fileinfo.pid;
    auto it2 = d->_id_to_info_cache.find(pid);
    if (it2 == d->_id_to_info_cache.end())
    {
        return QModelIndex();
    }
    fileinfo = it2.value();
    if (pid != fileinfo.id)
    {
        return QModelIndex();
    }
    return createIndex(fileinfo.row, 0, pid);
}

int ButianyunFileSystemModel::rowCount(const QModelIndex &parent) const
{
    quintptr pid = parent.internalId();
    auto parent_fileinfo = fileInfo(parent);
    if (0 == pid && !parent.parent().isValid())
    {
        pid = 1;
    }
    if (!parent_fileinfo.isValid(pid))
    {
        return 0;
    }

    QFileInfoList list = butianyun_get_file_info_list(parent_fileinfo.filePath);
    return list.length();
}

int ButianyunFileSystemModel::columnCount(const QModelIndex &parent) const
{
    return ButianyunFileInfoRoleCount;
}

bool ButianyunFileSystemModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

QVariant ButianyunFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::Horizontal == orientation)
    {
        if (Qt::DisplayRole == role)
        {
        switch (section + Qt::UserRole + 1)
        {
            case Role_ButianyunFileName:
                 return tr("Name");
            case Role_ButianyunFileSize:
                 return tr("Size");
            case Role_ButianyunFileType:
                 return tr("Type");
            case Role_ButianyunFileLastModified:
                 return tr("Data Modified");
            default:
                 return QVariant();
            }
        }
    }
    else
    {
       if (Qt::DisplayRole == role)
       {
         return QString::number(section);
       }
    }
    return QVariant();
}

QVariant ButianyunFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole != role && Qt::DecorationRole != role)
    {
        return QVariant();
    }

    if (!index.isValid())
    {
        return QVariant();
    }
    if (index.column() >= ButianyunFileInfoRoleCount)
    {
        return false;
    }

    quintptr pid = index.internalId();
    if (0 == pid && !index.parent().isValid())
    {
        pid = 1;
    }
    auto fileinfo = fileInfo(index);

    if (!fileinfo.isValid(pid))
    {
        return QVariant();
    }

    QFileInfo fi(fileinfo.filePath);
    switch (index.column() + Qt::UserRole + 1)
    {
    case Role_ButianyunFileName:
        if (Qt::DisplayRole == role)
        {
            return fi.fileName();
        }
        else if (Qt::DecorationRole == role)
        {
            return fileinfo.fileicon();
        }

    case Role_ButianyunFileSize:
    {
         return fileinfo.filesize();
    }

    case Role_ButianyunFileType:
        return fileinfo.filetype();

    case Role_ButianyunFileLastModified:
        return fi.lastModified().toString("yyyy-MM-dd hh:mm");

    default:
         return QVariant();
    }
    return QVariant();
}

QHash<int,QByteArray> ButianyunFileSystemModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles = QAbstractItemModel::roleNames();
    roles[Role_ButianyunFileName] = QStringLiteral("FileName").toUtf8();
    roles[Role_ButianyunFileSize] = QStringLiteral("FileSize").toUtf8();
    roles[Role_ButianyunFileType] = QStringLiteral("FileType").toUtf8();
    roles[Role_ButianyunFileLastModified] = QStringLiteral("LastModified").toUtf8();
    return roles;
}

Qt::ItemFlags ButianyunFileSystemModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex ButianyunFileSystemModel::index(const QString& filePath, const QModelIndex &parent) const
{
    ButianyunFileSystemModelPrivate* d = const_cast<ButianyunFileSystemModel*>(this)->d_func();
    auto it = d->_path_to_id_cache.find(filePath);
    if (it == d->_path_to_id_cache.end())
    {
        return QModelIndex();
    }
    quintptr pid = parent.internalId();
    if (0 == pid && !parent.parent().isValid())
    {
        pid = 1;
    }
    auto it2 = d->_id_to_info_cache.find(it.value());
    if (it2 == d->_id_to_info_cache.end())
    {
        return QModelIndex();
    }
    ButianyunFileInfo fileinfo = it2.value();
    if (fileinfo.pid != pid && fileinfo.level > 0)
    {
        return QModelIndex();
    }
    return createIndex(fileinfo.row, 0, pid);
}
