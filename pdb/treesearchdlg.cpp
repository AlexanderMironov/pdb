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
#include "treesearchdlg.h"
#include "ui_treesearchdlg.h"
#include "mytree.h"
#include "GDefinitions.h"
#include "../CommonInclude/pdb/VariantPtr.h"
#include "rootoftree.h"
#include "dbacccesssafe.h"
#include "logger.h"
#include "../CommonInclude/pdb/pdb_style.h"
//#include "waiter.h"
//

//
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QtSql>
//
//
//
TreeSearchDlg::TreeSearchDlg(QWidget *parent) :
    QDialog(parent)
    ,ui(new Ui::TreeSearchDlg)
    ,m_uiTreeColNum(0)
    ,m_uiNodeColNum (1)
    ,m_uiLocationColNum(2)
{
    ui->setupUi(this);
    //
    m_bFillModeOn = false;
    setSignalSocketLinks();
    //
    setFormLayout();
    //
    this->ui->m_SearchResults->setSelectionMode( QAbstractItemView::SingleSelection );
    //this->ui->m_checkSearchInAttach->setToolTip(tr("Search is possible only in non-binary files and can take long time"));
    //
    setHeaderParams();
    //
    //disable search button
    this->ui->m_bSearch->setEnabled(false);
    //
    onFromSearchClick();
    onToSearchClick();
    //
    //ui->m_checkSearchInAttach->setEnabled(false);
    //ui->m_checkWholeWords->setEnabled(false);
}

TreeSearchDlg::~TreeSearchDlg()
{
    delete ui;
}

void TreeSearchDlg::setHeaderParams ()
{
    this->ui->m_SearchResults->setColumnCount(3);
    QStringList str_att_header;
    str_att_header<<"Tree" << "Node" << "Location";
    this->ui->m_SearchResults->setHorizontalHeaderLabels(str_att_header);
    //
    this->ui->m_SearchResults->setColumnWidth(m_uiTreeColNum,100);
    this->ui->m_SearchResults->setColumnWidth(m_uiNodeColNum,245);
    this->ui->m_SearchResults->setColumnWidth(m_uiLocationColNum,70);
    //
    QHeaderView *header = this->ui->m_SearchResults->horizontalHeader();
    header->setDefaultAlignment(Qt::AlignCenter);
    //
    header->setSectionResizeMode(m_uiTreeColNum,       QHeaderView::Fixed);
    header->setSectionResizeMode(m_uiNodeColNum,       QHeaderView::Stretch);
    header->setSectionResizeMode(m_uiLocationColNum,   QHeaderView::Fixed);
}

bool TreeSearchDlg::getEarliestDateFromDB ()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    Q_ASSERT(ptr_db);
    if (NULL == ptr_db)
        return false;
    //
    QDateTime   attach_change;
    //
    QSqlQuery qry(*ptr_db);
    //
    // select from both tables because it is modification, but not creation date.
    //
    QString str_select_str = QString ("select min(node_tbl.last_change) as node_change, min(attachments.last_change) as attach_change from node_tbl, attachments;");
    //
    if (!qry.prepare( str_select_str ))
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err, &str_select_str );
        return false;

    } else if( !qry.exec() )
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err, &str_select_str );
        return false;
    }else
    {
        while( qry.next() )
        {
            m_dtEarliestDateTime = qry.value(0).toDateTime();
            attach_change = qry.value(1).toDateTime();
            break;
        }; // while( qry.next() )
    }; //if (!qry.prepare( str_select_str ))
    //
    if ( attach_change < m_dtEarliestDateTime )
        m_dtEarliestDateTime = attach_change;
    //
    return true;
}

