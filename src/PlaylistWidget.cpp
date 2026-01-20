#include "PlaylistWidget.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

PlaylistWidget::PlaylistWidget(QWidget *parent)
    : QListWidget(parent)
{
    connect(this, &QListWidget::itemClicked, this, &PlaylistWidget::onItemClicked);
    
    // Set some styling
    setStyleSheet("QListWidget { background-color: #2b2b2b; color: #ffffff; border: none; }"
                  "QListWidget::item { padding: 8px; }"
                  "QListWidget::item:selected { background-color: #3d3d3d; }"
                  "QListWidget::item:hover { background-color: #353535; }");
}

PlaylistWidget::~PlaylistWidget() {
}

void PlaylistWidget::loadDirectory(const QString &dirPath) {
    clearPlaylist();
    currentDirectory = dirPath;
    
    QDir dir(dirPath);
    if (!dir.exists()) {
        return;
    }
    
    // Get all files in directory
    QStringList filters;
    filters << "*.mp4" << "*.mkv" << "*.avi" << "*.mov" << "*.wmv" << "*.flv" << "*.webm";
    
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files, QDir::Name);
    
    for (const QFileInfo &fileInfo : fileList) {
        if (isVideoFile(fileInfo.fileName())) {
            videoFiles.append(fileInfo.absoluteFilePath());
        }
    }
    
    populateList();
}

void PlaylistWidget::clearPlaylist() {
    clear();
    videoFiles.clear();
    currentDirectory.clear();
}

QString PlaylistWidget::getCurrentFile() const {
    QListWidgetItem *item = currentItem();
    if (item) {
        int index = row(item);
        if (index >= 0 && index < videoFiles.size()) {
            return videoFiles[index];
        }
    }
    return QString();
}

void PlaylistWidget::setCurrentFile(const QString &filePath) {
    int index = videoFiles.indexOf(filePath);
    if (index >= 0) {
        setCurrentRow(index);
    }
}

void PlaylistWidget::onItemClicked(QListWidgetItem *item) {
    int index = row(item);
    if (index >= 0 && index < videoFiles.size()) {
        emit videoSelected(videoFiles[index]);
    }
}

bool PlaylistWidget::isVideoFile(const QString &fileName) const {
    QStringList videoExtensions;
    videoExtensions << "mp4" << "mkv" << "avi" << "mov" << "wmv" << "flv" << "webm";
    
    QFileInfo fileInfo(fileName);
    QString ext = fileInfo.suffix().toLower();
    
    return videoExtensions.contains(ext);
}

void PlaylistWidget::populateList() {
    clear();
    
    for (const QString &filePath : videoFiles) {
        QFileInfo fileInfo(filePath);
        addItem(fileInfo.fileName());
    }
}
