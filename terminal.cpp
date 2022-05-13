#include "terminal.hpp"
#include <iostream>
#define UBICACIO_ZONA_ESPERA ubicacio(-1, 0, 0)
#define UBICACIO_NO_EXISTEIX ubicacio(-1, -1, -1)

//DEBUGGER LLISTA DE FORATS LLIURES, utilizar bajo riesgo. No apto para personas sensibles. (consulte a su farmaceutico)

// cout << endl;
// cout << "################" << endl;
// cout << filas << " " << plaza << endl;
// node *forats_auxiliar_breve = _head;
// while (forats_auxiliar_breve != NULL)
// {
//     cout << forats_auxiliar_breve->ub.filera() << " " << forats_auxiliar_breve->ub.placa() << " " << forats_auxiliar_breve->ub.pis() << " " << endl;
//     forats_auxiliar_breve = forats_auxiliar_breve->seg;
// }
// cout << "################" << endl;



/* Constructora. Crea una terminal buida amb n fileres de m places
cadascuna, i una alçada màxima d'apilament h; a més fixa l'estratègia
d'inserció i retirada dels contenidors respecte el paràmetre st.
Genera un error si n=0, m=0, h=0, h > HMAX o
st no pertany a {FIRST_FIT, LLIURE}. */
terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error){
    // Comprovem que les dades introduïdes son les correctes. En cas contrari es genera una excepció. 
    if(n == 0) throw error(NumFileresIncorr);
    if(m == 0) throw error(NumPlacesIncorr);
    if(h == 0 or h > HMAX) throw error(AlcadaMaxIncorr);
    if(st != terminal::FIRST_FIT and st != terminal::LLIURE) throw error(EstrategiaIncorr);
    // S'assigna la informació base a les dades primàries de la terminal.
    this->filas = n;
    this->plaza = m;
    this->altura = h;
    this->strategy = st;
    this->grua = 0;

    // Aqui el que fem es crear un mapa buit amb les seves places, pisos y files corresponents.
    // A la vegada, vam creant una llista d'ubicacions que té totes les places del pis 0. 
    datos2 aux_datos2 = {"", 0};
    mapa = new datos2**[n];
    for(nat x = 0; x < n; ++x) {
        mapa[x] = new datos2*[m];
        node* aux;
        for(nat y = 0; y < m; ++y) {
            // Creem el forats. Nomès guardem la plaça més baixa i els pisos de cada filera com a forats.
            node* forats = new node;
            forats -> ub = ubicacio(x, y, 0);
            if(y==0 && x==0) _head=forats;
            else{
                forats->ant=aux;
                aux->seg=forats;
            }
            mapa[x][y] = new datos2[h];

            for(nat z = 0; z < h; ++z) {
                mapa[x][y][z] = aux_datos2;
            }
            aux=forats;
        }
    }
    ctlg = new cataleg<datos>(n*m*h);
}

/*
PRE:
POST: Fem una copia del node n. 
*/
terminal::node* terminal::copia_nodes_forats(node *n){
    node* aux = NULL;
    if(n != NULL){
        aux = new node;
        aux->ub = n->ub;
        aux->seg = copia_nodes_forats(n->seg);
        if(aux->seg != NULL) aux->seg->ant=aux;
    }
    return aux;
}

/*
PRE: 
POST: Fem un delete del node.
*/
void terminal::destruir_llista_forats(node *n){
    if(n != NULL){
        destruir_llista_forats(n->seg);
        delete(n);
    }
}

// TODO: Remove BST!!!!!!!


