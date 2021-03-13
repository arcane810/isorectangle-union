#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define sz(x) (int)x.size()
#define int long long
#define ld long double

const int MOD = 1e9 + 7;
const int MAX_N = 1e6 + 5;
using pii = pair<long long, long long>;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 20000;

    int m = 1000;

    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        int x1, x2, y1, y2;
        x1 = rng() % m;
        x2 = rng() % m;
        y1 = rng() % m;
        y2 = rng() % m;
        if (x1 > x2) {
            swap(x1, x2);
        }
        if (y1 > y2) {
            swap(y1, y2);
        }
        cout << rng() % m << " " << rng() % m << " " << rng() % m << " "
             << rng() % m << "\n";
    }

    return 0;
}
