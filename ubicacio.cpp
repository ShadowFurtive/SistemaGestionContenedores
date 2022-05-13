#include "ubicacio.hpp"

/* Constructora. Crea la ubicació <i, j, k>. Genera un error si
<i, j, k> no pertany a {<u, v, w> | u >= 0 ^ v >= 0 ^ w >= 0}
 o a {<-1, 0, 0>,  <-1,-1,-1>}. */
ubicacio::ubicacio(int i, int j, int k) throw(error){
    if(i >= 0 and j >= 0 and k >= 0){
        fila = i;
        plaza = j;
        piso = k;
    } else if(i == -1 and j == 0 and k == 0){
        fila = i;
        plaza = j;
        piso = k;
    } else if(i == -1 and j == -1 and k == -1){
        fila = i;
        plaza = j;
        piso = k;        
    } else {
        throw error(UbicacioIncorrecta);
    }
}

/* Constructora per còpia, assignació i destructora. */
ubicacio::ubicacio(const ubicacio& u) throw(error){
    fila = u.fila;
    plaza = u.plaza;
    piso = u.piso;
}
ubicacio& ubicacio::operator=(const ubicacio& u) throw(error){
    if (&u == this)
        return *this;

    fila = u.fila;
    plaza = u.plaza;
    piso = u.piso;

    return *this;
}
ubicacio::~ubicacio() throw(){

}

/* Consultors. Retornen respectivament el primer, segon i tercer
component de la ubicació. */
int ubicacio::filera() const throw(){
    return fila;
}
int ubicacio::placa() const throw(){
    return plaza;
}
int ubicacio::pis() const throw(){
    return piso;
}

/* Operadors de comparació.
L'operador d'igualtat retorna cert si i només si les dues ubicacions
tenen la mateixa filera, plaça i piso. L'operador menor retorna cert si
i només si la filera del paràmetre implícit és més petit que la
d'u, o si les dues fileres són iguals i la plaça del paràmetre
implícit és més petita que la d'u, o si les fileres i les places
coincideixen i el pis del paràmetre implícit és més petit que el d'u.
La resta d'operadors es defineixen consistentment respecte <. */
bool ubicacio::operator==(const ubicacio &u) const throw(){
    return (fila == u.fila and plaza == u.plaza and piso == u.piso);
}
bool ubicacio::operator!=(const ubicacio &u) const throw(){
    return (fila != u.fila or plaza != u.plaza or piso != u.piso);
}
bool ubicacio::operator<(const ubicacio &u) const throw(){
    // if(fila > u.fila || plaza > u.plaza || piso > u.piso) return false;

    bool pass = false;
    if(fila < u.fila){
        pass = true;
    } else {
        // fila == u.fila
        if(fila == u.fila){
            if(plaza < u.plaza) pass = true;
            else {
                if(plaza == u.plaza){
                    // plaza == u.plaza
                    if(piso < u.piso) pass = true;
                    // piso == u.piso
                }
            }
        }
    }

    return pass;
}
bool ubicacio::operator<=(const ubicacio &u) const throw(){
    return (*this < u or *this == u);
}
bool ubicacio::operator>(const ubicacio &u) const throw(){
    return not (*this <= u);
}
bool ubicacio::operator>=(const ubicacio &u) const throw(){
    return not (*this < u);
}