/* Constructora per còpia, assignació i destructora. */
/*
PRE:
POST: Copiem tota la informació de la terminal B al parametre implicit.
Cost: O(n^3) 
*/
terminal::terminal(const terminal& b) throw(error){
    if(&b != this){
        plaza = b.plaza; 
        filas = b.filas;
        altura = b.altura;
        strategy = b.strategy;
        grua = b.grua;

        ctlg = b.ctlg;
        ze = b.ze;

        // Destrucció del mapa actual
        for (int i = 0; i < filas; ++i){
            for (int j = 0; j < plaza; ++j) delete[] mapa[i][j];
            delete[] mapa[i];
        }
        delete[] mapa;

        // Creació del mapa actual
        datos2 aux_datos2 = {"", 0};
        mapa = new datos2**[b.filas];
        for(nat x = 0; x < b.filas; ++x) {
            mapa[x] = new datos2*[b.plaza];
            for(nat y = 0; y < b.plaza; ++y) {
                mapa[x][y] = new datos2[b.altura];
                for(nat z = 0; z < b.altura; ++z) {
                    mapa[x][y][z] = aux_datos2;
                }
            }
        }

        // Assignació del mapa del b al mapa actual
        for(nat x = 0; x < b.filas; ++x) {
            for(nat y = 0; y < b.plaza; ++y) {
                for(nat z = 0; z < b.altura; ++z) {
                    mapa[x][y][z] = b.mapa[x][y][z];
                }
            }
        }

        // Vaciar zona de espera. La clase list ja disposa de l'operador
        // d'assignació.
        ze=b.ze;
        // TODO: Copy nodes of bst

        // Destrucció i còpia de forats
        destruir_llista_forats(_head);
        _head = copia_nodes_forats(b._head);
    }
}

/*
PRE: 
POST: Copiem tota la informació de la terminal B al parametre implicit.
Cost: O(n^3)
*/
terminal& terminal::operator=(const terminal& b) throw(error){
    if(&b != this){
        plaza = b.plaza; 
        filas = b.filas;
        altura = b.altura;
        strategy = b.strategy;
        grua = b.grua;

        ctlg = b.ctlg;

        // TODO: Copy nodes of bst
        ze = b.ze;

        // Destrucció del mapa actual
        for (int i = 0; i < filas; ++i){
            for (int j = 0; j < plaza; ++j) delete[] mapa[i][j];
            delete[] mapa[i];
        }
        delete[] mapa;

        // Creació del mapa actual
        datos2 aux_datos2 = {"", 0};
        mapa = new datos2**[b.filas];
        for(nat x = 0; x < b.filas; ++x) {
            mapa[x] = new datos2*[b.plaza];
            for(nat y = 0; y < b.plaza; ++y) {
                mapa[x][y] = new datos2[b.altura];
                for(nat z = 0; z < b.altura; ++z) {
                    mapa[x][y][z] = aux_datos2;
                }
            }
        }

        // Assignació del mapa del b al mapa actual
        for(nat x = 0; x < b.filas; ++x) {
            for(nat y = 0; y < b.plaza; ++y) {
                for(nat z = 0; z < b.altura; ++z) {
                    mapa[x][y][z] = b.mapa[x][y][z];
                }
            }
        }

        // Vaciar zona de espera. La clase list ja disposa de l'operador
        // d'assignació.
        ze=b.ze;

        // Destrucció i còpia de forats
        destruir_llista_forats(_head);
        _head = copia_nodes_forats(b._head);
    }
    return *this;
}


/*
PRE: 
POST: Eliminem tots els elements que componeixen el parametre implicit.
*/
terminal::~terminal() throw(){
    //Eliminem els arrays del mapa de terminal per els valors de files, plaza y pisos. 
     for (int i = 0; i < filas; ++i){
        for (int j = 0; j < plaza; ++j){
            delete[] mapa[i][j];
        }
        delete[] mapa[i];
    }
    delete[] mapa;
    // Eliminem la llista d'ubicacions lliures
    destruir_llista_forats(_head);
    // TODO: Destroy nodes of bst
    _head=NULL;
    // Buïdem la zona d'espera.
    ze.clear();
    // Buïdem el catàleg.
    delete(ctlg);
}

/*
PRE:
POST:
Cost: O()
*/
terminal::node_ze* terminal::insereix_bst(node_ze* n, string k){
  if(n == NULL){
    node_ze* t1 = new node_ze;
    t1->info = k;
    return t1;
  } else {
    if(k < n->info) n->fe = insereix_bst(n->fe, k);
    else n->fd = insereix_bst(n->fd, k);
  }

  return n;
}


terminal::node_ze* terminal::elimina_maxim(node_ze* p) throw(){
    node_ze *p_orig = p, *pare = NULL;
    while (p->fd != NULL) {
        pare = p;
        p = p->fd;
    }
    if (pare != NULL) {
        pare->fd = p->fe; // p és fill dret de pare
        p->fe = p_orig;
    }

    return p;
}

terminal::node_ze* terminal::ajunta(node_ze* t1, node_ze* t2) throw(){
    if (t1 == NULL) return t2;
    if (t2 == NULL) return t1;

    node_ze *p = elimina_maxim(t1);
    p->fd = t2;
    return p;
}


