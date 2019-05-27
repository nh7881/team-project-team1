#pragma once
#ifndef PRINT_HASH_H
#define PRINT_HASH_H

#include <iostream>
#include "KISA_SHA256.h"

std::ostream & operator<<(std::ostream& o, const BYTE * hash);

#endif