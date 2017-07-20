#pragma once

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <map>
#include <list>
#include <memory>

namespace Ui {
  class testTriStateCheckbox;
}

class testTriStateCheckbox : public QMainWindow
{
  Q_OBJECT

public:
  // 設定データ構造体定義
  struct StDataElem {
    Qt::CheckState checkState;  // 選択状況
    std::string name;           // 変数名
    std::string category;       // カテゴリ名
  };

  using Data = std::shared_ptr<StDataElem>;                         // データ型（変数名、カテゴリ名）
  using MapByName = std::map<std::string, Data>;                    // 名前別データマップ型（データ名、データのペア）
  using MapByFirstAlphabet = std::map<std::string, MapByName>;      // アルファベット別データマップ型（データ名の先頭アルファベット、名前別データマップのペア）
  using MapByCategory = std::map<std::string, MapByFirstAlphabet>;  // カテゴリ別データマップ型（カテゴリ名、アルファベット別データマップのペア）
  MapByCategory m_CategorizedDatas;   // カテゴリ別データマップ

  explicit testTriStateCheckbox(QWidget *parent = 0);
  ~testTriStateCheckbox();

private:
  Ui::testTriStateCheckbox *ui;

  void test();
  void setupSyncDataTree();
  void checkuncheckSyncData(Qt::CheckState state);
  void createMapByCategory(std::list<Data> dataList);
  Qt::CheckState verifyCheckStatus(std::string category, std::string name = "");

private slots:
    void on_actionCheckAll_triggered();
    void on_actionUncheckAll_triggered();
    void on_actionClear_triggered();

public slots:
    void showFont(QTreeWidgetItem *item);
    void updateStyles(QTreeWidgetItem *item, int column);
};
