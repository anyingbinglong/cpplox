// Related header
#include "expression_impls.hpp"
// C standard headers
// C++ standard headers
#include <utility>
// Third-party headers
// This project's headers
#include "expression_visitor.hpp"

using std::make_shared;
using std::move;
using std::shared_ptr;
using std::static_pointer_cast;
using std::vector;

namespace motts { namespace lox {
    /*
        struct Binary_expr
    */

    Binary_expr::Binary_expr(shared_ptr<const Expr>&& left_arg, Token&& op_arg, shared_ptr<const Expr>&& right_arg) :
        left {move(left_arg)},
        op {move(op_arg)},
        right {move(right_arg)}
    {}

    void Binary_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Binary_expr>(owner_this));
    }

    /*
        struct Grouping_expr
    */

    Grouping_expr::Grouping_expr(shared_ptr<const Expr>&& expr_arg) :
        expr {move(expr_arg)}
    {}

    void Grouping_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Grouping_expr>(owner_this));
    }

    /*
        struct Literal_expr
    */

    Literal_expr::Literal_expr(Literal&& value_arg) :
        value {move(value_arg)}
    {}

    void Literal_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Literal_expr>(owner_this));
    }

    /*
        struct Unary_expr
    */

    Unary_expr::Unary_expr(Token&& op_arg, shared_ptr<const Expr>&& right_arg) :
        op {move(op_arg)},
        right {move(right_arg)}
    {}

    void Unary_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Unary_expr>(owner_this));
    }

    /*
        struct Var_expr
    */

    Var_expr::Var_expr(Token&& name_arg) :
        name {move(name_arg)}
    {}

    void Var_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Var_expr>(owner_this));
    }

    shared_ptr<const Expr> Var_expr::make_assignment_expression(
        shared_ptr<const Expr>&& lhs_expr,
        shared_ptr<const Expr>&& rhs_expr,
        const Runtime_error& /*throwable_if_not_lvalue*/
    ) const {
        return make_shared<Assign_expr>(Token{static_pointer_cast<const Var_expr>(lhs_expr)->name}, move(rhs_expr));
    }

    /*
        struct Assign_expr
    */

    Assign_expr::Assign_expr(Token&& name_arg, std::shared_ptr<const Expr>&& value_arg) :
        name {move(name_arg)},
        value {move(value_arg)}
    {}

    void Assign_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Assign_expr>(owner_this));
    }

    /*
        struct Logical_expr
    */

    Logical_expr::Logical_expr(shared_ptr<const Expr>&& left_arg, Token&& op_arg, shared_ptr<const Expr>&& right_arg) :
        left {move(left_arg)},
        op {move(op_arg)},
        right {move(right_arg)}
    {}

    void Logical_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Logical_expr>(owner_this));
    }

    /*
        struct Call_expr
    */

    Call_expr::Call_expr(
        shared_ptr<const Expr>&& callee_arg,
        Token&& closing_paren_arg,
        vector<shared_ptr<const Expr>>&& arguments_arg
    ) :
        callee {move(callee_arg)},
        closing_paren {move(closing_paren_arg)},
        arguments {move(arguments_arg)}
    {}

    void Call_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Call_expr>(owner_this));
    }

    /*
        struct Get_expr
    */

    Get_expr::Get_expr(shared_ptr<const Expr>&& object_arg, Token&& name_arg) :
        object {move(object_arg)},
        name {move(name_arg)}
    {}

    void Get_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Get_expr>(owner_this));
    }

    shared_ptr<const Expr> Get_expr::make_assignment_expression(
        shared_ptr<const Expr>&& lhs_expr,
        shared_ptr<const Expr>&& rhs_expr,
        const Runtime_error& /*throwable_if_not_lvalue*/
    ) const {
        return make_shared<Set_expr>(
            shared_ptr<const Expr>{static_pointer_cast<const Get_expr>(lhs_expr)->object},
            Token{static_pointer_cast<const Get_expr>(lhs_expr)->name},
            move(rhs_expr)
        );
    }

    /*
        struct Set_expr
    */

    Set_expr::Set_expr(shared_ptr<const Expr>&& object_arg, Token&& name_arg, shared_ptr<const Expr>&& value_arg) :
        object {move(object_arg)},
        name {move(name_arg)},
        value {move(value_arg)}
    {}

    void Set_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const Set_expr>(owner_this));
    }

    /*
        struct This_expr
    */

    This_expr::This_expr(Token&& keyword_arg) :
        keyword {move(keyword_arg)}
    {}

    void This_expr::accept(const shared_ptr<const Expr>& owner_this, Expr_visitor& visitor) const {
        visitor.visit(static_pointer_cast<const This_expr>(owner_this));
    }
}}