void TreeSearchDlg::setFormLayout()
{
    m_ptrFormLayout = new QGridLayout(this);
    int i_row = 0;
    //->addWidget(ptr_widget, row,column,rowSpan,columnSpan, aligment);
    m_ptrFormLayout->addWidget(this->ui->SearchLabel, i_row,0,1,1, Qt::AlignLeft);
    m_ptrFormLayout->addWidget(this->ui->m_eSearchString, i_row, 1, 1, 5);
    //
    // i_row++;
    //m_ptrFormLayout->addWidget(this->ui->m_lblPatternSyntax, i_row,0,1,1, Qt::AlignLeft);
    //m_ptrFormLayout->addWidget(this->ui->m_cmbPatternSyntax, i_row, 1, 1, 5);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblSearchOptions, i_row,0,1,6);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkCaseSensitive, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkSearchInNode, i_row,3,1,3);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkRegExp, i_row,0,1,3);
    //m_ptrFormLayout->addWidget(this->ui->m_checkWholeWords, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkSearchInDescriptor, i_row,3,1,3);
    //
    i_row++;
    //m_ptrFormLayout->addWidget(this->ui->m_checkRegExp, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkCurrentTree, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkSearchInANames, i_row,3,1,3);
    //
    //i_row++;
    //m_ptrFormLayout->addWidget(this->ui->m_checkCurrentTree, i_row,0,1,3);
    //m_ptrFormLayout->addWidget(this->ui->m_checkSearchInAttach, i_row,3,1,3);
    //
    // earlieast search day is the 1-st day from db (attachment or node)
    // latest day is always today
    getEarliestDateFromDB ();
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkBoxFrom, i_row,0,1,1);
    m_ptrFormLayout->addWidget(this->ui->m_dateTimeEdit_from, i_row,1,1,2);
    this->ui->m_dateTimeEdit_from->setToolTip(tr("earliest creation or modification date"));
    this->ui->m_dateTimeEdit_from->setDateTimeRange(m_dtEarliestDateTime, QDateTime::currentDateTime());
    this->ui->m_dateTimeEdit_from->setCalendarPopup(true);
    //..
    m_ptrFormLayout->addWidget(this->ui->m_checkBoxTo, i_row,3,1,1);
    m_ptrFormLayout->addWidget(this->ui->m_dateTimeEdit_to, i_row,4,1,2);
    this->ui->m_dateTimeEdit_to->setToolTip(tr("latest creation or modification date"));
    this->ui->m_dateTimeEdit_to->setDateTimeRange(m_dtEarliestDateTime, QDateTime::currentDateTime());
    this->ui->m_dateTimeEdit_to->setDateTime(QDateTime::currentDateTime());
    this->ui->m_dateTimeEdit_to->setCalendarPopup(true);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblSearchResults, i_row,0,1,6);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_SearchResults, i_row,0,1,6);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblFake2, i_row,0,1,6);
    this->ui->LblFake2->setVisible(false);
    //
    //i_row++;
    //m_ptrFormLayout->addWidget(this->ui->m_progressBar, i_row,0,1,6);
    //
    //this is fake element, just for space
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblFake, i_row,0,1,6);
    this->ui->LblFake->setVisible(false);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_bSearch, i_row,0,1,2);
    m_ptrFormLayout->addWidget(this->ui->m_bClearResult, i_row,2,1,2);
    m_ptrFormLayout->addWidget(this->ui->m_bHide, i_row,4,1,2);
    //
    this->setLayout(m_ptrFormLayout);
}

