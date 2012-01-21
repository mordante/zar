/*
 * Copyright (C) 2011 - 2012 by Mark de Wever <koraq@xs4all.nl>
 * Part of the zar project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 *
 * See the COPYING file for more details.
 */

/**
 * @file
 * Defines several validation and failure macros.
 *
 * The macros all throw a lib::texception upon failure. The header also defines
 * some wrappers for static assertions. This allows to validate macro
 * parameters.
 */

#ifndef LIB_EXCEPTION_VALIDATE_TPP_INCLUDED
#define LIB_EXCEPTION_VALIDATE_TPP_INCLUDED

#include "lib/exception/exception.hpp"
#include "lib/string/concatenate.tpp"

namespace lib {

/**
 * Contains wrappers for static_assert.
 *
 * Since static asserts can only be used on types and in some of the macros we
 * need to validate macro parameters, there needs to be a way to extract the
 * type from the parameter. Therefore there are some wrapper functions, whose
 * names are based on the name of the type traits as defined in the standard.
 */
namespace static_assert_wrapper {

/**
 * Tests whether @p T and @p U have the same type.
 *
 * The type @p T must be specified, the type @p U is determined by the function
 * parameter.
 */
template<class T, class U>
inline void
is_same(const U&)
{
	static_assert(std::is_same<T, U>::value, "The types must be the same.");
}

/** Tests whether @p T is an @c enum. */
template<class T>
inline void
is_enum(const T&)
{
	static_assert(std::is_enum<T>::value, "The type must be an enum type.");

}

} // namespace static_assert_wrapper
} // namespace lib

/**
 * Validates whether the @p condition is true.
 *
 * This function is intended to replace @c std::assert(condition).
 *
 * @pre                           @p condition can be evaluated as a boolean
 *                                expression.
 *
 * @param condition               The condition to validate. It the condition
 *                                evaluates to @c true, nothing happens, if it
 *                                evaluates to @c false an @ref lib::texception
 *                                is thrown.
 */
#define VALIDATE(condition)                                                  \
	do {                                                                     \
		if(!(condition)) {                                                   \
			throw ::lib::texception(                                         \
					  ::lib::texception::ttype::internal_failure             \
					, ::lib::concatenate(                                    \
						  "Conditional failure in function '"                \
						, __PRETTY_FUNCTION__                                \
						, "' file '"                                         \
						, __FILE__                                           \
						, "' line '"                                         \
						, __LINE__                                           \
						, "' condition '"                                    \
						, #condition                                         \
						, "'"));                                             \
		}                                                                    \
	} while(0)


/**
 * Throws an @ref lib::texception.
 *
 * This function is intended to replace @c std::assert(false).
 */
#define FAIL                                                                 \
	do {                                                                     \
		throw ::lib::texception(                                             \
				  ::lib::texception::ttype::internal_failure                 \
				, ::lib::concatenate(                                        \
					  "Unconditional failure in function '"                  \
					, __PRETTY_FUNCTION__                                    \
					, "' file '"                                             \
					, __FILE__                                               \
					, "' line '"                                             \
					, __LINE__                                               \
					, "'"));                                                 \
	} while(0)

/**
 * Throws an @ref lib::texception.
 *
 * This function is intended to replace @c std::assert(false).
 *
 * @param message                 A message to add to the exception thrown,
 *                                this can be used to aid debugging.
 */
#define FAIL_MSG(message)                                                    \
	do {                                                                     \
		throw ::lib::texception(                                             \
				  ::lib::texception::ttype::internal_failure                 \
				, ::lib::concatenate(                                        \
					  "Unconditional failure in function '"                  \
					, __PRETTY_FUNCTION__                                    \
					, "' file '"                                             \
					, __FILE__                                               \
					, "' line '"                                             \
					, __LINE__                                               \
					, "' message '"                                          \
					, message                                                \
					, "'"));                                                 \
	} while(0)

/**
 * Validates the range of an @c enum in @c operator<<().
 *
 * The typical use case is:
 * @code
 * switch(enumerate) {
 *     case member_1 : ...; return;
 *     case member_2 : ...; return;
 * }
 * ENUM_FAIL_OUTPUT(enumerate);
 * @endcode
 * Since the enumerate cannot be converted to a string its numeric value is
 * used as error instead. When used in a normal function and with a valid @c
 * operator<< @ref ENUM_FAIL_RANGE can be used instead.
 *
 * @note Normally the compiler can warn if a member of an enum is not handled,
 * at least when the @c switch doesn't have a default handler. So the error
 * should normally never trigger.
 *
 * @note When using the macro's in @ref lib/string/enumerate.tpp this macro
 * won't be needed in user code.
 *
 * @pre                           typeof(enumerate) returns an @c enum type.
 *
 * @param enumerate               The enumerate which failed.
 */