terminal::node_ze* terminal::esborra_node_bst(node_ze* n, string k) throw(){
    node_ze* p = n;
    if (n != NULL){
        if (k < n->info) n->fe = esborra_node_bst(n->fe, k);
        else if (k > n->info) n->fd = esborra_node_bst(n->fd, k);
        else {
            n = ajunta(n->fe, n->fd);
            delete(p);
        }
    }

    return n;
}

// TODO: Compexity time/*
/*
PRE: 
POST: Retorna un booleà indicant si s'ha inserit el contenidor en la termina.
En cas que el booleà sigui True, retorna la primera ubicació (la plaça mínima).
*/
pair<bool, ubicacio> terminal::aa_insereix_contenidor(const contenidor &c) throw(error){
    node *f = _head; 
    pair<bool, ubicacio> result = make_pair(false, ubicacio(-1,0,0));
    if(f!=NULL){
        int places = c.longitud() / 10;
        int num_places=0;
        int pis, filera, placa_loc;
        pis = f->ub.pis();
        filera=f->ub.filera();
        placa_loc=f->ub.placa();
        /* 
            Busca el forat on pot anar el contenidor. 
            result.first és True quan es troben m nodes (on m es la longitud del contenidor dividit entre 10) 
            en el "forats" que es troben en el mateix pis, mateix plaça i mateixa filera.
            Si no es troba cal forat on el contenidor capigue, passa a estar en l'àrea d'espera.
        */
        while (f != NULL and not result.first){ 
            if (f->ub.pis() == pis and f->ub.filera() == filera and (f->ub.placa()-placa_loc==0))
                num_places += 1;
            else{
                num_places = 1;
                pis = f->ub.pis();
                filera = f->ub.filera();
            }
            if (num_places == places) result.first = true;
            if (not result.first){
                placa_loc = f->ub.placa()+1;
                f = f->seg;
            }
        }

        if (result.first == true){
            /*
                f conté el node més amb la plaça mes baixa. Per exemple si el contenidor era de 30 peus, contindrá
                el node on ha anat el primer peu. Per exemple:
                A30* A30 A30
                Llavors, f conté el node amb l'asterisc.
            */
            result.second = ubicacio(f->ub.filera(), f->ub.placa()-(places-1), pis);
            int fila = f->ub.filera();
            int altura_actual = f->ub.pis();
            int longi=places;
            /*
                Per cada placa, en el cas de A30, seríen tres iteracions.
            */
            while (places > 0){
                int placa = f->ub.placa();
                mapa[fila][placa][pis].matricula = c.matricula();
                // A cada node d'f, li pujem una altura.
                f->ub = ubicacio(fila, placa, altura_actual + 1);
                // S'ha posat el this per a no confondre les variables.
                if(f->ub.pis()>=this->altura){
                    // Eliminem els nodes si sobrepasen l'altura de la terminal
                    node* aux;
                    if(f->seg != NULL and f->ant == NULL){
                        aux = f->seg;
                        aux->ant=f->ant; 
                        _head=aux;
                        delete(f);
                        f=aux->ant;
                    }else if(f->seg == NULL and f->ant != NULL){
                        aux = f->ant;
                        aux->seg = NULL;
                        delete(f);
                        f=aux;
                    } else if(f->seg == NULL and f->ant == NULL){
                        _head = NULL;
                        aux = _head;
                        delete(f);
                    } else {
                        aux = f->seg;
                        node* a = f->ant;
                        a->seg = aux;
                        aux->ant = a;
                        delete(f);
                        f=a;
                    }
                    // idk what this doing hEEEE
                    longi--;
                    places--;
                }
                else{
                    places--;
                    if(places>0) f=f->ant;
                }
            }
            grua += 1;
            if(strategy==terminal::LLIURE){
                if(f != NULL){
                    node* aux2 = f;
                    while(longi > 1){
                        aux2 = aux2->seg;
                        longi--;
                    }
                    this->ordenar_apartir_de_node(f, aux2);
                }
            }
        }
    }

    return result;
}

