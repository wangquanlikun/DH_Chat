#ifndef WINDOWUI_H
#define WINDOWUI_H

#include <QWidget>
#include <QMessageBox>
#include <QString>
#include "encry_num.h"

#define NUM_MODE 1
#define TEXT_MODE 2

QT_BEGIN_NAMESPACE
namespace Ui {
class WindowUI;
}
QT_END_NAMESPACE

class WindowUI : public QWidget
{
    Q_OBJECT

public:
    WindowUI(QWidget *parent = nullptr);
    ~WindowUI();

private slots:
    void on_public_rand_clicked();
    void on_public_confirm_clicked();
    void on_rand_a_clicked();
    void on_gene_A_clicked();
    void on_generate_share_key_clicked();
    void on_reset_clicked();
    void on_start_encry_clicked();
    void on_start_decry_clicked();

    void on_ENCRY_Return_clicked();
    void on_ENCRY_NUM_MODE_clicked();
    void on_ENCRY_TEXT_MODE_clicked();
    void on_ENCRY_CLEAR_clicked();
    void on_ENCRY_START_clicked();

    void on_DECRY_Return_clicked();
    void on_DECRY_CLEAR_clicked();
    void on_DECRY_START_clicked();

private:
    Ui::WindowUI *ui;
    Encry_num encry_num;
    bool can_run_encry;
    int mode;
};
#endif // WINDOWUI_H
