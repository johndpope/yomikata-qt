#include "indexer.h"

#include <algorithm>

#include "archivelister.h"
#include "debug.h"

using std::sort;

Indexer::Indexer(const Archive &archive, QObject *parent)
    : QObject(parent), _archive(archive)
{
    _archiveLister = NULL;
}

Indexer::~Indexer()
{
}

void Indexer::reset()
{
    // Stop any current lister
    if (_archiveLister != NULL)
    {
        delete _archiveLister;
    }

    // Clear the current indexer
    _files.clear();

    // Create a new archive lister
    _archiveLister = new ArchiveLister(_archive, this);

    // Connect to it
    connect(_archiveLister, SIGNAL(entryFound(const QByteArray &, int, int)),
            SLOT(entryFound(const QByteArray &, int, int)));
    connect(_archiveLister, SIGNAL(finished()), SLOT(listingFinished()));

    // Start it
    _archiveLister->start();

    // Start timing
    _listingTime.restart();
}

int Indexer::numPages() const
{
    return _files.size();
}

QByteArray Indexer::pageName(int index) const
{
    Q_ASSERT(index >= 0 && index < (int) _files.size());
    return _files[index].name;
}

int Indexer::uncompressedSize(int index) const
{
    Q_ASSERT(index >= 0 && index < (int) _files.size());
    return _files[index].uncompressedSize;
}

void Indexer::entryFound(const QByteArray &filename, int compressedSize, int uncompressedSize)
{
    // Add the entry to the list
    FileInfo temp;
    temp.name = filename;
    temp.compressedSize = compressedSize;
    temp.uncompressedSize = uncompressedSize;
    _files.push_back(temp);
}

bool Indexer::FileInfo::operator < (const Indexer::FileInfo &other) const
{
    return QString::localeAwareCompare(QString::fromLocal8Bit(name), QString::fromLocal8Bit(other.name)) < 0;
}

void Indexer::listingFinished()
{
    // Deallocate the lister
    _archiveLister->deleteLater();
    _archiveLister = NULL;

    // Sort all the entries
    sort(_files.begin(), _files.end());

    // Notify the steward
    debug()<<"Listing finished:"<<_listingTime.elapsed()<<" ms"
            <<"--"<<_files.size()<<"entries";

    if (_files.size() > 0)
    {
        debug()<<"File"<<_files.front().name;
    }

    emit built();
}