/*
    Donat ??? nodes, els ordena de major a menor segons la ubicació.
    Tenim uns criteris pel qual no podem aplicar el MergeSort o altre tipus
    d'ordenació perquè la forma en la que ordenem els nodes segons la ubicació,
    no ens ho permet.

    Donat els nodes n i p, ordena el node n. ???????????????????????????
*/
void terminal::ordenar_apartir_de_node(node* n, node* p){
    try {
        bool seguir = true;
        while(seguir){
            if(p->seg != NULL){
                if(p->seg->ub.filera() > p->ub.filera()) seguir = false;
                if(p->seg->ub.pis()>=p->ub.pis() and p->seg->ub.placa() > p->ub.placa()) seguir = false;
                if(p->seg->ub.filera() <= p->ub.filera() and seguir){
                    if(p->seg->ub.pis() <= p->ub.pis()){
                        node* aux = p->seg;
                        p->seg = aux->seg;
                        if(aux->seg != NULL) aux->seg->ant = p;
                        if(n->ant != NULL) n->ant->seg = aux;
                        else _head = aux;
                        aux->ant = n->ant;
                        aux->seg = n;
                        n->ant = aux;
                    } else seguir = false;
                }
            } else seguir = false;
        }
    } catch(...) {

    }
}

// terminal::node* terminal::unir(node* first, node* second){
//     // If first linked list is empty 
//     if (first == NULL) return second;
  
//     // If second linked list is empty  
//     if (second == NULL) return first;  
  
//     // Pick the smaller value  
//     if (first->ub.filera() < second->ub.filera()  and first->ub.pis() < second->ub.pis()){
//         first->seg = unir(first->seg, second);   
//         first->seg->ant = first;  
//         first->ant = NULL;
//         return first;
//     } else {
//         second->seg = unir(first, second->seg);  
//         second->seg->ant = second;  
//         second->ant = NULL;
//         return second;  
//     }
// }

/* Col·loca el contenidor c en l'àrea d'emmagatzematge de la terminal o
en l'àrea d'espera si no troba lloc en l'àrea d'emmagatzematge usant
l'estratègia prefixada en el moment de crear la terminal. Si el
contenidor c es col·loca en l'àrea d'emmagatzematge pot succeir que
un o més contenidors de l'àrea d'espera puguin ser moguts a l'àrea
d'emmagatzematge.
En aquest cas es mouran els contenidors de l'àrea d'espera a l'àrea
d'emmagatzematge seguint l'ordre que indiqui l'estratègia que s'està
usant. Finalment, genera un error si ja existís a la terminal un
contenidor amb una matrícula idèntica que la del contenidor c. */
void terminal::insereix_contenidor(const contenidor &c) throw(error){
    if (on(c.matricula()) != UBICACIO_ZONA_ESPERA and on(c.matricula()) != UBICACIO_NO_EXISTEIX)
        throw error(MatriculaDuplicada);
    pair<bool, ubicacio> inserit = aa_insereix_contenidor(c);

    datos aux;
    aux.ub = inserit.second;
    aux.cont = c;
    if (not inserit.first){
        ze.push_front(c.matricula());
        ze_bst = insereix_bst(ze_bst, c.matricula());

        ctlg->assig(c.matricula(), aux);
    } else {
        ctlg->assig(c.matricula(), aux);
        // Provem d'inserir els contenidors de la zona d'espera.
        this->aa_insereix_contenidor_ze();
    }
}

// TODO: Compexity time
/* Retira de la terminal el contenidor c la matrícula del qual és igual
a m. Aquest contenidor pot estar a l'àrea d'emmagatzematge o a l'àrea
d'espera. Si el contenidor estigués a l'àrea d'emmagatzematge llavors
s'hauran de moure a l'àrea d'espera tots els contenidors que siguin
necessaris per netejar la part superior de c, s'hauran de retirar
possiblement diversos contenidors, començant pel contenidor sense cap
altre a sobre amb el número de plaça més baix (més a l'esquerra) i així
successivament (veure exemple amb detall a la subsecció Estratègia
FIRST_FIT). Un cop s'hagi eliminat el contenidor indicat, s'intenta
moure contenidors de l'àrea d'espera a l'àrea d'emmagatzematge, seguint
l'ordre que indiqui l'estratègia que s'està usant. Genera un error si a
la terminal no hi ha cap contenidor la matrícula del qual sigui igual a m. */
void terminal::retira_contenidor(const string &m) throw(error){

    ubicacio contenidor_a_retirar = on(m);
    if (contenidor_a_retirar == UBICACIO_NO_EXISTEIX)
        throw error(MatriculaInexistent);
    // El contenidor c no es es troba a l'àrea d'emmagatzematge.
    else if(contenidor_a_retirar != UBICACIO_ZONA_ESPERA){
        this->aa_retira_contenidor(m);
        ctlg->elimina(m);
        // Provem d'inserir els contenidors de la zona d'espera.
        this->aa_insereix_contenidor_ze();
    } else if(contenidor_a_retirar == UBICACIO_ZONA_ESPERA){
        ze.remove(m);
        ze_bst = esborra_node_bst(ze_bst, m);
        ctlg->elimina(m);
    }
}