void TreeSearchDlg::setSignalSocketLinks()
{
    //
    //process signals from buttons
    //
    QObject::connect(this->ui->m_bHide,         SIGNAL(clicked()),              this, SLOT( onHideButton  () ));
    QObject::connect(this->ui->m_bClearResult,  SIGNAL(clicked()),              this, SLOT( onClearButton () ));
    QObject::connect(this->ui->m_bSearch,       SIGNAL(clicked()),              this, SLOT( onSearchButton() ));
    //when search string changed
    QObject::connect(this->ui->m_eSearchString, SIGNAL(textChanged(QString)),   this, SLOT (onChangeSearchString(QString) ));
    //
    QObject::connect( this->ui->m_checkRegExp,             SIGNAL(stateChanged(int)),              this, SLOT( onChangeRegExpState (int)  ));
    //
    //when search conditions are changed (in combination with search line status enable or disable search button)
    QObject::connect(this->ui->m_checkSearchInNode,         SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    QObject::connect(this->ui->m_checkSearchInDescriptor,   SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    QObject::connect(this->ui->m_checkSearchInANames,       SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    //QObject::connect(this->ui->m_checkSearchInAttach,       SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    //
    //process click to the selected item
    QObject::connect(this->ui->m_SearchResults, SIGNAL ( itemSelectionChanged () ), this, SLOT( onSearchTableClick() ) );
    //
    QObject::connect( this->ui->m_checkBoxFrom, SIGNAL(clicked()), this, SLOT(onFromSearchClick()) );
    QObject::connect( this->ui->m_checkBoxTo, SIGNAL(clicked()),    this, SLOT(onToSearchClick())  );
}

void TreeSearchDlg::keyPressEvent (QKeyEvent * event)
{
    int key_code = event->key();
    //
    switch( key_code )
    {
    case Qt::Key_Escape:
        onHideButton();
        return;
    default:
        break;
    };
    //
    QDialog::keyPressEvent(event);
};

void TreeSearchDlg::setPtrToComboBox (QComboBox*     ptr_combo)
{
    m_ptrCombo = ptr_combo;
};

void TreeSearchDlg::setPtrToTree (MyTree*        ptr_to_tree)
{
    m_ptrTree = ptr_to_tree;
}
//-----------------------------------------------------------------------------------------
void    TreeSearchDlg::onFromSearchClick()
{
    if (this->ui->m_checkBoxFrom->checkState() == Qt::Checked)
    {
        this->ui->m_dateTimeEdit_from->setEnabled(true);
    }
    else
    {
        this->ui->m_dateTimeEdit_from->setEnabled(false);
    };
}

void    TreeSearchDlg::onToSearchClick()
{
    if (this->ui->m_checkBoxTo->checkState() == Qt::Checked)
    {
        this->ui->m_dateTimeEdit_to->setEnabled(true);
    }
    else
    {
        this->ui->m_dateTimeEdit_to->setEnabled(false);
    };
}

void TreeSearchDlg::onChangeRegExpState (int value)
{
    switch (value)
    {
        case Qt::Checked:
            this->ui->SearchLabel->setText(tr("Search pattern"));
            //
            this->ui->m_checkCaseSensitive->setChecked(false);
            this->ui->m_checkCaseSensitive->setEnabled(false);
            //
            //this->ui->m_checkWholeWords->setChecked(false);
            //this->ui->m_checkWholeWords->setEnabled(false);
        break;
        case Qt::Unchecked:
            this->ui->SearchLabel->setText(tr("Search text:"));
            //
            this->ui->m_checkCaseSensitive->setEnabled(true);
            this->ui->m_checkCaseSensitive->setChecked(false);
            //
            //this->ui->m_checkWholeWords->setEnabled(true);
        break;
    };
}

//-----------------------------------------------------------------------------------------
void TreeSearchDlg::onClickSearchArray()
{
    QString str_search = this->ui->m_eSearchString->text();
    enableSearchButton(str_search);
}

bool TreeSearchDlg::isSearchArrayDefined()
{
    if (
            (this->ui->m_checkSearchInNode->checkState() == Qt::Checked)        ||
            (this->ui->m_checkSearchInDescriptor->checkState() == Qt::Checked)  ||
            (this->ui->m_checkSearchInANames->checkState() == Qt::Checked)      //||
            //(this->ui->m_checkSearchInAttach->checkState() == Qt::Checked)
        )
    {
        return true;
    };
    //
    return false;
}

void TreeSearchDlg::enableSearchButton  (const QString& str_search_string)
{
    if ( (str_search_string.length() != 0) && isSearchArrayDefined () )
    {
        this->ui->m_bSearch->setEnabled(true);
    }
    else
    {
        this->ui->m_bSearch->setEnabled(false);
    };
};
//
// click "search" button here
//
void TreeSearchDlg::onSearchButton()
{
    startSearch ();
};

QString TreeSearchDlg::generateTimeCondition (const QString& str_time_tbl_name )
{
    QString str_res;
    QString str_date_from;
    QString str_date_to;
    //
    if(ui->m_checkBoxFrom->checkState() == Qt::Checked)
    {
        str_date_from = ui->m_dateTimeEdit_from->date().toString("yyyy-MM-dd");
    };
    //
    if(ui->m_checkBoxTo->checkState() == Qt::Checked)
    {
        str_date_to = ui->m_dateTimeEdit_to->date().toString("yyyy-MM-dd");
    };
    //
    if ((str_date_from.length() > 0) && (str_date_to.length() > 0)) //both dates were defined
    {
        str_res = QString("(%1.last_change BETWEEN '%2' AND '%3')").arg(str_time_tbl_name).arg(str_date_from).arg(str_date_to);
    } else if(str_date_from.length() > 0)
    {
        str_res = QString("(%1.last_change >= '%2')").arg(str_time_tbl_name).arg(str_date_from);
    } else if(str_date_to.length() > 0)
    {
         str_res = QString("(%1.last_change <= '%2')").arg(str_time_tbl_name).arg(str_date_to);
    };
    //
    return str_res;
};

QString TreeSearchDlg::generateNodeSQLRequest (bool b_search_in_node)
{
    QString str_res = "select root_tbl.id_tree, root_tbl.tree_name, node_tbl.id_node, node_tbl.node_name from node_tbl,root_tbl";
    QString str_common_condition = " WHERE ";
    QString str_tree_condition;
    QString str_match_condition;
    const QString str_join_condition = " AND (root_tbl.id_tree=node_tbl.id_tree)";
    const QString str_time_cond = generateTimeCondition("node_tbl");
    QString str_search_field;
    //
    if (b_search_in_node)
    {
        str_search_field = "node_name";
    }else
    {
        str_search_field = "node_descriptor";
    };
    //
    if ( this->ui->m_checkCurrentTree->checkState() == Qt::Checked )
    {
        str_tree_condition = QString ("node_tbl.id_tree = %1").arg(getActualTreeID());
    };
    //
    //do not forget about this: if (this->ui->m_checkRegExp->checkState() == Qt::Checked)
    //
    // node name condition
    if (this->ui->m_checkRegExp->checkState() == Qt::Checked)
    {

    }else  //non-regexp query
    {
        if (this->ui->m_checkCaseSensitive->checkState() == Qt::Checked)
        {
            str_match_condition  = "(";
            str_match_condition  += str_search_field;
            str_match_condition  += " LIKE BINARY '%";
            str_match_condition += this->ui->m_eSearchString->text();
            str_match_condition += "%')";
        }else{ //node_name LIKE '%швейц%';
            str_match_condition  = "(";
            str_match_condition  += str_search_field;
            str_match_condition  += " LIKE '%";
            str_match_condition += this->ui->m_eSearchString->text();
            str_match_condition += "%')";
        };
    };
    //
    if (str_tree_condition.length() > 0)
    {
        str_common_condition += "(";
    };
    //
    str_common_condition += str_match_condition;
    //
    if (str_tree_condition.length() > 0)
    {
        str_common_condition += ") AND (";
        str_common_condition += str_tree_condition;
        str_common_condition += ")";
    };
    //
    str_common_condition += str_join_condition;
    //
    if( str_time_cond.length() > 0 )
    {
        str_common_condition += " AND ";
        str_common_condition += str_time_cond;
    };
    //
    str_common_condition += ";";
    str_res += str_common_condition;
    //
    return str_res;
};

QString TreeSearchDlg::generateAttNameSQLReq ()
{
    QString str_res                  = "select root_tbl.id_tree, root_tbl.tree_name, node_tbl.id_node, node_tbl.node_name from node_tbl, root_tbl, attachments";
    QString str_common_condition     = " WHERE ";
    QString str_tree_condition;
    QString str_match_condition;
    const QString str_join_condition = "(root_tbl.id_tree = node_tbl.id_tree) AND (attachments.id_parent = node_tbl.id_node)";
    const QString str_time_cond      = generateTimeCondition("attachments");
    //
    //
    if ( this->ui->m_checkCurrentTree->checkState() == Qt::Checked )
    {
        str_tree_condition = QString (" AND (root_tbl.id_tree = %1)").arg(getActualTreeID());
    };
    //
    if (this->ui->m_checkCaseSensitive->checkState() == Qt::Checked)
    {
        str_match_condition  = " AND (attach_name LIKE BINARY '%";
        str_match_condition += this->ui->m_eSearchString->text();
        str_match_condition += "%')";
    }else{ //node_name LIKE '%швейц%';
        str_match_condition  = " AND (attach_name LIKE '%";
        str_match_condition += this->ui->m_eSearchString->text();
        str_match_condition += "%')";
    };
    //concatenate it
    //
    str_res += str_common_condition;
    str_res += str_join_condition;
    str_res += str_match_condition;
    //
    if ( str_tree_condition.length() > 0)
    {
        str_res += str_tree_condition;
    };
    //
    if(str_time_cond.length()>0)
    {
        str_res += " AND ";
        str_res += str_time_cond;
    };
    //
    str_res += ";";
    //
    return str_res;
};

int TreeSearchDlg::getActualTreeID ()
{
    QVariant root_back_data     = m_ptrCombo->itemData( m_ptrCombo->currentIndex() );
    RootOfTree* ptr_root        = VariantPtr<RootOfTree>::asPtr( root_back_data );
    Q_ASSERT ( ptr_root );
    TreeLeaf* ptr_root_node     = ptr_root->getChildLeaf();
    return ptr_root_node->getTreeID();
};

void TreeSearchDlg::fillTable (const SearchResArray& search_array)
{
    this->ui->m_SearchResults->setRowCount(search_array.size());
    //
    for (unsigned int i = 0; i < search_array.size(); ++i)
    {
        fillDataRow(i,search_array[i]);
    };
};

void    TreeSearchDlg::fillDataRow (int i_row_num, const SearchRequestRes &res_element)
{
    m_bFillModeOn = true;
    QTableWidgetItem* ptr_cell_tree  = makeCellTree(res_element);
    QTableWidgetItem* ptr_cell_node  = makeCellNode(res_element);
    QTableWidgetItem* ptr_cell_flags = makeCellFlag(res_element);
    //
    this->ui->m_SearchResults->setItem( i_row_num, m_uiTreeColNum,       ptr_cell_tree  );
    this->ui->m_SearchResults->setItem( i_row_num, m_uiNodeColNum,       ptr_cell_node  );
    this->ui->m_SearchResults->setItem( i_row_num, m_uiLocationColNum,   ptr_cell_flags );
    m_bFillModeOn = false;
};

QTableWidgetItem* TreeSearchDlg::makeCellTree(const SearchRequestRes& res_element)
{
    QTableWidgetItem* ptr_item = new QTableWidgetItem( res_element.getTreeName() );
    ptr_item->setData(Qt::UserRole, QVariant(res_element.getTreeID()));
    //
    return ptr_item;
};

QTableWidgetItem* TreeSearchDlg::makeCellNode(const SearchRequestRes& res_element)
{
    QTableWidgetItem* ptr_item = new QTableWidgetItem( res_element.getNodeName() );
    ptr_item->setData(Qt::UserRole, QVariant(res_element.getNodeID()));
    return ptr_item;
};

QTableWidgetItem* TreeSearchDlg::makeCellFlag(const SearchRequestRes& res_element)
{
    QTableWidgetItem* ptr_item = new QTableWidgetItem( res_element.getSearchFlag() );
    return ptr_item;
};

void TreeSearchDlg::startSearch  ()
{
    SearchResArray search_array;
    //
    if(this->ui->m_checkSearchInNode->checkState() == Qt::Checked)
    {
        QString str_node_query = generateNodeSQLRequest (true); //search in  node
        if (false == makeUniversalSearch(search_array,str_node_query,"N")) //"N" means "search in node names"
        {
            return; //something goes wrong with database;
        };
    };
    //
    if(this->ui->m_checkSearchInDescriptor->checkState() == Qt::Checked)
    {
        QString str_node_query = generateNodeSQLRequest (false); //search in  node descriptors
        if (false == makeUniversalSearch(search_array,str_node_query,"D")) //"D" means "search in node  descriptors"
        {
            return; //something goes wrong with database;
        };
    };
    //
    if (this->ui->m_checkSearchInANames->checkState() == Qt::Checked)
    {
        QString str_node_query = generateAttNameSQLReq();
        //
        if (false == makeUniversalSearch(search_array,str_node_query,"A")) //"A" means "search in attachments names"
        {
            return; //something goes wrong with database;
        };
    };
    //
    //ToDo: add search in attachments
    //
    clearSearchResults();
    //
    fillTable(search_array);
    //
    //m_Searcher.start(QThread::IdlePriority);
    //
    return;
};

bool TreeSearchDlg::makeUniversalSearch (SearchResArray& search_array, const QString& str_query, const QString& str_search_flag)
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    if ( !qry.prepare( str_query ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        QMessageBox box;
        box.setText("Unable to get exec the query. Stop. ");
        box.exec();
        //
        return false;
    };
    //
    while (qry.next())
    {
        const int       i_tree          =  qry.value(0).toInt();
        const QString   str_tree_name   =  qry.value(1).toString();
        const int       i_node          =  qry.value(2).toInt();
        const QString   str_node_name   =  qry.value(3).toString();
        //
        SearchRequestRes res;
        res.setTreeID   (i_tree);
        res.setTreeName (str_tree_name);
        res.setNodeID   (i_node);
        res.setNodeName (str_node_name);
        res.setSearchFlag(str_search_flag);
        //
        bool b_already_exist = false;
        //
        for (unsigned int i = 0; i < search_array.size(); ++i)
        {
            if(search_array[i].getNodeID() == res.getNodeID())
            {
                b_already_exist = true;
                search_array[i].setSearchFlag(str_search_flag); //just add flag to existing element
                break;
            };
        };
        //
        if(false == b_already_exist)
        {
            search_array.push_back(res);
        };
    };
    //
    return true;
};

void TreeSearchDlg::onChangeSearchString(QString str_search)
{
    enableSearchButton(str_search);
};

void TreeSearchDlg::onHideButton()
{
    hide();
}

void TreeSearchDlg::onClearButton()
{
    this->ui->m_eSearchString->setText(tr(""));
    //
    this->ui->m_SearchResults->setRowCount(0);
}
/*
void TreeSearchDlg::lockInterface (bool b_lock )
{
    this->ui->m_eSearchString->setEnabled(!b_lock);
    //
    this->ui->m_checkSearchInNode->setEnabled(!b_lock);
    this->ui->m_checkSearchInDescriptor->setEnabled(!b_lock);
    this->ui->m_checkSearchInANames->setEnabled(!b_lock);
    this->ui->m_checkSearchInAttach->setEnabled(!b_lock);
    //
    this->ui->m_checkCaseSensitive->setEnabled(!b_lock);
    //
    this->ui->m_checkRegExp->setEnabled(!b_lock);
    if (this->ui->m_checkRegExp->checkState() != Qt::Checked)
    {
        this->ui->m_checkWholeWords->setEnabled(!b_lock);
    };
    //
    this->ui->m_checkCurrentTree->setEnabled(!b_lock);
    //
    this->ui->m_bClearResult->setEnabled(!b_lock);
    this->ui->m_bHide->setEnabled(!b_lock);
    //
    //this->ui->m_progressBar->setVisible(b_lock);

};
*/

void TreeSearchDlg::clearSearchResults()
{
    this->ui->m_SearchResults->setRowCount(0);
}

void TreeSearchDlg::onSearchTableClick()
{
    //
    //react on table result click
    //
    QList<QTableWidgetItem* > selection_list = this->ui->m_SearchResults->selectedItems();
    //
    if (selection_list.size() != 1)
    {
        return;
    };
    //
    QTableWidgetItem* ptr_item = selection_list.at(0);
    //
    QTableWidgetItem* ptr_tree = this->ui->m_SearchResults->item(ptr_item->row(), m_uiTreeColNum);
    const int i_tree_id = ptr_tree->data(Qt::UserRole).toInt();
    QTableWidgetItem* ptr_node = this->ui->m_SearchResults->item(ptr_item->row(), m_uiNodeColNum);
    const int i_node_id = ptr_node->data(Qt::UserRole).toInt();
    //
    if (getActualTreeID() != i_tree_id)
    {
        switchToTheTree( i_tree_id );
    };
    //
    TreeLeaf* ptr_actual_top_leaf = (TreeLeaf*) m_ptrTree->topLevelItem(0);
    //search the child-parent chain for the node with defined iD
    std::stack<int> search_stack;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
    {
        return;
    };
    //
    if ( createNodeSearchPath(ptr_db, search_stack, i_node_id) == false )
    {
        return;
    };
    //
    getAnExpandTargetNode(search_stack, ptr_actual_top_leaf);
    //
    return;
};

void TreeSearchDlg::getAnExpandTargetNode (std::stack<int>& search_stack, TreeLeaf* ptr_actual_node)
{
    if(NULL == ptr_actual_node)
    {
        return;
    };
    //
    const int i_node_id = search_stack.top();
    search_stack.pop();
    //
    ptr_actual_node->extractAndFillChildList();
    //
    TreeLeaf*  ptr_child_node = ptr_actual_node->getChildNode(i_node_id);
    //
    if (search_stack.size() == 0)
    {
        if(ptr_child_node)
        {
            m_ptrTree->setCurrentItem(ptr_child_node);
        };
        //
        return;
    };
    //
    getAnExpandTargetNode(search_stack, ptr_child_node);
};

bool TreeSearchDlg::createNodeSearchPath (QSqlDatabase* ptr_db, std::stack<int>& search_stack, int current_node_id)
{
    search_stack.push(current_node_id); //push actual value into the stack
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_query = QString ("select id_parent from node_tbl where id_node = %1;").arg(current_node_id);
    //
    if ( !qry.prepare( str_query ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        QMessageBox box;
        box.setText("Unable to get exec the query. Stop. ");
        box.exec();
        //
        return false;
    };
    //
    qry.next();
    //
    const int i_parent_id =  qry.value(0).toInt();
    //
    if (0 == i_parent_id)
    {
        return true;
    };
    //
    return createNodeSearchPath(ptr_db, search_stack, i_parent_id);
};

void TreeSearchDlg::switchToTheTree (int i_tree_id)
{
    const int i_actual_index = m_ptrCombo->currentIndex();

    for (int i = 0; i < m_ptrCombo->count(); i++ )
    {
        QVariant root_data = m_ptrCombo->itemData(i);
        RootOfTree* ptr_root = VariantPtr<RootOfTree>::asPtr( root_data );
        Q_ASSERT (ptr_root);
        //
        if (ptr_root->getID() == i_tree_id)
        {
            if (i_actual_index != i )
                m_ptrCombo->setCurrentIndex(i);
            //
            return;
        };
    };
};

void TreeSearchDlg::showEvent (QShowEvent *)
{
    this->ui->m_eSearchString->setFocus();
}
