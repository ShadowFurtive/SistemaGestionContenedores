using namespace std;
/* Constructora. Crea un catàleg buit on numelems és el nombre
aproximat d'elements que com a màxim s'inseriran al catàleg. */
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error){
    nat nombre_primer = numelems;
    bool trobat = false;
    while(not trobat){
        if(es_primer(nombre_primer)) trobat = true;
        else nombre_primer++;
    }

    _M = nombre_primer;
    table = new node_hash*[_M];

    for (int i = 0; i < _M; i++){
        table[i] = NULL;
    }
    _quants = 0;
}

template <typename Valor>
bool cataleg<Valor>::es_primer(nat n){
    //loop from 2 to n/2 to check for factors
    for (nat i = 2; i <= n/2; i++){
        if (n % i == 0) return false;
    }

    return true;
}
/* Constructora per còpia, assignació i destructora. */
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error){
    for (int i = 0; i < _M; i++){
        esborra_nodes(table[i]);
    }
    for (int i = 0; i < c._M; i++){
        table[i] = copia_nodes(c.table[i]);
    }

    _M = c._M;
    _quants = c._quants;
}

template <typename Valor>
typename cataleg<Valor>::node_hash* cataleg<Valor>::copia_nodes(node_hash* m) {
/* Pre: cert */
/* Post: si m és NULL, el resultat és NULL; sinó,
   el resultat apunta al primer node d'un arbre binari
   de nodes que són còpia de l'arbre apuntat per m */
  node_hash* n;
  if (m == NULL) n = NULL;
  else {
    n = new node_hash;
    try {
        n->_c = m->_c;
        n->_v = m->_v;
        n->_seg = copia_nodes(m->_seg);
    } catch(...) {
      delete n;
      throw;
    }
  }
  return n;
};

template <typename Valor>
void cataleg<Valor>::esborra_nodes(node_hash* m) {
/* Pre: cert */
/* Post: no fa res si m és NULL, sinó allibera
   espai dels nodes de l'arbre binari apuntat per m */
  if (m != NULL) {
    esborra_nodes(m->_seg);
    delete m;
  }
};

template <typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c) throw(error){
    if(&c != this){
        for (int i = 0; i < _M; i++){
            esborra_nodes(table[i]);
        }
        for (int i = 0; i < c._M; i++){
            table[i] = copia_nodes(c.table[i]);
        }

        _M = c._M;
        _quants = c._quants;        
    }

    return *this;
}

template <typename Valor>
cataleg<Valor>::~cataleg() throw() {
    for (int i = 0; i < _M; i++){
        esborra_nodes(table[i]);
    }
    _quants = 0;
}

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
En cas que la clau k ja existeixi en el catàleg actualitza el valor
associat. Genera un error en cas que la clau sigui l'string buit. */
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error){
    if(k == "" || k == " ") throw error(ClauStringBuit);

    int x = util::hash(k) % _M;
    node_hash *p = table[x];
    bool found = false;
    while (p != NULL and not found) {
        if (p->_c == k) found = true;
        else p = p->_seg;
    }

    // Només canviem el valor associat
    if (found) p->_v = v;
    else {
        // Cal crear un nou node i l'afegim al principi
        node_hash* aux = new node_hash;
        aux->_c = k;
        aux->_v = v;
        aux->_seg = table[x];
        // table[x] = new node_hash(k, v, table[x]);
        table[x] = aux;        
        ++_quants;
    }

    if(_quants / _M > .90){
        // Fem la redispersió
        redispersio();
    }
}

/*
PRE
POST
*/
template <typename Valor>
bool cataleg<Valor>::aa_existeix(node_hash *m, const string &k){
    bool exist = false;

    while (m != NULL and not exist) {
        if (m->_c == k) {
            exist = true;
        }
        else {
            m = m->_seg;
        }
    }

    return exist;
}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
en cas contrari. */
template <typename Valor>
bool cataleg<Valor>::existeix(const string &k) const throw() {
    if(_M > 0){
        int i = util::hash(k) % _M;
        return aa_existeix(table[i], k);
    }

    return false;
}

template <typename Valor>
Valor cataleg<Valor>::quin(node_hash *m, const string &k){
    bool exist = false;

    while (m != NULL and not exist) {
        if (m->_c == k) {
            exist = true;
        }
        else {
            m = m->_seg;
        }
    }

    return m->_v;
}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
clau k llavors genera un error. Exemple:
   cataleg<int> ct;
   ...
   int n = ct["dia"]; */
template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error) {
    if(_M > 0){
        int i = util::hash(k) % _M;
        if(not aa_existeix(table[i], k)) throw error(ClauInexistent);
        return quin(table[i], k);
        // Look for it.

    } else {
        throw error(ClauInexistent);
    }
}

/* Retorna el nombre d'elements que s'han inserit en el catàleg
fins aquest moment. */
template <typename Valor>
nat cataleg<Valor>::quants() const throw() {
    return _quants;
}

/* Elimina del catàleg el parell que té com a clau k.
En cas que la clau k no existeixi en el catàleg genera un error. */
template <typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error) {
    if(_M > 0){
        int i = util::hash(k) % _M;
        node_hash *p = table[i], *ant = NULL;
        bool trobat = false;
        while (p != NULL and not trobat) {
            if (p->_c == k) {
                trobat = true;
            }
            else {
                ant = p;
                p = p->_seg;
            }
        }
        if (trobat) {
            if (ant == NULL) {
                table[i] = p->_seg; // Era el primer
            }
            else {
                ant->_seg = p->_seg;
            }
            delete(p);
            --_quants;
        } else {
            throw error(ClauInexistent);
        }
    } else {
        throw error(ClauInexistent);
    }

}

/*
void call_registry::redispersio(){
  // nat mida_abans = _size;
  nat mida_abans = _size;
  _size = _size * 2 + 1;
  node_hash **nova_taula = new node_hash*[_size]();

  for(int i = 0; i < mida_abans; i++){
    node_hash *p = _taula[i];
    while(p != NULL){
      node_hash *aux = p;
      p = p->seg;
      node_hash *&nou = nova_taula[funcio_hash(aux->_k)];
      aux->seg = nou;
      nou = aux;
    }
  }
  // eliminem la _taula
  delete[] _taula;
  _taula = nova_taula;
}
*/
/*
PRE:
POST:


*/

template <typename Valor>
void cataleg<Valor>::redispersio(){
    int antic = _M;
    _M = _M * 2 + 1; //mantenim numero prim.
    node_hash **nova_taula = new node_hash*[_M];
    for (int i = 0; i < _M; ++i){
        nova_taula[i] = NULL;
    }
    for (int i = 0; i < antic; i++){
        node_hash *p = table[i];
        while (p != NULL){
            int x = util::hash(p->_c) % _M;
            node_hash *aux = p->_seg;
            p->_seg = nova_taula[x];
            nova_taula[x] = p;
            p = aux;
        }
    }

    delete[] table;
    table = nova_taula;
}
