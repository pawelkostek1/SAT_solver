#include "Variable.h"

Variable::Variable(int _literal,int _value) {
    literal = _literal;
    value = _value;
    letter = 'A' + _literal - 1;
	activity = 0.0;
}

Variable::~Variable()
{
}

Variable::Variable()
{
}