/*
PRE:
POST: Tots els contenidors que tenia damunt m s'han retirat, incloent m. I s'ha actualitzat els "forats".
*/
void terminal::aa_retira_contenidor(const string &m) throw(error) {
    ubicacio contenidor_a_retirar = on(m);
    /*
        Recorrem m (on m es el tamany del contenidor entre 10) vegades.
        En cada iteració ens col·loquem un a una altura amunt. Si veïem que amunt encara hi ha contenidors, fem una crida recursiva fins
        a retirar el que està amunt de tot i començem a retirar els contenidors.
        Retirem el contenidor d'esquerre a dreta. Des de la plaça més baixa fins a la més alta.
    */
    for (int pl = 0; pl < (int)longitud(m) / 10; pl++){
        if(contenidor_a_retirar.pis()<(altura-1)){
            string matricula_cont_aux = mapa[contenidor_a_retirar.filera()][contenidor_a_retirar.placa()+pl][contenidor_a_retirar.pis()+1].matricula;
            /*
                Recordem que per a nosaltres si en el mapa, la matrícula de contenidor és buida, significa
                que allá no n'hi ha cap contenidor.            
            */
            if (matricula_cont_aux != ""){
                this->aa_retira_contenidor(matricula_cont_aux);
                ze.push_front(matricula_cont_aux);
                ze_bst = insereix_bst(ze_bst, matricula_cont_aux);
            }
        }
    }
    ubicacio x = on(m);
    contenidor c = ctlg->operator[](m).cont;
    // Posem l'estat del contenidor com "a la zona d'espera".
    datos aux;
    aux.ub = ubicacio(-1, 0, 0);
    aux.cont = c;
    ctlg->assig(c.matricula(), aux);
    grua += 1;

    // Comença el procès d'actualitzar els forats.
    node* fp = _head;
    node* fg=NULL;

    // Mirem si la ubicació nova que anem a inserir és més petita que la ubicació del node principal (_head).
    if(fp!=NULL && (fp->ub.placa()>x.placa() || fp->ub.filera()>x.filera())){
        fg=fp;
        fp=NULL;
    }
    else{
        // Busquem el anterior d'una ubicació més gran a la nostra.
        bool posicio_trobada=false;
        ubicacio nova = ubicacio(x.filera(), x.placa(), x.pis()+1);
        while(fp!=NULL and fp->seg!=NULL and not posicio_trobada){
            if(fp->seg->ub <= nova) fp=fp->seg;
            else posicio_trobada=true;
        }
        if(fp!=NULL) fg=fp->seg;
    } 

    // I actualitzem els forats.
    for (int y = 0; y < (int)longitud(m) / 10; y++){
        if(fp!=NULL && fp->ub.placa()==x.placa()+y && fp->ub.filera()==x.filera()){
            fp->ub = ubicacio(x.filera(), x.placa()+y, x.pis());
            fp = fp->seg;
        }
       else{
           node* nou = new node;
           nou->ub=ubicacio(x.filera(), x.placa()+y, x.pis());
           if(fp==NULL and fg==NULL){
               _head = nou;
           }
           else if(fp!=NULL and fg==NULL){
               nou->ant=fp;
               nou->seg=NULL;
               fp->seg=nou;
           }
           else if(fp==NULL && fg!=NULL){
               fg->ant=nou;
               nou->seg=fg;
               nou->ant=NULL;
               _head=nou;
           }
           else if(fp!=NULL && fg!=NULL){
               fg->ant=nou;
               fp->seg=nou;
               nou->seg=fg;
               nou->ant=fp;
           }
            fp=nou;
       }
        mapa[contenidor_a_retirar.filera()][x.placa() + y][x.pis()].matricula = "";
    }
    if(strategy == terminal::LLIURE){
        int longi = longitud(m) / 10;
        if(fp != NULL){
            node* aux2 = fp;
            while(longi > 1){
                aux2 = aux2->ant;
                longi--;
            }
        }
    }
}

