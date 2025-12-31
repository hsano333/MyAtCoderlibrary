
#include <bits/stdc++.h>
#include <cmath>
#include <atcoder/dsu>
using namespace std;
using namespace atcoder;


using uint = unsigned int;
using ull = unsigned long long;
using ll = long long;
using ull = unsigned long long;
using P = pair<int,int>;

#define rep(i,n) for (int i = 0; i < (uint)(n); i++)
#define rep1(i,n) for (int i = 1; i <= (uint)(n); i++)
#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()


#define inf 1000000000

std::uint64_t nCk(unsigned n, unsigned k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    std::uint64_t result = 1;
    for (unsigned i = 1; i <= k; ++i) {
        result = result * (n - i + 1) / i;
    }
    return result;
}

//ref https://atcoder.jp/contests/typical90/tasks/typical90_u
class DirectedGraph
{
    private:
        vector<vector<int>> graph_;
        vector<vector<int>> reverse_graph_;
        vector<bool> checked_;
        vector<bool> visited_;
        vector<int> parent_;
        dsu cycle_dsu_;
        int N;

    public:
        explicit DirectedGraph(vector<vector<int>>graph, vector<vector<int>>reverse):
            graph_(std::move(graph)), 
            reverse_graph_(std::move(reverse))
        {
            this->N = this->graph_.size();
            this->visited_ = vector<bool>(this->N, false);
            this->checked_ = vector<bool>(this->N, false);
            this->cycle_dsu_ = dsu(this->N);
        }

        void searchCycle()
        {
            auto dfs = [this](int v, auto&& dfs_ref) -> void{
                this->visited_[v] = true;
                for(int nv : this->graph_[v]){
                    if(!this->visited_[nv]) dfs_ref(nv, dfs_ref);
                }
                this->parent_.push_back(v);
            };

            auto reverse_dfs = [this](int v, auto&& dfs_ref) -> void{
                this->visited_[v] = true;
                for(int nv : this->reverse_graph_[v]){
                    if(!this->visited_[nv]){
                        this->cycle_dsu_.merge(v, nv);
                        dfs_ref(nv, dfs_ref);
                    }
                }
            };

            this->visited_ = vector<bool>(this->N, false);
            for(int node = 1; node < this->N; node++){
                if(!this->visited_[node]){
                    dfs(node, dfs);
                }
            }
            reverse(all(this->parent_));
            this->visited_ = vector<bool>(this->N, false);
            for(int node: this->parent_){
                if(!this->visited_[node]){
                    reverse_dfs(node, reverse_dfs);
                }
            }
        }

        dsu& getCycleDsu(){
            return this->cycle_dsu_;
        }

};


int main()
{
    int N, M;
    cin >> N >> M;
    vector<vector<int>> G(N+1);
    vector<vector<int>> H(N+1);

    int A, B;
    rep1(i, M){
        cin >> A >> B;
        G[A].push_back(B);
        H[B].push_back(A);
    }

    DirectedGraph dfs(std::move(G), std::move(H));
    dfs.searchCycle();
    dsu ref_dsu = dfs.getCycleDsu();
    ll sum = 0;
    for(auto group: ref_dsu.groups()){
        if(group.size() > 1){
            sum += nCk(group.size(), 2);
        }
    }

    cout << sum << endl;
    return 0;
}
