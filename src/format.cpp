#include "lib/ASTNodes.h"
#include "lib/mParser.h"
#include "lib/lex.h"
#include <iostream>
#include <string>
#include <ostream>

// Utility function to create an indentation string
std::string indentString(int indentLevel) {
    return std::string(indentLevel * 4, ' '); // 4 spaces per indent level
}

// Forward declarations with the new signature including std::ostream
void formatAST(std::ostream& os, const std::unique_ptr<ASTNode>& node, int indent = 0);

void formatBinaryOpNode(std::ostream& os, const BinaryOpNode* node, int indent) {
    os << '(';
    formatAST(os, node->left, 0);  
    os << ' ' << node->op.value << ' ';    
    formatAST(os, node->right, 0);
    os << ')';
}

void formatNumberNode(std::ostream& os, const NumberNode* node, int indent) {
    os << indentString(indent) << node->value.value;
}

void formatBooleanNode(std::ostream& os, const BooleanNode* node, int indent) {
    os << indentString(indent) << node->value.value;
}

void formatVariableNode(std::ostream& os, const VariableNode* node, int indent) {
    os << indentString(indent) << node->identifier.value;
}

void formatIfNode(std::ostream& os, const IfNode* node, int indent) {
    os << indentString(indent) << "if ";
    formatAST(os, node->condition, 0);
    os << " {\n";
    formatAST(os, node->trueBranch, indent + 1);
    if (node->falseBranch) {
        os << "\n" << indentString(indent) << "}\n" << indentString(indent) << "else {\n";
        formatAST(os, node->falseBranch, indent + 1);
    }
    os << "\n" << indentString(indent) << "}";
}

void formatAssignmentNode(std::ostream& os, const AssignmentNode* node, int indent) {
    os << indentString(indent) << "(" << node->identifier.value;
    os << " = ";
    formatAST(os, node->expression, 0);
    os << ")";
}

void formatWhileNode(std::ostream& os, const WhileNode* node, int indent) {
    os << indentString(indent) << "while ";
    formatAST(os, node->condition, 0);
    os << " {\n";
    formatAST(os, node->body, indent + 1);
    os << "\n" << indentString(indent) << "}";
}

void formatPrintNode(std::ostream& os, const PrintNode* node, int indent) {
    os << indentString(indent) << "print ";
    formatAST(os, node->expression, 0);
}

void formatBlockNode(std::ostream& os, const BlockNode* node, int indent) {
    bool isFirstStatement = true;
    for (const auto& stmt : node->statements) {
        if (!isFirstStatement) {
            os << "\n";
        }
        formatAST(os, stmt, indent);
        isFirstStatement = false;
    }
}


void formatAST(std::ostream& os, const std::unique_ptr<ASTNode>& node, int indent) {
    if (!node) return;

    switch (node->getType()) {
        case ASTNode::Type::BinaryOpNode:
            formatBinaryOpNode(os, static_cast<const BinaryOpNode*>(node.get()), indent);
            break;
        case ASTNode::Type::NumberNode:
            formatNumberNode(os, static_cast<const NumberNode*>(node.get()), indent);
            break;
        case ASTNode::Type::BooleanNode:
            formatBooleanNode(os, static_cast<const BooleanNode*>(node.get()), indent);
            break;
        case ASTNode::Type::VariableNode:
            formatVariableNode(os, static_cast<const VariableNode*>(node.get()), indent);
            break;
        case ASTNode::Type::AssignmentNode:
            formatAssignmentNode(os, static_cast<const AssignmentNode*>(node.get()), indent);
            break;
        case ASTNode::Type::PrintNode:
            formatPrintNode(os, static_cast<const PrintNode*>(node.get()), indent);
            break;
        case ASTNode::Type::IfNode:
            formatIfNode(os, static_cast<const IfNode*>(node.get()), indent);
            break;
        case ASTNode::Type::WhileNode:
            formatWhileNode(os, static_cast<const WhileNode*>(node.get()), indent);
            break;
        case ASTNode::Type::BlockNode:
            formatBlockNode(os, static_cast<const BlockNode*>(node.get()), indent);
            break;
        default:
            os << indentString(indent) << "/* Unknown node type */";
            break;
    }
}

int main() {
    std::ostream& os = std::cout;
    std::string line;
    std::string inputCode;
    while (std::getline(std::cin, line)) {
        inputCode += line + "\n";
    }

    Lexer lexer(inputCode);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    std::unique_ptr<ASTNode> ast = parser.parse();

    formatAST(std::cout, ast);
    os << std::endl;

    return 0;
}
