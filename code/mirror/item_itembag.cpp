#include "item_itembag.h"
#include <QMessageBox>

extern QWidget *g_widget;
extern Dlg_Detail *g_dlg_detail;

extern RoleInfo_False g_falseRole;

item_itemBag::item_itemBag(CPlayer *w_player)
	: player(w_player)
{
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	CurrentPage = 1;

	m_item = player->get_bag_item();

	popMenu = new QMenu();
	action_use = new QAction(QStringLiteral("使用"), this);
	action_storage = new QAction(QStringLiteral("入库"), this);
	action_sale = new QAction(QStringLiteral("销售"), this);
	popMenu->addAction(action_use);
//	popMenu->addAction(action_storage);
	popMenu->addAction(action_sale);

	connect(ui.tableWidget, SIGNAL(cellEntered(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));

	connect(action_use, SIGNAL(triggered(bool)), this, SLOT(on_action_use(bool)));
//	connect(action_storage, SIGNAL(triggered(bool)), this, SLOT(on_action_storage(bool)));
	connect(action_sale, SIGNAL(triggered(bool)), this, SLOT(on_action_sale(bool)));
}

item_itemBag::~item_itemBag()
{

}

void item_itemBag::updateInfo()
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp = "";
	quint32 ID;

	pages = (m_item->size() + row_Count * Col_Count - 1) / (row_Count * Col_Count);
	if (pages == 0)
		pages = 1;
	if (CurrentPage > pages)
		CurrentPage = pages;

	ui.edit_page_cur->setText(QString::number(CurrentPage));
	ui.edit_page_all->setText(QString::number(pages));

	//必须先清除背包显示，否则当前道具种类小于之前道具种类时会在最尾显示原道具的假像。
	ui.tableWidget->clear();
	MapItem::const_iterator iter = m_item->constBegin();
	for (quint32 i = 0; i < (CurrentPage - 1) * (row_Count * Col_Count); i++, iter++) { ; }

	for (; iter != m_item->constEnd(); iter++)
	{
		ID = iter.key();
		const Info_Item *itemItem = FindItem_Item(ID);
		if (itemItem == nullptr)
		{
			continue;
		}

		ui.tableWidget->setItem(row_cur, col_cur++, new QTableWidgetItem(QIcon(itemItem->icon), strTmp));
		if (col_cur >= Col_Count)
		{
			++row_cur;
			col_cur = 0;
		}
	}
}

void item_itemBag::on_btn_pgUp_clicked()
{
	if (CurrentPage > 1)
	{
		--CurrentPage;
		updateInfo();
	}
}
void item_itemBag::on_btn_pgDn_clicked()
{
	if (CurrentPage < pages)
	{
		++CurrentPage;
		updateInfo();
	}
}

void item_itemBag::ShowItemInfo(int row, int column)
{
	ShowItemInfo_item(row, column, CurrentPage, m_item, player->get_voc(), player->get_lv());
}

void item_itemBag::ShowContextMenu(QPoint pos)
{
	g_dlg_detail->hide();

	//如果右击空白单元格，不弹出右键菜单。
	if (m_item->size() > GetCurrentCellIndex(CurrentPage))
	{
		popMenu->exec(ui.tableWidget->mapToGlobal(pos));
	}
}

void item_itemBag::on_action_use(bool checked)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, CurrentPage, m_item);

	const Info_Item* item = FindItem_Item(ID);
	if (player->get_lv() < item->level)
	{
		QString message = QStringLiteral("等级不足！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		return;
	}
	if (item->vocation != 0 && item->vocation != player->get_voc())
	{
		QString message = QStringLiteral("职业不符合！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		return;
	}
	if (item->type == et_NoEffect)
	{
		QString message = QStringLiteral("任务道具，不可右键使用！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		return;
	}
	emit UsedItem(ID);
}

void item_itemBag::on_action_sale(bool checked)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, CurrentPage, m_item);
	quint32 Number = m_item->value(ID);
	quint32 index = row * ui.tableWidget->columnCount() + col;

	const Info_Item *itemitem = FindItem_Item(ID);
	if (itemitem != NULL)
	{
		player->set_coin(player->get_coin() + (itemitem->coin >> 1));

		g_falseRole.coin += Number * (itemitem->coin >> 1);
		m_item->remove(ID);
		emit UpdatePlayerInfoSignals();		
	}
}

void item_itemBag::on_action_storage(bool checked)
{
}