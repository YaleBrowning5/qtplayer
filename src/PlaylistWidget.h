#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QListWidget>
#include <QString>
#include <QStringList>

class PlaylistWidget : public QListWidget {
    Q_OBJECT

public:
    explicit PlaylistWidget(QWidget *parent = nullptr);
    ~PlaylistWidget();

    // Directory operations
    void loadDirectory(const QString &dirPath);
    void clearPlaylist();
    
    // Get file list
    QStringList getVideoFiles() const { return videoFiles; }
    
    // Current selection
    QString getCurrentFile() const;
    void setCurrentFile(const QString &filePath);

signals:
    void videoSelected(const QString &filePath);

private slots:
    void onItemClicked(QListWidgetItem *item);

private:
    QStringList videoFiles;
    QString currentDirectory;
    
    // Helper functions
    bool isVideoFile(const QString &fileName) const;
    void populateList();
};

#endif // PLAYLISTWIDGET_H
