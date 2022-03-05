#include <iostream>
#include <vector>
#include <list>
#include <iterator>
using namespace std;
/////////greske 

class Greska_ne_postoji_cvor {
public:
	friend ostream& operator<<(ostream& it, const Greska_ne_postoji_cvor& g) {
		it << "Ne postoji taj cvor";
		return it;
	}
};
class Greska_pun {
public:
	friend ostream& operator<<(ostream& it, const Greska_pun& g) {
		it << "Dodavanje vise cvorova nego sto je predvidjeno! ";
		return it;
	}
};
class Greska_vec_postoji {
public:
	friend ostream& operator<<(ostream& it, const Greska_vec_postoji& g) {
		it << "Vec postoji taj cvor/put";
		return it;
	}
};
///////////////////
class Graph {
	int n; //broj cvorova
	int e; //broj grana
	int nt;
	vector <int> indices;
	vector <int> edges;
	vector <int> id; //ako unesemo da je ime nekog cvora 5, on ne mora da 'bude' na tom indeksu u nisu indices

	void dodaj(int p,int k) { //dodaje granu p->k
		vector <int>::iterator it;
		it = edges.begin() + indices[p];
		edges.insert(it, k);
		for (int i = p + 1; i < indices.size(); i++) {
			indices[i]++;
		}
	} 
	void oduzmi(int p, int k) { //brise granu p->k
		vector <int> ::iterator it;
		for (int i = indices[p]; i < indices[p + 1]; i++) {
			if (edges[i] == k) {
				it = edges.begin() + i;
				edges.erase(it);
				break;
			}
		}
		for (int i = p + 1; i < indices.size(); i++) {
			indices[i]--;
		}
		
	}
	bool postoji_veza(int p, int k) {
		vector <int>::iterator it;
		for (it = edges.begin() + indices[p]; it != (edges.begin() + indices[p + 1]); it++) {
			if ((*it) == k) {
				return true;
			}
		}
		return false;
	}
	int pronadji(int i1) {
		int p = -1;
		for (int i = 0; i < id.size(); i++) {
			if (id[i] == i1) {
				p = i;
			}
		}
		if (p == -1) throw(Greska_ne_postoji_cvor());
		return p;
	}
	void brisi() {
		id.clear();
		indices.clear();
		edges.clear();
		n = nt = e = 0;
	}
public:
	Graph(int n1) :n(n1), e(0), nt(0) { 
		//praznu strukturu podataka pravi
		//moj broj i=3; sekvencijalna reprezentracija koriscenjem linearizovanih lista susednosti
		indices.reserve(n);
		indices.push_back(0); //poslednji kao
	}
	~Graph() { brisi(); }
	void add_node(int k) {
		id.push_back(k);

		if (nt==0) indices.push_back(0);
		else indices.push_back(indices.back());

		if (nt == n) { throw(Greska_pun()); }
		nt++;
	}
	void add_edge(int i1, int j1) {
		int p1 = pronadji(i1);
		int p2 = pronadji(j1);
		//nasli smo njihovu 'poziciju' u indices
		//dodavanje i1->j1
		dodaj(p1, p2);
		//dodavanje j1->i1
		dodaj(p2, p1);
		e++;
	}
	void delete_edge(int i1,int j1) {
		//nista se ne menja u indices
		int p1 = pronadji(i1);
		int p2 = pronadji(j1);
		//i1->j1 veza
		oduzmi(p1, p2);
		//j1->i1 veza
		oduzmi(p2, p1);
		e--;
	}
	void delete_node(int i1) {
		int p = pronadji(i1);
		//cout << "\n" << p << "\n";
		vector <int>::iterator it,it1,it2;
		//prvo da obrise sve veze tipa ?->i1
		for (int i = 0; i < id.size(); i++) {
			if (i != p && postoji_veza(i,p)) oduzmi(i, p);
		}
		 //cout << "\nVandredni: \n";
		 //ispis();
		//brisanje iz id
		it = id.begin() + p;
		id.erase(it);
		//brisanje iz edges
		it1 = edges.begin() + indices[p];
		it2 = edges.begin() + indices[p+1];
		int br_veza = indices[p + 1] - indices[p];
		 //cout << "Broj veza: " << br_veza << "\n";
		edges.erase(it1, it2);
		//sada zbog promene id-a mora smanjuje se bas za jedan, jedan cvor je obrisan
		for (int i = indices[p]; i < edges.size(); i++) {
			edges[i]--;
		}
		//brisanje iz indices i i njen update(update elemenata posle obrisanog)
		it = indices.begin() + p;
		for (it; it != indices.end(); it++) {
			*it -= br_veza;
		}
		it = indices.begin() + p;
		indices.erase(it);
		nt--;
	}
	void ispis() {
		/*
		for (int i = 0; i < id.size(); i++) {
			cout << id[i] << " ";
		}
		cout << "\n";
		for (int i = 0; i < indices.size(); i++) {
			cout << indices[i] << " ";
		}
		cout << "\n";
		for (int i = 0; i < edges.size(); i++) {
			cout << edges[i] << " ";
		}
		cout << "\nPravi ispis: \n"; 
		*/
		for (int i = 0; i < id.size(); i++) {
			cout << id[i] << "-";
			for (int j = indices[i]; j < indices[i + 1]; j++) {
				cout << id[edges[j]] << " ";
			}
			cout << "\n";
		}
		
	}
};

