#ifndef WINDOWUI_H
#define WINDOWUI_H

#include <QWidget>
#include <QMessageBox>
#include "encry_num.h"

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

private:
    Ui::WindowUI *ui;
    Encry_num encry_num;
    bool can_run_encry;
};
#endif // WINDOWUI_H
