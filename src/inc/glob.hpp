/**
 * @brief defines global functions for YumEngine backend.
 */

#pragma once

#include <memory>
#include <iostream>

/**
 * @brief Returns YumEngine's standard output
 * 
 * By default: set to `std::cout`
 * @return a shared pointer to the stream (`std::shared_ptr<std::ostream>`)
 */
std::shared_ptr<std::ostream> &G_out();

/**
 * @brief Returns YumEngine's standard error output
 * 
 * By default: set to `std::cerr`
 * @return a shared pointer to the stream (`std::shared_ptr<std::ostream>`)
 */
std::shared_ptr<std::ostream> &G_err();

/**
 * @brief Returns YumEngine's standard input (unused for now)
 * 
 * By default: set to `std::cin`
 * @return a shared pointer to the stream (`std::shared_ptr<std::istream>`)
 */
std::shared_ptr<std::istream> &G_in();