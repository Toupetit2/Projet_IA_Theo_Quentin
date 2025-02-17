#pragma once
#include <iostream>
#include <vector>
#include <memory>

enum class NodeState { SUCCESS, FAILURE, RUNNING };

class BTNode {
public:
    virtual ~BTNode();
    virtual NodeState execute() = 0;
};