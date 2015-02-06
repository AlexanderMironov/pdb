#include "searchrequestres.h"

SearchRequestRes::SearchRequestRes()
{
    m_iIdTree = 0;
    m_iIdNode = 0;
}

int SearchRequestRes::getTreeID() const
{
    return m_iIdTree;
};
void SearchRequestRes::setTreeID(int i_tree_id)
{
    m_iIdTree = i_tree_id;
};
//
const QString& SearchRequestRes::getTreeName() const
{
    return m_strTreeName;
};

void SearchRequestRes::setTreeName(const QString& str_tree_name)
{
    m_strTreeName = str_tree_name;
};
//
int SearchRequestRes::getNodeID() const
{
    return m_iIdNode;
};

void SearchRequestRes::setNodeID(int i_node_id)
{
    m_iIdNode= i_node_id;
};
//
const QString& SearchRequestRes::getNodeName() const
{
    return m_strNodeName;
};

void SearchRequestRes::setNodeName(const QString& str_node_name)
{
    m_strNodeName = str_node_name;
};

const QString& SearchRequestRes::getSearchFlag () const
{
    return m_strSearchFlag;
};

void SearchRequestRes::setSearchFlag(const QString& str_flag)
{
    m_strSearchFlag += str_flag;
};