/*
PRE:
POST: S'ha intentat insertar contenidors de la zona d'espera en la terminal. 
*/
void terminal::aa_insereix_contenidor_ze() throw(error){
    list<string>::iterator it = ze.begin();
    while(it != ze.end()){
        // S'accedeix d'aquest forma a l'operador [] perquè catàleg és un punter.
        contenidor cont = ctlg->operator[](*it).cont;
        pair<bool, ubicacio> inserit2 = aa_insereix_contenidor(cont);

        if (inserit2.first){
            datos aux2;
            aux2.ub = inserit2.second;
            aux2.cont = cont;
            ze.erase(it);
            ze_bst = esborra_node_bst(ze_bst, *it);
            // Tornem a començar desde 0 per a veure si podem continuar inserint els contenidors.
            it = ze.begin();
            ctlg->assig(cont.matricula(), aux2);
        } else ++it;
    }
}


/* Retorna la ubicació <i, j, k> del contenidor la matrícula del qual és
igual a m si el contenidor està a l'àrea d'emmagatzematge de la terminal,
la ubicació <-1, 0, 0> si el contenidor està a l'àrea d'espera, i la
ubicació <-1, -1, -1> si no existeix cap contenidor que tingui una
matrícula igual a m.
Cal recordar que si un contenidor té més de 10 peus, la seva ubicació
correspon a la plaça que tingui el número de plaça més petit. */
ubicacio terminal::on(const string &m) const throw(){
    if(ctlg->existeix(m)){
        return ctlg->operator[](m).ub;
    } else {
        return ubicacio(-1, -1, -1);
    }
}

/* Retorna la longitud del contenidor la matrícula del qual és igual
a m. Genera un error si no existeix un contenidor a la terminal
la matrícula del qual sigui igual a m. */
nat terminal::longitud(const string &m) const throw(error){
    if(ctlg->existeix(m)){
        return ctlg->operator[](m).cont.longitud();
    } else {
        throw error(MatriculaInexistent);
    }
}

/* Retorna la matrícula del contenidor que ocupa la ubicació u = <i, j, k>
o la cadena buida si la ubicació està buida. Genera un error si
i < 0, i >= n, j < 0, j >= m, k < 0 o k >= h, o sigui si <i, j, k> no
identifica una ubicació vàlida de l'àrea d'emmagatzematge. Cal observar
que si m, obtinguda amb t.contenidor_ocupa(u, m), és una matrícula (no
la cadena buida) pot succeir que u != t.on(m), ja que un contenidor pot 
ocupar diverses places i la seva ubicació es correspon amb la de la
plaça ocupada amb número de plaça més baix. */
void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error) {
    // Ubicacio --> Fila, Plaza, Piso
    // Terminal --> filas, plaza, altura

    if (u.filera() < 0 or u.filera() >= this->filas or u.placa() < 0 or u.placa() >= this->plaza or u.pis() < 0 or
        u.pis() >= this->altura){
        throw error(UbicacioNoMagatzem);
    } else {
        m = mapa[u.filera()][u.placa()][u.pis()].matricula;
    }
}

