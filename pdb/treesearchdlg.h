#ifndef TREESEARCHDLG_H
#define TREESEARCHDLG_H
//
/*
 This file is part of project pdb.

    pdb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    pdb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pdb.  If not, see <http://www.gnu.org/licenses/>.
*/
//
#include <QDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QDateTime>
//#include <QSqlDatabase>
//
#include <stack>
//
//#include "searcher.h"
//#include "searchrequest.h"
#include "searchrequestres.h"
//
class MyTree;
class TreeLeaf;
class QTableWidgetItem;
class QSqlDatabase;

namespace Ui {
class TreeSearchDlg;
}

class TreeSearchDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TreeSearchDlg(QWidget *parent = 0);
    ~TreeSearchDlg();
    //
    void        keyPressEvent       (QKeyEvent * event);
    void        showEvent           (QShowEvent *);
    //
    void        setPtrToComboBox    (QComboBox*     ptr_combo);
    void        setPtrToTree        (MyTree*        ptr_to_tree);

public slots:

private slots:

    void    onHideButton();
    void    onClearButton();
    void    onSearchButton();
    void    onChangeSearchString(QString);
    void    onClickSearchArray();
    void    onSearchTableClick();
    void    onChangeRegExpState (int);
    void    onFromSearchClick();
    void    onToSearchClick();

private:

    //
    void    setHeaderParams         ();
    int     getActualTreeID         ();
    //bool b_search_in_node - true means "search in node" false means "search in descriptors"
    QString generateNodeSQLRequest  (bool b_search_in_node);
    QString generateAttNameSQLReq   ();
    QString generateTimeCondition   (const QString &str_time_tbl_name);
    //
    bool    makeUniversalSearch     (SearchResArray& search_array, const QString& str_query, const QString &str_search_flag);
    //
    void    enableSearchButton      (const QString& str_search_string);
    //
    void    fillTable               (const SearchResArray& search_array);
    void    fillDataRow             (int i_row_num, const SearchRequestRes& res_element);
    //
    //recurcive call stopped when root has been found
    //
    bool    createNodeSearchPath    (QSqlDatabase* ptr_db, std::stack<int>& search_stack, int current_node_id);
    void    getAnExpandTargetNode   (std::stack<int>& search_stack, TreeLeaf *ptr_actual_node);
    //
    QTableWidgetItem*               makeCellTree(const SearchRequestRes& res_element);
    QTableWidgetItem*               makeCellNode(const SearchRequestRes& res_element);
    QTableWidgetItem*               makeCellFlag(const SearchRequestRes& res_element);
    // void    swapDlgMode             ();
    //
    void    startSearch             ();
    void    clearSearchResults      ();
    void    switchToTheTree         (int i_tree_id);
    void    setSignalSocketLinks    ();
    void    setFormLayout           ();
    bool    isSearchArrayDefined    ();
    bool    getEarliestDateFromDB   ();

private:
    Ui::TreeSearchDlg *ui;
    //
    QComboBox*              m_ptrCombo;
    MyTree*                 m_ptrTree;
    //
    bool                    m_bFillModeOn;
    //
    const unsigned int      m_uiTreeColNum;
    const unsigned int      m_uiNodeColNum;
    const unsigned int      m_uiLocationColNum;
    //
    QDateTime               m_dtEarliestDateTime;
    //
    QGridLayout*            m_ptrFormLayout;

};

#endif // TREESEARCHDLG_H
