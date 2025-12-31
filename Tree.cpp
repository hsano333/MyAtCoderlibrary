
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

//ref https://atcoder.jp/contests/typical90/tasks/typical90_z
class Tree
{
    public:
        explicit Tree(vector<vector<int>>graph, vector<vector<int>>reverse):
            graph_(std::move(graph)), 
            reverse_graph_(std::move(reverse))
        {
            this->N = this->graph_.size();
            this->visited_ = vector<bool>(this->N, false);
        }

        void searchMaximumIndependent()
        {
            this->searchTreeDepth();
            int max_depth = this->depth_vec_.size()-1;
            vector<vector<int>> dp(max_depth+1, vector<int>(2, 0));
            vector<bool> prev(max_depth+1, false);
            for(int depth=1; depth <=max_depth; depth++){ 
                dp[depth][0] = std::max(dp[depth-1][1], dp[depth-1][0]);
                dp[depth][1] = dp[depth-1][0] + this->depth_vec_[depth].size();
            }

            this->independent_depth_ = vector<int>();
            int max_depth_val = std::max(dp[max_depth][0], dp[max_depth][1]);
            bool next_depth_valid = false;
            if(dp[max_depth][1] > dp[max_depth][0]){
                this->independent_depth_.push_back(max_depth);
                next_depth_valid = true;
            }
            int next_value = max_depth_val;
            for(int depth=max_depth-1; depth >= 0; depth--){ 
                if(next_depth_valid){
                    next_depth_valid = false;
                    next_value = dp[depth][0];
                    continue;
                }

                if(next_value == dp[depth][0]){
                    next_value = dp[depth][0];
                }else{
                    next_value = dp[depth][1];
                    next_depth_valid = true;
                    this->independent_depth_.push_back(depth);
                }
            }
        }

        vector<int>& getIndependentDepth(){
            return this->independent_depth_;
        }
        vector<vector<int>>& getDepthVec(){
            return this->depth_vec_;
        }

    private:
        vector<vector<int>> graph_;
        vector<vector<int>> reverse_graph_;
        vector<bool> visited_;
        vector<int> depth_;
        vector<int> independent_depth_;
        vector<vector<int>> depth_vec_;
        vector<int> tree_roots_;
        int N;

        void searchRootTree()
        {
            this->tree_roots_.clear();
            vector<bool> visited(this->N, false);
            stack<int> st;
            st.push(1);
            while(!st.empty()){
                int v = st.top();
                st.pop();
                if(visited[v]) continue;
                visited[v] = true;
                if(this->reverse_graph_[v].empty()){
                    this->tree_roots_.push_back(v);
                }else{
                    for(int nv: this->reverse_graph_[v]){
                        if(!visited[nv]){
                            st.push(nv);
                        }
                    }
                }
            }
        }

        void searchTreeDepth()
        {
            auto dfs = [this](int v, int depth, auto&& dfs_ref) -> void{
                this->depth_[v] = depth;
                this->visited_[v] = true;
                for(int nv : this->graph_[v]){
                    if(!this->visited_[nv]){
                        dfs_ref(nv, depth+1, dfs_ref);
                    }
                }
                for(int nv : this->reverse_graph_[v]){
                    if(!this->visited_[nv]){
                        dfs_ref(nv, depth-1, dfs_ref);
                    }
                }
            };

            this->searchRootTree();
            this->visited_ = vector<bool>(this->N, false);
            this->depth_ = vector<int>(this->N, 1);
            dfs(this->tree_roots_[0], 1, dfs);
            int min_depth = *min_element(all(this->depth_));
            if(min_depth != 1){
                for(int i=0;i<this->depth_.size();i++){
                    this->depth_[i] += (1-min_depth);
                }
            }
            int max_depth = *max_element(all(this->depth_));
            this->depth_vec_ = vector<vector<int>>(max_depth+1);
            for(int node = 1; node < this->N; node++){
                this->depth_vec_[this->depth_[node]].push_back(node);
            }
        }
};


int main()
{
    int N;
    cin >> N;
    vector<vector<int>> G(N+1);
    vector<vector<int>> R(N+1);
    int A, B;
    rep1(i,N-1) {
        cin >> A >> B;
        G[A].push_back(B);
        R[B].push_back(A);
    }
    Tree dfs(std::move(G), std::move(R));
    dfs.searchMaximumIndependent();
    vector<int> indep_depth = dfs.getIndependentDepth();
    vector<vector<int>> depth_vec = dfs.getDepthVec();
    int cnt = 0;
    bool isValid = true;
    for(int d: indep_depth){
        for(int node: depth_vec[d]){
            cout << node << " ";
            cnt++;
            if(cnt == N/2){
                isValid = false;
                break;
            }
        }
        if(cnt == N/2){
            break;
        }
    }
    cout << endl;

    return 0;
}
