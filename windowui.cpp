#include "windowui.h"
#include "ui_windowui.h"
#include <random>

WindowUI::WindowUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WindowUI)
{
    ui->setupUi(this);
    setWindowTitle(tr("Encrypted Chat"));
    ui->Window_Page->setCurrentIndex(0);

    can_run_encry = false;
    ui->start_encry->setEnabled(false);
    ui->start_decry->setEnabled(false);
    ui->rand_a->setEnabled(false);
    ui->gene_A->setEnabled(false);
    ui->generate_share_key->setEnabled(false);

    ui->input_p->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->input_g->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->input_a->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->input_B->setButtonSymbols(QAbstractSpinBox::NoButtons);

    mode = NUM_MODE;
    ui->ENCRY_NUM_MODE->setChecked(true);
    ui->ENCRY_TEXT_MODE->setChecked(false);
}

WindowUI::~WindowUI() {
    delete ui;
}

void WindowUI::on_public_rand_clicked() {
    while(true){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(30000, 999999999);
        if(encry_num.set_p((unsigned long long)dis(gen))){
            ui->input_p->setValue((int)encry_num.p);
            encry_num.g = (unsigned long long)dis(gen);
            ui->input_g->setValue((int)encry_num.g);
            ui->public_rand->setEnabled(false);
            ui->public_confirm->setEnabled(false);
            ui->rand_a->setEnabled(true);
            ui->gene_A->setEnabled(true);
            return;
        }
    }
}


void WindowUI::on_public_confirm_clicked() {
    if(encry_num.set_p((unsigned long long)ui->input_p->value())){
        encry_num.g = (unsigned long long)ui->input_g->value();
        ui->public_rand->setEnabled(false);
        ui->public_confirm->setEnabled(false);
        ui->rand_a->setEnabled(true);
        ui->gene_A->setEnabled(true);
    }
    else{
        QMessageBox::warning(this,"输入非法","你输入的「大素数」并非为素数！  ");
    }
}


void WindowUI::on_rand_a_clicked() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, (int)encry_num.p - 1);
    ui->input_a->setValue(dis(gen));
    ui->rand_a->setEnabled(false);
}


void WindowUI::on_gene_A_clicked() {
    if((unsigned long long)ui->input_a->value() < encry_num.p) {
        encry_num.private_key = (unsigned long long)ui->input_a->value();
        encry_num.set_public_key();
        ui->output_A->append(QString::number(encry_num.public_key));
        ui->rand_a->setEnabled(false);
        ui->gene_A->setEnabled(false);
        ui->generate_share_key->setEnabled(true);
    }
    else
        QMessageBox::warning(this,"输入非法","你输入的「私钥」应当小于所使用的「大素数」！  ");
}


void WindowUI::on_generate_share_key_clicked() {
    if((unsigned long long)ui->input_B->value() < encry_num.p) {
        encry_num.set_share_key((unsigned long long)ui->input_B->value());
        ui->output_S->append(QString::number(encry_num.shared_key));
        ui->generate_share_key->setEnabled(false);
        ui->start_encry->setEnabled(true);
        ui->start_decry->setEnabled(true);
        can_run_encry = true;
    }
    else
        QMessageBox::warning(this,"输入非法","「对方公钥」应当是一个小于所使用的「大素数」的数！  ");
}


void WindowUI::on_reset_clicked() {
    encry_num.reset_num();

    can_run_encry = false;
    ui->public_rand->setEnabled(true);
    ui->public_confirm->setEnabled(true);
    ui->start_encry->setEnabled(false);
    ui->start_decry->setEnabled(false);
    ui->rand_a->setEnabled(false);
    ui->gene_A->setEnabled(false);
    ui->generate_share_key->setEnabled(false);

    ui->input_p->setValue(2);
    ui->input_g->setValue(2);
    ui->input_a->setValue(1);
    ui->input_B->setValue(1);
    ui->output_A->clear();
    ui->output_S->clear();
}


void WindowUI::on_start_encry_clicked() {
    ui->Window_Page->setCurrentIndex(2);
}


void WindowUI::on_start_decry_clicked() {
    ui->Window_Page->setCurrentIndex(1);
}


void WindowUI::on_ENCRY_Return_clicked() {
    ui->Window_Page->setCurrentIndex(0);
}


void WindowUI::on_ENCRY_NUM_MODE_clicked() {
    mode = NUM_MODE;
    ui->ENCRY_NUM_MODE->setChecked(true);
    ui->ENCRY_TEXT_MODE->setChecked(false);
}


void WindowUI::on_ENCRY_TEXT_MODE_clicked() {
    mode = TEXT_MODE;
    ui->ENCRY_NUM_MODE->setChecked(false);
    ui->ENCRY_TEXT_MODE->setChecked(true);
}


void WindowUI::on_ENCRY_CLEAR_clicked() {
    ui->ENCRY_Input->clear();
    ui->ENCRY_Output->clear();
}


void WindowUI::on_ENCRY_START_clicked() {
    QString input = ui->ENCRY_Input->toPlainText();
    if(mode == NUM_MODE) {
        bool is_num;
        int num = input.toInt(&is_num);
        if(!(is_num && QString::number(num) == input)){
            QMessageBox::warning(this,"输入非法","「数字模式」下只能输入有限长度10进制整数！\n其它数据请使用「文本模式」输入  ");
            return;
        }
    }
    ui->ENCRY_Output->clear();
    QString output = encry_num.encry(this->mode, input);
    ui->ENCRY_Output->append(output);
}


void WindowUI::on_DECRY_Return_clicked() {
    ui->Window_Page->setCurrentIndex(0);
}

void WindowUI::on_DECRY_CLEAR_clicked() {
    ui->DECRY_Input->clear();
    ui->DECRY_Output->clear();
}

void WindowUI::on_DECRY_START_clicked() {
    QString input = ui->DECRY_Input->toPlainText();
    if(input[0] != '1' && input[0] != '2'){
        QMessageBox::warning(this,"输入非法","错误的加密消息格式  ");
        return;
    }
    ui->DECRY_Output->clear();
    QString output = encry_num.decry(input);
    ui->DECRY_Output->append(output);
}
