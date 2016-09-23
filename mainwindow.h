#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void New();
    void Open();
    void Save();
    void SaveAs();
    void Export_as_PDF();
    void Exit();

    void Undo();
    void Redo();
    void Cut();
    void Copy();
    void Paste();
    void Find();

    void insertImage();
    void insertTable();
    void insertList();

    void setFontBold(bool);
    void setFontItalic(bool);
    void addUnderLine(bool);
    void setFont();
    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignJustify();
    void setFontColor();

private:
    Ui::MainWindow *ui;

    QString curFileName,curDir;  //当前文件和目录

    void setPath(const QString& filename);
    MainWindow* create();
    void load(const QString& filename);
    void _save(const QString& filename);
    void save();
    void saveas();
    void _close();
    void closeEvent(QCloseEvent *event);
    //void mergeCharFormat(QTextCharFormat format);
};

#endif // MAINWINDOW_H
