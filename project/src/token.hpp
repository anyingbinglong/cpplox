#pragma once

#include <ostream>
#include <string>

#include <boost/lexical_cast.hpp>

namespace motts { namespace lox {
    /*
    We want to serialize an enum's name, but C++ doesn't generate that for us.
    This will get better when C++ gets reflection and injection (https://youtu.be/4AfRAVcThyA?t=21m33s).
    But for now settle for x-macro technique (https://en.wikipedia.org/wiki/X_Macro).

    The enum field names are intentionally lowercase rather than uppercase.
    Uppercase isn't meant to denote a constant; it's meant to denote a macro.
    Historically, constants were implemented as macros, which is why we became
    accustomed to uppercasing them, but if we're not writing macros then we
    shouldn't uppercase the name. This manifests as a real practical issue because
    some other header already defined a macro named EOF, so if we try to use EOF as
    one of our enum field names, bad things will happen.
    */
    #define MOTTS_LOX_TOKEN_TYPE_NAMES \
        /* Single char tokens */ \
        X(left_paren) X(right_paren) \
        X(left_brace) X(right_brace) \
        X(comma) X(dot) X(minus) X(plus) X(semicolon) X(slash) X(star) \
        \
        /* One or two char tokens */ \
        X(bang) X(bang_equal) X(equal) X(equal_equal) \
        X(greater) X(greater_equal) X(less) X(less_equal) \
        \
        /* Literals */ \
        X(identifier) X(string) X(number) \
        \
        /* Keywords */ \
        /* These are real reserved words in C++, so mangle their names in some way */ \
        X(and_) X(class_) X(else_) X(false_) X(fun_) X(for_) X(if_) X(nil_) X(or_) \
        X(print_) X(return_) X(super_) X(this_) X(true_) X(var_) X(while_) \
        \
        X(eof)

    enum class Token_type {
        #define X(name) name,
        MOTTS_LOX_TOKEN_TYPE_NAMES
        #undef X
    };

    std::ostream& operator<<(std::ostream& os, Token_type token_type);

    /*
    In Java, the `Token` field `Object literal` can be assigned any value of any type,
    and we can implicitly convert it to a string without any extra work on our part.
    This is possible because `Object` has an overridable `toString` method,
    and the Java library comes with pre-defined derived types for strings, doubles, bools,
    and more, whose overridden `toString` will do the right thing.

    Not so in C++. In C++, there is no universal base class from which everything inherits,
    and no pre-defined derived types with overridden `toString` methods.

    I considered Boost.Any and Boost.Variant, but I settled on templated types
    that derive from `Token` with a virtual `to_string` function, which is pretty close
    to how the Java implementation works. This lets me generate derived types automatically
    for any literal type -- current and future -- each of which overrides `to_string`
    to do the right thing for that type. This also let's me construct non literal-type tokens
    that don't contain any dummy or empty literal value.
    */

    struct Token {
        Token_type type;
        std::string lexeme;

        // Warning! This field isn't used yet in the Scanning chapter and will emit a warning
        int line;

        Token(Token_type type_, const std::string& lexeme_, int line_);
        virtual std::string to_string() const;

        // Base class boilerplate
        virtual ~Token();
        Token(const Token&) = delete;
        Token& operator=(const Token&) = delete;
        Token(Token&&) = delete;
        Token& operator=(Token&&) = delete;
    };

    template<typename Literal_type>
        struct Token_literal : Token {
            Literal_type literal_value;

            Token_literal(
                Token_type type_,
                const std::string& lexeme_,
                const Literal_type& literal_value_,
                int line_
            );
            std::string to_string() const override;
        };

    // impl Token_literal

        template<typename Literal_type>
            Token_literal<Literal_type>::Token_literal(
                Token_type type_,
                const std::string& lexeme_,
                const Literal_type& literal_value_,
                int line_
            )
                : Token {type_, lexeme_, line_},
                  literal_value {literal_value_}
            {}

        template<typename Literal_type>
            std::string Token_literal<Literal_type>::to_string() const {
                return (
                    boost::lexical_cast<std::string>(type) + " " +
                    lexeme + " " +
                    boost::lexical_cast<std::string>(literal_value)
                );
            }

        // Numbers are expected to always have a fractional part, so specialize that case
        template<>
            inline std::string Token_literal<double>::to_string() const {
                const auto literal_value_str = boost::lexical_cast<std::string>(literal_value);
                return (
                    boost::lexical_cast<std::string>(type) + " " +
                    lexeme + " " +
                    literal_value_str + (literal_value_str.find(".") == std::string::npos ? ".0" : "")
                );
            }

    std::ostream& operator<<(std::ostream& os, const Token& token);
}}
