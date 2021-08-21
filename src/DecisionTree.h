#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__
#include "Agent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"
#include <map>

class DecisionTree
{
public:
	// constructor and destructor
	DecisionTree(Agent* agent);
	~DecisionTree();

	// getters and setters
	Agent* getAgent() const;
	LOSCondition* getLOSNode() const;
	RadiusCondition* getRadiusNode() const;
	CloseCombatCondition* getCloseCombatNode() const;
	void setAgent(Agent* agent);
	std::map<std::string, DisplayObject*>& getMap();

	// convenience functions
	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void Display();
	void Update();
	void clean();
	
	void MakeDecision(); // in order traversal
	
private:
	Agent* m_agent;

	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;

	void m_buildTree();

	std::vector<TreeNode*> m_treeNodeList;
	std::map<std::string, DisplayObject*> m_targets; // Knowledge of any targets.

	TreeNode* currentNode;
};


#endif /* defined (__DECISION_TREE__) */