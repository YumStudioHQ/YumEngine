/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE.                        *
 *                                                       *
 *********************************************************/

#pragma once

#include <new>
#include <stdexcept>
#include <system_error>

#include "glob.hpp"

#pragma once

#include <new>
#include <stdexcept>
#include <system_error>
#include <ios>
#include <memory>
#include <exception>
#include <typeinfo>

#include "glob.hpp"

#if defined (YUM_NEVER_SWALLOWS_INNER_ERRORS) and YUM_NEVER_SWALLOWS_INNER_ERRORS
# if defined (YUM_SHOULD_NEVER_THROW) and YUM_SHOULD_NEVER_THROW
#  if defined (YUM_PROMOTES_WARNINGS) and YUM_PROMOTES_WARNINGS
#    error "YUM_NEVER_SWALLOWS_INNER_ERRORS is true while YUM_NEVER_SWALLOWS_INNER_ERRORS is also true. (-DYUM_PROMOTES_WARNINGS promotes this warning to an error)"
#  elif defined ( YUM_IGNORES_WARNINGS )
#  else
#    warning "YUM_NEVER_SWALLOWS_INNER_ERRORS is true while YUM_NEVER_SWALLOWS_INNER_ERRORS is also true (YUM_SHOULD_NEVER_THROW takes the best)"
#    define YUM_TRIES_THIS(C) YUM_TRIES_PRECISELY ( C )
#    define YUM_AFTER_THROW()
#  endif
# else
#  define YUM_TRIES_THIS(C) try { C; } catch (...) { throw; }
#  define YUM_AFTER_THROW() auto YUM__current_ex = std::current_exception(); if (YUM__current_ex) std::rethrow_exception(YUM__current_ex); else throw;
# endif
#else
# define YUM_TRIES_THIS(C) YUM_TRIES_PRECISELY ( C )
# define YUM_AFTER_THROW()
#endif

#define YUM_TRIES_PRECISELY(C)                                                \
  try                                                                         \
  {                                                                           \
    C;                                                                        \
  }                                                                           \
  catch (const std::bad_alloc &e)                                             \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): memory allocation failed: " << e.what() << std::endl; \
  }                                                                           \
  catch (const std::bad_array_new_length &e)                                  \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): invalid array length: " << e.what() << std::endl;     \
  }                                                                           \
  catch (const std::bad_cast &e)                                              \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): bad cast: " << e.what() << std::endl;                 \
  }                                                                           \
  catch (const std::bad_typeid &e)                                            \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): bad typeid: " << e.what() << std::endl;               \
  }                                                                           \
  catch (const std::bad_weak_ptr &e)                                          \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): bad weak_ptr: " << e.what() << std::endl;             \
  }                                                                           \
  catch (const std::logic_error &e)                                           \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): logic error: " << e.what() << std::endl;              \
  }                                                                           \
  catch (const std::runtime_error &e)                                         \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): runtime error: " << e.what() << std::endl;            \
  }                                                                           \
  catch (const std::system_error &e)                                          \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): system error: " << e.what() << std::endl;             \
  }                                                                           \
  catch (const std::ios_base::failure &e)                                     \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): I/O failure: " << e.what() << std::endl;              \
  }                                                                           \
  catch (const std::exception &e)                                             \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): unknown std::exception: " << e.what() << std::endl;   \
  }                                                                           \
  catch (...)                                                                 \
  {                                                                           \
    (*G_err()) << "yum: " << __func__ << "(): unknown non-standard exception" << std::endl;         \
  }

/**
 * Here is a graph that can represent exceptions in C++
 * Source - https://stackoverflow.com/a
 * Posted by Mooing Duck, modified by community. See post 'Timeline' for change history
 * Retrieved 2025-11-16, License - CC BY-SA 4.0
 *
 * std::exception <exception> interface (debatable if you should catch this)
 *     std::bad_alloc <new> failure to allocate storage
 *         std::bad_array_new_length <new> invalid array length
 *     std::bad_cast <typeinfo> execution of an invalid dynamic-cast
 *     std::bad_exception <exception> signifies an incorrect exception was thrown
 *     std::bad_function_call <functional> thrown by "null" std::function
 *     std::bad_typeid <typeinfo> using typeinfo on a null pointer
 *     std::bad_weak_ptr <memory> constructing a shared_ptr from a bad weak_ptr
 *     std::logic_error <stdexcept> errors detectable before the program executes
 *         std::domain_error <stdexcept> parameter outside the valid range
 *         std::future_error <future> violated a std::promise/std::future condition
 *         std::invalid_argument <stdexcept> invalid argument
 *         std::length_error <stdexcept> length exceeds its maximum allowable size
 *         std::out_of_range <stdexcept> argument value not in its expected range
 *     std::runtime_error <stdexcept> errors detectable when the program executes
 *         std::overflow_error <stdexcept> arithmetic overflow error.
 *         std::underflow_error <stdexcept> arithmetic underflow error.
 *         std::range_error <stdexcept> range errors in internal computations
 *         std::regex_error <regex> errors from the regular expression library.
 *         std::system_error <system_error> from operating system or other C API
 *             std::ios_base::failure <ios> Input or output error
 *             std::filesystem::filesystem_error <filesystem> Filesystem error
 */