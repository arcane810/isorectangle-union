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

    int n = 100;

    int m = 80;

    cout << n << "\n";
    for (int i = 0; i < n / 5; i++) {
        int x1, x2, y1, y2;
        x1 = rng() % (m / 2);
        x2 = rng() % (m / 2);
        y1 = rng() % (m / 2);
        y2 = rng() % (m / 4);
        cout << x1 << " " << x1 + x2 + 1 << " " << y1 << " " << y1 + y2 + 1
             << "\n";
    }

    for (int i = 0; i < n / 5; i++) {
        int x1, x2, y1, y2;
        x1 = m + rng() % (m / 2);
        x2 = rng() % (m / 4);
        y1 = rng() % (m / 2);
        y2 = rng() % (m / 2);
        cout << x1 << " " << x1 + x2 + 1 << " " << y1 << " " << y1 + y2 + 1
             << "\n";
    }

    for (int i = 0; i < n / 5; i++) {
        int x1, x2, y1, y2;
        x1 = rng() % (m / 2);
        x2 = rng() % (m / 2);
        y1 = m + rng() % (m / 2);
        y2 = rng() % (m / 2);
        cout << x1 << " " << x1 + x2 + 1 << " " << y1 << " " << y1 + y2 + 1
             << "\n";
    }

    for (int i = 0; i < n / 5; i++) {
        int x1, x2, y1, y2;
        x1 = m + rng() % (m / 4);
        x2 = rng() % (m / 2);
        y1 = m + rng() % (m / 2);
        y2 = rng() % (m / 2);
        cout << x1 << " " << x1 + x2 + 1 << " " << y1 << " " << y1 + y2 + 1
             << "\n";
    }

    for (int i = 0; i < n / 5; i++) {
        int x1, x2, y1, y2;
        x1 = m / 2 + rng() % (m / 2);
        x2 = rng() % (m / 4);
        y1 = m / 2 + rng() % (m / 4);
        y2 = rng() % (m / 2);
        cout << x1 << " " << x1 + x2 + 1 << " " << y1 << " " << y1 + y2 + 1
             << "\n";
    }

    return 0;
}