/* Retorna el nombre de places de la terminal que en aquest instant
només hi cabrien un contenidor de 10 peus, però no un de més llarg.
Per exemple, la filera de la figura 1 de l'enunciat contribuirà amb
7 unitats a la fragmentació total (corresponen a les ubicacions
<f, 0, 1>, <f, 1, 2>, <f, 2, 1>, <f, 7, 1>, <f, 8, 0>, <f, 9, 1> i
<f, 10, 0>). */
nat terminal::fragmentacio() const throw(){
    nat fg = 0;
    node* aux = _head;
    /*
        No podem fer comparacions directament per ubicacions perquè de la forma que hem pensat els forats
        no ens ho permet. Altrament podríem haver fet fàcilment, per exemple:
        if(ub1 > ub2)    

        Tenim quatre casos base per a detectar la fragmentació. Que l'anterior i següent no siguin nulls, 
        que un d'ells sigui null o que els dos siguin nulls.
        Es mira a nivell general que cumpleixin la condició de fragmentació. La ubicació deu poder tenir espais 
        sols i nomès per a un contenidor. 
        Per tant, es comproven tots els casos posibles:
            - que estigui en la mateixa filera pero en pisos diferentes
            - que estigui en la mateixa filera i en el mateix pis però a una distància de placa major a 1 
            - Pot succeïr que estiguin en fileres diferents, per tant, es tracta com un cas en especial 
              i es mira  si cumpleix la condicio de fragmentació.
        Tot això es fa mirant la ubicacio anterior de la actual i la següent de la actual. 
   */
    while(aux != NULL){
        if(aux->ant!=NULL && aux->seg!=NULL){
            if(aux->ant->ub.filera()==aux->ub.filera() && aux->seg->ub.filera()==aux->ub.filera()){
                if(aux->ant->ub.pis()!=aux->ub.pis() && aux->seg->ub.pis()!=aux->ub.pis()) fg++;
                else if((aux->seg->ub.placa()-aux->ub.placa())>1 && (aux->ub.placa()-aux->ant->ub.placa())>1) fg++;
            }
            else if(aux->ant->ub.filera()==aux->ub.filera() && aux->seg->ub.filera()!=aux->ub.filera() ){
                if(aux->ant->ub.pis()!=aux->ub.pis() or (aux->ant->ub.placa()-aux->ub.placa())>1) fg++;
            }
            else if(aux->ant->ub.filera()!=aux->ub.filera() && aux->seg->ub.filera()==aux->ub.filera()  ){
                if(aux->seg->ub.pis()!=aux->ub.pis() or (aux->seg->ub.placa()-aux->ub.placa())>1) fg++;
            }
            else if(aux->ant->ub.filera()!=aux->ub.filera() && aux->seg->ub.filera()!=aux->ub.filera()) fg++;
        }
        else if(aux->ant==NULL && aux->seg!=NULL){
            if(aux->ub.filera()==aux->seg->ub.filera()){
                if(aux->seg->ub.pis()!=aux->ub.pis() or (aux->seg->ub.placa()-aux->ub.placa())>1) fg++;
            }
        }
        else if(aux->ant!=NULL && aux->seg==NULL){
            if(aux->ub.filera()==aux->ant->ub.filera()){
                if(aux->ant->ub.pis()!=aux->ub.pis() or (aux->ub.placa()-aux->ant->ub.placa())>1) fg++;
            }
        }
        else if(aux->ant==NULL && aux->seg==NULL) fg++;
        aux=aux->seg;
    }

    return fg;
}

/* Retorna el número d'operacions de grua realitzades des del moment
de creació de la terminal.
Es requereix d'una operació de grua per moure un contenidor
des de l'àrea d'espera a l'àrea d'emmagatzematge o viceversa.
També es requereix d'una operació de grua per inserir o
retirar directament un contenidor de l'àrea d'emmagatzematge.
En canvi no requereix cap operació de grua inserir o
retirar directament un contenidor de l'àrea d'espera. */
nat terminal::ops_grua() const throw(){
    return grua;
}

void terminal::ze_en_llista(node_ze* n, list<string> &l) const throw() {
    if(n != NULL){
        ze_en_llista(n->fe, l);
        l.push_back(n->info);
        ze_en_llista(n->fd, l);
    }
}

/* Retorna la llista de les matrícules de tots els contenidors
de l'àrea d'espera de la terminal, en ordre alfabètic creixent. */
void terminal::area_espera(list<string> &l) const throw(){
    if(ze_bst != NULL){
        node_ze* aux = ze_bst;
        ze_en_llista(aux, l);
    }
    // l = ze;
    // l.sort(); // TODO: cambiar a MS, QS... <- o hacerlo nosotros si la nota==0.
    // TODO: Check si ze és vacío.
}

/* Retorna el número de fileres de la terminal. */
nat terminal::num_fileres() const throw(){
    return filas;
}

/* Retorna el número de places per filera de la terminal. */
nat terminal::num_places() const throw(){
    return plaza;
}

/* Retorna l'alçada màxima d'apilament de la terminal. */
nat terminal::num_pisos() const throw(){
    return altura;
}

/* Retorna l'estratègia d'inserció i retirada de contenidors de
la terminal. */
terminal::estrategia terminal::quina_estrategia() const throw(){
    return strategy;
}
