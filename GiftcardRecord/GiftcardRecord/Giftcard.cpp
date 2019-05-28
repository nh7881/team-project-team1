#include <string>
#include "Giftcard.h"
using namespace std;

Giftcard::Giftcard(string _name, uint64_t _faceValue, uint64_t _marketValue) 
	: name(_name), faceValue(_faceValue), marketValue(_marketValue) {}


Giftcard::~Giftcard()
{
}
