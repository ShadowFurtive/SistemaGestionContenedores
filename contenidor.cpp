#include "contenidor.hpp"

contenidor::contenidor(const string &m, nat l) throw(error){
    if(m.size()<1) throw error(MatriculaIncorrecta);
    for(nat i=0; i<m.size(); i++){
        if(not(m[i]>=48 and m[i]<=57) and not(m[i]>=65 and m[i]<=90)) throw error(MatriculaIncorrecta);
    }
    if(l!=10 and l!=20 and l!=30) throw error(LongitudIncorrecta);
    _m=m;
    _long=l;
}

contenidor::contenidor(const contenidor &u) throw(error){
    _m=u._m;
    _long=u._long;
}
contenidor& contenidor::operator=(const contenidor &u) throw(error){
    if(this!=&u){
        _m=u._m;
        _long=u._long;
    }
    return *this;
}
contenidor::~contenidor() throw(){}

nat contenidor::longitud() const throw(){
    return _long;
}
string contenidor::matricula() const throw(){
    return _m;
}

bool contenidor::operator==(const contenidor &c) const throw(){
    if(c._m==_m and c._long==_long) return true;
    else return false;
}
bool contenidor::operator!=(const contenidor &c) const throw(){
    if(c._m!=_m or c._long!=_long) return true;
    else return false;
}
bool contenidor::operator<(const contenidor &c) const throw(){
    if(_m<c._m) return true;
    else if(_m==c._m){
        if(_long<c._long) return true;
    }
    return false;
}
bool contenidor::operator<=(const contenidor &c) const throw(){
    return (*this<c or *this==c);
}
bool contenidor::operator>(const contenidor &c) const throw(){
    return not(*this<=c);
}
bool contenidor::operator>=(const contenidor &c) const throw(){
    return not(*this<c);
}