#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <iterator>
#include <stack>
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
	vector <int> prices;
	vector <int> done; //da li je izgradjena ili ne
	vector <int> id; //ako unesemo da je ime nekog cvora 5, on ne mora da 'bude' na tom indeksu u nisu indices

	void dodaj(int p, int k, int c=0, int b=1) { //dodaje granu p->k
		vector <int>::iterator it;
		it = edges.begin() + indices[p];
		edges.insert(it, k);
		it = prices.begin() + indices[p];
		prices.insert(it, c);
		it = done.begin() + indices[p];
		done.insert(it, b);
		for (int i = p + 1; i < indices.size(); i++) {
			indices[i]++;
		}
	}
	void oduzmi(int p, int k) { //brise granu p->k
		vector <int>::iterator it;
		for (int i = indices[p]; i < indices[p + 1]; i++) {
			if (edges[i] == k) {
				it = edges.begin() + i;
				edges.erase(it);
				it = prices.begin() + i;
				prices.erase(it);
				it = done.begin() + i;
				done.erase(it);
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
	int maxi = 1000000;
	int izgradjena(int p, int k) {
		for (int i = indices[p]; i < indices[p + 1]; i++) {
			if (edges[i] == k) {
				return done[i];
			}
		}
		return 0;
	}
	int cena_veze(int p, int k) {
		for (int i =indices[p]; i < indices[p + 1]; i++) {
			if (edges[i] == k) {
				return prices[i];
			}
		}
		return maxi;
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
		prices.clear();
		done.clear();
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
	Graph(Graph const &g) { //kopirajuci konstruktor, koptira samo cvorove
		vector <int> idn = g.get_id();
		indices.reserve(idn.size());
		indices.push_back(0); //poslednji kao
		n = idn.size();
		e = 0;
		nt = 0;

		for (int i = 0; i < idn.size(); i++) {
			add_node(idn[i]);
		}
	}
	const vector <int>& get_id() const{
		return id;
	}
	void add_node(int k) {
		id.push_back(k);

		if (nt == 0) indices.push_back(0);
		else indices.push_back(indices.back());

		if (nt == n) { throw(Greska_pun()); }
		nt++;
	}
	void add_edge(int i1, int j1, int c=0, int b=1) {
		int p1 = pronadji(i1);
		int p2 = pronadji(j1);
		//nasli smo njihovu 'poziciju' u indices
		//dodavanje i1->j1
		dodaj(p1, p2, c, b);
		//dodavanje j1->i1
		dodaj(p2, p1, c, b);
		e++;
	}
	void delete_edge(int i1, int j1) {
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
		vector <int>::iterator it, it1, it2;
		//prvo da obrise sve veze tipa ?->i1
		for (int i = 0; i < id.size(); i++) {
			if (i != p && postoji_veza(i, p)) oduzmi(i, p);
		}
		//cout << "\nVandredni: \n";
		//ispis();
	   //brisanje iz id
		it = id.begin() + p;
		id.erase(it);
		//brisanje iz edges
		it1 = edges.begin() + indices[p];
		it2 = edges.begin() + indices[p + 1];
		int br_veza = indices[p + 1] - indices[p];
		//cout << "Broj veza: " << br_veza << "\n";
		edges.erase(it1, it2);
		it1 = prices.begin() + indices[p];
		it2 = prices.begin() + indices[p + 1];
		prices.erase(it1, it2);
		it1 = done.begin() + indices[p];
		it2 = done.begin() + indices[p + 1];
		done.erase(it1, it2);
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
		cout << "\n";
		for (int i = 0; i < prices.size(); i++) {
			cout << prices[i] << " ";
		}
		cout << "\nPravi ispis: \n";
		
		for (int i = 0; i < id.size(); i++) {
			cout << id[i] << "-";
			for (int j = indices[i]; j < indices[i + 1]; j++) {
				cout << id[edges[j]] << "("<< prices[j]<<") ";
			}
			cout << "\n";
		}

	}
	int kruskal() {
		struct Edge {
			int x, y; //njihov redni broj, preko id niza moze da se vidi sta je to tacno
			int c; //cena
			Edge(int x1,int y1,int c1):x(x1),y(y1),c(c1){}
		};
		class CompareClass {
		public:
			bool operator() (Edge* a, Edge* b) {
				if (a->c > b->c)
					return true;
				return false;
			}
		};


		priority_queue <Edge*, vector<Edge*>, CompareClass> pq;
		for (int i = 0; i < indices.size()-1; i++) {
			for (int j = indices[i]; j < indices[i + 1]; j++) {
				if (i <= edges[j]) {
					Edge* e = new Edge(i, edges[j], prices[j]);
					pq.push(e);
				}
			}
		}
		priority_queue <Edge*, vector<Edge*>, CompareClass> pq1 = pq;
		while (!pq1.empty()) {
			Edge *e=pq1.top();
			pq1.pop();
			cout << e->x << " " << e->y << " " << e->c << "\n";
		}
		int num = 0, suma = 0;
		vector <int> grupa;
		grupa.reserve(nt);
		for (int i = 0; i < nt; i++) {
			grupa.push_back(i);
		}
		//svaki cvor je nepovezan ni sa cim na pocetku
		while (num < nt - 1) {
			Edge* e = pq.top();
			pq.pop();
			if (grupa[e->x] != grupa[e->y]) {
				suma += e->c;
				num++;
				if (grupa[e->x] > grupa[e->y]) {
					for (int i = 0; i < grupa.size(); i++)
						if (grupa[i] == grupa[e->x]) grupa[i] = grupa[e->y];
				}
				else {
					for (int i = 0; i < grupa.size(); i++)
						if (grupa[i] == grupa[e->y]) grupa[i] = grupa[e->x];
				}
			}
			/*cout << "\n";
			for (int i = 0; i < nt; i++) {
				cout << grupa[i];
			}*/
		}
		return suma;
	}
	void dijkstra(int v) {
		v = pronadji(v);//njegov id kao
		int maxi = 1000000;
		set <int> S;
			vector <vector <vector<int>>> putevi(nt);
			vector <vector <int>> koliko(nt);//koliko neizgradjenih
			for (int i = 0; i < nt; i++) { //inicijalizacija putevi
				putevi[i]=vector<vector<int>>(1);
				putevi[i][0].reserve(1);
				//koliko[i] = vector<int>(1);
				koliko[i].push_back(0);
			}
		S.insert(v);
		  vector <int> d;
		  d.reserve(nt);
		  /*
		  vector <vector <int>> t(nt);
		  for (int i = 0; i < nt; i++) { //inicijalizacija t (prethodnici)
			t[i] = vector<int>(1); 
		  }
		  */
		for (int i = 0; i < nt; i++) { //pocetno, sa direktnim vezama
			int c = cena_veze(v, i);
			if (c < maxi) {
				d.push_back(cena_veze(v, i));
				//t[i][0] = v;
				putevi[i][0].push_back(v);
				koliko[i][0]= 1-izgradjena(v,i);//da li je izgradjena ili ne
			}
			else {
				d.push_back(maxi);
				//t[i][0] = -1;
				putevi[i][0].push_back(-1);
				koliko[i].push_back(-1);
			}
		}
		/*
		cout << "Probaaa\n";
		for (int k = 0; k < nt; k++) { cout << koliko[k][0] << " "; }
		*/

		for (int k = 0; k < nt; k++) {
			int min = maxi, poz = 0;
			for (int i = 0; i < d.size(); i++) {//trazenje min
				if (d[i] < min && !S.count(i)) {
					min = d[i];
					poz = i;
				}
			}
			if (d[poz] == maxi) break;
			S.insert(poz);
			for (int j = 0; j < nt; j++) {
				if (putevi[j][0][0] != -1) {
					if (!S.count(j) && d[poz] + cena_veze(poz, j) < d[j]) {
						d[j] = d[poz] + cena_veze(poz, j);
						//t[j].clear();
						//t[j].push_back(poz); //na poslednje mesto za taj element
						putevi[j][0].push_back(poz);
						koliko[j][0] =koliko[poz][0]+ 1-izgradjena(poz, j);
					}
					else if (!S.count(j) && d[poz] + cena_veze(poz, j) == d[j]) {
						//d[j] = d[poz] + cena_veze(poz, j);
						//t[j].push_back(poz);
						int br = putevi[j].size();
						putevi[j].push_back(vector<int>(1));
						koliko[j].push_back(koliko[poz][0]);
						putevi[j][br] = putevi[poz][0]; //bitno
						//koliko[j][br] = koliko[poz][0];
						putevi[j][br].push_back(poz);
						koliko[j][br] += 1 - izgradjena(poz, j);
					}
				}
			}

		}
		//ispis 
		//cout << "Ispis grafa: \n";
		//ispis();
		/*
		cout << "\n";
		cout << "\nNiz d:\n";
		for (int i = 0; i < t.size(); i++) {
			cout << d[i] << " ";
		}
		cout << "\nNiz t:\n";
		for (int i = 0; i < t.size(); i++) {
			for (int j = 0; j < t[i].size(); j++) {
				cout << t[i][j] << " ";
			}
			cout << "    ";
		}
		cout << "\n";
		*/
		//pravi ispis

		for (int i = 0; i < nt; i++) {
			if (putevi[i][0][0] != -1) {
				for (int j = 0; j < putevi[i].size(); j++) {
					for (int k = 0; k < putevi[i][j].size(); k++) {
						cout << id[putevi[i][j][k]] << "-";
					}
					cout << id[i];
					cout << "   broj neizgradjenih je: " << koliko[i][j];
					cout << "\n";
				}
			}
			else cout << "Nema puta \n";
		}
		/*
		cout << "\\\\\\\\\\\\\\n";
		for (int i = 0; i < nt; i++) {//put za sve cvorove
			int j = i;
			stack <int> s;
			if (t[j][0] != -1) {
				s.push(j);
				while (j != v) {
					j = t[j][0];
					s.push(j);
				}
				//ispis od V pa na dalje do i-tog
				if (!s.empty()) cout << id[s.top()];
				s.pop();
				while (!s.empty()) {
					cout << "-" << id[s.top()];
					s.pop();
				}
			}
			else {
				cout << "Nema puta";
			}
			cout << "\n";
		}
		*/
	}
};
int main() {
	/*
	try {
		int n = 5;
		Graph* gp = new Graph(n);
		Graph &g = *gp;
		for (int i = 0; i < n; i++) {
			g.add_node(i+1);
		}
		//5 je indeks naseg pocetnog cvora
		g.add_edge(1, 5, 5);
		g.add_edge(1, 3, 5);
		g.add_edge(5, 3, 11, 0);//
		g.add_edge(5, 2, 2, 0);//
		g.add_edge(5, 4, 8);
		g.add_edge(2, 4, 5, 0);
		g.add_edge(4, 3, 3);

		//g.delete_edge(5, 2);

		g.dijkstra(5);
		
		g.add_edge(0, 1, 18);
		g.add_edge(0, 4, 19);
		g.add_edge(0, 2, 33);
		g.add_edge(1, 2, 14);
		g.add_edge(1, 3, 10);
		g.add_edge(1, 5, 6);
		g.add_edge(2, 4, 21);
		g.add_edge(2, 5, 11);
		g.add_edge(3, 5, 5);
		g.add_edge(4, 5, 16);
		cout << "Kruskal: \n";
		cout << "\nMinamalna cena za izgradnju metroa: " << g.kruskal();
		
	}
	catch (Greska_ne_postoji_cvor & g) { cout << g; }
	catch (Greska_pun & g) { cout << g; }
	catch (Greska_vec_postoji & g) { cout << g; }
	*/
	
	int ulaz = 0, n;
	cout << "Unesi broj cvorova grafa:\n";
	cin >> n;
	cout << "Unesi:\n";
	cout << "1- add_node, 2- add_edge izgradjena, 3-add_edge neizgradjena, 4-ispis, 5-uklanjanje grane, 6-brisanje cvora, 7-brisanje iz memorije, -1-za kraj, ";
	cout<<"10-Kruskal, 11-Vatrogasci\n";
	Graph* gp = new Graph(n);
	Graph &g = *gp;
	try {
		while (ulaz != -1) {
			cin >> ulaz;
			int x, y, c, b;
			switch (ulaz) {
			case 1:
				cout << "Koji cvor dodajes? ";
				cin >> x;
				g.add_node(x);
				break;
			case 2:
				cout << "Koju izgradjenu granu dodajes i njena cena? (x,y) cena ";
				cin >> x >> y>> c;
				g.add_edge(x, y, c, 1);
				break;
			case 3:
				cout << "Koji neizgradjenu granu dodajes i njena cena? (x,y) cena ";
				cin >> x >> y>> c;
				g.add_edge(x, y, c, 0);
				break;
			case 4:
				cout << "\nIspis:\n";
				g.ispis();
				break;
			case 5:
				cout << "Koju granu uklanjas? (x,y) i (y,x) ";
				cin >> x >> y;
				g.delete_edge(x, y);
				break;
			case 6:
				cout << "Koji cvor brises? ";
				cin >> x;
				g.delete_node(x);
				break;
			case 7:
				cout << "Brisanje iz memorije!";
				g.~Graph();
				break;
			case 10:
				cout << "Kruskal: \n";
				cout << "\nMinamalna cena za izgradnju metroa: " << g.kruskal();
				break;
			case 11:
				cin>>x;
				g.dijkstra(x);
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