//#include <QtWidgets>
#include <QAction>
#include <QStandardItemModel>
#include <QTextCodec>
#include <sstream>
#include <iostream>

#include "testTriStateCheckbox.h"
#include "ui_testTriStateCheckbox.h"

testTriStateCheckbox::testTriStateCheckbox(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::testTriStateCheckbox)
{
  ui->setupUi(this);

  test();

  setupSyncDataTree();

  connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
          this, SLOT(showFont(QTreeWidgetItem*)));
  connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
          this, SLOT(updateStyles(QTreeWidgetItem*,int)));

  ui->treeWidget->setItemSelected(ui->treeWidget->topLevelItem(0), true);
  showFont(ui->treeWidget->topLevelItem(0));
}

testTriStateCheckbox::~testTriStateCheckbox()
{
  delete ui;
}

void testTriStateCheckbox::test()
{
  std::list<testTriStateCheckbox::Data> dataList;
  testTriStateCheckbox::StDataElem datas[] = {
    {Qt::Unchecked, "aaa", "1st"},
    {Qt::Unchecked, "abc", "1st"},
    {Qt::Unchecked, "abb", "1st"},
    {Qt::Unchecked, "ccc", "1st"},
    {Qt::Unchecked, "ddd", "1st"},
    {Qt::Unchecked, "def", "1st"},
    {Qt::Unchecked, "eee", "1st"},
    {Qt::Unchecked, "edc", "1st"},
    {Qt::Unchecked, "erf", "1st"},
    {Qt::Unchecked, "asd", "2nd"},
    {Qt::Unchecked, "aqw", "2nd"},
    {Qt::Unchecked, "azx", "2nd"},
    {Qt::Unchecked, "bbb", "2nd"},
    {Qt::Unchecked, "ggg", "2nd"},
    {Qt::Unchecked, "hhh", "2nd"},
    {Qt::Unchecked, "zzz", "3rd"},
    {Qt::Unchecked, "zaq", "3rd"},
    {Qt::Unchecked, "zxc", "3rd"},
    {Qt::Unchecked, "yyy", "3rd"}
  };
  for (auto data : datas) {
    dataList.push_back(std::make_shared<testTriStateCheckbox::StDataElem>(data));
  }

  createMapByCategory(dataList);

  auto categorizedDatas = m_CategorizedDatas;

  for (auto mapByCategory : categorizedDatas){
    std::string category = mapByCategory.first;
    for (auto mapByAlphabet : mapByCategory.second){
      std::string firstAlphabet = mapByAlphabet.first;
      for (auto mapByName : mapByAlphabet.second){
        std::string name = mapByName.second->name;
        std::stringstream ss;
        ss << " category=" << category << " 1stAlphabet=" << firstAlphabet << " name=" << name;
        std::cout << ss.str() << std::endl;
      }
    }
  }

  Qt::CheckState checkState;
  checkState = verifyCheckStatus("3rd");
  categorizedDatas["1st"]["A"]["aaa"]->checkState = Qt::Checked;
  checkState = verifyCheckStatus("3rd");
  checkState = verifyCheckStatus("1st");
  categorizedDatas["3rd"]["Z"]["zzz"]->checkState = Qt::Checked;
  categorizedDatas["3rd"]["Z"]["zaq"]->checkState = Qt::Checked;
  categorizedDatas["3rd"]["Z"]["zxc"]->checkState = Qt::Checked;
  categorizedDatas["3rd"]["Y"]["yyy"]->checkState = Qt::Checked;
  checkState = verifyCheckStatus("3rd");

}

void testTriStateCheckbox::setupSyncDataTree()
{
  auto syncDataTree = ui->treeWidget;

  syncDataTree->setColumnCount(1);
  syncDataTree->setHeaderLabels(QStringList() << tr("SyncDataSetting"));

  auto categorizedDatas = m_CategorizedDatas;

  QTextCodec* tc = QTextCodec::codecForLocale();

  for (auto mapByCategory : categorizedDatas) {
    std::string category = mapByCategory.first;
    QTreeWidgetItem *syncDataTreeItemCategory = new QTreeWidgetItem(syncDataTree);
    syncDataTreeItemCategory->setText(0, QString(tc->toUnicode(category.c_str())));
    syncDataTreeItemCategory->setCheckState(0, Qt::Unchecked);
    syncDataTreeItemCategory->setFlags(syncDataTreeItemCategory->flags() | Qt::ItemIsAutoTristate);

    for (auto mapByAlphabet : mapByCategory.second){
      std::string firstAlphabet = mapByAlphabet.first;
      QTreeWidgetItem *syncDataTreeItemAlphabet = new QTreeWidgetItem(syncDataTreeItemCategory);
      syncDataTreeItemAlphabet->setText(0, QString(tc->toUnicode(firstAlphabet.c_str())));
      syncDataTreeItemAlphabet->setCheckState(0, Qt::Unchecked);
      syncDataTreeItemAlphabet->setFlags(syncDataTreeItemAlphabet->flags() | Qt::ItemIsAutoTristate);

      for (auto mapByName : mapByAlphabet.second){
        std::string name = mapByName.second->name;
        QTreeWidgetItem *syncDataTreeItemName = new QTreeWidgetItem(syncDataTreeItemAlphabet);
        syncDataTreeItemName->setText(0, QString(tc->toUnicode(name.c_str())));
        syncDataTreeItemName->setCheckState(0, Qt::Unchecked);
        syncDataTreeItemName->setFlags(syncDataTreeItemName->flags() | Qt::ItemIsAutoTristate);
      }
    }
  }
}

