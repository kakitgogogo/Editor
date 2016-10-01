#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTextCharFormat>

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
    void Preview(QPrinter *printer);
    void PrintPreview();
    void Export_as_PDF();
    void Exit();

    void Find();
    void findNext();

    void insertImage();
    void insertTable();
    void insertList();

    void setFontBold(bool);
    void setFontItalic(bool);
    void addUnderLine(bool);
    void setFont();
    void setFontFamily(const QString& font);
    void setFontSize(const QString& fontSize);
    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignJustify();
    void setFontColor();
    void currentCharFormatChanged(QTextCharFormat format);

private:
    Ui::MainWindow *ui;

    QString defaultFontFamily,defaultFontSize;
    QString curFileName,curDir;  //当前文件和目录
    QLineEdit *lineEdit;

    void initWindow();
    void setPath(const QString& filename);
    MainWindow* create();
    void load(const QString& filename);
    void _save();
    void save();
    void saveas();
    void closeEvent(QCloseEvent *event);
    void mergeCharFormat(const QTextCharFormat& format);
};

#endif // MAINWINDOW_H
