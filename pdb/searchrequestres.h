#ifndef SEARCHREQUESTRES_H
#define SEARCHREQUESTRES_H
/*
class stores results of the search requests
*/
#include <QString>
#include <vector>

class SearchRequestRes
{
public:
    SearchRequestRes();
    //
    int getTreeID() const;
    void setTreeID(int i_tree_id);
    //
    const QString& getTreeName() const;
    void setTreeName(const QString& str_tree_name);
    //
    int getNodeID() const;
    void setNodeID(int i_node_id);
    //
    const QString& getNodeName() const;
    void setNodeName(const QString& str_node_name);
    //
    const QString& getSearchFlag () const;
    void setSearchFlag(const QString& str_flag);

private:

    int     m_iIdTree;
    QString m_strTreeName;
    int     m_iIdNode;
    QString m_strNodeName;
    QString m_strSearchFlag;
    //QString m_str
};
//
typedef std::vector<SearchRequestRes> SearchResArray;

#endif // SEARCHREQUESTRES_H