int main() {
	/*
	try {
		Graph* gp = new Graph(5);
		Graph g = *gp;
		g.add_node(5);
		g.add_node(0);
		g.add_node(2);
		g.add_edge(0, 5);
		g.add_edge(0, 2);
		g.add_edge(5, 2);

		g.ispis();
		
		//g.delete_edge(0, 5);
		//g.delete_edge(0, 2);
		//g.delete_edge(2, 5);
		//g.ispis();
	

		g.delete_node(5);
		//g.ispis();

		//g.delete_edge(5, 2);
		//g.ispis();
		g.add_node(3);
		g.add_edge(3, 2);
		//g.ispis();
		//g.delete_node(2);
		g.ispis();
	}
	catch (Greska_ne_postoji_cvor & g) { cout << g; }
	catch (Greska_pun & g) { cout << g; }
	*/
	int ulaz = 0, n;
	cout << "Unesi broj cvorava grafa:\n";
	cin >> n;
	cout << "Unesi: \n";
	cout << "1- add_node, 2- add_edge, 3-ispis, 4-uklanjanje grane, 5-brisanje cvora, 6-brisanje iz memorije, -1-za kraj\n";
	Graph* gp = new Graph(n);
	Graph &g = *gp;
	try {
		while (ulaz != -1) {
			cin >> ulaz;
			int x, y;
			switch (ulaz) {
			case 1:
				cout << "Koji cvor dodajes? ";
				cin >> x;
				g.add_node(x);
				break;
			case 2:
				cout << "Koji granu dodajes? (x,y) i (y,x) ";
				cin >> x >> y;
				g.add_edge(x, y);
				break;
			case 3:
				cout << "\nIspis:\n";
				g.ispis();
				break;
			case 4:
				cout << "Koju granu uklanjas? (x,y) i (y,x) ";
				cin >> x >> y;
				g.delete_edge(x, y);
				break;
			case 5:
				cout << "Koji cvor brises? ";
				cin >> x;
				g.delete_node(x);
				break;
			case 6:
				cout << "Brisanje iz memorije!";
				g.~Graph();
				break;
			case -1:
				cout << "Kraj!";
				break;
				return 0;
			default:
				throw(1);
				break;
			}
			cout << "\n";
		}
	}
	catch (Greska_ne_postoji_cvor & g) { cout << g; }
	catch (Greska_pun & g) { cout << g; }
	catch (Greska_vec_postoji & g) { cout << g; }
	catch (int p) { cout << "Greska. Neodgovarajuci zahtev."; }
}