using System.Collections;
using System.Collections.Generic;
using UnityEngine;
/// <summary>
/// 决策树节点
/// </summary>
public class TreeNode{
    public TreeNode Parent;
    public object Data;
    public List<TreeNode> Nodes;
    public int min;
    public int max;
}