void testTriStateCheckbox::createMapByCategory(std::list<Data> dataList)
{
  // カテゴリ別データマップをクリア
  m_CategorizedDatas.clear();

  // データリストの各要素をカテゴリ別データマップに格納する
  for (auto data : dataList)
  {
    auto name = data->name;         // 変数名
    auto category = data->category; // カテゴリ名

    // カテゴリ別データマップをカテゴリ検索して、アルファベット別データマップ要素を取得する
    MapByFirstAlphabet mapByFirstAlphabet;
    if (m_CategorizedDatas.find(category) != m_CategorizedDatas.end())
    {
      mapByFirstAlphabet = m_CategorizedDatas[category];
    }

    // アルファベット別データマップをカテゴリ検索して、データマップ要素を取得する
    std::string firstAlphabet = name.substr(0,1);
    std::transform(firstAlphabet.cbegin(), firstAlphabet.cend(), firstAlphabet.begin(), toupper);
    MapByName mapByName;
    if (mapByFirstAlphabet.find(firstAlphabet) != mapByFirstAlphabet.end())
    {
      mapByName = mapByFirstAlphabet[firstAlphabet];
    }

    // データマップにデータを追加/更新する
    mapByName[name] = data;

    // アルファベット別データマップにデータマップを追加/更新する
    mapByFirstAlphabet[firstAlphabet] = mapByName;

    // カテゴリ別データマップにアルファベット別データマップを追加/更新する
    m_CategorizedDatas[category] = mapByFirstAlphabet;
  }
}

Qt::CheckState testTriStateCheckbox::verifyCheckStatus(std::string category, std::string name)
{
  bool selected = false;
  bool unselected = false;

  auto categorizedDatas = m_CategorizedDatas;

  for (auto mapByCategory : categorizedDatas) {
    if (category != mapByCategory.first)
      continue;
    for (auto mapByAlphabet : mapByCategory.second) {
      if (name != "") {
        std::string firstAlphabet = name.substr(0,1);
        std::transform(firstAlphabet.cbegin(), firstAlphabet.cend(), firstAlphabet.begin(), toupper);
        if (firstAlphabet != mapByAlphabet.first)
          continue;
      }
      for (auto mapByName : mapByAlphabet.second){
        if (mapByName.second->checkState != Qt::Unchecked)
          selected = true;
        if (mapByName.second->checkState == Qt::Unchecked)
          unselected = true;
      }
    }
  }

  Qt::CheckState checkedStatus;
  if (!unselected)
    checkedStatus = Qt::Checked;
  else if (!selected)
    checkedStatus = Qt::Unchecked;
  else
    checkedStatus = Qt::PartiallyChecked;

  return checkedStatus;
}

void testTriStateCheckbox::on_actionClear_triggered()
{
    QTreeWidgetItem *currentItem = ui->treeWidget->currentItem();
    foreach (QTreeWidgetItem *item, ui->treeWidget->selectedItems())
        ui->treeWidget->setItemSelected(item, false);
    ui->treeWidget->setItemSelected(currentItem, true);
}

void testTriStateCheckbox::on_actionCheckAll_triggered()
{
    checkuncheckSyncData(Qt::Checked);
}

void testTriStateCheckbox::on_actionUncheckAll_triggered()
{
    checkuncheckSyncData(Qt::Unchecked);
}

void testTriStateCheckbox::checkuncheckSyncData(Qt::CheckState state)
{
    QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
    foreach (QTreeWidgetItem *item, items) {
        if (item->checkState(0) != state)
            item->setCheckState(0, state);
    }
}

void testTriStateCheckbox::showFont(QTreeWidgetItem *item)
{
    if (!item)
        return;

//    QString family;
//    QString style;
//    int weight;
//    bool italic;

//    if (item->parent()) {
//        family = item->parent()->text(0);
//        style = item->text(0);
//        weight = item->data(0, Qt::UserRole).toInt();
//        italic = item->data(0, Qt::UserRole + 1).toBool();
//    } else {
//        family = item->text(0);
//        style = item->child(0)->text(0);
//        weight = item->child(0)->data(0, Qt::UserRole).toInt();
//        italic = item->child(0)->data(0, Qt::UserRole + 1).toBool();
//    }

//    QString oldText = textEdit->toPlainText().trimmed();
//    bool modified = textEdit->document()->isModified();
//    textEdit->clear();
//    QFont font(family, 32, weight, italic);
//    font.setStyleName(style);
//    textEdit->document()->setDefaultFont(font);

//    QTextCursor cursor = textEdit->textCursor();
//    QTextBlockFormat blockFormat;
//    blockFormat.setAlignment(Qt::AlignCenter);
//    cursor.insertBlock(blockFormat);

//    if (modified)
//        cursor.insertText(QString(oldText));
//    else
//        cursor.insertText(QString("%1 %2").arg(family).arg(style));

//    textEdit->document()->setModified(modified);
}

void testTriStateCheckbox::updateStyles(QTreeWidgetItem *item, int column)
{
    if (!item || column != 0)
        return;

    Qt::CheckState state = item->checkState(0);
    QTreeWidgetItem *parent = item->parent();
}
