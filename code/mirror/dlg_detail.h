#ifndef DLG_DETAIL_H
#define DLG_DETAIL_H

#include <QDialog>
#include <QMouseEvent>
#include "ui_dlg_detail.h"
#include "def_item_equip.h"
#include "Player.h"

class Dlg_Detail : public QDialog
{
	Q_OBJECT

public:
	Dlg_Detail(QWidget *parent);
	~Dlg_Detail();
	//显示装备的详细信息
	void DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip);
	void DisplayItemInfo(QPoint pos, const Info_Item *item, quint32 no);

private slots:
	void on_btn_quit_clicked();

private:
	Ui::dlg_detail ui;

	qint32 lineSpacing;
};

#endif // DLG_DETAIL_H
