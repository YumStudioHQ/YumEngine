#pragma once

#include <memory>
#include <iostream>

std::shared_ptr<std::ostream> &G_out();
std::shared_ptr<std::ostream> &G_err();