#define ENUM_FAIL_OUTPUT(enumerate)                                          \
	do {                                                                     \
		lib::static_assert_wrapper::is_enum(enumerate);                      \
		throw ::lib::texception(                                             \
				  ::lib::texception::ttype::invalid_value                    \
				, ::lib::concatenate(                                        \
					  "Enumerate out of range during output, value '"        \
					, static_cast<unsigned>(enumerate)                       \
					, "' in function '"                                      \
					, __PRETTY_FUNCTION__                                    \
					, "' file '"                                             \
					, __FILE__                                               \
					, "' line '"                                             \
					, __LINE__                                               \
					, "'"));                                                 \
	} while(0)

/**
 * Validates the range of an @c enum in @c operator>>().
 *
 * The typical use case is:
 * @code
 * if(enumerate == "member_1") { ...; return; }
 * if(enumerate == "member_2") { ...; return; }
 *
 * ENUM_FAIL_INPUT(enumerate);
 * @endcode
 * @warning The compiler cannot verify whether all cases are handled properly
 * so this error might be more common as @ref ENUM_FAIL_OUTPUT.
 *
 * @note When using the macro's in @ref lib/string/enumerate.tpp this macro
 * won't be needed in user code. Also when using macro's in that file the same
 * parameter is used for both the input and output, making this error normally
 * also rare.
 *
 * @pre                           typeof(enumerate) == std::string.
 *
 * @param enumerate               The enumerate which failed.
 */
#define ENUM_FAIL_INPUT(enumerate)                                           \
	do {                                                                     \
		lib::static_assert_wrapper::is_same<std::string>(enumerate);         \
		throw ::lib::texception(                                             \
				  ::lib::texception::ttype::invalid_value                    \
				, ::lib::concatenate(                                        \
					  "Enumerate out of range during input, value '"         \
					, enumerate                                              \
					, "' in function '"                                      \
					, __PRETTY_FUNCTION__                                    \
					, "' file '"                                             \
					, __FILE__                                               \
					, "' line '"                                             \
					, __LINE__                                               \
					, "'"));                                                 \
	} while(0)

/**
 * Validates the range of an @c enum in a function.
 *
 * The typical use case is:
 * @code
 * switch(enumerate) {
 *     case member_1 : ...; return;
 *     case member_2 : ...; return;
 * }
 * ENUM_FAIL_OUTPUT(enumerate);
 * @endcode
 *
 * @note Normally the compiler can warn if a member of an enum is not handled,
 * at least when the @c switch doesn't have a default handler. So the error
 * should normally never trigger.
 *
 * @pre                           typeof(enumerate) returns an @c enum type.
 * @pre                           enumerate can be converted to a string with
 *                                @c operator<<, if not use
 *                                @ref ENUM_FAIL_OUTPUT instead.
 *
 * @param enumerate               The enumerate which failed.
 */
#define ENUM_FAIL_RANGE(enumerate)                                           \
	do {                                                                     \
		lib::static_assert_wrapper::is_enum(enumerate);                      \
		throw ::lib::texception(                                             \
				  ::lib::texception::ttype::internal_failure                 \
				, ::lib::concatenate(                                        \
					  "Enumerate out of range, value '"                      \
					, enumerate                                              \
					, "' in function '"                                      \
					, __PRETTY_FUNCTION__                                    \
					, "' file '"                                             \
					, __FILE__                                               \
					, "' line '"                                             \
					, __LINE__                                               \
					, "'"));                                                 \
	} while(0)

/**
 * Unconditional error when a not implemented part of code is reached.
 *
 * This function can be used when a not implemented part of code causes a fatal
 * error.
 *
 * @note The implementation uses a @c static_assert(true), when changing the
 * value of the boolean, the compilation will fail if this macro is used in the
 * code.
 */
#define NOT_IMPLEMENTED_YET                                                  \
	do {                                                                     \
	    static_assert(true, "Not implemented code is not allowed.");         \
		throw ::lib::texception(                                             \
				  ::lib::texception::ttype::not_implemented_yet              \
				, ::lib::concatenate(                                        \
					  "'Code not implemented in function '"                  \
					, __PRETTY_FUNCTION__                                    \
					, "' file '"                                             \
					, __FILE__                                               \
					, "' line '"                                             \
					, __LINE__                                               \
					, "'"));                                                 \
	} while(0)

#endif
