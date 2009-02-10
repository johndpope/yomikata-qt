#include "archive.h"

Archive::Archive(QObject *parent)
    : QObject(parent)
{
    _type = InvalidArchiveType;
}

Archive::~Archive()
{
}

void Archive::reset(const QString &filename)
{
    const struct
    {
        const char *ext;
        Type type;
        const char *programPath;
    }
    ARCHIVE_TYPES[] =
    {
        {".tar", Tar, "tar"},
        {".tar.gz", TarGz, "tar"},
        {".tgz", TarGz, "tar"},
        {".tar.bz", TarBz, "tar"},
        {".tar.bz2", TarBz, "tar"},
        {".tbz2", TarBz, "tar"},
        {".tar.Z", TarZ, "tar"}, // Using "compress"
        {".zip", Zip, "unzip"},
        {".cbz", Zip, "unzip"},
        {".rar", Rar, "unrar"},
        {".cbr", Rar, "unrar"},
        // Ace
        // 7zip
        {"", InvalidArchiveType, ""},
    };

    _filename = filename;
    _type = InvalidArchiveType;
    _programPath = "";

    for (int i = 0; *ARCHIVE_TYPES[i].ext != '\0'; i++)
    {
        if (_filename.endsWith(ARCHIVE_TYPES[i].ext, Qt::CaseInsensitive))
        {
            _type = ARCHIVE_TYPES[i].type;
            _programPath = ARCHIVE_TYPES[i].programPath;
        }
    }
}

const QString &Archive::filename() const
{
    return _filename;
}

Archive::Type Archive::type() const
{
    return _type;
}

const QString &Archive::programPath() const
{
    return _programPath;
}

#include "archive.moc"
